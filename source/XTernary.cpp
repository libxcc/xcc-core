#include <xcc-core/XTernary.h>



// constructor
XTernary::XTernary() noexcept
{
	this->_ternary_value = TernaryNull;
}

// constructor
XTernary::XTernary(value_type _Type) noexcept
{
	this->_ternary_value = _Type;
}

// constructor
XTernary::XTernary(bool _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(char _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(unsigned char _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(short _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(unsigned short _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(int _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(unsigned int _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(unsigned long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(long long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(unsigned long long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(const void* _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
}

// constructor
XTernary::XTernary(const XTernary& _Other) noexcept
{
	this->_ternary_value = _Other._ternary_value;
}

// constructor
XTernary::XTernary(XTernary&& _Other) noexcept
{
	this->_ternary_value = _Other._ternary_value;
}

// destructor
XTernary::~XTernary() noexcept = default;



// operator override =
XTernary& XTernary::operator = (value_type _Type) noexcept
{
	this->_ternary_value = _Type;
	return *this;
}

// operator override =
XTernary& XTernary::operator = (bool _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (char _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (unsigned char _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (short _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (unsigned short _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (int _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (unsigned int _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (unsigned long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (long long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (unsigned long long _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (const void* _Value) noexcept
{
	this->_ternary_value = XTernary::convert(_Value);
	return *this;
}

// operator override =
XTernary& XTernary::operator = (const XTernary& _Other) noexcept
{
	if(this != &_Other)
	{
		this->_ternary_value = _Other._ternary_value;
	}
	return *this;
}

// operator override =
XTernary& XTernary::operator = (XTernary&& _Other) noexcept
{
	if(this != &_Other)
	{
		this->_ternary_value = _Other._ternary_value;
	}
	return *this;
}



// operator override ==
bool XTernary::operator == (value_type _Type) const noexcept
{
	return this->_ternary_value == _Type;
}

// operator override ==
bool XTernary::operator == (bool _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (char _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (unsigned char _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (short _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (unsigned short _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (int _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (unsigned int _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (long _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (unsigned long _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (long long _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (unsigned long long _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (const void* _Value) const noexcept
{
	return this->_ternary_value == XTernary::convert(_Value);
}

// operator override ==
bool XTernary::operator == (const XTernary& _Other) const noexcept
{
	return this->_ternary_value == _Other._ternary_value;
}



// operator override !=
bool XTernary::operator != (value_type _Type) const noexcept
{
	return this->_ternary_value != _Type;
}

// operator override !=
bool XTernary::operator != (bool _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (char _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (unsigned char _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (short _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (unsigned short _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (int _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (unsigned int _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (long _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (unsigned long _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (long long _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (unsigned long long _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (const void* _Value) const noexcept
{
	return this->_ternary_value != XTernary::convert(_Value);
}

// operator override !=
bool XTernary::operator != (const XTernary& _Other) const noexcept
{
	return this->_ternary_value != _Other._ternary_value;
}



// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(bool _Value) noexcept
{
	return _Value ? TernaryTrue : TernaryFalse;
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(char _Value) noexcept
{
	return XTernary::convert(static_cast<long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(unsigned char _Value) noexcept
{
	return XTernary::convert(static_cast<unsigned long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(short _Value) noexcept
{
	return XTernary::convert(static_cast<long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(unsigned short _Value) noexcept
{
	return XTernary::convert(static_cast<unsigned long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(int _Value) noexcept
{
	return XTernary::convert(static_cast<long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(unsigned int _Value) noexcept
{
	return XTernary::convert(static_cast<unsigned long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(long _Value) noexcept
{
	return XTernary::convert(static_cast<long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(unsigned long _Value) noexcept
{
	return XTernary::convert(static_cast<unsigned long long>(_Value));
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(long long _Value) noexcept
{
	if(_Value == 0)
	{
		return TernaryNull;
	}
	else if(_Value > 0)
	{
		return TernaryTrue;
	}
	else
	{
		return TernaryFalse;
	}
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(unsigned long long _Value) noexcept
{
	if(_Value == 0)
	{
		return TernaryNull;
	}
	else
	{
		return TernaryTrue;
	}
}

// [opt] convert parameter to ternary type
XTernary::value_type XTernary::convert(const void* _Value) noexcept
{
	if(nullptr == _Value)
	{
		return TernaryNull;
	}
	else
	{
		return TernaryTrue;
	}
}



// swap
void XTernary::swap(XTernary& _Other) noexcept
{
	if(this != &_Other)
	{
		this->_ternary_value = _Other._ternary_value;
		_Other._ternary_value = this->_ternary_value;
	}
}



// [get] type
XTernary::value_type XTernary::type() const noexcept
{
	return this->_ternary_value;
}



// [get] Is Null
bool XTernary::isNull() const noexcept
{
	return this->_ternary_value == TernaryNull;
}

// [get] Is Null
bool XTernary::isEmpty() const noexcept
{
	return this->_ternary_value == TernaryNull;
}

// [get] Is Ture
bool XTernary::isTure() const noexcept
{
	return this->_ternary_value == TernaryTrue;
}

// [get] Is False
bool XTernary::isFalse() const noexcept
{
	return this->_ternary_value == TernaryFalse;
}



// [cnv] 转换为String
XString XTernary::toString() const noexcept
{
	switch (this->_ternary_value)
	{
		case TernaryTrue:		return "true";
		case TernaryFalse:		return "false";
		default:			return "null";
	}
}



// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (XTernary::value_type _Type, const XTernary& _Object) noexcept
{
	return _Object == _Type;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (bool _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (char _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (unsigned char _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (short _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (unsigned short _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (int _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (unsigned int _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (long _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (unsigned long _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (long long _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (unsigned long long _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (const void* _Value, const XTernary& _Object) noexcept
{
	return _Object == _Value;
}



// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (XTernary::value_type _Type, const XTernary& _Object) noexcept
{
	return _Object != _Type;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (bool _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (char _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (unsigned char _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (short _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (unsigned short _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (int _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (unsigned int _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (long _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (unsigned long _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (long long _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (unsigned long long _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (const void* _Value, const XTernary& _Object) noexcept
{
	return _Object != _Value;
}
