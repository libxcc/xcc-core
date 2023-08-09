#ifndef			_XCC_CORE_REFERENCE_COUNTER_H_
#define			_XCC_CORE_REFERENCE_COUNTER_H_

#include <xcc-core/header.h>
#include <xcc-core/sync/XMutex.h>


// Core: 引用计数器
class _XCOREAPI_ XReferenceCounter
{
private:
	mutable XMutex			_M_ref_mutex;
	x_uint64_t			_M_ref_count;

public:
	// constructor
	XReferenceCounter() noexcept;

	// constructor
	XReferenceCounter(x_uint64_t _Count) noexcept;

	// constructor
	XReferenceCounter(const XReferenceCounter& _Right) noexcept = default;

	// constructor
	XReferenceCounter(XReferenceCounter&& _Right) noexcept = default;

	// destructor
	virtual ~XReferenceCounter() noexcept = default;

public:
	// operator override =
	XReferenceCounter& operator = (const XReferenceCounter& _Right) noexcept = default;

	// operator override =
	XReferenceCounter& operator = (XReferenceCounter&& _Right) noexcept = default;

public:
	// 引用计数
	virtual x_uint64_t refCount() const noexcept final;

	// 是否独占资源
	virtual bool unique() const noexcept final;

	// 增加计数
	virtual void increase() noexcept final;

	// 减少计数
	virtual void reduce() noexcept final;
};


#endif
