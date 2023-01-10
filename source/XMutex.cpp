#include <xcc-core/XMutex.h>


// constructor
XMutex::XMutex(RecursionMode _Mode) noexcept
{
	this->_mutex_mode = _Mode;
#if defined(XCC_SYSTEM_WINDOWS)
	XCC_UNUSED(_Mode);
	this->_mutex_data = new(std::nothrow) CRITICAL_SECTION;
	XCC_CHECK_RETURN(_mutex_data);
	InitializeCriticalSection(static_cast<CRITICAL_SECTION*>(this->_mutex_data));
#else
	this->_mutex_data = new(std::nothrow) pthread_mutex_t;
	XCC_CHECK_RETURN(this->_mutex_data);

	switch(_Mode)
	{
		case XMutex::Recursive:
			{
				pthread_mutexattr_t	vAttr;
				pthread_mutexattr_init(&vAttr);
				pthread_mutexattr_settype(&vAttr, PTHREAD_MUTEX_RECURSIVE);
				pthread_mutex_init(static_cast<pthread_mutex_t*>(this->_mutex_data), &vAttr);
			}
			break;
		case XMutex::NonRecursive:
			{
				pthread_mutex_init(static_cast<pthread_mutex_t*>(this->_mutex_data), nullptr);
			}
			break;
		default:
			break;
	}
#endif
}

// destructor
XMutex::~XMutex() noexcept
{
	XCC_CHECK_RETURN(this->_mutex_data);
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vMutex = static_cast<CRITICAL_SECTION*>(this->_mutex_data);
	DeleteCriticalSection(vMutex);
	XCC_DELETE_PTR(vMutex);
#else
	auto		vMutex = static_cast<pthread_mutex_t*>(this->_mutex_data);
	pthread_mutex_destroy(vMutex);
	XCC_DELETE_PTR(vMutex);
#endif
	this->_mutex_data = nullptr;
}



// Gets the type of the lock
XMutex::RecursionMode XMutex::type() const noexcept
{
	return this->_mutex_mode;
}



// Lock
void XMutex::lock() noexcept
{
	XCC_CHECK_RETURN(this->_mutex_data);
#if defined(XCC_SYSTEM_WINDOWS)
	EnterCriticalSection(static_cast<CRITICAL_SECTION*>(this->_mutex_data));
#else
	pthread_mutex_lock(static_cast<pthread_mutex_t*>(this->_mutex_data));
#endif
}

// Unlock
void XMutex::unlock() noexcept
{
	XCC_CHECK_RETURN(this->_mutex_data);
#if defined(XCC_SYSTEM_WINDOWS)
	LeaveCriticalSection(static_cast<CRITICAL_SECTION*>(this->_mutex_data));
#else
	pthread_mutex_unlock(static_cast<pthread_mutex_t*>(this->_mutex_data));
#endif
}





// constructor
XMutexAuto::XMutexAuto(XMutex& _Mutex) noexcept : _data_mutex(_Mutex)
{
	this->_data_mutex.lock();
}

// destructor
XMutexAuto::~XMutexAuto() noexcept
{
	this->_data_mutex.unlock();
}
