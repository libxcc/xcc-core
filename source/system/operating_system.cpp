#include <xcc-core/system/operating_system.h>
#include <xcc-core/system/dynamic_library.h>
#include <xcc-core/system/shell.h>
#include <xcc-core/XTernary.h>
#if defined(XCC_SYSTEM_WINDOWS)
#include <UserEnv.h>
#else
#include <sys/utsname.h>
#endif



// kernel version
_XCOREAPI_ XString __xcall__ xcc::os::kernel_version() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef void(WINAPI* _Function_GetVersionNumbers)(long*, long*, long*);
		auto		vVersion_1 = static_cast<long>(0);
		auto		vVersion_2 = static_cast<long>(0);
		auto		vVersion_3 = static_cast<long>(0);
		auto		vModule = xcc::dynamic_library::open("ntdll.dll");
		if(vModule)
		{
			auto		GetVersionNumbers = (_Function_GetVersionNumbers)xcc::dynamic_library::find(vModule, "RtlGetNtVersionNumbers");
			if(GetVersionNumbers)
			{
				GetVersionNumbers(&vVersion_1, &vVersion_2, &vVersion_3);
			}
			xcc::dynamic_library::close(vModule);
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
_XCOREAPI_ XString __xcall__ xcc::os::build_version() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef void(WINAPI* _Function_GetVersionNumbers)(long*, long*, long*);
		auto		vVersion_1 = static_cast<long>(0);
		auto		vVersion_2 = static_cast<long>(0);
		auto		vVersion_3 = static_cast<long>(0);
		auto		vModule = xcc::dynamic_library::open("ntdll.dll");
		if(vModule)
		{
			auto		GetVersionNumbers = (_Function_GetVersionNumbers)xcc::dynamic_library::find(vModule, "RtlGetNtVersionNumbers");
			if(GetVersionNumbers)
			{
				GetVersionNumbers(&vVersion_1, &vVersion_2, &vVersion_3);
			}
			xcc::dynamic_library::close(vModule);
		}
		// 18362
		static_object_example = XString::format("%ld", vVersion_3 & 0xFFFF);
#elif defined(XCC_SYSTEM_DARWIN)
		// 16G2136
		static_object_example = xcc::shell::terminal_command("sw_vers -buildVersion");
#else
		static_object_example = "Unknown";
#endif
	}
	return static_object_example;
}

// system version
_XCOREAPI_ XString __xcall__ xcc::os::system_version() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		static_object_example = xcc::os::kernel_version();
#elif defined(XCC_SYSTEM_DARWIN)
		// 10.12.6
		static_object_example = xcc::shell::terminal_command("sw_vers -productVersion");
#else
		// 20.04
		static_object_example = xcc::shell::terminal_command("lsb_release -rs");
#endif
	}
	return static_object_example;
}

// release name
_XCOREAPI_ XString __xcall__ xcc::os::release_name() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		// Microsoft Windows
		static_object_example = "Microsoft Windows";
#elif defined(XCC_SYSTEM_DARWIN)
		// Mac OS X
		static_object_example = xcc::shell::terminal_command("sw_vers -productName");
#else
		// Ubuntu
		static_object_example = xcc::shell::terminal_command("lsb_release -is");
#endif
	}
	return static_object_example;
}

// product name
_XCOREAPI_ XString __xcall__ xcc::os::product_name() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		auto		vKernelVersion = xcc::os::kernel_version();
		auto		vIsServer = xcc::os::is_server_version();
		auto		vBuildVersion = xcc::os::build_version().toULLong();

		static_object_example = xcc::os::release_name() + " ";
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
		static_object_example = xcc::os::release_name();
#endif
	}
	return static_object_example;
}

// display name
_XCOREAPI_ XString __xcall__ xcc::os::display_name() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
		auto		vProductName = xcc::os::product_name();
		auto		vBuildVersion = xcc::os::build_version();

#if defined(XCC_SYSTEM_WINDOWS)
		static_object_example = XString::format("%s (build %s) %llu-Bit", vProductName.data(), vBuildVersion.data(), xcc::os::bits());
#else
		auto		vSystemVersion = xcc::os::system_version();
		static_object_example = XString::format("%s %s (build %s) %llu-Bit", vProductName.data(), vSystemVersion.data(), vBuildVersion.data(), xcc::os::bits());
#endif
	}
	return static_object_example;
}



// Check if it is server version
_XCOREAPI_ bool __xcall__ xcc::os::is_server_version() noexcept
{
	static XTernary		vValueServer = nullptr;
	if(vValueServer == nullptr)
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef BOOL(WINAPI* func_IsWindowsServer)();
		auto		vHandle = xcc::dynamic_library::open("Kernel32.dll");
		if(vHandle)
		{
			auto		vIsWindowsServer = (func_IsWindowsServer)xcc::dynamic_library::find(vHandle, "IsWindowsServer");
			if(vIsWindowsServer)
			{
				vValueServer = vIsWindowsServer() != 0;
			}
			xcc::dynamic_library::close(vHandle);
		}
#else
		vValueServer = false;
#endif
	}
	return vValueServer == true;
}

// Check if it is desktop version
_XCOREAPI_ bool __xcall__ xcc::os::is_desktop_version() noexcept
{
	return !xcc::os::is_server_version();
}



// bits
_XCOREAPI_ x_uint64_t __xcall__ xcc::os::bits() noexcept
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
		auto		vLongBit = xcc::shell::terminal_command("getconf LONG_BIT");
		static_object_example = vTempResult.toULLong();
#endif
	}
	return static_object_example;
}

// Whether it is a 32-bit system
_XCOREAPI_ bool __xcall__ xcc::os::is_32bit() noexcept
{
	return xcc::os::bits() == 32;
}

// Whether it is a 32-bit system
_XCOREAPI_ bool __xcall__ xcc::os::is_64bit() noexcept
{
	return xcc::os::bits() == 64;
}
