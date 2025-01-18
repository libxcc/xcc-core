#include <xcc-core/system/XDynamicLibrary.h>



// constructor
XDynamicLibrary::XDynamicLibrary() noexcept = default;

// destructor
XDynamicLibrary::~XDynamicLibrary() noexcept = default;



// 打开动态链接库文件
XDynamicLibrary::handle_type XDynamicLibrary::open(const XString& _FileName) noexcept
{
	return XDynamicLibrary::open(_FileName, RTLD_NOW);
}

// 以指定标记打开动态链接库文件
XDynamicLibrary::handle_type XDynamicLibrary::open(const XString& _FileName, int _Flags) noexcept
{
	return x_posix_dlopen(_FileName.data(), _Flags);
}

// 在动态链接库句柄中查找函数
XDynamicLibrary::func_type XDynamicLibrary::find(handle_type _Handle, const XString& _FuncName) noexcept
{
	return (func_type)x_posix_dlsym(_Handle, _FuncName.data());
}

// 关闭动态链接库句柄
void XDynamicLibrary::close(handle_type _Handle) noexcept
{
	if(nullptr == _Handle)
	{
		return;
	}
	x_posix_dlclose(_Handle);
}

// 最近一次的错误消息
XString XDynamicLibrary::error() noexcept
{
	return x_posix_dlerror();
}
