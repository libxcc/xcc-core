#include <source/serialize/XJsonPrivate.h>


// constructor
XJsonValue::XJsonValue() noexcept
{
	memberData = XJsonPrivate::newValue();
}

// constructor
XJsonValue::XJsonValue(XJsonValuePrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newValue();
}

// constructor
XJsonValue::XJsonValue(const XJsonValue& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonValue::XJsonValue(XJsonValue&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonValue::~XJsonValue() noexcept
{
	XJsonPrivate::free(memberData);
}



// constructor
XJsonValue::XJsonValue(bool _Status) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Boolean;
	memberData->value.v_boolean = _Status;
}

// constructor
XJsonValue::XJsonValue(x_int8_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = true;
	memberData->sign.s_integer = true;
	memberData->value.v_int64 = _Number;
}

// constructor
XJsonValue::XJsonValue(x_uint8_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = true;
	memberData->value.v_uint64 = _Number;
}

// constructor
XJsonValue::XJsonValue(x_int16_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = true;
	memberData->sign.s_integer = true;
	memberData->value.v_int64 = _Number;
}

// constructor
XJsonValue::XJsonValue(x_uint16_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = true;
	memberData->value.v_uint64 = _Number;
}

// constructor
XJsonValue::XJsonValue(x_int32_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = true;
	memberData->sign.s_integer = true;
	memberData->value.v_int64 = _Number;
}

// constructor
XJsonValue::XJsonValue(x_uint32_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = true;
	memberData->value.v_uint64 = _Number;
}

// constructor
XJsonValue::XJsonValue(long _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = true;
	memberData->sign.s_integer = true;
	memberData->value.v_int64 = _Number;
}

// constructor
XJsonValue::XJsonValue(unsigned long _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = true;
	memberData->value.v_uint64 = _Number;
}

// constructor
XJsonValue::XJsonValue(x_int64_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = true;
	memberData->sign.s_integer = true;
	memberData->value.v_int64 = _Number;
}

// constructor
XJsonValue::XJsonValue(x_uint64_t _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = true;
	memberData->value.v_uint64 = _Number;
}

// constructor
XJsonValue::XJsonValue(float _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = true;
	memberData->sign.s_integer = false;
	memberData->value.v_double = _Number;
}

// constructor
XJsonValue::XJsonValue(double _Number) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Number;
	memberData->sign.s_signed = true;
	memberData->sign.s_integer = false;
	memberData->value.v_double = _Number;
}

// constructor
XJsonValue::XJsonValue(const char* _String) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::String;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = false;
	memberData->value.v_string = new(std::nothrow) XString(_String);
}

// constructor
XJsonValue::XJsonValue(const XString& _String) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::String;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = false;
	memberData->value.v_string = new(std::nothrow) XString(_String);
}

// constructor
XJsonValue::XJsonValue(const XJsonArray& _JsonArray) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Array;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = false;
	memberData->value.v_array = new(std::nothrow) XJsonArray(_JsonArray);
}

// constructor
XJsonValue::XJsonValue(const XJsonObject& _JsonObject) noexcept
{
	memberData = XJsonPrivate::newValue();
	memberData->type = XJsonValue::Object;
	memberData->sign.s_signed = false;
	memberData->sign.s_integer = false;
	memberData->value.v_object = new(std::nothrow) XJsonObject(_JsonObject);
}



// operator overload =
XJsonValue& XJsonValue::operator = (const XJsonValue& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator overload =
XJsonValue& XJsonValue::operator = (XJsonValue&& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = _Object.memberData;
		_Object.memberData = nullptr;
	}
	return *this;
}



// 设置为空值
void XJsonValue::clear() noexcept
{
	XJsonPrivate::free(memberData);
	memberData = XJsonPrivate::newValue();
}



// 创建一个空值
XJsonValue XJsonValue::null() noexcept
{
	return {};
}

// 创建一个布尔值
XJsonValue XJsonValue::boolean(bool _Status) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::Boolean;
	vJsonValue.memberData->value.v_boolean = _Status;

	return vJsonValue;
}

// 创建一个整型
XJsonValue XJsonValue::number(x_int8_t _Number) noexcept
{
	return XJsonValue::number((x_int64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(x_uint8_t _Number) noexcept
{
	return XJsonValue::number((x_uint64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(x_int16_t _Number) noexcept
{
	return XJsonValue::number((x_int64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(x_uint16_t _Number) noexcept
{
	return XJsonValue::number((x_uint64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(x_int32_t _Number) noexcept
{
	return XJsonValue::number((x_int64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(x_uint32_t _Number) noexcept
{
	return XJsonValue::number((x_uint64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(long _Number) noexcept
{
	return XJsonValue::number((x_int64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(unsigned long _Number) noexcept
{
	return XJsonValue::number((x_uint64_t)_Number);
}

// 创建一个整型
XJsonValue XJsonValue::number(x_int64_t _Number) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::Number;
	vJsonValue.memberData->sign.s_signed = true;
	vJsonValue.memberData->sign.s_integer = true;
	vJsonValue.memberData->value.v_int64 = _Number;

	return vJsonValue;
}

// 创建一个整型
XJsonValue XJsonValue::number(x_uint64_t _Number) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::Number;
	vJsonValue.memberData->sign.s_signed = false;
	vJsonValue.memberData->sign.s_integer = true;
	vJsonValue.memberData->value.v_uint64 = _Number;

	return vJsonValue;
}

// 创建一个单精度浮点数
XJsonValue XJsonValue::number(float _Number) noexcept
{
	return XJsonValue::number((double)_Number);
}

// 创建一个双精度浮点数
XJsonValue XJsonValue::number(double _Number) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::Number;
	vJsonValue.memberData->sign.s_signed = true;
	vJsonValue.memberData->sign.s_integer = false;
	vJsonValue.memberData->value.v_double = _Number;

	return vJsonValue;
}

// 创建一个字符串
XJsonValue XJsonValue::string(const char* _String) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::String;
	vJsonValue.memberData->sign.s_signed = false;
	vJsonValue.memberData->sign.s_integer = false;
	vJsonValue.memberData->value.v_string = new(std::nothrow) XString(_String);

	return vJsonValue;
}

// 创建一个字符串
XJsonValue XJsonValue::string(const XString& _String) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::String;
	vJsonValue.memberData->sign.s_signed = false;
	vJsonValue.memberData->sign.s_integer = false;
	vJsonValue.memberData->value.v_string = new(std::nothrow) XString(_String);

	return vJsonValue;
}

// 创建一个数组
XJsonValue XJsonValue::array(const XJsonArray& _JsonArray) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::Array;
	vJsonValue.memberData->sign.s_signed = false;
	vJsonValue.memberData->sign.s_integer = false;
	vJsonValue.memberData->value.v_array = new(std::nothrow) XJsonArray(_JsonArray);

	return vJsonValue;
}

// 创建一个对象
XJsonValue XJsonValue::object(const XJsonObject& _JsonObject) noexcept
{
	XJsonValue	vJsonValue;

	vJsonValue.memberData->type = XJsonValue::Object;
	vJsonValue.memberData->sign.s_signed = false;
	vJsonValue.memberData->sign.s_integer = false;
	vJsonValue.memberData->value.v_object = new(std::nothrow) XJsonObject(_JsonObject);

	return vJsonValue;
}



// 当前值类型
XJsonValue::Type XJsonValue::type() const noexcept
{
	return memberData->type;
}



// 检查是否为空值
bool XJsonValue::isNull() const noexcept
{
	return memberData->type == XJsonValue::Null;
}

// 检查是否为布尔值
bool XJsonValue::isBoolean() const noexcept
{
	return memberData->type == XJsonValue::Boolean;
}

// 检查是否为数值
bool XJsonValue::isNumber() const noexcept
{
	return memberData->type == XJsonValue::Number;
}

// 检查是否为整型
bool XJsonValue::isInteger() const noexcept
{
	return memberData->type == XJsonValue::Number && memberData->sign.s_integer;
}

// 检查是否为浮点数
bool XJsonValue::isDouble() const noexcept
{
	return memberData->type == XJsonValue::Number && !(memberData->sign.s_integer);
}

// 检查是否为字符串
bool XJsonValue::isString() const noexcept
{
	return memberData->type == XJsonValue::String;
}

// 检查是否为数组
bool XJsonValue::isArray() const noexcept
{
	return memberData->type == XJsonValue::Array;
}

// 检查是否为对象
bool XJsonValue::isObject() const noexcept
{
	return memberData->type == XJsonValue::Object;
}



// 转换为布尔值
bool XJsonValue::toBoolean() const noexcept
{
	if(memberData->type == XJsonValue::Boolean)
	{
		return memberData->value.v_boolean;
	}
	return false;
}

// 转换为有符号整型
x_int64_t XJsonValue::toLLong() const noexcept
{
	if(memberData->type == XJsonValue::Number)
	{
		return memberData->value.v_int64;
	}
	return 0;
}

// 转换为无符号整型
x_uint64_t XJsonValue::toULLong() const noexcept
{
	if(memberData->type == XJsonValue::Number)
	{
		return memberData->value.v_uint64;
	}
	return 0;
}

// 转换为单精度浮点数
float XJsonValue::toFloat() const noexcept
{
	if(memberData->type == XJsonValue::Number)
	{
		return (float)memberData->value.v_double;
	}
	return 0.0f;
}

// 转换为双精度浮点数
double XJsonValue::toDouble() const noexcept
{
	if(memberData->type == XJsonValue::Number)
	{
		return memberData->value.v_double;
	}
	return 0.0f;
}

// 转换为字符串
XString XJsonValue::toString() const noexcept
{
	if(memberData->type == XJsonValue::String)
	{
		return *(memberData->value.v_string);
	}
	return {};
}

// 转换为数组
XJsonArray XJsonValue::toArray() const noexcept
{
	if(memberData->type == XJsonValue::Array)
	{
		return *(memberData->value.v_array);
	}
	return {};
}

// 转换为对象
XJsonObject XJsonValue::toObject() const noexcept
{
	if(memberData->type == XJsonValue::Object)
	{
		return *(memberData->value.v_object);
	}
	return {};
}
