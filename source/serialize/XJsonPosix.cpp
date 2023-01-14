#include <xcc-core/XJsonPosix.h>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cfloat>
#include <climits>
#include <cctype>

static const char* _StaticErrorPtr = nullptr;
static const unsigned char _StaticFirstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

// JSON String
static int __xcall__ XJson_strcasecmp(const char* _String1, const char* _String2) noexcept
{
	if(nullptr == _String1)
	{
		return (_String1 == _String2) ? 0 : 1;
	}
	if(nullptr == _String2)
	{
		return 1;
	}
	for(; tolower(*_String1) == tolower(*_String2); ++_String1, ++_String2)
	{
		if(*_String1 == 0)
		{
			return 0;
		}
	}
	return tolower(*(const unsigned char*)_String1) - tolower(*(const unsigned char*)_String2);
};

// JSON String
static char* __xcall__ XJson_strdup(const char* _String) noexcept
{
	auto		vLength = x_posix_strlen(_String) + 1;
	auto		vCopy = (char*)x_posix_malloc(vLength);
	if(nullptr == vCopy)
	{
		return nullptr;
	}
	x_posix_memcpy(vCopy, _String, vLength);
	return vCopy;
};

// 内部构造函数
static XCC_JSON_INFO* __xcall__ XJson_New_Item() noexcept
{
	auto		vNode = (XCC_JSON_INFO*)x_posix_malloc(sizeof(XCC_JSON_INFO));
	if(vNode)
	{
		x_posix_memset(vNode, 0, sizeof(XCC_JSON_INFO));
	}
	return vNode;
}

// 解析输入文本以生成一个数字，并将结果填充到item中
static const char* __xcall__ parse_number(XCC_JSON_INFO* _Item, const char* _Num) noexcept
{
	long double	vValueDouble = 0;
	auto		vValueULLong = 0ULL;
	long double	vScale = 0;
	int subscale = 0, signsubscale = 1;
	_Item->sign = 1;

	/* Could use sscanf for this? */
	if(*_Num == '-')
	{
		_Item->sign = -1, _Num++; /* Has sign? */
	}
	if(*_Num == '0')
	{
		_Num++; /* is zero */
	}
	if(*_Num >= '1' && *_Num <= '9')
	{
		do
		{
			vValueULLong *= 10;
			vValueULLong += (*_Num - '0');
			vValueDouble = (vValueDouble * 10.0) + (*_Num - '0');
			++_Num;
		}
		while(*_Num >= '0' && *_Num <= '9'); /* Number? */
	}
	if(*_Num == '.' && _Num[1] >= '0' && _Num[1] <= '9')
	{
		_Num++;
		do
		{
			vValueDouble = (vValueDouble * 10.0) + (*_Num++ - '0'), vScale--;
		}while(*_Num >= '0' && *_Num <= '9');
	} /* Fractional part? */
	if(*_Num == 'e' || *_Num == 'E') /* Exponent? */
	{
		_Num++;
		if(*_Num == '+')
		{
			_Num++;
		}
		else if(*_Num == '-')
		{
			signsubscale = -1, _Num++; /* With sign? */
		}
		while(*_Num >= '0' && *_Num <= '9')
		{
			subscale = (subscale * 10) + (*_Num++ - '0'); /* Number? */
		}
	}

	if(vScale == 0 && subscale == 0)
	{
		_Item->valuedouble = (double)(_Item->sign * (unsigned long long)vValueDouble);
		_Item->valueint = (unsigned long long)(_Item->sign * (unsigned long long)vValueULLong);
		_Item->type = XJson_Int;
	}
	else
	{
		vValueDouble = _Item->sign * vValueDouble * pow(10.0, (double)(vScale + subscale * signsubscale)); /* number = +/- number.fraction * 10^+/- exponent */
		_Item->valuedouble = (double)vValueDouble;
		_Item->valueint = (unsigned long long)vValueDouble;
		_Item->type = XJson_Double;
	}
	return _Num;
}

// 将给定项中的数字精确地呈现为字符串
static char* __xcall__ print_double(XCC_JSON_INFO* _Item) noexcept
{
	auto		vValue = _Item->valuedouble;
	auto		vString = (char*)x_posix_malloc(64); /* This is a nice tradeoff. */
	if(vString)
	{
		if(fabs(floor(vValue) - vValue) <= DBL_EPSILON)
		{
			sprintf(vString, "%.0f", vValue);
		}
		else if(fabs(vValue) < 1.0e-6 || fabs(vValue) > 1.0e9)
		{
			sprintf(vString, "%lf", vValue);
		}
		else
		{
			sprintf(vString, "%f", vValue);
		}
	}
	return vString;
}

// 将给定项中的数字精确地呈现为字符串
static char* __xcall__ print_int(XCC_JSON_INFO* _Item) noexcept
{
	auto		vString = (char*)x_posix_malloc(22); /* 2^64+1 can be represented in 21 chars. */
	if(vString)
	{
		if(_Item->sign == -1)
		{
			if((long long)_Item->valueint <= (long long)INT_MAX && (long long)_Item->valueint >= (long long)INT_MIN)
			{
				sprintf(vString, "%d", (int)_Item->valueint);
			}
			else
			{
				sprintf(vString, "%lld", (long long)_Item->valueint);
			}
		}
		else
		{
			if(_Item->valueint <= (unsigned long long)UINT_MAX)
			{
				sprintf(vString, "%u", (unsigned int)_Item->valueint);
			}
			else
			{
				sprintf(vString, "%llu", _Item->valueint);
			}
		}
	}
	return vString;
}

// 将输入文本解析为未转义的cstring，并填充项
static const char* __xcall__ parse_string(XCC_JSON_INFO* _Item, const char* _String) noexcept
{
	auto		vPtr = _String + 1;
	auto		vPtr2 = static_cast<char*>(nullptr);
	auto		vOut = static_cast<char*>(nullptr);
	auto		vLength = static_cast<int>(0);
	auto		vUC1 = static_cast<unsigned int>(0);;
	auto		vUC2 = static_cast<unsigned int>(0);;
	if(*_String != '\"')
	{
		_StaticErrorPtr = _String;
		return nullptr;
	} /* not a name! */

	while(*vPtr != '\"' && *vPtr && ++vLength)
	{
		if(*vPtr++ == '\\')
		{
			vPtr++; /* Skip escaped quotes. */
		}
	}

	vOut = (char*)x_posix_malloc(vLength + 1); /* This is how long we need for the name, roughly. */
	if(!vOut)
	{
		return nullptr;
	}

	vPtr = _String + 1;
	vPtr2 = vOut;
	while(*vPtr != '\"' && *vPtr)
	{
		if(*vPtr != '\\')
		{
			*vPtr2++ = *vPtr++;
		}
		else
		{
			vPtr++;
			switch(*vPtr)
			{
				case 'b':
					*vPtr2++ = '\b';
					break;
				case 'f':
					*vPtr2++ = '\f';
					break;
				case 'n':
					*vPtr2++ = '\n';
					break;
				case 'r':
					*vPtr2++ = '\r';
					break;
				case 't':
					*vPtr2++ = '\t';
					break;
				case 'u': /* transcode utf16 to utf8. */
					sscanf(vPtr + 1, "%4x", &vUC1);
					vPtr += 4; /* get the unicode char. */

					if((vUC1 >= 0xDC00 && vUC1 <= 0xDFFF) || vUC1 == 0)
					{
						break;	// check for invalid.
					}

					if(vUC1 >= 0xD800 && vUC1 <= 0xDBFF)	// UTF16 surrogate pairs.
					{
						if(vPtr[1] != '\\' || vPtr[2] != 'u')
						{
							break;	// missing second-half of surrogate.
						}
						sscanf(vPtr + 3, "%4x", &vUC2);
						vPtr += 6;
						if(vUC2 < 0xDC00 || vUC2 > 0xDFFF)
						{
							break;	// invalid second-half of surrogate.
						}
						vUC1 = 0x10000 | ((vUC1 & 0x3FF) << 10) | (vUC2 & 0x3FF);
					}

					vLength = 4;
					if(vUC1 < 0x80)
					{
						vLength = 1;
					}
					else if(vUC1 < 0x800)
					{
						vLength = 2;
					}
					else if(vUC1 < 0x10000)
					{
						vLength = 3;
					}
					vPtr2 += vLength;

					switch(vLength)
					{
						case 4:
							*--vPtr2 = ((vUC1 | 0x80) & 0xBF);
							vUC1 >>= 6;
						case 3:
							*--vPtr2 = ((vUC1 | 0x80) & 0xBF);
							vUC1 >>= 6;
						case 2:
							*--vPtr2 = ((vUC1 | 0x80) & 0xBF);
							vUC1 >>= 6;
						case 1:
							*--vPtr2 = (char)(vUC1 | _StaticFirstByteMark[vLength]);
					}
					vPtr2 += vLength;
					break;
				default:
					*vPtr2++ = *vPtr;
					break;
			}
			vPtr++;
		}
	}
	*vPtr2 = 0;
	if(*vPtr == '\"')
	{
		vPtr++;
	}
	_Item->valuestring = vOut;
	_Item->type = XJson_String;
	return vPtr;
}

// 将提供的cstring呈现为可打印的转义版本
static char* __xcall__ print_string_ptr(const char* _String) noexcept
{
	auto		vPtr1 = static_cast<const char*>(nullptr);
	auto		vPtr2 = static_cast<char*>(nullptr);
	auto		vOut = static_cast<char*>(nullptr);
	auto		vLength = static_cast<int>(0);
	auto		vToken = static_cast<unsigned char>(0);

	if(nullptr == _String)
	{
		return XJson_strdup("");
	}
	vPtr1 = _String;
	vToken = *vPtr1;
	while(vToken && ++vLength)
	{
		if(x_posix_strchr("\"\\\b\f\n\r\t", vToken))
		{
			vLength++;
		}
		else if(vToken < 32)
		{
			vLength += 5;
		}
		vPtr1++;
		vToken = *vPtr1;
	}

	vOut = (char*)x_posix_malloc(vLength + 3);
	if(!vOut)
	{
		return nullptr;
	}

	vPtr2 = vOut;
	vPtr1 = _String;
	*vPtr2++ = '\"';
	while(*vPtr1)
	{
		if((unsigned char)*vPtr1 > 31 && *vPtr1 != '\"' && *vPtr1 != '\\')
		{
			*vPtr2++ = *vPtr1++;
		}
		else
		{
			*vPtr2++ = '\\';
			switch(vToken = *vPtr1++)
			{
				case '\\':
					*vPtr2++ = '\\';
					break;
				case '\"':
					*vPtr2++ = '\"';
					break;
				case '\b':
					*vPtr2++ = 'b';
					break;
				case '\f':
					*vPtr2++ = 'f';
					break;
				case '\n':
					*vPtr2++ = 'n';
					break;
				case '\r':
					*vPtr2++ = 'r';
					break;
				case '\t':
					*vPtr2++ = 't';
					break;
				default:
					sprintf(vPtr2, "u%04x", vToken);
					vPtr2 += 5;
					break; /* escape and print */
			}
		}
	}
	*vPtr2++ = '\"';
	*vPtr2++ = 0;
	return vOut;
}

// 项目上的Invote print_string_ptr（这很有用）
static char* __xcall__ print_string(XCC_JSON_INFO* _Item) noexcept
{
	return print_string_ptr(_Item->valuestring);
}


// 预先公布这些原型
static const char* __xcall__ parse_value(XCC_JSON_INFO* _Item, const char* _Value) noexcept;

// 预先公布这些原型
static char* __xcall__ print_value(XCC_JSON_INFO* _Item, int depth, int _Format) noexcept;

// 预先公布这些原型
static const char* __xcall__ parse_array(XCC_JSON_INFO* _Item, const char* _Value) noexcept;

// 预先公布这些原型
static char* __xcall__ print_array(XCC_JSON_INFO* _Item, int depth, int _Format) noexcept;

// 预先公布这些原型
static const char* __xcall__ parse_object(XCC_JSON_INFO* _Item, const char* _Value) noexcept;

// 预先公布这些原型
static char* __xcall__ print_object(XCC_JSON_INFO* _Item, int depth, int _Format) noexcept;

// 跳转空白和 CR/LF
static const char* __xcall__ skip(const char* _In) noexcept
{
	while(_In && *_In && (unsigned char)*_In <= 32)
	{
		_In++;
	}
	return _In;
}


// 解析器核心-当遇到文本时，适当处理
static const char* __xcall__ parse_value(XCC_JSON_INFO* _Item, const char* _Value) noexcept
{
	if(nullptr == _Value)
	{
		return nullptr; /* Fail on null. */
	}
	if(0 == x_posix_strncmp(_Value, "null", 4))
	{
		_Item->type = XJson_NULL;
		return _Value + 4;
	}
	if(0 == x_posix_strncmp(_Value, "false", 5))
	{
		_Item->type = XJson_False;
		return _Value + 5;
	}
	if(0 == x_posix_strncmp(_Value, "true", 4))
	{
		_Item->type = XJson_True;
		_Item->valueint = 1;
		return _Value + 4;
	}
	if(*_Value == '\"')
	{
		return parse_string(_Item, _Value);
	}
	if(*_Value == '-' || (*_Value >= '0' && *_Value <= '9'))
	{
		return parse_number(_Item, _Value);
	}
	if(*_Value == '[')
	{
		return parse_array(_Item, _Value);
	}
	if(*_Value == '{')
	{
		return parse_object(_Item, _Value);
	}

	_StaticErrorPtr = _Value;
	return nullptr; /* failure. */
}

// 将值呈现为文本
static char* __xcall__ print_value(XCC_JSON_INFO* _Item, int _Depth, int _Format) noexcept
{
	auto		vOut = static_cast<char*>(nullptr);
	if(nullptr == _Item)
	{
		return nullptr;
	}
	switch((_Item->type) & 255)
	{
		case XJson_NULL:
			vOut = XJson_strdup("null");
			break;
		case XJson_False:
			vOut = XJson_strdup("false");
			break;
		case XJson_True:
			vOut = XJson_strdup("true");
			break;
		case XJson_Int:
			vOut = print_int(_Item);
			break;
		case XJson_Double:
			vOut = print_double(_Item);
			break;
		case XJson_String:
			vOut = print_string(_Item);
			break;
		case XJson_Array:
			vOut = print_array(_Item, _Depth, _Format);
			break;
		case XJson_Object:
			vOut = print_object(_Item, _Depth, _Format);
			break;
	}
	return vOut;
}

// 从输入文本生成数组
static const char* __xcall__ parse_array(XCC_JSON_INFO* _Item, const char* _Value) noexcept
{
	auto		vChild = static_cast<XCC_JSON_INFO*>(nullptr);
	if(*_Value != '[')
	{
		_StaticErrorPtr = _Value;
		return nullptr;
	} /* not an array! */

	_Item->type = XJson_Array;
	_Value = skip(_Value + 1);
	if(*_Value == ']')
	{
		return _Value + 1; /* empty array. */
	}

	_Item->child = vChild = XJson_New_Item();
	if(!_Item->child)
	{
		return nullptr; /* memory fail */
	}
	_Value = skip(parse_value(vChild, skip(_Value))); /* skip any spacing, get the value. */
	if(!_Value)
	{
		return nullptr;
	}

	while(*_Value == ',')
	{
		XCC_JSON_INFO* new_item = XJson_New_Item();
		if(!new_item)
		{
			return nullptr; /* memory fail */
		}
		vChild->next = new_item;
		new_item->prev = vChild;
		vChild = new_item;
		_Value = skip(parse_value(vChild, skip(_Value + 1)));
		if(!_Value)
		{
			return nullptr; /* memory fail */
		}
	}

	if(*_Value == ']')
	{
		return _Value + 1; /* end of array */
	}
	_StaticErrorPtr = _Value;
	return nullptr; /* malformed. */
}

// 将数组呈现为文本
static char* __xcall__ print_array(XCC_JSON_INFO* _Item, int _Depth, int _Format) noexcept
{
	char** entries;
	char* out = nullptr, * ptr = nullptr, * ret = nullptr;
	int len = 5;
	auto		vChild = _Item->child;
	int numentries = 0, i = 0, fail = 0;

	/* How many entries in the array? */
	while(vChild)
	{
		numentries++;
		vChild = vChild->next;
	}
	/* Allocate an array to hold the values for each */
	entries = (char**)x_posix_malloc(numentries * sizeof(char*));
	if(!entries)
	{
		return nullptr;
	}
	x_posix_memset(entries, 0, numentries * sizeof(char*));
	/* Retrieve all the results: */
	vChild = _Item->child;
	while(vChild && !fail)
	{
		ret = print_value(vChild, _Depth + 1, _Format);
		entries[i++] = ret;
		if(ret)
			len += (int)(x_posix_strlen(ret) + 2 + (_Format ? 1 : 0));
		else
			fail = 1;
		vChild = vChild->next;
	}

	/* If we didn't fail, try to malloc the output name */
	if(!fail)
	{
		out = (char*)x_posix_malloc(len);
	}
	/* If that fails, we fail. */
	if(!out)
	{
		fail = 1;
	}

	/* Handle failure. */
	if(fail)
	{
		for(i = 0; i < numentries; i++)
		{
			if(entries[i])
			{
				x_posix_free(entries[i]);
			}
		}
		x_posix_free(entries);
		return nullptr;
	}

	/* Compose the output array. */
	*out = '[';
	ptr = out + 1;
	*ptr = 0;
	for(i = 0; i < numentries; i++)
	{
		x_posix_strcpy(ptr, entries[i]);
		ptr += x_posix_strlen(entries[i]);
		if(i != numentries - 1)
		{
			*ptr++ = ',';
			if(_Format)
			{
				*ptr++ = ' ';
			}
			*ptr = 0;
		}
		x_posix_free(entries[i]);
	}
	x_posix_free(entries);
	*ptr++ = ']';
	*ptr++ = 0;
	return out;
}

// 从文本生成对象
static const char* __xcall__ parse_object(XCC_JSON_INFO* _Item, const char* _Value) noexcept
{
	auto		vChild = static_cast<XCC_JSON_INFO*>(nullptr);
	if(*_Value != '{')
	{
		_StaticErrorPtr = _Value;
		return nullptr;
	} /* not an object! */

	_Item->type = XJson_Object;
	_Value = skip(_Value + 1);
	if(*_Value == '}')
	{
		return _Value + 1; /* empty array. */
	}

	_Item->child = vChild = XJson_New_Item();
	if(!_Item->child)
	{
		return nullptr;
	}
	_Value = skip(parse_string(vChild, skip(_Value)));
	if(!_Value)
	{
		return nullptr;
	}
	vChild->name = vChild->valuestring;
	vChild->valuestring = nullptr;
	if(*_Value != ':')
	{
		_StaticErrorPtr = _Value;
		return nullptr;
	} /* fail! */
	_Value = skip(parse_value(vChild, skip(_Value + 1))); /* skip any spacing, get the value. */
	if(!_Value)
	{
		return nullptr;
	}

	while(*_Value == ',')
	{
		XCC_JSON_INFO* new_item = XJson_New_Item();
		if(!new_item)
		{
			return nullptr; /* memory fail */
		}
		vChild->next = new_item;
		new_item->prev = vChild;
		vChild = new_item;
		_Value = skip(parse_string(vChild, skip(_Value + 1)));
		if(!_Value)
		{
			return nullptr;
		}
		vChild->name = vChild->valuestring;
		vChild->valuestring = nullptr;
		if(*_Value != ':')
		{
			_StaticErrorPtr = _Value;
			return nullptr;
		} /* fail! */
		_Value = skip(parse_value(vChild, skip(_Value + 1))); /* skip any spacing, get the value. */
		if(!_Value)
		{
			return nullptr;
		}
	}

	if(*_Value == '}')
	{
		return _Value + 1; /* end of array */
	}
	_StaticErrorPtr = _Value;
	return nullptr; /* malformed. */
}

// 将对象呈现为文本
static char* __xcall__ print_object(XCC_JSON_INFO* _Item, int _Depth, int _Format) noexcept
{
	auto		entries = static_cast<char**>(nullptr);
	auto		names = static_cast<char**>(nullptr);
	auto		out = static_cast<char*>(nullptr);
	auto		ptr = static_cast<char*>(nullptr);
	auto		ret = static_cast<char*>(nullptr);
	auto		str = static_cast<char*>(nullptr);
	auto		len = static_cast<int>(7);
	auto		i = static_cast<int>(0);
	auto		j = static_cast<int>(0);
	auto		vChild = _Item->child;
	auto		numentries = static_cast<int>(0);
	auto		fail = static_cast<int>(0);

	// 计算条目的数量
	while(vChild)
	{
		numentries++;
		vChild = vChild->next;
	}

	// 为名称和对象分配空间
	entries = (char**)x_posix_malloc(numentries * sizeof(char*));
	if(!entries)
	{
		return nullptr;
	}
	names = (char**)x_posix_malloc(numentries * sizeof(char*));
	if(!names)
	{
		x_posix_free(entries);
		return nullptr;
	}
	x_posix_memset(entries, 0, sizeof(char*) * numentries);
	x_posix_memset(names, 0, sizeof(char*) * numentries);

	// 将所有结果收集到数组中
	vChild = _Item->child;
	_Depth++;
	if(_Format)
	{
		len += _Depth;
	}
	while(vChild)
	{
		names[i] = str = print_string_ptr(vChild->name);
		entries[i++] = ret = print_value(vChild, _Depth, _Format);
		if(str && ret)
		{
			len += (int)(x_posix_strlen(ret) + x_posix_strlen(str) + 2 + (_Format ? 2 + _Depth : 0));
		}
		else
		{
			fail = 1;
		}
		vChild = vChild->next;
	}

	// 尝试分配输出字符串
	if(!fail)
	{
		out = (char*)x_posix_malloc(len);
	}
	if(!out)
	{
		fail = 1;
	}

	// 失败处理
	if(fail)
	{
		for(i = 0; i < numentries; i++)
		{
			if(names[i])
			{
				x_posix_free(names[i]);
			}
			if(entries[i])
			{
				x_posix_free(entries[i]);
			}
		}
		x_posix_free(names);
		x_posix_free(entries);
		return nullptr;
	}

	/* Compose the output: */
	*out = '{';
	ptr = out + 1;
	if(_Format)
	{
		*ptr++ = '\n';
	}
	*ptr = 0;
	for(i = 0; i < numentries; i++)
	{
		if(_Format)
		{
			for(j = 0; j < _Depth; j++)
			{
				*ptr++ = '\t';
			}
		}
		x_posix_strcpy(ptr, names[i]);
		ptr += x_posix_strlen(names[i]);
		*ptr++ = ':';
		if(_Format)
		{
			*ptr++ = '\t';
		}
		x_posix_strcpy(ptr, entries[i]);
		ptr += x_posix_strlen(entries[i]);
		if(i != numentries - 1)
		{
			*ptr++ = ',';
		}
		if(_Format)
		{
			*ptr++ = '\n';
		}
		*ptr = 0;
		x_posix_free(names[i]);
		x_posix_free(entries[i]);
	}

	x_posix_free(names);
	x_posix_free(entries);
	if(_Format)
	{
		for(i = 0; i < _Depth - 1; i++)
		{
			*ptr++ = '\t';
		}
	}
	*ptr++ = '}';
	*ptr++ = 0;
	return out;
}


// 数组列表处理
static void __xcall__ suffix_object(XCC_JSON_INFO* _Prev, XCC_JSON_INFO* _Item) noexcept
{
	_Prev->next = _Item;
	_Item->prev = _Prev;
}

// 处理参考文献
static XCC_JSON_INFO* __xcall__ create_reference(XCC_JSON_INFO* _Item) noexcept
{
	auto		vReference = XJson_New_Item();
	if(!vReference)
	{
		return nullptr;
	}
	x_posix_memcpy(vReference, _Item, sizeof(XCC_JSON_INFO));
	vReference->name = nullptr;
	vReference->type |= XJson_IsReference;
	vReference->next = vReference->prev = nullptr;
	return vReference;
}


// 提供一个JSON块，这将返回一个可以查询的XCC_JSON_INFO对象。完成后请调用XJson_Delete
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_parse(const char* _Value) noexcept
{
	auto		vChild = XJson_New_Item();
	_StaticErrorPtr = nullptr;
	if(nullptr == vChild)
	{
		return nullptr;
	}

	if(nullptr == parse_value(vChild, skip(_Value)))
	{
		XJson_remove(vChild);
		return nullptr;
	}
	return vChild;
}

// 将XCC_JSON_INFO实体呈现为文本以进行传输/存储。完成后释放字符
_XCOREAPI_ char* __xcall__ XJson_Print(XCC_JSON_INFO* _Item) noexcept
{
	return print_value(_Item, 0, 1);
}

// 将XCC_JSON_INFO实体呈现为文本，以便传输/存储，而无需任何格式。完成后释放字符
_XCOREAPI_ char* __xcall__ XJson_PrintUnformatted(XCC_JSON_INFO* _Item) noexcept
{
	return print_value(_Item, 0, 0);
}

// 删除XCC_JSON_INFO实体及其所有子实体
_XCOREAPI_ void __xcall__ XJson_remove(XCC_JSON_INFO* _Json) noexcept
{
	auto		vNext = static_cast<XCC_JSON_INFO*>(nullptr);
	while(_Json)
	{
		vNext = _Json->next;
		if(!(_Json->type & XJson_IsReference) && _Json->child)
		{
			XJson_remove(_Json->child);
			_Json->child = nullptr;
		}
		if(!(_Json->type & XJson_IsReference) && _Json->valuestring)
		{
			x_posix_free(_Json->valuestring);
			_Json->valuestring = nullptr;
		}
		if(_Json->name)
		{
			x_posix_free(_Json->name);
			_Json->name = nullptr;
		}
		x_posix_free(_Json);
		_Json = vNext;
	}
}


// 返回数组（或对象）中的项数
_XCOREAPI_ int __xcall__ XJson_GetArraySize(XCC_JSON_INFO* _Array) noexcept
{
	auto		vChild = _Array->child;
	auto		vIndex = static_cast<int>(0);
	while(vChild)
	{
		vIndex++; vChild = vChild->next;
	}
	return vIndex;
}

// 从数组“array”中检索项目编号“item”。如果不成功，则返回NULL
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_GetArrayItem(XCC_JSON_INFO* _Array, int _Item) noexcept
{
	auto		vChild = _Array->child;
	while(vChild && _Item > 0)
	{
		_Item--;
		vChild = vChild->next;
	}
	return vChild;
}


// 从对象获取项“name”。不区分大小写
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_GetObjectItem(XCC_JSON_INFO* _Object, const char* _String) noexcept
{
	auto		vChild = _Object->child;
	while(vChild && XJson_strcasecmp(vChild->name, _String))
	{
		vChild = vChild->next;
	}
	return vChild;
}


// 用于分析失败的原因。这将返回一个指向解析错误的指针。当XJson_Parse（）返回0时定义
_XCOREAPI_ const char* __xcall__ XJson_GetErrorPtr() noexcept
{
	return _StaticErrorPtr;
}


// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateNull() noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_NULL;
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateTrue() noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_True;
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateFalse() noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_False;
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateBool(int _Number) noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = _Number ? XJson_True : XJson_False;
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateDouble(double _Number, int _Sign) noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_Double;
		vItem->valuedouble = _Number;
		vItem->valueint = (unsigned long long)_Number;
		vItem->sign = _Sign;
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateInt(unsigned long long _Number, int _Sign) noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_Int;
		vItem->valuedouble = (double)_Number;
		vItem->valueint = (unsigned long long)_Number;
		vItem->sign = _Sign;
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateString(const char* _String) noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_String;
		vItem->valuestring = XJson_strdup(_String);
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateArray() noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_Array;
	}
	return vItem;
}

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateObject() noexcept
{
	auto		vItem = XJson_New_Item();
	if(vItem)
	{
		vItem->type = XJson_Object;
	}
	return vItem;
}


// 创建int数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateIntArray(int* _Numbers, int _Sign, int _Count) noexcept
{
	auto		vCreate = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vPrev = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vArray = XJson_CreateArray();
	for(auto vIndex = 0; vArray && vIndex < _Count; vIndex++)
	{
		vCreate = XJson_CreateDouble((double)(long double)((unsigned int)_Numbers[vIndex]), _Sign);
		if(vIndex)
		{
			suffix_object(vPrev, vCreate);
		}
		else
		{
			vArray->child = vCreate;
		}
		vPrev = vCreate;
	}
	return vArray;
}

// 创建float数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateFloatArray(float* _Numbers, int _Count) noexcept
{
	auto		vCreate = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vPrev = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vArray = XJson_CreateArray();
	for(auto vIndex = 0; vArray && vIndex < _Count; vIndex++)
	{
		vCreate = XJson_CreateDouble((double)(long double)_Numbers[vIndex], -1);
		if(vIndex)
		{
			suffix_object(vPrev, vCreate);
		}
		else
		{
			vArray->child = vCreate;
		}
		vPrev = vCreate;
	}
	return vArray;
}

// 创建double数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateDoubleArray(double* _Numbers, int _Count) noexcept
{
	auto		vCreate = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vPrev = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vArray = XJson_CreateArray();
	for(auto vIndex = 0; vArray && vIndex < _Count; vIndex++)
	{
		vCreate = XJson_CreateDouble((double)(long double)_Numbers[vIndex], -1);
		if(vIndex)
		{
			suffix_object(vPrev, vCreate);
		}
		else
		{
			vArray->child = vCreate;
		}
		vPrev = vCreate;
	}
	return vArray;
}

// 创建const char*数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateStringArray(const char** _Strings, int _Count) noexcept
{
	auto		vCreate = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vPrev = static_cast<XCC_JSON_INFO*>(nullptr);
	auto		vArray = XJson_CreateArray();
	for(auto vIndex = 0; vArray && vIndex < _Count; vIndex++)
	{
		vCreate = XJson_CreateString(_Strings[vIndex]);
		if(vIndex)
		{
			suffix_object(vPrev, vCreate);
		}
		else
		{
			vArray->child = vCreate;
		}
		vPrev = vCreate;
	}
	return vArray;
}


// 将项附加到指定的数组/对象
_XCOREAPI_ void __xcall__ XJson_AddItemToArray(XCC_JSON_INFO* _Array, XCC_JSON_INFO* _Item) noexcept
{
	auto		vChild = _Array->child;
	if(nullptr == _Item)
	{
		return;
	}
	if(vChild)
	{
		while(vChild && vChild->next)
		{
			vChild = vChild->next;
		}
		suffix_object(vChild, _Item);
	}
	else
	{
		_Array->child = _Item;
	}
}

// 将项附加到指定的数组/对象
_XCOREAPI_ void __xcall__ XJson_AddItemToArrayHead(XCC_JSON_INFO* _Array, XCC_JSON_INFO* _Item) noexcept
{
	auto		vChild = _Array->child;
	if(nullptr == _Item)
	{
		return;
	}
	if(vChild)
	{
		_Item->prev = vChild->prev;
		_Item->next = vChild;
		vChild->prev = _Item;
		_Array->child = _Item;
	}
	else
	{
		_Array->child = _Item;
	}
}

// 将项附加到指定的数组/对象
_XCOREAPI_ void __xcall__ XJson_AddItemToObject(XCC_JSON_INFO* _Object, const char* _String, XCC_JSON_INFO* _Item) noexcept
{
	if(nullptr == _Item)
	{
		return;
	}
	if(_Item->name)
	{
		x_posix_free(_Item->name);
	}
	_Item->name = XJson_strdup(_String);
	XJson_AddItemToArray(_Object, _Item);
}


// 将对项的引用附加到指定的数组/对象。如果要将现有的XCC_JSON_INFO添加到新的XCC_JSON_INFO，但不希望损坏现有的XCC_JSON_INFO，请使用此选项
_XCOREAPI_ void __xcall__ XJson_AddItemReferenceToArray(XCC_JSON_INFO* _Array, XCC_JSON_INFO* _Item) noexcept
{
	XJson_AddItemToArray(_Array, create_reference(_Item));
}

// 将对项的引用附加到指定的数组/对象。如果要将现有的XCC_JSON_INFO添加到新的XCC_JSON_INFO，但不希望损坏现有的XCC_JSON_INFO，请使用此选项
_XCOREAPI_ void __xcall__ XJson_AddItemReferenceToObject(XCC_JSON_INFO* _Object, const char* _String, XCC_JSON_INFO* _Item) noexcept
{
	XJson_AddItemToObject(_Object, _String, create_reference(_Item));
}


// 从数组/对象中移除/取消匹配项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_DetachItemFromArray(XCC_JSON_INFO* _Array, int _Index) noexcept
{
	auto		vChild = _Array->child;
	while(vChild && _Index > 0)
	{
		vChild = vChild->next;
		_Index--;
	}
	if(nullptr == vChild)
	{
		return 0;
	}
	if(vChild->prev)
	{
		vChild->prev->next = vChild->next;
	}
	if(vChild->next)
	{
		vChild->next->prev = vChild->prev;
	}
	if(vChild == _Array->child)
	{
		_Array->child = vChild->next;
	}
	vChild->prev = vChild->next = 0;
	return vChild;
}

// 从数组/对象中移除/取消匹配项
_XCOREAPI_ void __xcall__ XJson_DeleteItemFromArray(XCC_JSON_INFO* _Array, int _Index) noexcept
{
	XJson_remove(XJson_DetachItemFromArray(_Array, _Index));
}

// 从数组/对象中移除/取消匹配项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_DetachItemFromObject(XCC_JSON_INFO* _Object, const char* _String) noexcept
{
	auto		vIndex = static_cast<int>(0);
	auto		vChild = _Object->child;
	while(vChild && XJson_strcasecmp(vChild->name, _String))
	{
		vIndex++;
		vChild = vChild->next;
	}
	if(vChild)
	{
		return XJson_DetachItemFromArray(_Object, vIndex);
	}
	return 0;
}

// 从数组/对象中移除/取消匹配项
_XCOREAPI_ void __xcall__ XJson_DeleteItemFromObject(XCC_JSON_INFO* _Object, const char* _String) noexcept
{
	XJson_remove(XJson_DetachItemFromObject(_Object, _String));
}

// 更新数组项
_XCOREAPI_ void __xcall__ XJson_ReplaceItemInArray(XCC_JSON_INFO* _Array, int _Index, XCC_JSON_INFO* _NewItem) noexcept
{
	auto		vChild = _Array->child;
	while(vChild && _Index > 0)
	{
		vChild = vChild->next;
		_Index--;
	}
	if(nullptr == vChild)
	{
		return;
	}
	_NewItem->next = vChild->next;
	_NewItem->prev = vChild->prev;
	if(_NewItem->next)
	{
		_NewItem->next->prev = _NewItem;
	}
	if(vChild == _Array->child)
	{
		_Array->child = _NewItem;
	}
	else
	{
		_NewItem->prev->next = _NewItem;
	}
	vChild->next = vChild->prev = 0;
	XJson_remove(vChild);
}

// 更新数组项
_XCOREAPI_ void __xcall__ XJson_ReplaceItemInObject(XCC_JSON_INFO* _Object, const char* _String, XCC_JSON_INFO* _NewItem) noexcept
{
	auto		vIndex = static_cast<int>(0);
	auto		vChild = _Object->child;
	while(vChild && XJson_strcasecmp(vChild->name, _String))
	{
		vIndex++;
		vChild = vChild->next;
	}
	if(vChild)
	{
		_NewItem->name = XJson_strdup(_String);
		XJson_ReplaceItemInArray(_Object, vIndex, _NewItem);
	}
}




// constructor
XJsonPosix::XJsonPosix(XCC_JSON_INFO* _JsonData) noexcept : _ThisJsonData(nullptr), _ThisExternJsonDataRef(_JsonData)
{
	this->_ThisJsonArrayRef = new(std::nothrow) std::map<unsigned int, XJsonPosix*>();
	this->_ThisJsonObjectRef = new(std::nothrow) std::map<UString, XJsonPosix*>();
}

// constructor
XJsonPosix::XJsonPosix() noexcept : _ThisJsonData(nullptr), _ThisExternJsonDataRef(nullptr)
{
	this->_ThisJsonArrayRef = new(std::nothrow) std::map<unsigned int, XJsonPosix*>();
	this->_ThisJsonObjectRef = new(std::nothrow) std::map<UString, XJsonPosix*>();
}

// constructor
XJsonPosix::XJsonPosix(const UString& _JsonString) noexcept : _ThisJsonData(nullptr), _ThisExternJsonDataRef(nullptr)
{
	this->_ThisJsonArrayRef = new(std::nothrow) std::map<unsigned int, XJsonPosix*>();
	this->_ThisJsonObjectRef = new(std::nothrow) std::map<UString, XJsonPosix*>();
	parse(_JsonString);
}

// constructor
XJsonPosix::XJsonPosix(const XByteArray& _JsonBytes) noexcept : _ThisJsonData(nullptr), _ThisExternJsonDataRef(nullptr)
{
	this->_ThisJsonArrayRef = new(std::nothrow) std::map<unsigned int, XJsonPosix*>();
	this->_ThisJsonObjectRef = new(std::nothrow) std::map<UString, XJsonPosix*>();
	parse(_JsonBytes);
}

// constructor
XJsonPosix::XJsonPosix(const XString& _JsonString) noexcept : _ThisJsonData(nullptr), _ThisExternJsonDataRef(nullptr)
{
	this->_ThisJsonArrayRef = new(std::nothrow) std::map<unsigned int, XJsonPosix*>();
	this->_ThisJsonObjectRef = new(std::nothrow) std::map<UString, XJsonPosix*>();
	parse(_JsonString);
}

// constructor
XJsonPosix::XJsonPosix(const XJsonPosix* _JsonObject) noexcept : _ThisJsonData(nullptr), _ThisExternJsonDataRef(nullptr)
{
	this->_ThisJsonArrayRef = new(std::nothrow) std::map<unsigned int, XJsonPosix*>();
	this->_ThisJsonObjectRef = new(std::nothrow) std::map<UString, XJsonPosix*>();
	if(_JsonObject)
	{
		parse(_JsonObject->toString());
	}
}

// constructor
XJsonPosix::XJsonPosix(const XJsonPosix& _JsonObject) noexcept : _ThisJsonData(nullptr), _ThisExternJsonDataRef(nullptr)
{
	this->_ThisJsonArrayRef = new(std::nothrow) std::map<unsigned int, XJsonPosix*>();
	this->_ThisJsonObjectRef = new(std::nothrow) std::map<UString, XJsonPosix*>();
	parse(_JsonObject.toString());
}

// destructor
XJsonPosix::~XJsonPosix() noexcept
{
	clear();
	XCC_DELETE_PTR(this->_ThisJsonArrayRef);
	XCC_DELETE_PTR(this->_ThisJsonObjectRef);
}



// [opt] operators overload =
XJsonPosix& XJsonPosix::operator =(const XJsonPosix& _Value) noexcept
{
	parse(_Value.toString());
	return *this;
}

// [opt] operators overload ==
bool XJsonPosix::operator==(const XJsonPosix& _Value) const noexcept
{
	return(this->toString() == _Value.toString());
}



bool XJsonPosix::appendEmptySubObject(const UString& _Key) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateObject();
	if(nullptr == vJsonStruct)
	{
		_ThisErrorMessage = "create sub empty object error!";
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	return true;
}

bool XJsonPosix::appendEmptySubArray(const UString& _Key) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateArray();
	if(nullptr == vJsonStruct)
	{
		_ThisErrorMessage = "create sub empty array error!";
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	return true;
}

XJsonPosix& XJsonPosix::operator[](const UString& _Key) noexcept
{
	auto		vIterator = _ThisJsonObjectRef->find(_Key);
	if(vIterator == _ThisJsonObjectRef->end())
	{
		auto		vJsonStruct = static_cast<XCC_JSON_INFO*>(nullptr);
		if(_ThisJsonData)
		{
			if(_ThisJsonData->type == XJson_Object)
			{
				vJsonStruct = XJson_GetObjectItem(_ThisJsonData, _Key.data());
			}
		}
		else if(_ThisExternJsonDataRef)
		{
			if(_ThisExternJsonDataRef->type == XJson_Object)
			{
				vJsonStruct = XJson_GetObjectItem(_ThisExternJsonDataRef, _Key.data());
			}
		}
		if(vJsonStruct)
		{
			auto		vJsonObject = new XJsonPosix(vJsonStruct);
			_ThisJsonObjectRef->insert(std::pair<UString, XJsonPosix*>(_Key, vJsonObject));
			return(*vJsonObject);
		}
		else
		{
			auto		vJsonObject = new XJsonPosix();
			_ThisJsonObjectRef->insert(std::pair<UString, XJsonPosix*>(_Key, vJsonObject));
			return(*vJsonObject);
		}
	}
	else
	{
		return(*(vIterator->second));
	}
}

// [get] 根据下标获取子项
XJsonPosix& XJsonPosix::operator[](unsigned int _Index) noexcept
{
	auto		vIterator = _ThisJsonArrayRef->find(_Index);
	if(vIterator == _ThisJsonArrayRef->end())
	{
		auto		vJsonStruct = static_cast<XCC_JSON_INFO*>(nullptr);
		if(_ThisJsonData)
		{
			if(_ThisJsonData->type == XJson_Array)
			{
				vJsonStruct = XJson_GetArrayItem(_ThisJsonData, _Index);
			}
		}
		else if(_ThisExternJsonDataRef)
		{
			if(_ThisExternJsonDataRef->type == XJson_Array)
			{
				vJsonStruct = XJson_GetArrayItem(_ThisExternJsonDataRef, _Index);
			}
		}
		if(vJsonStruct)
		{
			auto		vJsonObject = new XJsonPosix(vJsonStruct);
			_ThisJsonArrayRef->insert(std::pair<unsigned int, XJsonPosix*>(_Index, vJsonObject));
			return(*vJsonObject);
		}
		else
		{
			auto		vJsonObject = new XJsonPosix();
			_ThisJsonArrayRef->insert(std::pair<unsigned int, XJsonPosix*>(_Index, vJsonObject));
			return(*vJsonObject);
		}
	}
	else
	{
		return(*(vIterator->second));
	}
}



UString XJsonPosix::operator()(const UString& _Key) const noexcept
{
	auto		vJsonStruct = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		if(_ThisJsonData->type == XJson_Object)
		{
			vJsonStruct = XJson_GetObjectItem(_ThisJsonData, _Key.data());
		}
	}
	else if(_ThisExternJsonDataRef)
	{
		if(_ThisExternJsonDataRef->type == XJson_Object)
		{
			vJsonStruct = XJson_GetObjectItem(_ThisExternJsonDataRef, _Key.data());
		}
	}
	if(vJsonStruct == nullptr)
	{
		return "";
	}
	if(vJsonStruct->type == XJson_String)
	{
		return vJsonStruct->valuestring;
	}
	else if(vJsonStruct->type == XJson_Int)
	{
		char		vBuffer[128] = { 0 };
		if(vJsonStruct->sign == -1)
		{
			if((long long)vJsonStruct->valueint <= (long long)INT_MAX && (long long)vJsonStruct->valueint >= (long long)INT_MIN)
			{
				snprintf(vBuffer, sizeof(vBuffer), "%d", (int)vJsonStruct->valueint);
			}
			else
			{
				snprintf(vBuffer, sizeof(vBuffer), "%lld", (long long)vJsonStruct->valueint);
			}
		}
		else
		{
			if(vJsonStruct->valueint <= (unsigned long long)UINT_MAX)
			{
				snprintf(vBuffer, sizeof(vBuffer), "%u", (unsigned int)vJsonStruct->valueint);
			}
			else
			{
				snprintf(vBuffer, sizeof(vBuffer), "%llu", vJsonStruct->valueint);
			}
		}
		return vBuffer;
	}
	else if(vJsonStruct->type == XJson_Double)
	{
		char		vBuffer[128] = { 0 };
		if(fabs(vJsonStruct->valuedouble) < 1.0e-6 || fabs(vJsonStruct->valuedouble) > 1.0e9)
		{
			snprintf(vBuffer, sizeof(vBuffer), "%e", vJsonStruct->valuedouble);
		}
		else
		{
			snprintf(vBuffer, sizeof(vBuffer), "%f", vJsonStruct->valuedouble);
		}
		return vBuffer;
	}
	else if(vJsonStruct->type == XJson_False)
	{
		return "false";
	}
	else if(vJsonStruct->type == XJson_True)
	{
		return "true";
	}
	return "";
}

// [get] 根据下标获取子项
UString XJsonPosix::operator()(unsigned int _Index) const noexcept
{
	auto		vJsonStruct = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		if(_ThisJsonData->type == XJson_Array)
		{
			vJsonStruct = XJson_GetArrayItem(_ThisJsonData, _Index);
		}
	}
	else if(_ThisExternJsonDataRef)
	{
		if(_ThisExternJsonDataRef->type == XJson_Array)
		{
			vJsonStruct = XJson_GetArrayItem(_ThisExternJsonDataRef, _Index);
		}
	}
	if(vJsonStruct == nullptr)
	{
		return "";
	}
	if(vJsonStruct->type == XJson_String)
	{
		return vJsonStruct->valuestring;
	}
	else if(vJsonStruct->type == XJson_Int)
	{
		char		vBuffer[128] = { 0 };
		if(vJsonStruct->sign == -1)
		{
			if((long long)vJsonStruct->valueint <= (long long)INT_MAX && (long long)vJsonStruct->valueint >= (long long)INT_MIN)
			{
				snprintf(vBuffer, sizeof(vBuffer), "%d", (int)vJsonStruct->valueint);
			}
			else
			{
				snprintf(vBuffer, sizeof(vBuffer), "%lld", (long long)vJsonStruct->valueint);
			}
		}
		else
		{
			if(vJsonStruct->valueint <= (unsigned long long)UINT_MAX)
			{
				snprintf(vBuffer, sizeof(vBuffer), "%u", (unsigned int)vJsonStruct->valueint);
			}
			else
			{
				snprintf(vBuffer, sizeof(vBuffer), "%llu", vJsonStruct->valueint);
			}
		}
		return vBuffer;
	}
	else if(vJsonStruct->type == XJson_Double)
	{
		char		vBuffer[128] = { 0 };
		if(fabs(vJsonStruct->valuedouble) < 1.0e-6 || fabs(vJsonStruct->valuedouble) > 1.0e9)
		{
			snprintf(vBuffer, sizeof(vBuffer), "%e", vJsonStruct->valuedouble);
		}
		else
		{
			snprintf(vBuffer, sizeof(vBuffer), "%f", vJsonStruct->valuedouble);
		}
		return vBuffer;
	}
	else if(vJsonStruct->type == XJson_False)
	{
		return "false";
	}
	else if(vJsonStruct->type == XJson_True)
	{
		return "true";
	}
	return "";
}



// [fmt] from string
bool XJsonPosix::parse(const char* _Bytes) noexcept
{
	clear();
	_ThisJsonData = XJson_parse(_Bytes);
	if(nullptr == _ThisJsonData)
	{
		// _ThisErrorMessage = UString("prase json name error at ") + XJson_GetErrorPtr();
		_ThisErrorMessage = "prase json name error at ";
		return false;
	}
	return true;
}

// [fmt] from string
bool XJsonPosix::parse(const UString& strJson) noexcept
{
	clear();
	_ThisJsonData = XJson_parse(strJson.data());
	if(nullptr == _ThisJsonData)
	{
		// _ThisErrorMessage = UString("prase json name error at ") + XJson_GetErrorPtr();
		_ThisErrorMessage = "prase json name error at ";
		return false;
	}
	return true;
}

// [fmt] from bytes
bool XJsonPosix::parse(const XByteArray& _JsonBytes) noexcept
{
	return this->parse(UString(_JsonBytes.data(), _JsonBytes.size()));
}

// [fmt] from string
bool XJsonPosix::parse(const XString& _JsonString) noexcept
{
	return this->parse(_JsonString.toUString());
}



void XJsonPosix::clear() noexcept
{
	_ThisExternJsonDataRef = nullptr;
	if(_ThisJsonData)
	{
		XJson_remove(_ThisJsonData);
		_ThisJsonData = nullptr;
	}
	for(auto & vIterator : *_ThisJsonArrayRef)
	{
		if(vIterator.second)
		{
			delete (vIterator.second);
			vIterator.second = nullptr;
		}
	}
	_ThisJsonArrayRef->clear();
	for(auto & vIterator : *_ThisJsonObjectRef)
	{
		if(vIterator.second)
		{
			delete (vIterator.second);
			vIterator.second = nullptr;
		}
	}
	_ThisJsonObjectRef->clear();
}

// [get] 是否为空
bool XJsonPosix::empty() const noexcept
{
	if(_ThisJsonData)
	{
		return false;
	}
	else if(_ThisExternJsonDataRef)
	{
		return false;
	}
	return true;
}

// [get] 是否有值
bool XJsonPosix::exist() const noexcept
{
	return _ThisJsonData || _ThisExternJsonDataRef;
}

bool XJsonPosix::isArray() const noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}

	if(vFocusData == nullptr)
	{
		return false;
	}
	if(vFocusData->type == XJson_Array)
	{
		return true;
	}
	else
	{
		return false;
	}
}

XString XJsonPosix::toString() const noexcept
{
	auto		vJsonString = static_cast<char*>(nullptr);
	auto		vJsonData = XString();
	if(_ThisJsonData)
	{
		vJsonString = XJson_PrintUnformatted(_ThisJsonData);
	}
	else if(_ThisExternJsonDataRef)
	{
		vJsonString = XJson_PrintUnformatted(_ThisExternJsonDataRef);
	}
	if(vJsonString)
	{
		vJsonData = vJsonString;
		x_posix_free(vJsonString);
	}
	return vJsonData;
}

XString XJsonPosix::toFormattedString() const noexcept
{
	auto		vJsonString = static_cast<char*>(nullptr);
	auto		vJsonData = XString();
	if(_ThisJsonData)
	{
		vJsonString = XJson_Print(_ThisJsonData);
	}
	else if(_ThisExternJsonDataRef)
	{
		vJsonString = XJson_Print(_ThisExternJsonDataRef);
	}
	if(vJsonString)
	{
		vJsonData = vJsonString;
		x_posix_free(vJsonString);
	}
	return vJsonData;
}

const XString& XJsonPosix::getErrorMessage() const noexcept
{
	return _ThisErrorMessage;
}



// [get] 获取指定Key的子项
XCC_JSON_INFO* XJsonPosix::_get(const UString& _Key) const noexcept
{
	auto		vJsonStruct = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		if(_ThisJsonData->type == XJson_Object)
		{
			vJsonStruct = XJson_GetObjectItem(_ThisJsonData, _Key.data());
		}
	}
	else if(_ThisExternJsonDataRef)
	{
		if(_ThisExternJsonDataRef->type == XJson_Object)
		{
			vJsonStruct = XJson_GetObjectItem(_ThisExternJsonDataRef, _Key.data());
		}
	}
	return vJsonStruct;
}



// [get] 遍历
bool XJsonPosix::traverse(const std::function<void(const UString& _Key)>& _Lambda) const noexcept
{
	auto		vJsonStruct = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		if(_ThisJsonData->type == XJson_Object)
		{
			vJsonStruct = _ThisJsonData;
		}
	}
	else if(_ThisExternJsonDataRef)
	{
		if(_ThisExternJsonDataRef->type == XJson_Object)
		{
			vJsonStruct = _ThisExternJsonDataRef;
		}
	}
	if(vJsonStruct && _Lambda)
	{
		auto		vKeyArray = std::vector<UString>();
		auto		vChild = vJsonStruct->child;
		while (vChild)
		{
			if(vChild->name && strlen(vChild->name) > 0)
			{
				vKeyArray.emplace_back(vChild->name);
			}
			vChild = vChild->next;
		}
		for(const auto & vIterator :vKeyArray)
		{
			_Lambda(vIterator);
		}
		return true;
	}
	else
	{
		return false;
	}
}



// [get] 获取指定Key的值 Object
bool XJsonPosix::get(const UString& _Key, XJsonPosix& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vJsonString = XJson_Print(vJsonStruct);

	// Fix : basic_string::_M_construct null not valid
	auto		vJsonData = UString(vJsonString ? vJsonString : "");
	x_posix_free(vJsonString);
	if(_Value.parse(vJsonData))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// [get] 获取指定Key的值 String
bool XJsonPosix::get(const UString& _Key, UString& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_String)
	{
		return false;
	}
	_Value = vJsonStruct->valuestring;
	return true;
}

// [get] 获取指定Key的值 Bytes
bool XJsonPosix::get(const UString& _Key, XByteArray& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_String)
	{
		return false;
	}
	_Value = vJsonStruct->valuestring;
	return true;
}

// [get] 获取指定Key的值 Bytes
bool XJsonPosix::get(const UString& _Key, XString& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_String)
	{
		return false;
	}
	_Value = XString::fromUString(vJsonStruct->valuestring);
	return true;
}

// [get] 获取指定Key的值 Bool
bool XJsonPosix::get(const UString& _Key, bool& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type > XJson_True)
	{
		return false;
	}
	_Value = vJsonStruct->type == XJson_True;
	return true;
}

// [get] 获取指定Key的值 char
bool XJsonPosix::get(const UString& _Key, char& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (char)(vJsonStruct->valueint);
	return true;
}

// [get] 获取指定Key的值 unsigned char
bool XJsonPosix::get(const UString& _Key, unsigned char& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned char)(vJsonStruct->valueint);
	return true;
}

// [get] 获取指定Key的值 short
bool XJsonPosix::get(const UString& _Key, short& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (short)(vJsonStruct->valueint);
	return true;
}

// [get] 获取指定Key的值 unsigned short
bool XJsonPosix::get(const UString& _Key, unsigned short& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned short)(vJsonStruct->valueint);
	return true;
}

// [get] 获取指定Key的值 int
bool XJsonPosix::get(const UString& _Key, int& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (int)(vJsonStruct->valueint);
	return true;
}

// [get] 获取指定Key的值 unsigned int
bool XJsonPosix::get(const UString& _Key, unsigned int& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned int)(vJsonStruct->valueint);
	return true;
}

// [get] 获取指定Key的值 long
bool XJsonPosix::get(const UString& _Key, long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (long)vJsonStruct->valueint;
	return true;
}

// [get] 获取指定Key的值 unsigned long
bool XJsonPosix::get(const UString& _Key, unsigned long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned long)vJsonStruct->valueint;
	return true;
}

// [get] 获取指定Key的值 long long
bool XJsonPosix::get(const UString& _Key, long long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (long long)vJsonStruct->valueint;
	return true;
}

// [get] 获取指定Key的值 unsigned long long
bool XJsonPosix::get(const UString& _Key, unsigned long long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned long long)vJsonStruct->valueint;
	return true;
}

// [get] 获取指定Key的值 float
bool XJsonPosix::get(const UString& _Key, float& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Double)
	{
		return false;
	}
	_Value = (float)(vJsonStruct->valuedouble);
	return true;
}

// [get] 获取指定Key的值 double
bool XJsonPosix::get(const UString& _Key, double& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Key);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Double && vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = vJsonStruct->valuedouble;
	return true;
}



// [set] 添加子项 Object
bool XJsonPosix::append(const UString& _Key, const XJsonPosix& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}

	auto		vJsonStruct = XJson_parse(_Value.toString().data());
	if(nullptr == vJsonStruct)
	{
		///_ThisErrorMessage = UString("prase json name error at ") + XJson_GetErrorPtr();
		_ThisErrorMessage = "prase json name error";
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	auto		vIterator = _ThisJsonObjectRef->find(_Key);
	if(vIterator != _ThisJsonObjectRef->end())
	{
		if(vIterator->second)
		{
			delete (vIterator->second);
			vIterator->second = nullptr;
		}
		_ThisJsonObjectRef->erase(vIterator);
	}
	return true;
}

// [set] 添加子项 String
bool XJsonPosix::append(const UString& _Key, const UString& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateString(_Value.data());
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 添加子项 XByteArray
bool XJsonPosix::append(const UString& _Key, const XByteArray& _Value) noexcept
{
	return this->append(_Key, _Value.exist() ? UString(_Value.data(), _Value.size()) : UString());
}

// [set] 添加子项 XString
bool XJsonPosix::append(const UString& _Key, const XString& _Value) noexcept
{
	return this->append(_Key, _Value.toBytes());
}

// [set] 添加子项 Bool
bool XJsonPosix::append(const UString& _Key, bool _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateBool(_Value);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 添加子项 char
bool XJsonPosix::append(const UString& _Key, char _Value) noexcept
{
	return this->append(_Key, static_cast<long long>(_Value));
}

// [set] 添加子项 unsigned char
bool XJsonPosix::append(const UString& _Key, unsigned char _Value) noexcept
{
	return this->append(_Key, static_cast<unsigned long long>(_Value));
}

// [set] 添加子项 short
bool XJsonPosix::append(const UString& _Key, short _Value) noexcept
{
	return this->append(_Key, static_cast<long long>(_Value));
}

// [set] 添加子项 unsigned short
bool XJsonPosix::append(const UString& _Key, unsigned short _Value) noexcept
{
	return this->append(_Key, static_cast<unsigned long long>(_Value));
}

// [set] 添加子项 int
bool XJsonPosix::append(const UString& _Key, int _Value) noexcept
{
	return this->append(_Key, static_cast<long long>(_Value));
}

// [set] 添加子项 unsigned int
bool XJsonPosix::append(const UString& _Key, unsigned int _Value) noexcept
{
	return this->append(_Key, static_cast<unsigned long long>(_Value));
}

// [set] 添加子项 long
bool XJsonPosix::append(const UString& _Key, long _Value) noexcept
{
	return this->append(_Key, static_cast<long long>(_Value));
}

// [set] 添加子项 unsigned long
bool XJsonPosix::append(const UString& _Key, unsigned long _Value) noexcept
{
	return this->append(_Key, static_cast<unsigned long long>(_Value));
}

// [set] 添加子项 long long
bool XJsonPosix::append(const UString& _Key, long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 添加子项 unsigned long long
bool XJsonPosix::append(const UString& _Key, unsigned long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt(_Value, 1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 添加子项 float
bool XJsonPosix::append(const UString& _Key, float _Value) noexcept
{
	return this->append(_Key, static_cast<double>(_Value));
}

// [set] 添加子项 double
bool XJsonPosix::append(const UString& _Key, double _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateObject();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_AddItemToObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}



// [set] 移除子项
bool XJsonPosix::remove(const UString& _Key) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	XJson_DeleteItemFromObject(vFocusData, _Key.data());
	auto		vIterator = _ThisJsonObjectRef->find(_Key);
	if(vIterator != _ThisJsonObjectRef->end())
	{
		if(vIterator->second)
		{
			delete (vIterator->second);
			vIterator->second = nullptr;
		}
		_ThisJsonObjectRef->erase(vIterator);
	}
	return true;
}



// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, const XJsonPosix& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_parse(_Value.toString().data());
	if(nullptr == vJsonStruct)
	{
		// _ThisErrorMessage = UString("prase json name error at ") + XJson_GetErrorPtr();
		_ThisErrorMessage = "prase json name error at ";
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	auto		vIterator = _ThisJsonObjectRef->find(_Key);
	if(vIterator != _ThisJsonObjectRef->end())
	{
		if(vIterator->second)
		{
			delete (vIterator->second);
			vIterator->second = nullptr;
		}
		_ThisJsonObjectRef->erase(vIterator);
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, const UString& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateString(_Value.data());
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, const XByteArray& _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, const XString& _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, bool _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateBool(_Value);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, char _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, unsigned char _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, short _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, unsigned short _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, int _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, unsigned int _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, 1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, long _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, unsigned long _Value) noexcept
{
	this->remove(_Key);
	return this->append(_Key, _Value);
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, unsigned long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, 1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, float _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(const UString& _Key, double _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Object)
	{
		_ThisErrorMessage = "not a json object! json array?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInObject(vFocusData, _Key.data(), vJsonStruct);
	if(nullptr == XJson_GetObjectItem(vFocusData, _Key.data()))
	{
		return false;
	}
	return true;
}



// [get] 获取子项 Object
XJsonPosix XJsonPosix::toObject(const UString& _Key) const noexcept
{
	auto		vValue = XJsonPosix();
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 String
UString XJsonPosix::toString(const UString& _Key) const noexcept
{
	auto		vValue = UString("");
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 Bytes
XByteArray XJsonPosix::toBytes(const UString& _Key) const noexcept
{
	auto		vValue = UString("");
	get(_Key, vValue);
	return XByteArray(vValue.data(), vValue.size());
}

// [get] 获取子项 XString
XString XJsonPosix::toXString(const UString& _Key) const noexcept
{
	auto		vValue = UString("");
	get(_Key, vValue);
	return XString::fromUString(vValue);
}

// [get] 获取子项 bool
bool XJsonPosix::toBool(const UString& _Key) const noexcept
{
	auto		vValue = false;
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 char
char XJsonPosix::toChar(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<char>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 unsigned char
unsigned char XJsonPosix::toUChar(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<unsigned char>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 short
short XJsonPosix::toShort(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<short>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 unsigned short
unsigned short XJsonPosix::toUShort(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<unsigned short>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 int
int XJsonPosix::toInt(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<int>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 unsigned int
unsigned int XJsonPosix::toUInt(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<unsigned int>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 long
long XJsonPosix::toLong(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<long>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 unsigned long
unsigned long XJsonPosix::toULong(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<unsigned long>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 long long
long long XJsonPosix::toLLong(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<long long>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 unsigned long long
unsigned long long XJsonPosix::toULLong(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<unsigned long long>(0);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 float
float XJsonPosix::toFloat(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<float>(0.0f);
	get(_Key, vValue);
	return vValue;
}

// [get] 获取子项 double
double XJsonPosix::toDouble(const UString& _Key) const noexcept
{
	auto		vValue = static_cast<double>(0.0f);
	get(_Key, vValue);
	return vValue;
}



// [get] 获取数组大小
int XJsonPosix::arraySize() const noexcept
{
	if(_ThisJsonData)
	{
		if(_ThisJsonData->type == XJson_Array)
		{
			return(XJson_GetArraySize(_ThisJsonData));
		}
	}
	else if(_ThisExternJsonDataRef)
	{
		if(_ThisExternJsonDataRef->type == XJson_Array)
		{
			return(XJson_GetArraySize(_ThisExternJsonDataRef));
		}
	}
	return 0;
}



// [get] 根据下标获取子项
XCC_JSON_INFO* XJsonPosix::_get(int _Index) const noexcept
{
	auto		vJsonStruct = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		if(_ThisJsonData->type == XJson_Array)
		{
			vJsonStruct = XJson_GetArrayItem(_ThisJsonData, _Index);
		}
	}
	else if(_ThisExternJsonDataRef)
	{
		if(_ThisExternJsonDataRef->type == XJson_Array)
		{
			vJsonStruct = XJson_GetArrayItem(_ThisExternJsonDataRef, _Index);
		}
	}
	return vJsonStruct;
}



// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, XJsonPosix& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vJsonString = XJson_Print(vJsonStruct);
	auto		vJsonData = UString(vJsonString);
	x_posix_free(vJsonString);
	if(_Value.parse(vJsonData))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, UString& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_String)
	{
		return false;
	}
	_Value = vJsonStruct->valuestring;
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, XByteArray& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_String)
	{
		return false;
	}
	_Value = vJsonStruct->valuestring;
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, XString& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_String)
	{
		return false;
	}
	_Value = XString::fromUString(vJsonStruct->valuestring);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, bool& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type > XJson_True)
	{
		return false;
	}
	_Value = vJsonStruct->type ? true : false;
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, char& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (char)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, unsigned char& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned char)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, short& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (short)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, unsigned short& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned short)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, int& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (int)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, unsigned int& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned int)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (long)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, unsigned long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned long)(vJsonStruct->valueint);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, long long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (long long)vJsonStruct->valueint;
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, unsigned long long& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Int)
	{
		return false;
	}
	_Value = (unsigned long long)vJsonStruct->valueint;
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, float& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Double)
	{
		return false;
	}
	_Value = (float)(vJsonStruct->valuedouble);
	return true;
}

// [get] 根据下标获取子项
bool XJsonPosix::get(int _Index, double& _Value) const noexcept
{
	auto		vJsonStruct = this->_get(_Index);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	if(vJsonStruct->type != XJson_Double)
	{
		return false;
	}
	_Value = vJsonStruct->valuedouble;
	return true;
}



bool XJsonPosix::prepend(const XJsonPosix& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_parse(_Value.toString().data());
	if(nullptr == vJsonStruct)
	{
		// _ThisErrorMessage = UString("prase json name error at ") + XJson_GetErrorPtr();
		_ThisErrorMessage = "prase json name error at ";
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArrayHead(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	for(auto vIterator = _ThisJsonArrayRef->begin(); vIterator != _ThisJsonArrayRef->end();)
	{
		if(vIterator->second)
		{
			delete (vIterator->second);
			vIterator->second = nullptr;
		}
		_ThisJsonArrayRef->erase(vIterator++);
	}
	return true;
}

bool XJsonPosix::prepend(const UString& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateString(_Value.data());
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArrayHead(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(const XByteArray& _Value) noexcept
{
	return this->prepend(_Value.exist() ? UString(_Value.data(), _Value.size()) : UString());
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(const XString& _Value) noexcept
{
	return this->prepend(_Value.toBytes());
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(bool _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateBool(_Value);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArrayHead(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(char _Value) noexcept
{
	return this->prepend(static_cast<long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(unsigned char _Value) noexcept
{
	return this->prepend(static_cast<unsigned long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(short _Value) noexcept
{
	return this->prepend(static_cast<long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(unsigned short _Value) noexcept
{
	return this->prepend(static_cast<unsigned long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(int _Value) noexcept
{
	return this->prepend(static_cast<long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(unsigned int _Value) noexcept
{
	return this->prepend(static_cast<unsigned long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(long _Value) noexcept
{
	return this->prepend(static_cast<long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(unsigned long _Value) noexcept
{
	return this->prepend(static_cast<unsigned long long>(_Value));
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArrayHead(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(unsigned long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArrayHead(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(float _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArrayHead(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组开头添加子项
bool XJsonPosix::prepend(double _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArrayHead(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}



// [set] 在数组结尾添加子项
bool XJsonPosix::append(const XJsonPosix& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_parse(_Value.toString().data());
	if(nullptr == vJsonStruct)
	{
		// _ThisErrorMessage = UString("prase json name error at ") + XJson_GetErrorPtr();
		_ThisErrorMessage = "prase json name error";
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArray(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	auto		vLastIndex = (unsigned int)XJson_GetArraySize(vFocusData) - 1;
	for(auto vIterator = _ThisJsonArrayRef->begin(); vIterator != _ThisJsonArrayRef->end();)
	{
		if(vIterator->first >= vLastIndex)
		{
			if(vIterator->second)
			{
				delete (vIterator->second);
				vIterator->second = nullptr;
			}
			_ThisJsonArrayRef->erase(vIterator++);
		}
		else
		{
			vIterator++;
		}
	}
	return true;
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(const UString& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateString(_Value.data());
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArray(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(const XByteArray& _Value) noexcept
{
	return this->append(_Value.exist() ? UString(_Value.data(), _Value.size()) : UString());
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(const XString& _Value) noexcept
{
	return this->append(_Value.toBytes());
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(bool _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateBool(_Value);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArray(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(char _Value) noexcept
{
	return this->append(static_cast<long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(unsigned char _Value) noexcept
{
	return this->append(static_cast<unsigned long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(short _Value) noexcept
{
	return this->append(static_cast<long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(unsigned short _Value) noexcept
{
	return this->append(static_cast<unsigned long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(int _Value) noexcept
{
	return this->append(static_cast<long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(unsigned int _Value) noexcept
{
	return this->append(static_cast<unsigned long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(long _Value) noexcept
{
	return this->append(static_cast<long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(unsigned long _Value) noexcept
{
	return this->append(static_cast<unsigned long long>(_Value));
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArray(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(unsigned long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, 1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArray(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(float _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArray(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}

// [set] 在数组结尾添加子项
bool XJsonPosix::append(double _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(_ThisJsonData)
	{
		vFocusData = _ThisJsonData;
	}
	else if(_ThisExternJsonDataRef)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		_ThisJsonData = XJson_CreateArray();
		vFocusData = _ThisJsonData;
	}

	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	auto		vArraySizeBeforeAdd = XJson_GetArraySize(vFocusData);
	XJson_AddItemToArray(vFocusData, vJsonStruct);
	auto		vArraySizeAfterAdd = XJson_GetArraySize(vFocusData);
	if(vArraySizeAfterAdd == vArraySizeBeforeAdd)
	{
		return false;
	}
	return true;
}



// [set] 移除子项
bool XJsonPosix::remove(int _Index) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	XJson_DeleteItemFromArray(vFocusData, _Index);
	for(auto vIterator = _ThisJsonArrayRef->begin(); vIterator != _ThisJsonArrayRef->end();)
	{
		if(vIterator->first >= (unsigned int)_Index)
		{
			if(vIterator->second)
			{
				delete (vIterator->second);
				vIterator->second = nullptr;
			}
			_ThisJsonArrayRef->erase(vIterator++);
		}
		else
		{
			vIterator++;
		}
	}
	return true;
}



// [set] 替换子项
bool XJsonPosix::replace(int _Index, const XJsonPosix& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_parse(_Value.toString().data());
	if(nullptr == vJsonStruct)
	{
		// _ThisErrorMessage = UString("prase json name error at ") + XJson_GetErrorPtr();
		return false;
	}
	XJson_ReplaceItemInArray(vFocusData, _Index, vJsonStruct);
	if(nullptr == XJson_GetArrayItem(vFocusData, _Index))
	{
		return false;
	}
	auto		vIterator = _ThisJsonArrayRef->find(_Index);
	if(vIterator != _ThisJsonArrayRef->end())
	{
		if(vIterator->second)
		{
			delete (vIterator->second);
			vIterator->second = nullptr;
		}
		_ThisJsonArrayRef->erase(vIterator);
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, const UString& _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateString(_Value.data());
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInArray(vFocusData, _Index, vJsonStruct);
	if(XJson_GetArrayItem(vFocusData, _Index) == NULL)
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, const XByteArray& _Value) noexcept
{
	return this->replace(_Index, _Value.exist() ? UString(_Value.data(), _Value.size()) : UString());
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, const XString& _Value) noexcept
{
	return this->replace(_Index, _Value.toBytes());
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, bool _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateBool(_Value);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInArray(vFocusData, _Index, vJsonStruct);
	if(XJson_GetArrayItem(vFocusData, _Index) == nullptr)
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, char _Value) noexcept
{
	return this->replace(_Index, static_cast<long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, unsigned char _Value) noexcept
{
	return this->replace(_Index, static_cast<unsigned long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, short _Value) noexcept
{
	return this->replace(_Index, static_cast<long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, unsigned short _Value) noexcept
{
	return this->replace(_Index, static_cast<unsigned long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, int _Value) noexcept
{
	return this->replace(_Index, static_cast<long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, unsigned int _Value) noexcept
{
	return this->replace(_Index, static_cast<unsigned long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, long _Value) noexcept
{
	return this->replace(_Index, static_cast<long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, unsigned long _Value) noexcept
{
	return this->replace(_Index, static_cast<unsigned long long>(_Value));
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)((unsigned long long)_Value), -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInArray(vFocusData, _Index, vJsonStruct);
	if(XJson_GetArrayItem(vFocusData, _Index) == NULL)
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, unsigned long long _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateInt((unsigned long long)_Value, 1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInArray(vFocusData, _Index, vJsonStruct);
	if(XJson_GetArrayItem(vFocusData, _Index) == NULL)
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, float _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInArray(vFocusData, _Index, vJsonStruct);
	if(nullptr == XJson_GetArrayItem(vFocusData, _Index))
	{
		return false;
	}
	return true;
}

// [set] 替换子项
bool XJsonPosix::replace(int _Index, double _Value) noexcept
{
	auto		vFocusData = static_cast<XCC_JSON_INFO*>(nullptr);
	if(nullptr == _ThisJsonData)
	{
		vFocusData = _ThisExternJsonDataRef;
	}
	else
	{
		vFocusData = _ThisJsonData;
	}
	if(nullptr == vFocusData)
	{
		_ThisErrorMessage = "json data is null!";
		return false;
	}
	if(vFocusData->type != XJson_Array)
	{
		_ThisErrorMessage = "not a json array! json object?";
		return false;
	}
	auto		vJsonStruct = XJson_CreateDouble((double)_Value, -1);
	if(nullptr == vJsonStruct)
	{
		return false;
	}
	XJson_ReplaceItemInArray(vFocusData, _Index, vJsonStruct);
	if(nullptr == XJson_GetArrayItem(vFocusData, _Index))
	{
		return false;
	}
	return true;
}
