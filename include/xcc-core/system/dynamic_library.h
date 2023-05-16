#ifndef			_XCC_CORE_DYNAMIC_LIBRARY_H_
#define			_XCC_CORE_DYNAMIC_LIBRARY_H_

#include <xcc-core/header.h>
#include <xcc-core/container/string.h>


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



// dynamic library
namespace xcc::dynamic_library
{
	using				handle_type = HANDLE;
	using				func_type = void*;
}

// dynamic library
namespace xcc::dynamic_library
{
	// Load the specified dynamic link library
	_XCOREAPI_ handle_type __xcall__ open(const XString& _FileName) noexcept;

	// Load the specified dynamic library link library according to the flags tag
	_XCOREAPI_ handle_type __xcall__ open(const XString& _FileName, int _Flags) noexcept;

	// Find the specified function in the handle
	_XCOREAPI_ func_type __xcall__ find(handle_type _Handle, const XString& _FuncName) noexcept;

	// close handle
	_XCOREAPI_ void __xcall__ close(handle_type _Handle) noexcept;
}


#endif
