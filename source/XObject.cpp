#include <xcc-core/XObject.h>



// XCC - 基础对象 - 私有数据
struct XObjectPrivate
{
	XObject*				parent = nullptr;	// 父对象
	XString					objectName;		// 对象名称

	std::set<XObject*>			childArray;		// 子级列表

	std::map<x_uint64_t, XVariant>		userData;		// 用户数据
};



// constructor
XObject::XObject(XObject* _Parent) noexcept
{
	d_ptr = new(std::nothrow) XObjectPrivate();
	setParent(_Parent);
	setObjectName("XObject");
}

// destructor
XObject::~XObject() noexcept
{
	for(const auto & vIterator : d_ptr->childArray)
	{
		delete vIterator;
	}
	delete d_ptr;
}



// [set] 父对象
void XObject::setParent(XObject* _Parent) noexcept
{
	// 从原父对象移除
	if(d_ptr->parent)
	{
		auto&		vChildArray = d_ptr->parent->d_ptr->childArray;
		auto		vIterator = vChildArray.find(this);
		if(vIterator != vChildArray.end())
		{
			vChildArray.erase(vIterator);
		}
		d_ptr->parent = nullptr;
	}

	// 添加进新父对象
	if(_Parent)
	{
		auto&		vChildArray = _Parent->d_ptr->childArray;
		auto		vIterator = vChildArray.find(this);
		if(vIterator == vChildArray.end())
		{
			vChildArray.insert(std::set<XObject*>::value_type(this));
		}
	}

	// 更新父对象
	d_ptr->parent = _Parent;
}

// [get] 父对象
XObject* XObject::parent() const noexcept
{
	return d_ptr->parent;
}

// [set] 对象名称
void XObject::setObjectName(const XString& _Name) noexcept
{
	d_ptr->objectName = _Name;
}

// [get] 对象名称
XString XObject::objectName() const noexcept
{
	return d_ptr->objectName;
}

// [set] 用户数据
void XObject::setUserData(x_uint64_t _Key, const XVariant& _Value) noexcept
{
	auto		vIterator = d_ptr->userData.find(_Key);
	if(vIterator == d_ptr->userData.end())
	{
		d_ptr->userData.insert(std::map<x_uint64_t, XVariant>::value_type(_Key, _Value));
	}
	else
	{
		vIterator->second = _Value;
	}
}

// [get] 用户数据
XVariant XObject::userData(x_uint64_t _Key) const noexcept
{
	auto		vIterator = d_ptr->userData.find(_Key);
	if(vIterator == d_ptr->userData.end())
	{
		return {};
	}
	else
	{
		return vIterator->second;
	}
}
