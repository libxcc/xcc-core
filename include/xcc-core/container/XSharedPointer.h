#ifndef			_XCC_CORE_SHARED_POINTER_H_
#define			_XCC_CORE_SHARED_POINTER_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XReferenceCounter.h>


// Core: 智能指针
template <class _Ty>
class _XCOREAPI_ XSharedPointer
{
public:
	using				_Dy = void(*)(_Ty*);

private:
	XReferenceCounter*		_M_ref_count;
	_Ty*				_M_origin_ptr;
	_Dy				_M_res_delete;

public:
	// constructor
	XSharedPointer() noexcept
	{
		_M_ref_count = nullptr;
		_M_origin_ptr = nullptr;
		_M_res_delete = nullptr;
	}

	// constructor
	XSharedPointer(std::nullptr_t)noexcept : XSharedPointer()
	{
	}

	// constructor
	template <typename _Dy>
	XSharedPointer(std::nullptr_t, _Dy _Delete)noexcept : XSharedPointer()
	{
		_M_res_delete = _Delete;
	}

	// constructor
	explicit XSharedPointer(_Ty* _Pointer) noexcept
	{
		_M_origin_ptr = _Pointer;
		_M_ref_count = nullptr;
		_M_res_delete = nullptr;
		if (_M_origin_ptr != nullptr)
		{
			_M_ref_count = new(std::nothrow) XReferenceCounter(1);
		}
	}

	// constructor
	template <typename _Dy>
	XSharedPointer(_Ty* _Pointer, _Dy _Delete) noexcept
	{
		_M_origin_ptr = _Pointer;
		_M_ref_count = nullptr;
		_M_res_delete = _Delete;
		if (_M_origin_ptr != nullptr)
		{
			_M_ref_count = new(std::nothrow) XReferenceCounter(1);
		}
	}

	// constructor
	XSharedPointer(const XSharedPointer& _Right) noexcept
	{
		_M_origin_ptr = _Right._M_origin_ptr;
		_M_ref_count = _Right._M_ref_count;
		_M_res_delete = _Right._M_res_delete;
		if (_M_origin_ptr != nullptr)
		{
			_M_ref_count->increase();
		}
	}

	// constructor
	XSharedPointer(XSharedPointer&& _Right) noexcept
	{
		_M_origin_ptr = _Right._M_origin_ptr;
		_M_ref_count = _Right._M_ref_count;
		_M_res_delete = _Right._M_res_delete;

		_Right._M_origin_ptr = nullptr;
		_Right._M_ref_count = nullptr;
		_Right._M_res_delete = nullptr;
	}

	// destructor
	virtual ~XSharedPointer() noexcept
	{
		if (_M_origin_ptr != nullptr && _M_ref_count->refCount() == 0)
		{
			if(_M_res_delete)
			{
				_M_res_delete(_M_origin_ptr);
			}
			else
			{
				delete _M_origin_ptr;
			}
			delete _M_ref_count;
			_M_origin_ptr = nullptr;
			_M_ref_count = nullptr;
		}
	}

public:
	// operator override =
	XSharedPointer& operator = (const XSharedPointer& _Right)noexcept
	{
		if(this != &_Right)
		{
			SharedPtr(_Right).swap(*this);
		}
		return *this;
	}

	// operator override =
	XSharedPointer& operator = (XSharedPointer&& _Right)noexcept
	{
		if(this != &_Right)
		{
			XSharedPointer(std::move(_Right)).swap(*this);
		}
		return *this;
	}

public:
	// operator override bool
	explicit operator bool() const noexcept
	{
		return _M_origin_ptr != nullptr;
	}

	// operator override ->
	_Ty* operator->() const noexcept
	{
		return _M_origin_ptr;
	}

	// operator override *
	_Ty& operator*() const noexcept
	{
		return *_M_origin_ptr;
	}

public:
	// 获取原始指针对象
	_Ty* get()const noexcept
	{
		return _M_origin_ptr;
	}

	// 获取引用计数
	int useCount() const noexcept
	{
		return _M_origin_ptr == nullptr ? 0 : _M_ref_count->refCount();
	}

	// 重置指针
	void reset() noexcept
	{
		XSharedPointer().swap(*this);
	}

	// 重置指针
	void reset(std::nullptr_t) noexcept
	{
		reset();
	}

	// 重置指针
	template <typename _Dy>
	void reset(std::nullptr_t, _Dy _Delete) noexcept
	{
		reset(nullptr);
		_M_res_delete = _Delete;
	}

	// 重置指针
	void reset(_Ty* _Pointer)noexcept
	{
		SharedPtr(_Pointer).swap(*this);
	}

	// 重置指针
	template <typename _Dy>
	void reset(_Ty* _Pointer, _Dy _Delete)noexcept
	{
		SharedPtr(_Pointer, _Delete).swap(*this);
	}

	// 是否独占资源
	bool unique()noexcept
	{
		return _M_ref_count != nullptr && _M_ref_count->unique();
	}

	// 交换对象内容
	void swap(XSharedPointer& _Right) noexcept
	{
		auto		T_origin_ptr = _M_origin_ptr;
		auto		T_ref_count = _M_ref_count;
		auto		T_res_delete = _M_res_delete;

		_M_origin_ptr = _Right._M_origin_ptr;
		_M_ref_count = _Right._M_ref_count;
		_M_res_delete = _Right._M_res_delete;

		_Right._M_origin_ptr = T_origin_ptr;
		_Right._M_ref_count = T_ref_count;
		_Right._M_res_delete = T_res_delete;
	}

public:
	// 创建一个对象
	static XSharedPointer<_Ty> make(std::nullptr_t)noexcept
	{
		return new(std::nothrow) XSharedPointer<_Ty>(nullptr);
	}

	// 创建一个对象
	template <typename _Dy>
	static XSharedPointer<_Ty> make(std::nullptr_t, _Dy _Delete)noexcept
	{
		return new(std::nothrow) XSharedPointer<_Ty>(nullptr, _Delete);
	}

	// 创建一个对象
	static XSharedPointer<_Ty> make(_Ty* _Pointer) noexcept
	{
		return new(std::nothrow) XSharedPointer<_Ty>(_Pointer);
	}

	// 创建一个对象
	template <typename _Dy>
	static XSharedPointer<_Ty> make(_Ty* _Pointer, _Dy _Delete) noexcept
	{
		return new(std::nothrow) XSharedPointer<_Ty>(_Pointer, _Delete);
	}
};


#endif
