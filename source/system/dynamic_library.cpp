#include <xcc-core/system/dynamic_library.h>



// Load the specified dynamic link library
_XCOREAPI_ xcc::dynamic_library::handle_type __xcall__ xcc::dynamic_library::open(const XString& _FileName) noexcept
{
	return xcc::dynamic_library::open(_FileName, RTLD_NOW);
}

// Load the specified dynamic library link library according to the flags tag
_XCOREAPI_ xcc::dynamic_library::handle_type __xcall__ xcc::dynamic_library::open(const XString& _FileName, int _Flags) noexcept
{
	return x_posix_dlopen(_FileName.data(), _Flags);
}

// Find the specified function in the handle
_XCOREAPI_ xcc::dynamic_library::func_type __xcall__ xcc::dynamic_library::find(handle_type _Handle, const XString& _FuncName) noexcept
{
	return (func_type)x_posix_dlsym(_Handle, _FuncName.data());
}

// close handle
_XCOREAPI_ void __xcall__ xcc::dynamic_library::close(handle_type _Handle) noexcept
{
	if(_Handle == nullptr)
	{
		return;
	}
	x_posix_dlclose(_Handle);
}
