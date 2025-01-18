#include <xcc-core/XCoreApplication.h>
#include <xcc-core/filesystem/XFileInfo.h>
#if defined(XCC_SYSTEM_DARWIN)
#include <mach-o/dyld.h>
#endif



// 实例指针
XCoreApplication*		XCoreApplication::d_self = nullptr;



// 核心应用程序的私有数据
struct XCoreApplicationPrivate
{
	std::vector<XString>			arguments;		// 参数列表
};


// constructor
XCoreApplication::XCoreApplication(int _Argc, char** _Argv) noexcept
{
	XCoreApplication::d_self = this;

	d_ptr = new(std::nothrow) XCoreApplicationPrivate();
	if(nullptr == d_ptr)
	{
		return;
	}

	// 构建参数列表
	if(_Argc > 0 && _Argv)
	{
		d_ptr->arguments.resize(_Argc);
		for(auto vIndex = 0; vIndex < _Argc; ++vIndex)
		{
			d_ptr->arguments[vIndex] = _Argv[vIndex];
		}
	}
}

// destructor
XCoreApplication::~XCoreApplication() noexcept
{
	delete d_ptr;

	XCoreApplication::d_self = nullptr;
}



// [get] 当前实例指针
XCoreApplication* XCoreApplication::example() noexcept
{
	return XCoreApplication::d_self;
}

// [get] 当前应用程序参数列表
std::vector<XString> XCoreApplication::arguments() noexcept
{
	if(XCoreApplication::d_self && XCoreApplication::d_self->d_ptr)
	{
		return XCoreApplication::d_self->d_ptr->arguments;
	}
	return {};
}



// [get] 应用程序当前位数
x_uint64_t XCoreApplication::appCurrentBit() noexcept
{
#if defined(XCC_PLATFORM_BIT_32)
	return 32;
#elif defined(XCC_PLATFORM_BIT_64)
	return 64;
#else
	return 0;
#endif
}



// [get] 应用程序文件路径
XString XCoreApplication::applicationFilePath() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		wchar_t		vDirectory[XCC_PATH_MAX] = { 0 };
		::GetModuleFileNameW(::GetModuleHandleW(nullptr), vDirectory, XCC_PATH_MAX);
		static_object_example = XFileInfo::pathToCommon(XString::fromWString(vDirectory));
#else
#if defined(XCC_SYSTEM_DARWIN)
		char		vDirectory[XCC_PATH_MAX];
		x_uint32_t	vLength = XCC_PATH_MAX;
		if(_NSGetExecutablePath(vDirectory, &vLength) != 0)
		{
			vDirectory[0] = '\0';
		}
		else
		{
			char*		vAbsolutePath = realpath(vDirectory, NULL);
			if(vAbsolutePath)
			{
				strncpy(vDirectory, vAbsolutePath, vLength);
				free(vAbsolutePath);
			}
		}
		static_object_example = XString::fromUString(vDirectory);
#else
		char		vDirectory[XCC_PATH_MAX] = { 0 };
		auto		vCount = readlink("/proc/self/exe", vDirectory, XCC_PATH_MAX);
		if(0 <= vCount || vCount <= XCC_PATH_MAX)
		{
			static_object_example = XString::fromUString(vDirectory);
		}
#endif
#endif
	}
	return static_object_example;
}

// [get] 应用程序目录路径
XString XCoreApplication::applicationDirPath() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
		const auto&	vAppFilePath = XCoreApplication::applicationFilePath();
		auto		vFind = vAppFilePath.rfind('/');
		if(vFind != XString::npos)
		{
			static_object_example = vAppFilePath.left(vFind);
		}
	}
	return static_object_example;
}

// [get] 应用程序文件名称
XString XCoreApplication::applicationFileName() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
		const auto&	vAppFilePath = XCoreApplication::applicationFilePath();
		auto		vFind = vAppFilePath.rfind('/');
		if(vFind != XString::npos)
		{
			static_object_example = vAppFilePath.substr(vFind + 1);
		}
	}
	return static_object_example;
}

// [get] 应用程序文件除去后缀的名称
XString XCoreApplication::applicationFileStem() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
		const auto&	vAppFileName = XCoreApplication::applicationFileName();
		auto		vPos = vAppFileName.rfind(".");
		if(vPos == XString::npos)
		{
			static_object_example = vAppFileName;
		}
		else
		{
			static_object_example = vAppFileName.left(vPos);
		}
	}
	return static_object_example;
}



// [set] 应用程序运行目录
XString XCoreApplication::setCurrentDirectory(const XString& _Directory) noexcept
{
	auto		vDirectory = XCoreApplication::currentDirectory();
	auto		vNewDirectory = XFileInfo::pathToNative(_Directory);
	x_posix_chdir(vNewDirectory.data());
	return vDirectory;
}

// [get] 应用程序运行目录
XString XCoreApplication::currentDirectory() noexcept
{
	char		vDirectory[XCC_PATH_MAX] = { 0 };
	x_posix_getcwd(vDirectory, XCC_PATH_MAX);
	auto		vDirPath = XFileInfo::pathToCommon(vDirectory);
	return vDirPath;
}
