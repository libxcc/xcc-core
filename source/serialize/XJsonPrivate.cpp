#include <source/serialize/XJsonPrivate.h>


// 创建数组
XJsonArrayPrivate* XJsonPrivate::newArray() noexcept
{
	return new(std::nothrow) XJsonArrayPrivate();
}

// 创建数组迭代器
XJsonArrayIterPrivate* XJsonPrivate::newArrayIter(XJsonArrayPrivate* _Array, x_size_t _Index) noexcept
{
	auto		vPrivate = new(std::nothrow) XJsonArrayIterPrivate();
	if(vPrivate)
	{
		vPrivate->array = _Array;
		if(_Array)
		{
			if(_Index >= _Array->data.size())
			{
				vPrivate->iter = vPrivate->array->data.end();
			}
			else
			{
				vPrivate->iter = vPrivate->array->data.begin() + (x_ssize_t)_Index;
			}
		}

	}
	return vPrivate;
}

// 创建对象
XJsonObjectPrivate* XJsonPrivate::newObject() noexcept
{
	return new(std::nothrow) XJsonObjectPrivate();
}

// 创建对象迭代器
XJsonObjectIterPrivate* XJsonPrivate::newObjectIter(XJsonObjectPrivate* _Object, const XString& _Key) noexcept
{
	auto		vPrivate = new(std::nothrow) XJsonObjectIterPrivate();
	if(vPrivate)
	{
		vPrivate->object = _Object;
		if(vPrivate->object)
		{
			if(_Key.exist())
			{
				vPrivate->iter = vPrivate->object->find(_Key);
			}
			else
			{
				vPrivate->iter = vPrivate->object->end();
			}
		}
	}
	return vPrivate;
}

// 创建值
XJsonValuePrivate* XJsonPrivate::newValue() noexcept
{
	return new(std::nothrow) XJsonValuePrivate();
}



// 释放数据
void XJsonPrivate::free(XJsonArrayPrivate* _Object) noexcept
{
	delete _Object;
}

// 释放数据
void XJsonPrivate::free(XJsonArrayIterPrivate* _Object) noexcept
{
	delete _Object;
}

// 释放数据
void XJsonPrivate::free(XJsonObjectPrivate* _Object) noexcept
{
	delete _Object;
}

// 释放数据
void XJsonPrivate::free(XJsonObjectIterPrivate* _Object) noexcept
{
	delete _Object;
}

// 释放数据
void XJsonPrivate::free(XJsonValuePrivate* _Object) noexcept
{
	if(_Object == nullptr)
	{
		return;
	}

	switch (_Object->type)
	{
		case XJsonValue::String:
			delete _Object->value.v_string;
			break;
		case XJsonValue::Array:
			delete _Object->value.v_array;
			break;
		case XJsonValue::Object:
			delete _Object->value.v_object;
			break;
		default:
			break;
	}
	delete _Object;
}

// 释放数据
void XJsonPrivate::free(const char* _Object) noexcept
{
	delete[] _Object;
}



// 复制数据
XJsonArrayPrivate* XJsonPrivate::clone(XJsonArrayPrivate* _Src) noexcept
{
	auto		vClone = XJsonPrivate::newArray();
	if(vClone)
	{
		try
		{
			vClone->data = _Src->data;
		}
		catch(...)
		{
			XLOG_ERROR(XCC_CORE_LOG_TAG, u8"[ %s : %d ] cannot allocate memory", __XFUNCSIG__, __XLINE__);
		}
	}
	return vClone;
}

// 复制数据
XJsonArrayIterPrivate* XJsonPrivate::clone(XJsonArrayIterPrivate* _Src) noexcept
{
	auto		vClone = new(std::nothrow) XJsonArrayIterPrivate();
	if(vClone)
	{
		vClone->array = _Src->array;
		vClone->iter = _Src->iter;
	}
	return vClone;
}

// 复制数据
XJsonObjectPrivate* XJsonPrivate::clone(XJsonObjectPrivate* _Src) noexcept
{
	auto		vClone = XJsonPrivate::newObject();
	if(vClone)
	{
		try
		{
			vClone->data = _Src->data;
		}
		catch(...)
		{
			XLOG_ERROR(XCC_CORE_LOG_TAG, u8"[ %s : %d ] cannot allocate memory", __XFUNCSIG__, __XLINE__);
		}
	}
	return vClone;
}

// 复制数据
XJsonObjectIterPrivate* XJsonPrivate::clone(XJsonObjectIterPrivate* _Src) noexcept
{
	auto		vClone = new(std::nothrow) XJsonObjectIterPrivate();
	if(vClone)
	{
		vClone->object = _Src->object;
		vClone->iter = _Src->iter;
	}
	return vClone;
}

// 复制数据
XJsonValuePrivate* XJsonPrivate::clone(XJsonValuePrivate* _Src) noexcept
{
	auto		vClone = XJsonPrivate::newValue();
	if(vClone)
	{
		x_posix_memcpy(vClone, _Src, sizeof(XJsonValuePrivate));
		switch (_Src->type)
		{
			case XJsonValue::String:
				vClone->value.v_string = new(std::nothrow) XString(*(_Src->value.v_string));
				break;
			case XJsonValue::Array:
				vClone->value.v_array = new(std::nothrow) XJsonArray(*(_Src->value.v_array));
				break;
			case XJsonValue::Object:
				vClone->value.v_object = new(std::nothrow) XJsonObject(*(_Src->value.v_object));
				break;
			default:
				break;
		}
	}
	return vClone;
}



// 预计长度 - 新行
x_size_t XJsonPrivate::calc_length_new_line(XJsonDocument::JsonFormat _JsonFormat, x_size_t _Layer) noexcept
{
	if(_JsonFormat == XJsonDocument::Indented)
	{
		// \r\n + TAB
		return 2 + _Layer;
	}
	return 0;
}

// 预计长度
x_size_t XJsonPrivate::calc_length(x_int64_t _Number) noexcept
{
	auto		vFormat = XString::format("%llu", _Number);
	return vFormat.size();
}

// 预计长度
x_size_t XJsonPrivate::calc_length(x_uint64_t _Number) noexcept
{
	auto		vFormat = XString::format("%llu", _Number);
	return vFormat.size();
}

// 预计长度
x_size_t XJsonPrivate::calc_length(double _Number) noexcept
{
	XString		vFormat;

	if(fabs(floor(_Number) - _Number) <= DBL_EPSILON)
	{
		vFormat = XString::format("%.0f", _Number);
	}
	else if(fabs(_Number) < 1.0e-6 || fabs(_Number) > 1.0e9)
	{
		vFormat = XString::format("%lf", _Number);
	}
	else
	{
		vFormat = XString::format("%f", _Number);
	}
	return vFormat.size();
}

// 预计长度
x_size_t XJsonPrivate::calc_length(XJsonArrayPrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, x_size_t _Layer) noexcept
{
	// [
	x_size_t		vLength = 1;

	for(const auto & vIterator : _Src->data)
	{
		// \n + TAB
		vLength += calc_length_new_line(_JsonFormat, _Layer);

		// Layer >> TAB
		vLength += _JsonFormat == XJsonDocument::Indented ? 1 : 0;

			// Value
		vLength += XJsonPrivate::calc_length(vIterator.memberData, _JsonFormat, _Layer + 1);

		// ,
		++vLength;
	}

	// \n + TAB
	if(!_Src->data.empty())
	{
		vLength += calc_length_new_line(_JsonFormat, _Layer);
	}

	// ]
	++vLength;

	return vLength;
}

// 预计长度
x_size_t XJsonPrivate::calc_length(XJsonObjectPrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, x_size_t _Layer) noexcept
{
	// [
	x_size_t		vLength = 1;

	for(const auto & vIterator : _Src->data)
	{
		// \n + TAB
		vLength += calc_length_new_line(_JsonFormat, _Layer);

		// Layer >> TAB
		vLength += _JsonFormat == XJsonDocument::Indented ? 1 : 0;

		// Key
		vLength += vIterator.first.size() + 2;

		// :
		++vLength;

		// Value
		vLength += XJsonPrivate::calc_length(vIterator.second.memberData, _JsonFormat, _Layer + 1);

		// ,
		++vLength;
	}

	// \n + TAB
	if(!_Src->data.empty())
	{
		vLength += calc_length_new_line(_JsonFormat, _Layer);
	}

	// }
	++vLength;

	return vLength;
}

// 预计长度
x_size_t XJsonPrivate::calc_length(XJsonValuePrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, x_size_t _Layer) noexcept
{
	switch (_Src->type)
	{
		case XJsonValue::Null:
			return 4;
		case XJsonValue::Boolean:
			if(_Src->value.v_boolean)
			{
				return 4;
			}
			else
			{
				return 5;
			}
		case XJsonValue::Number:
			if(_Src->sign.s_integer)
			{
				if(_Src->sign.s_signed)
				{
					return XJsonPrivate::calc_length(_Src->value.v_int64);
				}
				else
				{
					return XJsonPrivate::calc_length(_Src->value.v_uint64);
				}
			}
			else
			{
				return XJsonPrivate::calc_length(_Src->value.v_double);
			}
		case XJsonValue::String:
			return serialize_string_escape(*(_Src->value.v_string)).size() + 2;
		case XJsonValue::Array:
			return XJsonPrivate::calc_length(_Src->value.v_array->memberData, _JsonFormat, _Layer);
		case XJsonValue::Object:
			return XJsonPrivate::calc_length(_Src->value.v_object->memberData, _JsonFormat, _Layer);
		default:
			return 0;
	}
}



// 序列化
bool XJsonPrivate::serialize(XJsonValuePrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, char** _Bytes, x_size_t* _Size) noexcept
{
	if(_Src == nullptr || _Bytes == nullptr || _Size == nullptr)
	{
		return false;
	}

	auto		vStatus = false;
	auto		vLength = XJsonPrivate::calc_length(_Src, _JsonFormat, 0);
	auto		vBuffer = new(std::nothrow) char[vLength + 1];
	if(vBuffer)
	{
		x_posix_memset(vBuffer, 0, vLength + 1);

		auto		vContext = new(std::nothrow) XJsonSerializePrivate();
		if(vContext)
		{
			vContext->data = vBuffer;
			vContext->pos = 0;
			vContext->size = vLength;
			vContext->format = _JsonFormat;

			vStatus = XJsonPrivate::serialize_value(vContext, _Src, 0);
			if(vStatus)
			{
				*_Bytes = vBuffer;
				*_Size = vContext->pos;
				vBuffer[vContext->pos] = 0;
			}

			delete vContext;
		}

		if(!vStatus)
		{
			delete[] vBuffer;
		}
	}

	return vStatus;
}

// 序列化 - 转义
XString XJsonPrivate::serialize_string_escape(const XString& _String) noexcept
{
	auto		vEscape = XString();
	auto		vIndex = XString::pos_type(0);
	auto		vFind = XString::pos_type(0);

	while(true)
	{
		vFind = _String.find('\"', vIndex);
		if(vFind == XString::npos)
		{
			vEscape += _String.mid(vIndex);
			break;
		}
		else
		{
			vEscape += _String.mid(vIndex, vFind - vIndex);
			vEscape += "\\\"";
			vIndex = vFind + 1;
		}
	}
	return vEscape;
}

// 序列化 - 字符
bool XJsonPrivate::serialize_char(XJsonSerializePrivate* _Context, char _Char) noexcept
{
	auto		vData = _Context->data + _Context->pos;
	if(_Context->pos + 1 >= _Context->size)
	{
		return false;
	}
	*vData = _Char;
	++(_Context->pos);
	return true;
}

// 序列化 - 新行
bool XJsonPrivate::serialize_new_line(XJsonSerializePrivate* _Context, x_size_t _Layer) noexcept
{
	if(_Context->format == XJsonDocument::Indented)
	{
		if(!XJsonPrivate::serialize_char(_Context, '\r') || !XJsonPrivate::serialize_char(_Context, '\n'))
		{
			return false;
		}

		if(_Context->pos + _Layer >= _Context->size)
		{
			return false;
		}

		auto		vData = _Context->data + _Context->pos;
		while(_Layer != 0)
		{
			*vData = '\t';

			--_Layer;
			++vData;
			++_Context->pos;
		}
	}
	return true;
}

// 序列化 - 空值
bool XJsonPrivate::serialize_null(XJsonSerializePrivate* _Context) noexcept
{
	auto		vData = _Context->data + _Context->pos;

	if(_Context->pos + 4 >= _Context->size)
	{
		return false;
	}
	x_posix_strncpy(vData, "null", 4);
	_Context->pos += 4;
	return true;
}

// 序列化 - 布尔
bool XJsonPrivate::serialize_boolean(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private) noexcept
{
	auto		vData = _Context->data + _Context->pos;
	if(_Private->value.v_boolean)
	{
		if(_Context->pos + 4 >= _Context->size)
		{
			return false;
		}
		x_posix_strncpy(vData, "true", 4);
		_Context->pos += 4;
	}
	else
	{
		if(_Context->pos + 5 >= _Context->size)
		{
			return false;
		}
		x_posix_strncpy(vData, "false", 5);
		_Context->pos += 5;
	}
	return true;
}

// 序列化 - 数字
bool XJsonPrivate::serialize_number(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private) noexcept
{
	char		vBuffer[64] = {0};
	auto		vData = _Context->data + _Context->pos;
	if(_Private->sign.s_integer)
	{
		// 2^64+1 可以用 21 个字符表示
		if(_Private->sign.s_signed)
		{
			x_posix_sprintf(vBuffer, "%lld", _Private->value.v_int64);
		}
		else
		{
			x_posix_sprintf(vBuffer, "%llu", _Private->value.v_int64);
		}
	}
	else
	{
		auto		vDouble = _Private->value.v_double;
		if(fabs(floor(vDouble) - vDouble) <= DBL_EPSILON)
		{
			x_posix_sprintf(vBuffer, "%.0f", vDouble);
		}
		else if(fabs(vDouble) < 1.0e-6 || fabs(vDouble) > 1.0e9)
		{
			x_posix_sprintf(vBuffer, "%lf", vDouble);
		}
		else
		{
			x_posix_sprintf(vBuffer, "%f", vDouble);
		}
	}

	auto		vLength = x_posix_strlen(vBuffer);
	if(_Context->pos + vLength >= _Context->size)
	{
		return false;
	}

	x_posix_strncpy(vData, vBuffer, vLength);
	_Context->pos += vLength;
	return true;
}

// 序列化 - 字符串
bool XJsonPrivate::serialize_string(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private) noexcept
{
	auto		vString = _Private->value.v_string;
	if(vString == nullptr)
	{
		return false;
	}
	return XJsonPrivate::serialize_string(_Context, *vString);
}

// 序列化 - 字符串
bool XJsonPrivate::serialize_string(XJsonSerializePrivate* _Context, const XString& _String) noexcept
{
	auto		vValue = serialize_string_escape(_String);
	auto		vData = _Context->data + _Context->pos;
	if(_Context->pos + vValue.size() + 2 >= _Context->size)
	{
		return false;
	}

	x_posix_strncpy(vData + 0, "\"", 1);
	x_posix_strncpy(vData + 1, vValue.data(), vValue.size());
	x_posix_strncpy(vData + vValue.size() + 1, "\"", 1);
	_Context->pos += vValue.size() + 2;
	return true;
}

// 序列化 - 值
bool XJsonPrivate::serialize_value(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private, x_size_t _Layer) noexcept
{
	switch (_Private->type)
	{
		case XJsonValue::Null:
			return XJsonPrivate::serialize_null(_Context);
		case XJsonValue::Boolean:
			return XJsonPrivate::serialize_boolean(_Context, _Private);
		case XJsonValue::Number:
			return XJsonPrivate::serialize_number(_Context, _Private);
		case XJsonValue::String:
			return XJsonPrivate::serialize_string(_Context, _Private);
		case XJsonValue::Array:
			return XJsonPrivate::serialize_array(_Context, _Private->value.v_array->memberData, _Layer);
		case XJsonValue::Object:
			return XJsonPrivate::serialize_object(_Context, _Private->value.v_object->memberData, _Layer);
		default:
			return false;
	}
}

// 序列化 - 数组
bool XJsonPrivate::serialize_array(XJsonSerializePrivate* _Context, XJsonArrayPrivate* _Private, x_size_t _Layer) noexcept
{
	// [
	if(!XJsonPrivate::serialize_char(_Context, '['))
	{
		return false;
	}

	// 循环序列化子项
	auto		vSemicolonMark = false;
	for(const auto& vIterator : _Private->data)
	{
		// 根据状态添加 ,
		if(vSemicolonMark)
		{
			// ,
			if(!XJsonPrivate::serialize_char(_Context, ','))
			{
				return false;
			}
		}
		else
		{
			vSemicolonMark = true;
		}

		// \n + Tab
		if(!XJsonPrivate::serialize_new_line(_Context, _Layer + 1))
		{
			return false;
		}

		// 序列化值
		if(!XJsonPrivate::serialize_value(_Context, vIterator.memberData, _Layer + 1))
		{
			return false;
		}
	}

	// \n + Tab
	if(!_Private->data.empty() && !XJsonPrivate::serialize_new_line(_Context, _Layer))
	{
		return false;
	}

	// ]
	if(!XJsonPrivate::serialize_char(_Context, ']'))
	{
		return false;
	}

	return _Context->pos <= _Context->size;
}

// 序列化 - 对象
bool XJsonPrivate::serialize_object(XJsonSerializePrivate* _Context, XJsonObjectPrivate* _Private, x_size_t _Layer) noexcept
{
	// {
	if(!XJsonPrivate::serialize_char(_Context, '{'))
	{
		return false;
	}

	// 循环序列化子项
	auto		vSemicolonMark = false;
	for(const auto& vIterator : _Private->data)
	{
		// 根据状态添加 ,
		if(vSemicolonMark)
		{
			// ,
			if(!XJsonPrivate::serialize_char(_Context, ','))
			{
				return false;
			}
		}
		else
		{
			vSemicolonMark = true;
		}

		// \n + Tab
		if(!XJsonPrivate::serialize_new_line(_Context, _Layer + 1))
		{
			return false;
		}

		// 序列化键
		if(!XJsonPrivate::serialize_string(_Context, vIterator.first))
		{
			return false;
		}

		// :
		if(!XJsonPrivate::serialize_char(_Context, ':'))
		{
			return false;
		}

		// 序列化值
		if(!XJsonPrivate::serialize_value(_Context, vIterator.second.memberData, _Layer + 1))
		{
			return false;
		}
	}

	// \n + Tab
	if(!_Private->data.empty() && !XJsonPrivate::serialize_new_line(_Context, _Layer))
	{
		return false;
	}

	// }
	if(!XJsonPrivate::serialize_char(_Context, '}'))
	{
		return false;
	}

	return _Context->pos <= _Context->size;
}



// 反序列化
bool XJsonPrivate::deserialize(const char* _Bytes, x_size_t _Size, XJsonValuePrivate** _Src) noexcept
{
	if(_Src == nullptr || _Bytes == nullptr)
	{
		return false;
	}

	auto		vStatus = false;
	auto		vContext = new(std::nothrow) XJsonDeserializePrivate();
	if(vContext)
	{
		vContext->data = _Bytes;
		vContext->pos = 0;
		vContext->size = _Size;

		auto		vPrivate = XJsonPrivate::newValue();
		vStatus = XJsonPrivate::deserialize_value(vContext, vPrivate);
		if(vStatus)
		{
			*_Src = vPrivate;
		}
		else
		{
			XJsonPrivate::free(vPrivate);
		}
		delete vContext;
	}
	return vStatus;
}

// 反序列化 - 跳过
bool XJsonPrivate::deserialize_skip(XJsonDeserializePrivate* _Context) noexcept
{
	while(_Context->pos < _Context->size)
	{
		auto		vChar = (unsigned char)_Context->data[_Context->pos];
		if(vChar > 32)
		{
			return true;
		}
		++(_Context->pos);
	}
	return false;
}

// 反序列化 - 数字
bool XJsonPrivate::deserialize_number(XJsonDeserializePrivate* _Context, XJsonValuePrivate* _Private) noexcept
{
	auto		vData = _Context->data + _Context->pos;
	x_uint64_t 	vValueULLong = 0;
	long double	vValueDouble = 0;
	long double	vScale = 0;
	int		vSubScale = 0;
	int		vSignSubScale = 1;

	// 检查符号
	if(*vData == '-')
	{
		_Private->sign.s_signed = true;
		vData++;
	}
	else
	{
		_Private->sign.s_signed = false;
	}

	// 检查是否为0
	if(*vData == '0')
	{
		vData++;
	}

	// 格式化整型
	if(*vData >= '1' && *vData <= '9')
	{
		do
		{
			vValueULLong *= 10;
			vValueULLong += (*vData - '0');
			vValueDouble = (vValueDouble * 10.0) + (*vData - '0');
			++vData;
		}
		while(*vData >= '0' && *vData <= '9'); /* Number? */
	}

	// 格式化浮点数
	if(*vData == '.' && vData[1] >= '0' && vData[1] <= '9')
	{
		vData++;
		do
		{
			vValueDouble = (vValueDouble * 10.0) + (*vData++ - '0'), vScale--;
		}while(*vData >= '0' && *vData <= '9');
	}

	// 检查小数标记
	if(*vData == 'e' || *vData == 'E')
	{
		vData++;
		if(*vData == '+')
		{
			vData++;
		}
		else if(*vData == '-')
		{
			vSignSubScale = -1;
			vData++;
		}
		while(*vData >= '0' && *vData <= '9')
		{
			vSubScale = (vSubScale * 10) + (*vData++ - '0');
		}
	}

	if(vScale == 0 && vSubScale == 0)
	{
		if(_Private->sign.s_signed)
		{
			_Private->value.v_int64 = -(x_int64_t)vValueULLong;
		}
		else
		{
			_Private->value.v_uint64 = vValueULLong;
		}
		_Private->sign.s_integer = true;
	}
	else
	{
		// number = +/- number.fraction * 10^+/- exponent
		if(_Private->sign.s_signed)
		{
			vValueDouble = -1 * vValueDouble * pow(10.0, (double)(vScale + vSubScale * vSignSubScale));
		}
		else
		{
			vValueDouble = vValueDouble * pow(10.0, (double)(vScale + vSubScale * vSignSubScale));
		}
		_Private->value.v_double = (double)vValueDouble;
		_Private->sign.s_integer = false;
	}

	_Context->pos = vData - _Context->data;
	return _Context->pos <= _Context->size;
}

// 反序列化 - 字符串
bool XJsonPrivate::deserialize_string(XJsonDeserializePrivate* _Context, XJsonValuePrivate* _Private) noexcept
{
	auto		vByteCheck = _Context->data + _Context->pos;

	// 检查是否为字符串
	if(*vByteCheck != '\"')
	{
		return false;
	}

	// 查找结尾引号
	x_size_t 	vStrLength = 0;
	while(true)
	{
		++vByteCheck;

		auto		vChar = *vByteCheck;
		if(vChar == 0 || _Context->pos + vStrLength >= _Context->size)
		{
			// 如果找到末尾，则返回失败
			return false;
		}
		else if(vChar == '\"' && *(vByteCheck - 1) != '\\')
		{
			// 找到结尾则跳出
			break;
		}
		++vStrLength;
	}

	// 结束
	auto		vData = _Context->data + _Context->pos + 1;
	*(_Private->value.v_string) = XString(vData, vStrLength);
	_Context->pos += vStrLength;
	_Context->pos += 2;
	return _Context->pos <= _Context->size;
}

// 反序列化 - 数组
bool XJsonPrivate::deserialize_array(XJsonDeserializePrivate* _Context, XJsonArrayPrivate* _Private) noexcept
{
	auto		vData = _Context->data + _Context->pos;

	// 检查格式
	if(*vData != '[')
	{
		return false;
	}
	_Context->pos += 1;

	// 跳过无关数据
	if(!XJsonPrivate::deserialize_skip(_Context))
	{
		return false;
	}

	// 检查数组是否存在内容
	vData = _Context->data + _Context->pos;
	if(*vData == ']')
	{
		_Context->pos += 1;
		return true;
	}

	// 格式化子级
	if(!XJsonPrivate::deserialize_array_item(_Context, _Private))
	{
		return false;
	}

	// 循环格式化剩下子级
	vData = _Context->data + _Context->pos;
	while(*vData == ',')
	{
		++(_Context->pos);

		// 格式化子级
		if(!XJsonPrivate::deserialize_array_item(_Context, _Private))
		{
			return false;
		}

		vData = _Context->data + _Context->pos;
	}

	// 结束
	vData = _Context->data + _Context->pos;
	++(_Context->pos);
	return *vData == ']';
}

// 反序列化 - 数组 - 子项
bool XJsonPrivate::deserialize_array_item(XJsonDeserializePrivate* _Context, XJsonArrayPrivate* _Private) noexcept
{
	// 首选创建一个子级
	auto		vChildFirst = XJsonPrivate::newValue();
	if(vChildFirst == nullptr)
	{
		return false;
	}

	// 格式化子级
	if(!XJsonPrivate::deserialize_value(_Context, vChildFirst) || !XJsonPrivate::deserialize_skip(_Context))
	{
		return false;
	}

	// 加入列表
	_Private->data.emplace_back(vChildFirst);
	return true;
}

// 反序列化 - 对象
bool XJsonPrivate::deserialize_object(XJsonDeserializePrivate* _Context, XJsonObjectPrivate* _Private) noexcept
{
	auto		vData = _Context->data + _Context->pos;

	// 检查格式
	if(*vData != '{')
	{
		return false;
	}
	_Context->pos += 1;

	// 跳过无关数据
	if(!XJsonPrivate::deserialize_skip(_Context))
	{
		return false;
	}

	// 检查对象是否存在内容
	vData = _Context->data + _Context->pos;
	if(*vData == '}')
	{
		_Context->pos += 1;
		return true;
	}

	// 反序列化子级
	if(!XJsonPrivate::deserialize_object_item(_Context, _Private))
	{
		return false;
	}

	// 循环格式化剩下子级
	vData = _Context->data + _Context->pos;
	while(*vData == ',')
	{
		++(_Context->pos);

		// 反序列化子级
		if(!XJsonPrivate::deserialize_object_item(_Context, _Private))
		{
			return false;
		}

		vData = _Context->data + _Context->pos;
	}

	// 结束
	vData = _Context->data + _Context->pos;
	++(_Context->pos);
	return *vData == '}' && _Private->duplicate_check();
}

// 反序列化 - 对象 - 子项
bool XJsonPrivate::deserialize_object_item(XJsonDeserializePrivate* _Context, XJsonObjectPrivate* _Private) noexcept
{
	// 创建子级Key
	auto		vChildKey = XJsonPrivate::newValue();

	// 格式化子级
	if(!XJsonPrivate::deserialize_value(_Context, vChildKey) || !XJsonPrivate::deserialize_skip(_Context))
	{
		XJsonPrivate::free(vChildKey);
		return false;
	}

	// 检查子级格式
	if(vChildKey->type != XJsonValue::String || vChildKey->value.v_string == nullptr || vChildKey->value.v_string->empty())
	{
		XJsonPrivate::free(vChildKey);
		return false;
	}

	// 保存Key并释放子级指针
	auto		vItemKey = *vChildKey->value.v_string;
	XJsonPrivate::free(vChildKey);

	// 跳过无关数据
	if(!XJsonPrivate::deserialize_skip(_Context))
	{
		return false;
	}

	// 检查是否存在分割符
	auto		vData = _Context->data + _Context->pos;
	if(*vData != ':')
	{
		return false;
	}

	// 首选创建一个子级
	auto		vChildValue = XJsonPrivate::newValue();
	if(vChildValue == nullptr)
	{
		return false;
	}

	// 格式化子级
	++(_Context->pos);
	if(!XJsonPrivate::deserialize_skip(_Context) || !XJsonPrivate::deserialize_value(_Context, vChildValue) || !XJsonPrivate::deserialize_skip(_Context))
	{
		XJsonPrivate::free(vChildValue);
		return false;
	}

	// 加入列表
	_Private->insert(vItemKey, XJsonValue(vChildValue));
	return true;
}

// 反序列化 - 值
bool XJsonPrivate::deserialize_value(XJsonDeserializePrivate* _Context, XJsonValuePrivate* _Private) noexcept
{
	if(_Private == nullptr || _Context->pos >= _Context->size)
	{
		return false;
	}

	if(!XJsonPrivate::deserialize_skip(_Context))
	{
		return false;
	}

	auto		vBuffer = _Context->data + _Context->pos;
	if(0 == x_posix_strncmp(vBuffer, "null", 4))
	{
		_Private->type = XJsonValue::Null;
		_Context->pos += 4;
	}
	else if(0 == x_posix_strncmp(vBuffer, "false", 5))
	{
		_Private->type = XJsonValue::Boolean;
		_Private->value.v_boolean = false;
		_Context->pos += 5;
	}
	else if(0 == x_posix_strncmp(vBuffer, "true", 4))
	{
		_Private->type = XJsonValue::Boolean;
		_Private->value.v_boolean = true;
		_Context->pos += 4;
	}
	else if(*vBuffer == '-' || (*vBuffer >= '0' && *vBuffer <= '9'))
	{
		_Private->type = XJsonValue::Number;
		return XJsonPrivate::deserialize_number(_Context, _Private);
	}
	else if(*vBuffer == '\"')
	{
		_Private->type = XJsonValue::String;
		_Private->value.v_string = new(std::nothrow) XString();
		return XJsonPrivate::deserialize_string(_Context, _Private);
	}
	else if(*vBuffer == '[')
	{
		_Private->type = XJsonValue::Array;
		_Private->value.v_array = new(std::nothrow) XJsonArray(XJsonPrivate::newArray());
		return XJsonPrivate::deserialize_array(_Context, _Private->value.v_array->memberData);
	}
	else if(*vBuffer == '{')
	{
		_Private->type = XJsonValue::Object;
		_Private->value.v_object = new(std::nothrow) XJsonObject(XJsonPrivate::newObject());
		return XJsonPrivate::deserialize_object(_Context, _Private->value.v_object->memberData);
	}
	else
	{
		XLOG_WARNING(XCC_CORE_LOG_TAG, u8"[ %s : %d ] Unable to parse Json", __XFUNCSIG__, __XLINE__);
	}

	return true;
}
