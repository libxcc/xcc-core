#include <xcc-core/system/XOperatingSystem.h>
#include <xcc-core/system/XDynamicLibrary.h>
#include <xcc-core/XTernary.h>
#if defined(XCC_SYSTEM_WINDOWS)
#include <WtsApi32.h>
#include <UserEnv.h>
#else
#include <sys/utsname.h>
#include <xcc-core/system/XShell.h>
#endif



// constructor
XOperatingSystem::XOperatingSystem() noexcept = default;

// destructor
XOperatingSystem::~XOperatingSystem() noexcept = default;



// kernel version
XString XOperatingSystem::kernelVersion() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef void(WINAPI* _Function_GetVersionNumbers)(long*, long*, long*);
		auto		vVersion_1 = static_cast<long>(0);
		auto		vVersion_2 = static_cast<long>(0);
		auto		vVersion_3 = static_cast<long>(0);
		auto		vModule = XDynamicLibrary::open("ntdll.dll");
		if(vModule)
		{
			auto		GetVersionNumbers = (_Function_GetVersionNumbers)XDynamicLibrary::find(vModule, "RtlGetNtVersionNumbers");
			if(GetVersionNumbers)
			{
				GetVersionNumbers(&vVersion_1, &vVersion_2, &vVersion_3);
			}
			XDynamicLibrary::close(vModule);
		}
		// 10.0
		static_object_example = XString::format("%ld.%ld", vVersion_1, vVersion_2);
#else
		// Darwin:16.7.0	WSL:4.4.0-18362-Microsoft	Linux:4.4.0
		struct utsname		vVersion;
		if(0 == uname(&vVersion))
		{
			static_object_example = vVersion.release;
		}
#endif
	}
	return static_object_example;
}

// build version
XString XOperatingSystem::buildVersion() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef void(WINAPI* _Function_GetVersionNumbers)(long*, long*, long*);
		auto		vVersion_1 = static_cast<long>(0);
		auto		vVersion_2 = static_cast<long>(0);
		auto		vVersion_3 = static_cast<long>(0);
		auto		vModule = XDynamicLibrary::open("ntdll.dll");
		if(vModule)
		{
			auto		GetVersionNumbers = (_Function_GetVersionNumbers)XDynamicLibrary::find(vModule, "RtlGetNtVersionNumbers");
			if(GetVersionNumbers)
			{
				GetVersionNumbers(&vVersion_1, &vVersion_2, &vVersion_3);
			}
			XDynamicLibrary::close(vModule);
		}
		// 18362
		static_object_example = XString::format("%ld", vVersion_3 & 0xFFFF);
#elif defined(XCC_SYSTEM_DARWIN)
		// 16G2136
		static_object_example = XShell::terminalCommand("sw_vers -buildVersion");
#else
		static_object_example = "Unknown";
#endif
	}
	return static_object_example;
}

// system version
XString XOperatingSystem::systemVersion() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		static_object_example = XOperatingSystem::kernelVersion();
#elif defined(XCC_SYSTEM_DARWIN)
		// 10.12.6
		static_object_example = XShell::terminalCommand("sw_vers -productVersion");
#else
		// 20.04
		static_object_example = XShell::terminalCommand("lsb_release -rs");
#endif
	}
	return static_object_example;
}

// release name
XString XOperatingSystem::releaseName() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		// Microsoft Windows
		static_object_example = "Microsoft Windows";
#elif defined(XCC_SYSTEM_DARWIN)
		// Mac OS X
		static_object_example = XShell::terminalCommand("sw_vers -productName");
#else
		// Ubuntu
		static_object_example = XShell::terminalCommand("lsb_release -is");
#endif
	}
	return static_object_example;
}

// product name
XString XOperatingSystem::productName() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		auto		vKernelVersion = XOperatingSystem::kernelVersion();
		auto		vIsServer = XOperatingSystem::isServerVersion();
		auto		vBuildVersion = XOperatingSystem::buildVersion().toULLong();

		static_object_example = XOperatingSystem::releaseName() + " ";
		if(vKernelVersion == "10.0" || vKernelVersion == "6.4")
		{
			if(vBuildVersion >= 21996)
			{
				static_object_example += vIsServer ? "Server 2016 R2" : "11";
			}
			else
			{
				static_object_example += vIsServer ? "Server 2016" : "10";
			}
		}
		else if(vKernelVersion == "6.3")
		{
			static_object_example += vIsServer ? "Server 2012 R2" : "8.1";
		}
		else if(vKernelVersion == "6.2")
		{
			static_object_example += vIsServer ? "Server 2012" : "8";
		}
		else if(vKernelVersion == "6.1")
		{
			static_object_example += vIsServer ? "Server 2008 R2" : "7";
		}
		else if(vKernelVersion == "6.0")
		{
			static_object_example += vIsServer ? "Server 2008" : "Vista";
		}
		else
		{
			static_object_example += "Unknown";
		}
#else
		static_object_example = XOperatingSystem::releaseName();
#endif
	}
	return static_object_example;
}

// display name
XString XOperatingSystem::displayName() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
		auto		vProductName = XOperatingSystem::productName();
		auto		vBuildVersion = XOperatingSystem::buildVersion();

#if defined(XCC_SYSTEM_WINDOWS)
		static_object_example = XString::format("%s (build %s) %llu-Bit", vProductName.data(), vBuildVersion.data(), XOperatingSystem::bits());
#else
		auto		vSystemVersion = XOperatingSystem::systemVersion();
		static_object_example = XString::format("%s %s (build %s) %llu-Bit", vProductName.data(), vSystemVersion.data(), vBuildVersion.data(), XOperatingSystem::bits());
#endif
	}
	return static_object_example;
}



// 检查是否为服务端系统
bool XOperatingSystem::isServerVersion() noexcept
{
	static XTernary		vValueServer = nullptr;
	if(vValueServer == nullptr)
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef BOOL(WINAPI* func_IsWindowsServer)();
		auto		vHandle = XDynamicLibrary::open("Kernel32.dll");
		if(vHandle)
		{
			auto		vIsWindowsServer = (func_IsWindowsServer)XDynamicLibrary::find(vHandle, "IsWindowsServer");
			if(vIsWindowsServer)
			{
				vValueServer = vIsWindowsServer() != 0;
			}
			XDynamicLibrary::close(vHandle);
		}
#else
		vValueServer = false;
#endif
	}
	return vValueServer == true;
}

// 检查是否为桌面系统
bool XOperatingSystem::isDesktopVersion() noexcept
{
	return !XOperatingSystem::isServerVersion();
}



// 操作系统位数
x_uint64_t XOperatingSystem::bits() noexcept
{
	static x_uint64_t 	static_object_example = 0;
	if(static_object_example == 0)
	{
#if defined(XCC_SYSTEM_WINDOWS)
		SYSTEM_INFO	vSystemInfo;
		GetNativeSystemInfo(&vSystemInfo);
		switch(vSystemInfo.wProcessorArchitecture)
		{
			case PROCESSOR_ARCHITECTURE_AMD64:
			case PROCESSOR_ARCHITECTURE_IA64:
			case PROCESSOR_ARCHITECTURE_ALPHA64:
			case PROCESSOR_ARCHITECTURE_ARM64:
				static_object_example = 64;
				break;
			default:
				static_object_example = 32;
				break;
		}
#else
		auto		vLongBit = XShell::terminalCommand("getconf LONG_BIT");
		static_object_example = vLongBit.toULLong();
#endif
	}
	return static_object_example;
}

// 操作系统是否为32位数
bool XOperatingSystem::isBit32() noexcept
{
	return XOperatingSystem::bits() == 32;
}

// 操作系统是否为64位数
bool XOperatingSystem::isBit64() noexcept
{
	return XOperatingSystem::bits() == 64;
}



// 本机名称
XString XOperatingSystem::hostName() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	wchar_t		vHostName[XCC_PATH_MAX] = { 0 };
	DWORD		vLength = XCC_PATH_MAX;
	GetComputerNameW(vHostName, &vLength);
	return XString::fromWString(vHostName);
#else
	char		vHostName[XCC_PATH_MAX] = { 0 };
	gethostname(vHostName, XCC_PATH_MAX);
	return XString::fromUString(vHostName);
#endif
}



// 用户名称
XString XOperatingSystem::userName() noexcept
{
	static XString 		vStaticCurrentUser = nullptr;
	if(vStaticCurrentUser.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		auto		vUserProfileDirectory = XString();
		auto		vProcess = ::GetCurrentProcess();
		HANDLE		vToken = nullptr;
		auto		vSync = ::OpenProcessToken(vProcess, TOKEN_QUERY, &vToken);
		if (vSync)
		{
			DWORD		vLength = 0;
			// First call, to determine size of the strings (with '\0').
			vSync = ::GetUserProfileDirectoryW(vToken, nullptr, &vLength);
			if (!vSync && vLength != 0)
			{
				auto		vBuffer = new(std::nothrow) wchar_t[vLength];
				if(vBuffer)
				{
					vSync = ::GetUserProfileDirectoryW(vToken, vBuffer, &vLength);
					if (vSync)
					{
						vUserProfileDirectory = XString::fromWString(vBuffer);
						vUserProfileDirectory.replace("\\", "/");
						auto		vFind = vUserProfileDirectory.rfind('/');
						if(vFind != XString::npos)
						{
							vStaticCurrentUser = vUserProfileDirectory.substr(vFind + 1);
						}
					}
					delete[] vBuffer;
				}
			}
			::CloseHandle(vToken);
		}

		if(vStaticCurrentUser.empty())
		{
			LPWSTR		vInfoBuffer = nullptr;
			DWORD		vInfoLength = 0;
			auto		vQuery = WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTSUserName, &vInfoBuffer, &vInfoLength);
			if (vQuery && vInfoBuffer && vInfoLength)
			{
				vStaticCurrentUser = XString::fromWString(WString(vInfoBuffer, vInfoLength));
			}
			else
			{
				wchar_t		vUserName[XCC_SIZE_KB] = { 0 };
				DWORD		vUserLength = XCC_SIZE_KB - 1;
				GetUserNameW(vUserName, &vUserLength);
				vStaticCurrentUser = XString::fromWString(vUserName);
			}
			if(vInfoBuffer)
			{
				WTSFreeMemory(vInfoBuffer);
			}
		}
#else
		vStaticCurrentUser = XString::fromUString(getenv("USER"));
#endif
	}
	return vStaticCurrentUser;
}

// 用户主目录
XString XOperatingSystem::userHome() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	return XString::fromWString(L"C:/Users/") + XOperatingSystem::userName();
#endif
#if defined(XCC_SYSTEM_LINUX)
	return XString("/home/") + XOperatingSystem::userName();
#endif
#if defined(XCC_SYSTEM_DARWIN)
	return XString("/Users/") + XOperatingSystem::userName();
#endif
}
