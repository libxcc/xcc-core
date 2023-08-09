#ifndef			_XCC_CORE_EXCEPTION_H_
#define			_XCC_CORE_EXCEPTION_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


///  <summary>
///  Exception handling class of Xanadu
///  </summary>
class _XCOREAPI_ XException
{
private:
	// Exception description
	XString				_explain;

public:
	// constructor
	XException() noexcept;

	// constructor
	explicit XException(const char* _Explain) noexcept;

	// constructor
	explicit XException(const XString& _Explain) noexcept;

	// constructor
	XException(const XException& _Exception) noexcept;

	// destructor
	virtual ~XException() noexcept;

public:
	// operator override =
	XException& operator = (const char* _Explain) noexcept;

	// operator override =
	XException& operator = (const XString& _Explain) noexcept;

	// operator override =
	XException& operator = (const XException& _Exception) noexcept;

public:
	// [opt] 核心转储注册
	static void coreDumpRegistry() noexcept;

	// [opt] 核心转储反注册
	static void coreDumpUnregister() noexcept;

public:
	// Get exception description
	virtual const char* what() const noexcept;

public:
	// [std] range_error
	static std::range_error range_error() noexcept;

	// [std] range_error
	static std::range_error range_error(const char* _Message) noexcept;

	// [std] range_error
	static std::range_error range_error(const std::string& _Message) noexcept;

	// [std] range_error
	static std::range_error range_error(const XString& _Message) noexcept;
};

#endif
