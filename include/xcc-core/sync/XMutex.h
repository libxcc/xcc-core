#ifndef			_XCC_CORE_MUTEX_H_
#define			_XCC_CORE_MUTEX_H_

#include <xcc-core/header.h>

///  <summary>
///  Implementation of cross platform XMutex
///  </summary>
class _XCOREAPI_ XMutex
{
public:
	// Types of XMutex
	enum RecursionMode
	{
		Recursive,
		NonRecursive,
	};

private:
	// Private data of XMutex
	HANDLE				_mutex_data;

	// Types of locks
	RecursionMode			_mutex_mode;

public:
	// constructor
	explicit XMutex(RecursionMode _Mode = Recursive) noexcept;

	// destructor
	virtual ~XMutex() noexcept;

public:
	// Gets the type of the lock
	virtual RecursionMode type() const noexcept;

public:
	// Lock
	virtual void lock() noexcept;

	// Unlock
	virtual void unlock() noexcept;
};



///  <summary>
///  A class that can release XMutex automatically
///  </summary>
class _XCOREAPI_ XMutexLocker
{
private:
	XMutex&				_data_mutex;

public:
	// constructor
	explicit XMutexLocker(XMutex& _Mutex) noexcept;

	// destructor
	virtual ~XMutexLocker() noexcept;
};



// Macro used to call auto destruct lock conveniently
#define				XCC_MUTEX_LOCKER(_Mutex)		XMutexLocker			xcc_mutex_locker(_Mutex)
#define				XCC_MUTEX_GUARD(_Mutex)			std::lock_guard<std::mutex>	std_mutex_locker(_Mutex)

#endif
