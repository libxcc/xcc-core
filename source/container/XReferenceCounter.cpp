#include <xcc-core/container/XReferenceCounter.h>



// constructor
XReferenceCounter::XReferenceCounter() noexcept
{
	_M_ref_count = 0;
}

// constructor
XReferenceCounter::XReferenceCounter(x_uint64_t _Count) noexcept
{
	_M_ref_count = _Count;
}



// 引用计数
x_uint64_t XReferenceCounter::refCount() const noexcept
{
	XCC_MUTEX_LOCKER(_M_ref_mutex);
	return _M_ref_count;
}

// 是否独占资源
bool XReferenceCounter::unique() const noexcept
{
	XCC_MUTEX_LOCKER(_M_ref_mutex);
	return _M_ref_count == 1;
}

// 增加计数
void XReferenceCounter::increase() noexcept
{
	XCC_MUTEX_LOCKER(_M_ref_mutex);
	++_M_ref_count;
}

// 减少计数
void XReferenceCounter::reduce() noexcept
{
	XCC_MUTEX_LOCKER(_M_ref_mutex);
	--_M_ref_count;
}
