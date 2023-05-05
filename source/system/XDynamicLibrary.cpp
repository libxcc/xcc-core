#include <xcc-core/system/XDynamicLibrary.h>



// constructor
XDynamicLibrary::XDynamicLibrary() noexcept = default;

// destructor
XDynamicLibrary::~XDynamicLibrary() noexcept = default;



// Load the specified dynamic link library
XDynamicLibrary::handle_type XDynamicLibrary::open(const XString& _FileName) noexcept
{
	return XDynamicLibrary::open(_FileName, RTLD_NOW);
}

// Load the specified dynamic library link library according to the flags tag
XDynamicLibrary::handle_type XDynamicLibrary::open(const XString& _FileName, int _Flags) noexcept
{
	return x_posix_dlopen(_FileName.data(), _Flags);
}

// Find the specified function in the handle
XDynamicLibrary::func_type XDynamicLibrary::find(handle_type _Handle, const XString& _FuncName) noexcept
{
	return (func_type)x_posix_dlsym(_Handle, _FuncName.data());
}

// close handle
void XDynamicLibrary::close(handle_type _Handle) noexcept
{
	if(_Handle == nullptr)
	{
		return;
	}
	x_posix_dlclose(_Handle);
}
