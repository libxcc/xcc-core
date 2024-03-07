#ifndef			_XCC_CORE_OBJECT_H_
#define			_XCC_CORE_OBJECT_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>
#include <xcc-core/XVariant.h>


// XCC - 基础对象 - 私有数据
struct XObjectPrivate;
typedef struct XObjectPrivate			XObjectPrivate;

// XCC - 基础对象
class _XCOREAPI_ XObject
{
private:
	// 私有数据
	XObjectPrivate*				d_ptr;

private:
	XCC_DISABLE_COPY(XObject);
	XCC_DISABLE_MOVE(XObject);

public:
	// constructor
	explicit XObject(XObject* _Parent) noexcept;

	// destructor
	virtual ~XObject() noexcept;

public:
	// [set] 父对象
	virtual void setParent(XObject* _Parent) noexcept final;

	// [get] 父对象
	virtual XObject* parent() const noexcept final;

	// [set] 对象名称
	virtual void setObjectName(const XString& _Name) noexcept final;

	// [get] 对象名称
	virtual XString objectName() const noexcept final;

	// [set] 用户数据
	virtual void setUserData(x_uint64_t _Key, const XVariant& _Value) noexcept final;

	// [get] 用户数据
	virtual XVariant userData(x_uint64_t _Key) const noexcept final;
};

#endif
