#include <source/serialize/XJsonPrivate.h>


// constructor
XJsonObjectT::XJsonObjectT() noexcept
{
	memberData = XJsonPrivate::newObject();
}

// constructor
XJsonObjectT::XJsonObjectT(XJsonObjectPrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newObject();
}

// constructor
XJsonObjectT::XJsonObjectT(const XJsonObjectT& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonObjectT::XJsonObjectT(XJsonObjectT&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonObjectT::~XJsonObjectT() noexcept
{
	XJsonPrivate::free(memberData);
}



// operator overload =
XJsonObjectT& XJsonObjectT::operator = (const XJsonObjectT& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator overload =
XJsonObjectT& XJsonObjectT::operator = (XJsonObjectT&& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = _Object.memberData;
		_Object.memberData = nullptr;
	}
	return *this;
}



// operator overload []
XJsonValue& XJsonObjectT::operator[](const XString& _Key)
{
	auto		vIterator = memberData->data.find(_Key);
	if(vIterator == memberData->data.end())
	{
		memberData->data.insert(std::map<XString, XJsonValue>::value_type(_Key, XJsonValue::null()));
		vIterator = memberData->data.find(_Key);
	}
	return vIterator->second;
}

// operator overload []
const XJsonValue& XJsonObjectT::operator[](const XString& _Key) const
{
	auto		vIterator = memberData->data.find(_Key);
	if(vIterator == memberData->data.end())
	{
		memberData->data.insert(std::map<XString, XJsonValue>::value_type(_Key, XJsonValue::null()));
		vIterator = memberData->data.find(_Key);
	}
	return vIterator->second;
}



// 子项数量
Xcc::size_t XJsonObjectT::size() const noexcept
{
	return memberData->data.size();
}

// 是否不存在子项
bool XJsonObjectT::empty() const noexcept
{
	return memberData->data.empty();
}

// 是否存在子项
bool XJsonObjectT::exist() const noexcept
{
	return !memberData->data.empty();
}



// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, bool _Status) noexcept
{
	this->insert(_Key, XJsonValue::boolean(_Status));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::int8_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::uint8_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::int16_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::uint16_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::int32_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::uint32_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::int64_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, Xcc::uint64_t _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, float _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, double _Number) noexcept
{
	this->insert(_Key, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, const XString& _String) noexcept
{
	this->insert(_Key, XJsonValue::string(_String));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, const XJsonValue& _JsonValue) noexcept
{
	return this->replace(_Key, _JsonValue);
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, const XJsonArray& _JsonArray) noexcept
{
	this->insert(_Key, XJsonValue::array(_JsonArray));
}

// 插入至指定下标
void XJsonObjectT::insert(const XString& _Key, const XJsonObjectT& _JsonObject) noexcept
{
	this->insert(_Key, XJsonValue::object(_JsonObject));
}



// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, bool _Status) noexcept
{
	this->replace(_Key, XJsonValue::boolean(_Status));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::int8_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::uint8_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::int16_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::uint16_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::int32_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::uint32_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::int64_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, Xcc::uint64_t _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, float _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, double _Number) noexcept
{
	this->replace(_Key, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, const XString& _String) noexcept
{
	this->replace(_Key, XJsonValue::string(_String));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, const XJsonValue& _JsonValue) noexcept
{
	auto		vIterator = memberData->data.find(_Key);
	if(vIterator == memberData->data.end())
	{
		memberData->data.insert(std::map<XString, XJsonValue>::value_type(_Key, _JsonValue));
		return;
	}
	vIterator->second = _JsonValue;
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, const XJsonArray& _JsonArray) noexcept
{
	this->replace(_Key, XJsonValue::array(_JsonArray));
}

// 替换指定下标
void XJsonObjectT::replace(const XString& _Key, const XJsonObjectT& _JsonObject) noexcept
{
	this->replace(_Key, XJsonValue::object(_JsonObject));
}



// 删除指定下标
void XJsonObjectT::remove(const XString& _Key) noexcept
{
	auto		vIterator = memberData->data.find(_Key);
	if(vIterator == memberData->data.end())
	{
		return;
	}
	memberData->data.erase(vIterator);
}



// 分离指定下标
XJsonValue XJsonObjectT::detach(const XString& _Key) noexcept
{
	auto		vIterator = memberData->data.find(_Key);
	if(vIterator == memberData->data.end())
	{
		return {};
	}
	auto		vJsonValue = vIterator->second;
	memberData->data.erase(vIterator);
	return vJsonValue;
}
