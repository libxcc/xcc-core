#include <xcc-core/system/dynamic_library.h>



// constructor
XCC_NAMESPACE_PREFIX::dynamic_library::dynamic_library() noexcept = default;

// destructor
XCC_NAMESPACE_PREFIX::dynamic_library::~dynamic_library() noexcept = default;



// Load the specified dynamic link library
XCC_NAMESPACE_PREFIX::dynamic_library::handle_type XCC_NAMESPACE_PREFIX::dynamic_library::open(const XString& _FileName) noexcept
{
	return dynamic_library::open(_FileName, RTLD_NOW);
}

// Load the specified dynamic library link library according to the flags tag
XCC_NAMESPACE_PREFIX::dynamic_library::handle_type XCC_NAMESPACE_PREFIX::dynamic_library::open(const XString& _FileName, int _Flags) noexcept
{
	return x_posix_dlopen(_FileName.data(), _Flags);
}

// Find the specified function in the handle
XCC_NAMESPACE_PREFIX::dynamic_library::func_type XCC_NAMESPACE_PREFIX::dynamic_library::find(handle_type _Handle, const XString& _FuncName) noexcept
{
	return (func_type)x_posix_dlsym(_Handle, _FuncName.data());
}

// close handle
void XCC_NAMESPACE_PREFIX::dynamic_library::close(handle_type _Handle) noexcept
{
	if(_Handle == nullptr)
	{
		return;
	}
	x_posix_dlclose(_Handle);
}
