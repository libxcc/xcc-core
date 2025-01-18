#include <xcc-core/filesystem/XStandardPath.h>
#include <xcc-core/XCoreApplication.h>
#include <xcc-core/system/XOperatingSystem.h>
#include <xcc-core/filesystem/XFolder.h>



// constructor
XStandardPath::XStandardPath() noexcept = default;

// destructor
XStandardPath::~XStandardPath() noexcept = default;



// 用户的主目录
XString XStandardPath::homeLocation() noexcept
{
	return XOperatingSystem::userHome();
}

// 用户的桌面目录
XString XStandardPath::desktopLocation() noexcept
{
	return XStandardPath::homeLocation() + "/Desktop";
}

// 用户的文档目录
XString XStandardPath::documentsLocation() noexcept
{
	return XStandardPath::homeLocation() + "/Documents";
}

// 用户的下载目录
XString XStandardPath::downloadLocation() noexcept
{
	return XStandardPath::homeLocation() + "/Downloads";
}



// 应用程序XCC目录名称 - com.libxcc.[app]
XString XStandardPath::appStandardName() noexcept
{
	static auto	static_object_example = XString();
	if(nullptr == static_object_example)
	{
		auto		vAppPrefix = XString("com.libxcc.");
		auto		vAppStem = XCoreApplication::applicationFileStem().toLower();
		static_object_example = vAppPrefix + vAppStem;
	}
	return static_object_example;
}

// 应用程序的配置目录
XString XStandardPath::appConfigLocation() noexcept
{
	auto		vPath = XStandardPath::systemConfigLocation() + "/" + appStandardName();
	XFolder::mkpath(vPath);
	return vPath;
}

// 应用程序的数据目录
XString XStandardPath::appDataLocation() noexcept
{
	auto		vPath = XStandardPath::systemDataLocation() + "/" + appStandardName();
	XFolder::mkpath(vPath);
	return vPath;
}

// 应用程序的缓存目录
XString XStandardPath::appCacheLocation() noexcept
{
	auto		vPath = XStandardPath::systemCacheLocation() + "/" + appStandardName();
	XFolder::mkpath(vPath);
	return vPath;
}

// 应用程序的临时目录
XString XStandardPath::appTempLocation() noexcept
{
	auto		vPath = XStandardPath::systemTempLocation() + "/" + appStandardName();
	XFolder::mkpath(vPath);
	return vPath;
}

// 应用程序的Env目录
XString XStandardPath::appEnvLocation() noexcept
{
	static auto	static_object_example = XString();
	if(nullptr == static_object_example)
	{
		auto		vApplicationName = XCoreApplication::applicationFileStem().toLower();
#if defined(XCC_SYSTEM_DARWIN)
		auto		vDirectory = XStandardPath::homeLocation() + "/.config" + "/com.xanadu." + vApplicationName;
#else
		auto		vDirectory = systemConfigLocation() + "/com.xanadu." + vApplicationName;
#endif
		XFolder::mkpath(vDirectory);
		static_object_example = vDirectory;
	}
	return static_object_example;
}



// 系统提供的配置目录
XString XStandardPath::systemConfigLocation() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPath = XStandardPath::homeLocation() + "/AppData/Local";
#endif
#if defined(XCC_SYSTEM_LINUX)
	auto		vPath = XStandardPath::homeLocation() + "/.config";
#endif
#if defined(XCC_SYSTEM_DARWIN)
	auto		vPath = XStandardPath::homeLocation() + "/Library/Preferences";
#endif

	XFolder::mkpath(vPath);
	return vPath;
}

// 系统提供的数据目录
XString XStandardPath::systemDataLocation() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPath = XString("C:/ProgramData");
#endif
#if defined(XCC_SYSTEM_LINUX)
	auto		vPath = XStandardPath::homeLocation() + "/.local/share";
#endif
#if defined(XCC_SYSTEM_DARWIN)
	auto		vPath = XStandardPath::homeLocation() + "/Library/Application Support";
#endif

	XFolder::mkpath(vPath);
	return vPath;
}

// 系统提供的缓存目录
XString XStandardPath::systemCacheLocation() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPath = XStandardPath::homeLocation() + "/AppData/Local/cache";
#endif
#if defined(XCC_SYSTEM_LINUX)
	auto		vPath = XStandardPath::homeLocation() + "/.cache";
#endif
#if defined(XCC_SYSTEM_DARWIN)
	auto		vPath = XStandardPath::homeLocation() + "/Library/Caches";
#endif

	XFolder::mkpath(vPath);
	return vPath;
}

// 系统提供的临时目录
XString XStandardPath::systemTempLocation() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPath = XStandardPath::homeLocation() + "/AppData/Local/Temp";
#endif
#if defined(XCC_SYSTEM_LINUX)
	auto		vPath = XString("/tmp");
#endif
#if defined(XCC_SYSTEM_DARWIN)
	auto		vPath = XString("/private/tmp");
#endif

	XFolder::mkpath(vPath);
	return vPath;
}

// 系统提供的自启动目录
XString XStandardPath::systemAutorunLocation() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPath = XStandardPath::homeLocation() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup";
#endif
#if defined(XCC_SYSTEM_LINUX)
	auto		vPath = XString("/etc/profile.d");
#endif
#if defined(XCC_SYSTEM_DARWIN)
	auto		vPath = XStandardPath::homeLocation() + "/Library/LaunchAgents";
#endif

	XFolder::mkpath(vPath);
	return vPath;
}



// 返回指定类型的目录位置
XString XStandardPath::standardLocations(XStandardPath::StandardLocation _Type) noexcept
{
	switch (_Type)
	{
		case HomeLocation:			return homeLocation();
		case DesktopLocation:			return desktopLocation();
		case DocumentsLocation:			return documentsLocation();
		case DownloadLocation:			return downloadLocation();

		case AppDataLocation:			return appDataLocation();
		case AppConfigLocation:			return appConfigLocation();
		case AppCacheLocation:			return appCacheLocation();
		case AppTempLocation:			return appTempLocation();
		case AppEnvLocation:			return appEnvLocation();

		case SystemConfigLocation:		return systemConfigLocation();
		case SystemDataLocation:		return systemDataLocation();
		case SystemCacheLocation:		return systemCacheLocation();
		case SystemTempLocation:		return systemTempLocation();
		case SystemAutorunLocation:		return systemAutorunLocation();

		default:				return nullptr;
	}
}
