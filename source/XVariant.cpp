#include <xcc-core/XVariant.h>


// Structure
XVariant::XVariant() noexcept
{
	this->_variant_type = XVariant::VariantInvalid;
	this->_variant_data._VNull = 0LL;
}

// constructor
XVariant::XVariant(value_type _Type) noexcept
{
	this->_variant_type = _Type;
	this->_variant_data._VNull = 0LL;
}

// constructor
XVariant::XVariant(bool _Value) noexcept
{
	this->_variant_type = VariantBoolean;
	this->_variant_data._VBool = _Value;
}

// constructor
XVariant::XVariant(x_int8_t _Value) noexcept
{
	this->_variant_type = VariantChar;
	this->_variant_data._VInt64S = _Value;
}

// constructor
XVariant::XVariant(x_uint8_t _Value) noexcept
{
	this->_variant_type = VariantUChar;
	this->_variant_data._VInt64U = _Value;
}

// constructor
XVariant::XVariant(x_int16_t _Value) noexcept
{
	this->_variant_type = VariantShort;
	this->_variant_data._VInt64S = _Value;
}

// constructor
XVariant::XVariant(x_uint16_t _Value) noexcept
{
	this->_variant_type = VariantUShort;
	this->_variant_data._VInt64U = _Value;
}

// constructor
XVariant::XVariant(x_int32_t _Value) noexcept
{
	this->_variant_type = VariantInteger;
	this->_variant_data._VInt64S = _Value;
}

// constructor
XVariant::XVariant(x_uint32_t _Value) noexcept
{
	this->_variant_type = VariantUInteger;
	this->_variant_data._VInt64U = _Value;
}

// constructor
XVariant::XVariant(long _Value) noexcept
{
	this->_variant_type = VariantLong;
	this->_variant_data._VInt64S = _Value;
}

// constructor
XVariant::XVariant(unsigned long _Value) noexcept
{
	this->_variant_type = VariantULong;
	this->_variant_data._VInt64U = _Value;
}

// constructor
XVariant::XVariant(x_int64_t _Value) noexcept
{
	this->_variant_type = VariantLLong;
	this->_variant_data._VInt64S = _Value;
}

// constructor
XVariant::XVariant(x_uint64_t _Value) noexcept
{
	this->_variant_type = VariantULLong;
	this->_variant_data._VInt64U = _Value;
}

// constructor
XVariant::XVariant(float _Value) noexcept
{
	this->_variant_type = VariantFloat;
	this->_variant_data._VDouble = _Value;
}

// constructor
XVariant::XVariant(double _Value) noexcept
{
	this->_variant_type = VariantDouble;
	this->_variant_data._VDouble = _Value;
}

// constructor
XVariant::XVariant(const char* _Value) noexcept
{
	this->_variant_data._VNull = 0;
	this->_variant_type = VariantByteArray;
	this->_variant_data._VObject = new(std::nothrow) XByteArray(_Value);
}

// constructor
XVariant::XVariant(const XString& _Value) noexcept
{
	this->_variant_data._VNull = 0;
	this->_variant_type = VariantString;
	this->_variant_data._VObject = new(std::nothrow) XString(_Value);
}

// constructor
XVariant::XVariant(const XByteArray& _Value) noexcept
{
	this->_variant_data._VNull = 0;
	this->_variant_type = VariantByteArray;
	this->_variant_data._VObject = new(std::nothrow) XByteArray(_Value);
}

// constructor
XVariant::XVariant(const XVariant& _Other) noexcept
{
	this->_variant_data._VNull = 0;
	this->_copy(_Other);
}

// constructor
XVariant::XVariant(XVariant&& _Other) noexcept
{
	if(this == &_Other)
	{
		return;
	}
	this->_variant_type = _Other._variant_type;
	this->_variant_data = _Other._variant_data;

	_Other._variant_type = VariantInvalid;
	_Other._variant_data._VObject = nullptr;
}

// destructor
XVariant::~XVariant() noexcept
{
	this->clear();
}





// operator =
XVariant& XVariant::operator = (bool _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantBoolean;
	this->_variant_data._VBool = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_int8_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantChar;
	this->_variant_data._VInt64S = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_uint8_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantUChar;
	this->_variant_data._VInt64U = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_int16_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantShort;
	this->_variant_data._VInt64S = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_uint16_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantUShort;
	this->_variant_data._VInt64U = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_int32_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantInteger;
	this->_variant_data._VInt64S = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_uint32_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantUInteger;
	this->_variant_data._VInt64U = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (long _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantLong;
	this->_variant_data._VInt64S = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (unsigned long _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantULong;
	this->_variant_data._VInt64U = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_int64_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantLLong;
	this->_variant_data._VInt64S = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (x_uint64_t _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantULLong;
	this->_variant_data._VInt64U = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (float _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantFloat;
	this->_variant_data._VDouble = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (double _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantDouble;
	this->_variant_data._VDouble = _Value;
	return *this;
}

// operator =
XVariant& XVariant::operator = (const char* _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantString;
	this->_variant_data._VObject = new(std::nothrow) XString(_Value);
	return *this;
}

// operator =
XVariant& XVariant::operator = (const XString& _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantString;
	this->_variant_data._VObject = new(std::nothrow) XString(_Value);
	return *this;
}

// operator =
XVariant& XVariant::operator = (const XByteArray& _Value) noexcept
{
	this->clear();
	this->_variant_type = VariantByteArray;
	this->_variant_data._VObject = new(std::nothrow) XByteArray(_Value);
	return *this;
}

// operator =
XVariant& XVariant::operator = (const XVariant& _Other) noexcept
{
	if(this == &_Other)
	{
		return *this;
	}
	this->_copy(_Other);
	return *this;
}

// operator =
XVariant& XVariant::operator = (XVariant&& _Other) noexcept
{
	if(this != &_Other)
	{
		auto 		vTempType = this->_variant_type;
		auto 		vTempData = this->_variant_data;

		this->_variant_type = _Other._variant_type;
		this->_variant_data = _Other._variant_data;

		_Other._variant_type = vTempType;
		_Other._variant_data = vTempData;
	}
	return *this;
}





// Data copy
void XVariant::_copy(const XVariant& _Other) noexcept
{
	this->clear();

	this->_variant_type = _Other._variant_type;
	this->_variant_data._VNull = 0LL;
	switch (this->_variant_type)
	{
		case VariantBoolean:
		case VariantChar:
		case VariantUChar:
		case VariantShort:
		case VariantUShort:
		case VariantInteger:
		case VariantUInteger:
		case VariantLong:
		case VariantULong:
		case VariantLLong:
		case VariantULLong:
		case VariantFloat:
		case VariantDouble:
			this->_variant_data = _Other._variant_data;
			break;
		case VariantString:
		{
			this->_variant_data._VObject = new(std::nothrow) XString();
			if(this->_variant_data._VObject)
			{
				*(static_cast<XString*>(this->_variant_data._VObject)) = *(static_cast<XString*>(_Other._variant_data._VObject));
			}
		}
			break;
		case VariantByteArray:
		{
			this->_variant_data._VObject = new(std::nothrow) XByteArray();
			if(this->_variant_data._VObject)
			{
				*(static_cast<XByteArray*>(this->_variant_data._VObject)) = *(static_cast<XByteArray*>(_Other._variant_data._VObject));
			}
		}
			break;
		default:
			this->_variant_data._VNull = 0LL;
			break;
	};
}





// Get the current type
XVariant::value_type XVariant::type() const noexcept
{
	return this->_variant_type;
}

// Clear current content
void XVariant::clear() noexcept
{
	switch (this->_variant_type)
	{
		case VariantString:
		{
			auto		vString = static_cast<XString*>(this->_variant_data._VObject);
			delete vString;
		}
			break;
		case VariantByteArray:
		{
			auto		vBytes = static_cast<XByteArray*>(this->_variant_data._VObject);
			delete vBytes;
		}
			break;
		default:
			this->_variant_data._VNull = 0LL;
			break;
	};
	this->_variant_type = VariantInvalid;
	this->_variant_data._VNull = 0LL;
}





// Convert to bool
bool XVariant::toBool() const noexcept
{
	switch (this->_variant_type)
	{
		case VariantBoolean:
			return this->_variant_data._VBool;
		case VariantChar:
		case VariantUChar:
		case VariantShort:
		case VariantUShort:
		case VariantInteger:
		case VariantUInteger:
		case VariantLong:
		case VariantULong:
		case VariantLLong:
		case VariantULLong:
			return this->_variant_data._VInt64U;
		case VariantFloat:
		case VariantDouble:
			return this->_variant_data._VDouble == 0.0f;
		case VariantString:
			return 0 == this->toString().compare("true", xcc::CaseInsensitive);
		case VariantByteArray:
			return 0 == this->toBytes().compare("true");
		default:
			return this->_variant_data._VNull;
	};
}

// Convert to char
x_int8_t XVariant::toChar() const noexcept
{
	return static_cast<x_int8_t>(this->toLLong());
}

// Convert to x_uint8_t
x_uint8_t XVariant::toUChar() const noexcept
{
	return static_cast<x_uint8_t>(this->toULLong());
}

// Convert to short
x_int16_t XVariant::toShort() const noexcept
{
	return static_cast<x_int16_t>(this->toLLong());
}

// Convert to x_uint16_t
x_uint16_t XVariant::toUShort() const noexcept
{
	return static_cast<x_uint16_t>(this->toULLong());
}

// Convert to int
x_int32_t XVariant::toInt() const noexcept
{
	return static_cast<x_int32_t>(this->toLLong());
}

// Convert to x_uint32_t
x_uint32_t XVariant::toUInt() const noexcept
{
	return static_cast<x_uint32_t>(this->toULLong());
}

// Convert to long
long XVariant::toLong() const noexcept
{
	return static_cast<long>(this->toLLong());
}

// Convert to unsigned long
unsigned long XVariant::toULong() const noexcept
{
	return static_cast<unsigned long>(this->toULLong());
}

// Convert to x_int64_t
x_int64_t XVariant::toLLong() const noexcept
{
	switch (this->_variant_type)
	{
		case VariantBoolean:
			return this->_variant_data._VBool ? 1LL : 0LL;
		case VariantChar:
		case VariantShort:
		case VariantInteger:
		case VariantLong:
		case VariantLLong:
			return this->_variant_data._VInt64S;
		case VariantUChar:
		case VariantUShort:
		case VariantUInteger:
		case VariantULong:
		case VariantULLong:
			return static_cast<x_int64_t>(this->_variant_data._VInt64U);
		case VariantFloat:
		case VariantDouble:
			return static_cast<x_int64_t>(this->_variant_data._VDouble);
		case VariantString:
			return x_posix_atoll(this->toString().data());
		case VariantByteArray:
			return x_posix_atoll(this->toBytes().data());
		default:
			return this->_variant_data._VNull;
	};
}

// Convert to x_uint64_t
x_uint64_t XVariant::toULLong() const noexcept
{
	switch (this->_variant_type)
	{
		case VariantBoolean:
			return this->_variant_data._VBool ? 1ULL : 0ULL;
		case VariantChar:
		case VariantShort:
		case VariantInteger:
		case VariantLong:
		case VariantLLong:
			return static_cast<x_uint64_t>(this->_variant_data._VInt64S);
		case VariantUChar:
		case VariantUShort:
		case VariantUInteger:
		case VariantULong:
		case VariantULLong:
			return this->_variant_data._VInt64U;
		case VariantFloat:
		case VariantDouble:
			return static_cast<x_uint64_t>(this->_variant_data._VDouble);
		case VariantString:
			return x_posix_atoll(this->toString().data());
		case VariantByteArray:
			return x_posix_atoll(this->toBytes().data());
		default:
			return this->_variant_data._VNull;
	};
}

// Convert to float
float XVariant::toFloat() const noexcept
{
	return static_cast<float>(this->toDouble());
}

// Convert to double
double XVariant::toDouble() const noexcept
{
	switch (this->_variant_type)
	{
		case VariantBoolean:
			return 0.0f;
		case VariantChar:
		case VariantShort:
		case VariantInteger:
		case VariantLong:
		case VariantLLong:
			return static_cast<double>(this->_variant_data._VInt64S);
		case VariantUChar:
		case VariantUShort:
		case VariantUInteger:
		case VariantULong:
		case VariantULLong:
			return static_cast<double>(this->_variant_data._VInt64U);
		case VariantFloat:
		case VariantDouble:
			return this->_variant_data._VDouble;
		case VariantString:
			return x_posix_atof(this->toString().data());
		case VariantByteArray:
			return x_posix_atof(this->toBytes().data());
		default:
			return static_cast<double>(this->_variant_data._VNull);
	};
}

// Convert to XString
XString XVariant::toString() const noexcept
{
	switch (this->_variant_type)
	{
		case VariantBoolean:
			return this->_variant_data._VBool ? "true" : "false";
		case VariantChar:
		case VariantShort:
		case VariantInteger:
		case VariantLong:
		case VariantLLong:
			return XString::number(this->_variant_data._VInt64S);
		case VariantUChar:
		case VariantUShort:
		case VariantUInteger:
		case VariantULong:
		case VariantULLong:
			return XString::number(this->_variant_data._VInt64U);
		case VariantFloat:
		case VariantDouble:
			return XString::number(this->_variant_data._VDouble);
		case VariantString:
			return *(static_cast<XString*>(this->_variant_data._VObject));
		case VariantByteArray:
			return XString::fromBytes(*(static_cast<XByteArray*>(this->_variant_data._VObject)));
		default:
			return "";
	};
}

// Convert to XByteArray
XByteArray XVariant::toBytes() const noexcept
{
	switch (this->_variant_type)
	{
		case VariantBoolean:
		case VariantChar:
		case VariantUChar:
		case VariantShort:
		case VariantUShort:
		case VariantInteger:
		case VariantUInteger:
		case VariantLLong:
		case VariantULLong:
		case VariantFloat:
		case VariantDouble:
		case VariantString:
			return static_cast<XString*>(this->_variant_data._VObject)->toBytes();
		case VariantByteArray:
			return *(static_cast<XByteArray*>(this->_variant_data._VObject));
		default:
			return {};
	};
}
