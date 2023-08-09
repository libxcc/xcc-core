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



// operator override =
XJsonArray& XJsonArray::operator = (const XJsonArray& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator override =
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



// operator override []
XJsonValue& XJsonArray::operator[](x_size_t _Index)
{
	return memberData->data[_Index];
}

// operator override []
const XJsonValue& XJsonArray::operator[](x_size_t _Index) const
{
	return memberData->data[_Index];
}



// 子项数量
x_size_t XJsonArray::size() const noexcept
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

// 清空
void XJsonArray::clear() noexcept
{
	return memberData->data.clear();
}



// 正向迭代器起点
XJsonArray::iterator XJsonArray::begin() noexcept
{
	return iterator(XJsonPrivate::newArrayIter(memberData, 0));
}

// 正向迭代器终点
XJsonArray::iterator XJsonArray::end() noexcept
{
	return iterator(XJsonPrivate::newArrayIter(memberData, this->size()));
}

// 正向只读迭代器起点
XJsonArray::const_iterator XJsonArray::begin() const noexcept
{
	return const_iterator(XJsonPrivate::newArrayIter(memberData, 0));
}

// 正向只读迭代器终点
XJsonArray::const_iterator XJsonArray::end() const noexcept
{
	return const_iterator(XJsonPrivate::newArrayIter(memberData, this->size()));
}



// 插入至最前
void XJsonArray::prepend(const XJsonValue& _JsonValue) noexcept
{
	memberData->data.insert(memberData->data.begin(), _JsonValue);
}

// 插入至末尾
void XJsonArray::append(const XJsonValue& _JsonValue) noexcept
{
	memberData->data.insert(memberData->data.end(), _JsonValue);
}

// 插入至指定下标
void XJsonArray::insert(x_size_t _Index, const XJsonValue& _JsonValue) noexcept
{
	if(_Index >= memberData->data.size())
	{
		memberData->data.insert(memberData->data.end(), _JsonValue);
	}
	else
	{
		memberData->data.insert(memberData->data.begin() + (x_ssize_t)_Index, _JsonValue);
	}
}

// 替换指定下标
void XJsonArray::replace(x_size_t _Index, const XJsonValue& _JsonValue) noexcept
{
	this->insert(_Index, _JsonValue);
	this->remove(_Index + 1);
}

// 删除指定下标
void XJsonArray::remove(x_size_t _Index) noexcept
{
	if(_Index >= memberData->data.size())
	{
		return;
	}
	auto		vIterator = memberData->data.begin() + (x_ssize_t)_Index;
	memberData->data.erase(vIterator);
}

// 分离指定下标
XJsonValue XJsonArray::detach(x_size_t _Index) noexcept
{
	if(_Index >= memberData->data.size())
	{
		return {};
	}
	auto		vIterator = memberData->data.begin() + (x_ssize_t)_Index;
	auto		vJsonValue = *vIterator;
	memberData->data.erase(vIterator);
	return vJsonValue;
}



// 查找迭代器
XJsonArray::iterator XJsonArray::find(x_size_t _Index) noexcept
{
	return iterator(XJsonPrivate::newArrayIter(memberData, _Index));
}

// 查找迭代器
XJsonArray::const_iterator XJsonArray::find(x_size_t _Index) const noexcept
{
	return const_iterator(XJsonPrivate::newArrayIter(memberData, _Index));
}

// 删除迭代器
XJsonArray::iterator XJsonArray::remove(iterator _Iterator) noexcept
{
	if(_Iterator.isValid())
	{
		auto		vIndex = _Iterator.index();
		auto		vIterator = memberData->data.erase(_Iterator.memberData->iter);
		if(vIterator != memberData->data.end())
		{
			return iterator(XJsonPrivate::newArrayIter(memberData, vIndex));
		}
	}
	return this->end();
}

// 删除迭代器
XJsonArray::const_iterator XJsonArray::remove(const_iterator _Iterator) noexcept
{
	if(_Iterator.isValid())
	{
		auto		vIndex = _Iterator.index();
		auto		vIterator = memberData->data.erase(_Iterator.memberData->iter);
		if(vIterator != memberData->data.end())
		{
			return const_iterator(XJsonPrivate::newArrayIter(memberData, vIndex));
		}
	}
	return const_iterator(XJsonPrivate::newArrayIter(memberData, this->size()));
}

// 分离迭代器
XJsonValue XJsonArray::detach(iterator _Iterator) noexcept
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
XJsonValue XJsonArray::detach(const_iterator _Iterator) noexcept
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
XJsonArray::iterator::iterator() noexcept
{
	memberData = XJsonPrivate::newArrayIter(nullptr, 0);
}

// constructor
XJsonArray::iterator::iterator(XJsonArrayIterPrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newArrayIter(nullptr, 0);
}

// constructor
XJsonArray::iterator::iterator(const iterator& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonArray::iterator::iterator(iterator&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonArray::iterator::~iterator() noexcept
{
	XJsonPrivate::free(memberData);
}



// operator override =
XJsonArray::iterator& XJsonArray::iterator::operator =(const iterator& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator override =
XJsonArray::iterator& XJsonArray::iterator::operator =(iterator&& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = _Object.memberData;
		_Object.memberData = nullptr;
	}
	return *this;
}



// operator override =
bool XJsonArray::iterator::operator ==(const iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->array == _Object.memberData->array && memberData->iter == _Object.memberData->iter;
	}
	return false;
}

// operator override =
bool XJsonArray::iterator::operator !=(const iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->array != _Object.memberData->array || memberData->iter != _Object.memberData->iter;
	}
	return true;
}



// operator override ++
XJsonArray::iterator& XJsonArray::iterator::operator ++() noexcept
{
	operator += (1);
	return *this;
}

// operator override ++
XJsonArray::iterator XJsonArray::iterator::operator ++() const noexcept
{
	auto		vIterator = *this;
	vIterator += 1;
	return vIterator;
}

// operator override --
XJsonArray::iterator& XJsonArray::iterator::operator --() noexcept
{
	operator -= (1);
	return *this;
}

// operator override --
XJsonArray::iterator XJsonArray::iterator::operator --() const noexcept
{
	auto		vIterator = *this;
	vIterator -= 1;
	return vIterator;
}

// operator override +
XJsonArray::iterator XJsonArray::iterator::operator +(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator += _Size;
	return vIterator;
}

// operator override -
XJsonArray::iterator XJsonArray::iterator::operator -(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator -= _Size;
	return vIterator;
}

// operator override +=
XJsonArray::iterator& XJsonArray::iterator::operator +=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		memberData->iter += _Size;
	}
	return *this;
}

// operator override -=
XJsonArray::iterator& XJsonArray::iterator::operator -=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		memberData->iter -= _Size;
	}
	return *this;
}



// operator override *
XJsonValue& XJsonArray::iterator::operator *()
{
	return *(memberData->iter);
}

// operator override *
const XJsonValue& XJsonArray::iterator::operator *() const
{
	return *(memberData->iter);
}



// 是否有效
bool XJsonArray::iterator::isValid() const noexcept
{
	if(memberData && memberData->array && memberData->iter != memberData->array->data.end())
	{
		return true;
	}
	return false;
}

// 下标
x_size_t XJsonArray::iterator::index() const noexcept
{
	if(this->isValid())
	{
		return std::distance(memberData->array->data.begin(), memberData->iter);
	}
	return 0;
}

// Value
XJsonValue& XJsonArray::iterator::value()
{
	return *(memberData->iter);
}

// Value
const XJsonValue& XJsonArray::iterator::value() const
{
	return *(memberData->iter);
}





// constructor
XJsonArray::const_iterator::const_iterator() noexcept
{
	memberData = XJsonPrivate::newArrayIter(nullptr, 0);
}

// constructor
XJsonArray::const_iterator::const_iterator(XJsonArrayIterPrivate* _PrivateData) noexcept
{
	memberData = _PrivateData ? _PrivateData : XJsonPrivate::newArrayIter(nullptr, 0);
}

// constructor
XJsonArray::const_iterator::const_iterator(const const_iterator& _Object) noexcept
{
	memberData = XJsonPrivate::clone(_Object.memberData);
}

// constructor
XJsonArray::const_iterator::const_iterator(const_iterator&& _Object) noexcept
{
	memberData = _Object.memberData;
	_Object.memberData = nullptr;
}

// destructor
XJsonArray::const_iterator::~const_iterator() noexcept
{
	XJsonPrivate::free(memberData);
}



// operator override =
XJsonArray::const_iterator& XJsonArray::const_iterator::operator =(const const_iterator& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = XJsonPrivate::clone(_Object.memberData);
	}
	return *this;
}

// operator override =
XJsonArray::const_iterator& XJsonArray::const_iterator::operator =(const_iterator&& _Object) noexcept
{
	if(this != &_Object)
	{
		XJsonPrivate::free(memberData);
		memberData = _Object.memberData;
		_Object.memberData = nullptr;
	}
	return *this;
}



// operator override =
bool XJsonArray::const_iterator::operator ==(const const_iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->array == _Object.memberData->array && memberData->iter == _Object.memberData->iter;
	}
	return false;
}

// operator override =
bool XJsonArray::const_iterator::operator !=(const const_iterator& _Object) noexcept
{
	if(memberData && _Object.memberData)
	{
		return memberData->array != _Object.memberData->array || memberData->iter != _Object.memberData->iter;
	}
	return true;
}



// operator override ++
XJsonArray::const_iterator& XJsonArray::const_iterator::operator ++() noexcept
{
	operator += (1);
	return *this;
}

// operator override ++
XJsonArray::const_iterator XJsonArray::const_iterator::operator ++() const noexcept
{
	auto		vIterator = *this;
	vIterator += 1;
	return vIterator;
}

// operator override --
XJsonArray::const_iterator& XJsonArray::const_iterator::operator --() noexcept
{
	operator -= (1);
	return *this;
}

// operator override --
XJsonArray::const_iterator XJsonArray::const_iterator::operator --() const noexcept
{
	auto		vIterator = *this;
	vIterator -= 1;
	return vIterator;
}

// operator override +
XJsonArray::const_iterator XJsonArray::const_iterator::operator +(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator += _Size;
	return vIterator;
}

// operator override -
XJsonArray::const_iterator XJsonArray::const_iterator::operator -(x_ssize_t _Size) const noexcept
{
	auto		vIterator = *this;
	vIterator -= _Size;
	return vIterator;
}

// operator override +=
XJsonArray::const_iterator& XJsonArray::const_iterator::operator +=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		memberData->iter += _Size;
	}
	return *this;
}

// operator override -=
XJsonArray::const_iterator& XJsonArray::const_iterator::operator -=(x_ssize_t _Size) noexcept
{
	if(memberData)
	{
		memberData->iter -= _Size;
	}
	return *this;
}



// operator override *
const XJsonValue& XJsonArray::const_iterator::operator *() const
{
	return *(memberData->iter);
}



// 是否有效
bool XJsonArray::const_iterator::isValid() const noexcept
{
	if(memberData && memberData->array && memberData->iter != memberData->array->data.end())
	{
		return true;
	}
	return false;
}

// 下标
x_size_t XJsonArray::const_iterator::index() const noexcept
{
	if(this->isValid())
	{
		return std::distance(memberData->array->data.begin(), memberData->iter);
	}
	return 0;
}

// Value
const XJsonValue& XJsonArray::const_iterator::value() const
{
	return *(memberData->iter);
}
