#include <source/serialize/XJsonPrivate.h>


// constructor
XJsonObject::XJsonObject() noexcept
{
	memberData = XJsonPrivate::newObject();
}

// constructor
XJsonObject::XJsonObject(XJsonObjectPrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newObject();
}

// constructor
XJsonObject::XJsonObject(const XJsonObject& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonObject::XJsonObject(XJsonObject&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonObject::~XJsonObject() noexcept
{
	XJsonPrivate::free(memberData);
}



// operator =
XJsonObject& XJsonObject::operator = (const XJsonObject& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator =
XJsonObject& XJsonObject::operator = (XJsonObject&& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = _Object.memberData;
		_Object.memberData = nullptr;
	}
	return *this;
}



// operator []
XJsonValue& XJsonObject::operator[](const XString& _Key)
{
	auto		vIterator = memberData->find(_Key);
	if(vIterator == memberData->end())
	{
		memberData->insert(_Key, XJsonValue::null());
		vIterator = memberData->find(_Key);
	}
	return vIterator->second;
}

// operator []
const XJsonValue& XJsonObject::operator[](const XString& _Key) const
{
	auto		vIterator = memberData->find(_Key);
	if(vIterator == memberData->end())
	{
		memberData->insert(_Key, XJsonValue::null());
		vIterator = memberData->find(_Key);
	}
	return vIterator->second;
}



// 子项数量
x_size_t XJsonObject::size() const noexcept
{
	return memberData->size();
}

// 是否不存在子项
bool XJsonObject::empty() const noexcept
{
	return memberData->empty();
}

// 是否存在子项
bool XJsonObject::exist() const noexcept
{
	return !memberData->empty();
}

// 清空
void XJsonObject::clear() noexcept
{
	return memberData->clear();
}



// 正向迭代器起点
XJsonObject::iterator XJsonObject::begin() noexcept
{
	if(memberData->empty())
	{
		return iterator(XJsonPrivate::newObjectIter(memberData, nullptr));
	}
	else
	{
		return iterator(XJsonPrivate::newObjectIter(memberData, memberData->begin()->first));
	}
}

// 正向迭代器终点
XJsonObject::iterator XJsonObject::end() noexcept
{
	return iterator(XJsonPrivate::newObjectIter(memberData, nullptr));
}

// 正向只读迭代器起点
XJsonObject::const_iterator XJsonObject::begin() const noexcept
{
	if(memberData->empty())
	{
		return const_iterator(XJsonPrivate::newObjectIter(memberData, nullptr));
	}
	else
	{
		return const_iterator(XJsonPrivate::newObjectIter(memberData, memberData->begin()->first));
	}
}

// 正向只读迭代器终点
XJsonObject::const_iterator XJsonObject::end() const noexcept
{
	return const_iterator(XJsonPrivate::newObjectIter(memberData, nullptr));
}



// 插入至指定键
void XJsonObject::insert(const XString& _Key, const XJsonValue& _JsonValue) noexcept
{
	return this->replace(_Key, _JsonValue);
}

// 替换指定键
void XJsonObject::replace(const XString& _Key, const XJsonValue& _JsonValue) noexcept
{
	auto		vIterator = memberData->find(_Key);
	if(vIterator == memberData->end())
	{
		memberData->insert(_Key, _JsonValue);
		return;
	}
	vIterator->second = _JsonValue;
}

// 删除指定键
void XJsonObject::remove(const XString& _Key) noexcept
{
	auto		vIterator = memberData->find(_Key);
	if(vIterator == memberData->end())
	{
		return;
	}
	memberData->erase(vIterator);
}

// 分离指定键
XJsonValue XJsonObject::detach(const XString& _Key) noexcept
{
	auto		vIterator = memberData->find(_Key);
	if(vIterator == memberData->end())
	{
		return {};
	}
	auto		vJsonValue = vIterator->second;
	memberData->erase(vIterator);
	return vJsonValue;
}



// 查找迭代器
XJsonObject::iterator XJsonObject::find(const XString& _Key) noexcept
{
	return iterator(XJsonPrivate::newObjectIter(memberData, _Key));
}

// 查找迭代器
XJsonObject::const_iterator XJsonObject::find(const XString& _Key) const noexcept
{
	return const_iterator(XJsonPrivate::newObjectIter(memberData, _Key));
}

// 删除迭代器
XJsonObject::iterator XJsonObject::remove(iterator _Iterator) noexcept
{
	if(_Iterator.isValid())
	{
		auto		vIterator = memberData->erase(_Iterator.memberData->iter);
		if(vIterator != memberData->end())
		{
			return iterator(XJsonPrivate::newObjectIter(memberData, vIterator->first));
		}
	}
	return this->end();
}

// 删除迭代器
XJsonObject::const_iterator XJsonObject::remove(const_iterator _Iterator) noexcept
{
	if(_Iterator.isValid())
	{
		auto		vIterator = memberData->erase(_Iterator.memberData->iter);
		if(vIterator != memberData->end())
		{
			return const_iterator(XJsonPrivate::newObjectIter(memberData, vIterator->first));
		}
	}
	return const_iterator(XJsonPrivate::newObjectIter(memberData, nullptr));
}

// 分离迭代器
XJsonValue XJsonObject::detach(iterator _Iterator) noexcept
{
	if(_Iterator.isValid())
	{
		auto		vJsonValue = _Iterator.value();
		this->remove(_Iterator);
		return vJsonValue;
	}
	return {};
}

// 分离迭代器
XJsonValue XJsonObject::detach(const_iterator _Iterator) noexcept
{
	if(_Iterator.isValid())
	{
		auto		vJsonValue = _Iterator.value();
		this->remove(_Iterator);
		return vJsonValue;
	}
	return {};
}





// constructor
XJsonObject::iterator::iterator() noexcept
{
	memberData = XJsonPrivate::newObjectIter(nullptr, nullptr);
}

// constructor
XJsonObject::iterator::iterator(XJsonObjectIterPrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newObjectIter(nullptr, nullptr);
}

// constructor
XJsonObject::iterator::iterator(const iterator& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonObject::iterator::iterator(iterator&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonObject::iterator::~iterator() noexcept
{
	XJsonPrivate::free(memberData);
}



// operator =
XJsonObject::iterator& XJsonObject::iterator::operator =(const iterator& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator =
XJsonObject::iterator& XJsonObject::iterator::operator =(iterator&& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = _Object.memberData;
		_Object.memberData = nullptr;
	}
	return *this;
}



// operator =
bool XJsonObject::iterator::operator ==(const iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->object == _Object.memberData->object && memberData->iter == _Object.memberData->iter;
	}
	return false;
}

// operator =
bool XJsonObject::iterator::operator !=(const iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->object != _Object.memberData->object || memberData->iter != _Object.memberData->iter;
	}
	return true;
}



// operator ++
XJsonObject::iterator& XJsonObject::iterator::operator ++() noexcept
{
	operator += (1);
	return *this;
}

// operator ++
XJsonObject::iterator XJsonObject::iterator::operator ++() const noexcept
{
	auto		vIterator = *this;
	vIterator += 1;
	return vIterator;
}

// operator --
XJsonObject::iterator& XJsonObject::iterator::operator --() noexcept
{
	operator -= (1);
	return *this;
}

// operator --
XJsonObject::iterator XJsonObject::iterator::operator --() const noexcept
{
	auto		vIterator = *this;
	vIterator -= 1;
	return vIterator;
}

// operator +
XJsonObject::iterator XJsonObject::iterator::operator +(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator += _Size;
	return vIterator;
}

// operator -
XJsonObject::iterator XJsonObject::iterator::operator -(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator -= _Size;
	return vIterator;
}

// operator +=
XJsonObject::iterator& XJsonObject::iterator::operator +=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		while(memberData->iter != memberData->object->data.end() && _Size != 0)
		{
			if(_Size > 0)
			{
				++(memberData->iter);
				--_Size;
			}
			else
			{
				--(memberData->iter);
				++_Size;
			}
		}
	}
	return *this;
}

// operator -=
XJsonObject::iterator& XJsonObject::iterator::operator -=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		while(memberData->iter != memberData->object->data.end() && _Size != 0)
		{
			if(_Size > 0)
			{
				--(memberData->iter);
				--_Size;
			}
			else
			{
				++(memberData->iter);
				++_Size;
			}
		}
	}
	return *this;
}



// operator *
XJsonValue& XJsonObject::iterator::operator *()
{
	return memberData->iter->second;
}

// operator *
const XJsonValue& XJsonObject::iterator::operator *() const
{
	return memberData->iter->second;
}



// 是否有效
bool XJsonObject::iterator::isValid() const noexcept
{
	if(memberData && memberData->object && memberData->iter != memberData->object->data.end())
	{
		return true;
	}
	return false;
}

// Key
const XString& XJsonObject::iterator::key() const
{
	return memberData->iter->first;
}

// Value
XJsonValue& XJsonObject::iterator::value()
{
	return memberData->iter->second;
}

// Value
const XJsonValue& XJsonObject::iterator::value() const
{
	return memberData->iter->second;
}





// constructor
XJsonObject::const_iterator::const_iterator() noexcept
{
	memberData = XJsonPrivate::newObjectIter(nullptr, nullptr);
}

// constructor
XJsonObject::const_iterator::const_iterator(XJsonObjectIterPrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newObjectIter(nullptr, nullptr);
}

// constructor
XJsonObject::const_iterator::const_iterator(const const_iterator& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonObject::const_iterator::const_iterator(const_iterator&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonObject::const_iterator::~const_iterator() noexcept
{
	XJsonPrivate::free(memberData);
}



// operator =
XJsonObject::const_iterator& XJsonObject::const_iterator::operator =(const const_iterator& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator =
XJsonObject::const_iterator& XJsonObject::const_iterator::operator =(const_iterator&& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = _Object.memberData;
		_Object.memberData = nullptr;
	}
	return *this;
}



// operator =
bool XJsonObject::const_iterator::operator ==(const const_iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->object == _Object.memberData->object && memberData->iter == _Object.memberData->iter;
	}
	return false;
}

// operator =
bool XJsonObject::const_iterator::operator !=(const const_iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->object != _Object.memberData->object || memberData->iter != _Object.memberData->iter;
	}
	return true;
}



// operator ++
XJsonObject::const_iterator& XJsonObject::const_iterator::operator ++() noexcept
{
	operator += (1);
	return *this;
}

// operator ++
XJsonObject::const_iterator XJsonObject::const_iterator::operator ++() const noexcept
{
	auto		vIterator = *this;
	vIterator += 1;
	return vIterator;
}

// operator --
XJsonObject::const_iterator& XJsonObject::const_iterator::operator --() noexcept
{
	operator -= (1);
	return *this;
}

// operator --
XJsonObject::const_iterator XJsonObject::const_iterator::operator --() const noexcept
{
	auto		vIterator = *this;
	vIterator -= 1;
	return vIterator;
}

// operator +
XJsonObject::const_iterator XJsonObject::const_iterator::operator +(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator += _Size;
	return vIterator;
}

// operator -
XJsonObject::const_iterator XJsonObject::const_iterator::operator -(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator -= _Size;
	return vIterator;
}

// operator +=
XJsonObject::const_iterator& XJsonObject::const_iterator::operator +=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		memberData->iter += _Size;
		/*
		while(memberData->iter != memberData->object->data.end() && _Size != 0)
		{
			if(_Size > 0)
			{
				++(memberData->iter);
				--_Size;
			}
			else
			{
				--(memberData->iter);
				++_Size;
			}
		}
		*/
	}
	return *this;
}

// operator -=
XJsonObject::const_iterator& XJsonObject::const_iterator::operator -=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		memberData->iter -= _Size;
		/*
		while(memberData->iter != memberData->object->data.end() && _Size != 0)
		{
			if(_Size > 0)
			{
				--(memberData->iter);
				--_Size;
			}
			else
			{
				++(memberData->iter);
				++_Size;
			}
		}
		*/
	}
	return *this;
}



// operator *
const XJsonValue& XJsonObject::const_iterator::operator *() const
{
	return memberData->iter->second;
}



// 是否有效
bool XJsonObject::const_iterator::isValid() const noexcept
{
	if(memberData && memberData->object && memberData->iter != memberData->object->data.end())
	{
		return true;
	}
	return false;
}

// Key
const XString& XJsonObject::const_iterator::key() const
{
	return memberData->iter->first;
}

// Value
const XJsonValue& XJsonObject::const_iterator::value() const
{
	return memberData->iter->second;
}
