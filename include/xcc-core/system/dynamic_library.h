#ifndef			_XCC_CORE_DYNAMIC_LIBRARY_H_
#define			_XCC_CORE_DYNAMIC_LIBRARY_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// Defines a dynamic library loading flag that does not exist under Windows
#if defined(XCC_SYSTEM_WINDOWS) && !defined(RTLD_LOCAL)
#define		RTLD_LOCAL		0
#define		RTLD_LAZY		1
#define		RTLD_NOW		2
#define		RTLD_GLOBAL		4
#define		RTLD_NODELETE		8
#define		RTLD_NOLOAD		16
#define		RTLD_DEEPBIND		32
#endif



XCC_NAMESPACE_BEGIN


// Loading and processing of dynamic link library
class _XCOREAPI_ dynamic_library
{
public:
	using				handle_type = HANDLE;
	using				func_type = void*;

private:
	XCC_DISABLE_COPY(dynamic_library);
	XCC_DISABLE_MOVE(dynamic_library);

public:
	// constructor
	dynamic_library() noexcept;

	// destructor
	virtual ~dynamic_library() noexcept;

public:
	// Load the specified dynamic link library
	static handle_type open(const XString& _FileName) noexcept;

	// Load the specified dynamic library link library according to the flags tag
	static handle_type open(const XString& _FileName, int _Flags) noexcept;

	// Find the specified function in the handle
	static func_type find(handle_type _Handle, const XString& _FuncName) noexcept;

	// close handle
	static void close(handle_type _Handle) noexcept;
};



XCC_NAMESPACE_END

typedef XCC_NAMESPACE_PREFIX::dynamic_library		XDynamicLibrary;

#endif
