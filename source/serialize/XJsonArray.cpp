#include <source/serialize/XJsonPrivate.h>


// constructor
XJsonArray::XJsonArray() noexcept
{
	memberData = XJsonPrivate::newArray();
}

// constructor
XJsonArray::XJsonArray(XJsonArrayPrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newArray();
}

// constructor
XJsonArray::XJsonArray(const XJsonArray& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonArray::XJsonArray(XJsonArray&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonArray::~XJsonArray() noexcept
{
	XJsonPrivate::free(memberData);
}



// operator overload =
XJsonArray& XJsonArray::operator = (const XJsonArray& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator overload =
XJsonArray& XJsonArray::operator = (XJsonArray&& _Object) noexcept
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
XJsonValue& XJsonArray::operator[](Xcc::size_t _Index)
{
	return memberData->data[_Index];
}

// operator overload []
const XJsonValue& XJsonArray::operator[](Xcc::size_t _Index) const
{
	return memberData->data[_Index];
}



// 子项数量
Xcc::size_t XJsonArray::size() const noexcept
{
	return memberData->data.size();
}

// 是否不存在子项
bool XJsonArray::empty() const noexcept
{
	return memberData->data.empty();
}

// 是否存在子项
bool XJsonArray::exist() const noexcept
{
	return !memberData->data.empty();
}



// 插入至最前
void XJsonArray::prepend(bool _Status) noexcept
{
	this->prepend(XJsonValue::boolean(_Status));
}

// 插入至最前
void XJsonArray::prepend(Xcc::int8_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(Xcc::uint8_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(Xcc::int16_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(Xcc::uint16_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(Xcc::int32_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(Xcc::uint32_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(Xcc::int64_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(Xcc::uint64_t _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(float _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(double _Number) noexcept
{
	this->prepend(XJsonValue::number(_Number));
}

// 插入至最前
void XJsonArray::prepend(const XString& _String) noexcept
{
	this->prepend(XJsonValue::string(_String));
}

// 插入至最前
void XJsonArray::prepend(const XJsonValue& _JsonValue) noexcept
{
	memberData->data.insert(memberData->data.begin(), _JsonValue);
}

// 插入至最前
void XJsonArray::prepend(const XJsonArray& _JsonArray) noexcept
{
	this->prepend(XJsonValue::array(_JsonArray));
}

// 插入至最前
void XJsonArray::prepend(const XJsonObjectT& _JsonObject) noexcept
{
	this->prepend(XJsonValue::object(_JsonObject));
}



// 插入至末尾
void XJsonArray::append(bool _Status) noexcept
{
	this->append(XJsonValue::boolean(_Status));
}

// 插入至末尾
void XJsonArray::append(Xcc::int8_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(Xcc::uint8_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(Xcc::int16_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(Xcc::uint16_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(Xcc::int32_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(Xcc::uint32_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(Xcc::int64_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(Xcc::uint64_t _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(float _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(double _Number) noexcept
{
	this->append(XJsonValue::number(_Number));
}

// 插入至末尾
void XJsonArray::append(const XString& _String) noexcept
{
	this->append(XJsonValue::string(_String));
}

// 插入至末尾
void XJsonArray::append(const XJsonValue& _JsonValue) noexcept
{
	memberData->data.insert(memberData->data.end(), _JsonValue);
}

// 插入至末尾
void XJsonArray::append(const XJsonArray& _JsonArray) noexcept
{
	this->append(XJsonValue::array(_JsonArray));
}

// 插入至末尾
void XJsonArray::append(const XJsonObjectT& _JsonObject) noexcept
{
	this->append(XJsonValue::object(_JsonObject));
}



// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, bool _Status) noexcept
{
	this->insert(_Index, XJsonValue::boolean(_Status));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::int8_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::uint8_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::int16_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::uint16_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::int32_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::uint32_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::int64_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, Xcc::uint64_t _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, float _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, double _Number) noexcept
{
	this->insert(_Index, XJsonValue::number(_Number));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, const XString& _String) noexcept
{
	this->insert(_Index, XJsonValue::string(_String));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, const XJsonValue& _JsonValue) noexcept
{
	if(_Index >= memberData->data.size())
	{
		memberData->data.insert(memberData->data.end(), _JsonValue);
	}
	else
	{
		memberData->data.insert(memberData->data.begin() + _Index, _JsonValue);
	}
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, const XJsonArray& _JsonArray) noexcept
{
	this->insert(_Index, XJsonValue::array(_JsonArray));
}

// 插入至指定下标
void XJsonArray::insert(Xcc::size_t _Index, const XJsonObjectT& _JsonObject) noexcept
{
	this->insert(_Index, XJsonValue::object(_JsonObject));
}



// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, bool _Status) noexcept
{
	this->replace(_Index, XJsonValue::boolean(_Status));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::int8_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::uint8_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::int16_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::uint16_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::int32_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::uint32_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::int64_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, Xcc::uint64_t _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, float _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, double _Number) noexcept
{
	this->replace(_Index, XJsonValue::number(_Number));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, const XString& _String) noexcept
{
	this->replace(_Index, XJsonValue::string(_String));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, const XJsonValue& _JsonValue) noexcept
{
	this->insert(_Index, _JsonValue);
	this->remove(_Index + 1);
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, const XJsonArray& _JsonArray) noexcept
{
	this->replace(_Index, XJsonValue::array(_JsonArray));
}

// 替换指定下标
void XJsonArray::replace(Xcc::size_t _Index, const XJsonObjectT& _JsonObject) noexcept
{
	this->replace(_Index, XJsonValue::object(_JsonObject));
}



// 删除指定下标
void XJsonArray::remove(Xcc::size_t _Index) noexcept
{
	if(_Index >= memberData->data.size())
	{
		return;
	}
	auto		vIterator = memberData->data.begin() + (int)_Index;
	memberData->data.erase(vIterator);
}



// 分离指定下标
XJsonValue XJsonArray::detach(Xcc::size_t _Index) noexcept
{
	if(_Index >= memberData->data.size())
	{
		return {};
	}
	auto		vIterator = memberData->data.begin() + (int)_Index;
	auto		vJsonValue = *vIterator;
	memberData->data.erase(vIterator);
	return vJsonValue;
}
