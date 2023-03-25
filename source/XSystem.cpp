#include <xcc-core/XSystem.h>
#include <xcc-core/XHash.h>
#include <xcc-core/XLibrary.h>
#include <xcc-core/XShell.h>
#include <xcc-core/XTernary.h>
#include <xcc-core/XUuid.h>
#if defined(XCC_SYSTEM_WINDOWS)
#include <WtsApi32.h>
#include <userenv.h>
#else
#include <sys/utsname.h>
#endif


//static system version value
static std::int64_t					_StaticSystemVersion = 0;


//initialize
XSystem::XSystem() noexcept = default;

//release
XSystem::~XSystem() noexcept = default;



// The current version of the operating system
std::int64_t XSystem::SystemVersion() noexcept
{
	if(!_StaticSystemVersion)
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef void(WINAPI* _Function_GetVersionNumbers)(long*, long*, long*);
		auto		vVersion_1 = static_cast<long>(0);
		auto		vVersion_2 = static_cast<long>(0);
		auto		vVersion_3 = static_cast<long>(0);
		auto		vIsServer = isServer();
		auto		vModule = XLibrary::dllOpen("ntdll.dll");
		if(vModule)
		{
			auto		GetVersionNumbers = (_Function_GetVersionNumbers) XLibrary::dllFind(vModule, "RtlGetNtVersionNumbers");
			if(GetVersionNumbers)
			{
				GetVersionNumbers(&vVersion_1, &vVersion_2, &vVersion_3);
				switch(vVersion_1)
				{
					case 10:
						_StaticSystemVersion = vIsServer ? XCC_OS_VERSION_WINDOWS_2016 : XCC_OS_VERSION_WINDOWS_10;
						break;
					case 6:
						switch(vVersion_2)
						{
							case 0:
								_StaticSystemVersion = vIsServer ? XCC_OS_VERSION_WINDOWS_2008 : XCC_OS_VERSION_WINDOWS_VISTA;
								break;
							case 1:
								_StaticSystemVersion = vIsServer ? XCC_OS_VERSION_WINDOWS_2008_R2 : XCC_OS_VERSION_WINDOWS_7;
								break;
							case 2:
								_StaticSystemVersion = vIsServer ? XCC_OS_VERSION_WINDOWS_2012 : XCC_OS_VERSION_WINDOWS_8;
								break;
							case 3:
								_StaticSystemVersion = vIsServer ? XCC_OS_VERSION_WINDOWS_2012_R2 : XCC_OS_VERSION_WINDOWS_8_1;
								break;
							default:
								break;
						}
						break;
					default:
						_StaticSystemVersion = XCC_OS_VERSION_UNKNOWN;
						break;
				}
			}
			XLibrary::dllClose(vModule);
		}
#endif
#if defined(XCC_SYSTEM_LINUX)
		//XCC_PLATFORM_NOSUPPORT;
#endif
#if defined(XCC_SYSTEM_DARWIN)
		struct utsname		vVersion;
		if(0 == uname(&vVersion))
		{
			auto	vValue1 = x_posix_atoi(vVersion.release);
			switch(vValue1)
			{
				case 12:
					_StaticSystemVersion = XCC_OS_VERSION_MACOS_1012;
					break;
				case 13:
					_StaticSystemVersion = XCC_OS_VERSION_MACOS_1013;
					break;
				case 14:
					_StaticSystemVersion = XCC_OS_VERSION_MACOS_1014;
					break;
				case 15:
					_StaticSystemVersion = XCC_OS_VERSION_MACOS_1015;
					break;
				case 16:
					_StaticSystemVersion = XCC_OS_VERSION_MACOS_1100;
					break;
				default:
					_StaticSystemVersion = XCC_OS_VERSION_UNKNOWN;
					break;
			}
		}
#endif
	}
	return _StaticSystemVersion;
}

// The name of the user who is now logged in
XString XSystem::currentUser() noexcept
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

// Computer name
XString XSystem::hostName() noexcept
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

// Gets the directory for the current user
XString XSystem::userHome() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	return XString::fromWString(L"C:/Users/") + XSystem::currentUser();
#endif
#if defined(XCC_SYSTEM_LINUX)
	return XString("/home/") + XSystem::currentUser();
#endif
#if defined(XCC_SYSTEM_DARWIN)
	return XString("/Users/") + XSystem::currentUser();
#endif
}

// Whether the operating system is 32-bit
bool XSystem::is_32bit() noexcept
{
	return !XSystem::is_64bit();
}

// Whether the operating system is 64-bit
bool XSystem::is_64bit() noexcept
{
	static XTernary		vValue64Bit = nullptr;
	if(vValue64Bit == nullptr)
	{
#if defined(XCC_SYSTEM_WINDOWS)
		SYSTEM_INFO	vSystemInfo;
		GetNativeSystemInfo(&vSystemInfo);
		switch(vSystemInfo.wProcessorArchitecture)
		{
			case PROCESSOR_ARCHITECTURE_AMD64:
			case PROCESSOR_ARCHITECTURE_IA64:
			case PROCESSOR_ARCHITECTURE_ALPHA64:
				vValue64Bit = true;
				break;
			default:
				vValue64Bit = false;
				break;
		}
#else
		vValue64Bit = true;
#endif
	}
	return vValue64Bit == true;
}

// Whether the operating system is a server version
bool XSystem::isServer() noexcept
{
	static XTernary		vValueServer = nullptr;
	if(vValueServer == nullptr)
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef BOOL(WINAPI* _Function_IsWindowsServer)();
		auto		vHandle = XLibrary::dllOpen("Kernel32.dll");
		if(vHandle)
		{
			auto		vIsWindowsServer = (_Function_IsWindowsServer) XLibrary::dllFind(vHandle, "IsWindowsServer");
			if(vIsWindowsServer)
			{
				vValueServer = vIsWindowsServer() != 0;
			}
			XLibrary::dllClose(vHandle);
		}
#else
		vValueServer = false;
#endif
	}
	return vValueServer == true;
}

// Whether the operating system is a desktop version
bool XSystem::isDesktop() noexcept
{
	return !XSystem::isServer();
}

// Native system String
XString XSystem::nativeString() noexcept
{
	static XString			_StaticNativeString;
	if (_StaticNativeString.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		auto		vSystemString = XString("Microsoft Windows ");
#endif
#if defined(XCC_SYSTEM_LINUX)
		auto		vSystemString = XString("Linux ");
#endif
#if defined(XCC_SYSTEM_DARWIN)
		auto		vSystemString = XString("Mac OS ");
#endif
		//拼接系统版本
		switch(XSystem::SystemVersion())
		{
#if defined(XCC_SYSTEM_WINDOWS)
			case XCC_OS_VERSION_WINDOWS_VISTA:
				vSystemString += "Vista";
				break;
			case XCC_OS_VERSION_WINDOWS_2008:
				vSystemString += "Server 2008";
				break;
			case XCC_OS_VERSION_WINDOWS_7:
				vSystemString += "7";
				break;
			case XCC_OS_VERSION_WINDOWS_2008_R2:
				vSystemString += "Server 2008 R2";
				break;
			case XCC_OS_VERSION_WINDOWS_8:
				vSystemString += "8";
				break;
			case XCC_OS_VERSION_WINDOWS_2012:
				vSystemString += "Server 2012";
				break;
			case XCC_OS_VERSION_WINDOWS_8_1:
				vSystemString += "8.1";
				break;
			case XCC_OS_VERSION_WINDOWS_2012_R2:
				vSystemString += "Server 2012 R2";
				break;
			case XCC_OS_VERSION_WINDOWS_10:
				vSystemString += "10";
				break;
			case XCC_OS_VERSION_WINDOWS_2016:
				vSystemString += "Server 2016";
				break;
#endif
#if defined(XCC_SYSTEM_LINUX)
			case XCC_OS_VERSION_LINUX_1404:
				vSystemString += "Linux 14.04";
				break;
			case XCC_OS_VERSION_LINUX_1410:
				vSystemString += "Linux 14.10";
				break;
			case XCC_OS_VERSION_LINUX_1504:
				vSystemString += "Linux 15.04";
				break;
			case XCC_OS_VERSION_LINUX_1510:
				vSystemString += "Linux 15.10";
				break;
			case XCC_OS_VERSION_LINUX_1604:
				vSystemString += "Linux 16.04";
				break;
			case XCC_OS_VERSION_LINUX_1610:
				vSystemString += "Linux 16.10";
				break;
			case XCC_OS_VERSION_LINUX_1704:
				vSystemString += "Linux 17.04";
				break;
			case XCC_OS_VERSION_LINUX_1710:
				vSystemString += "Linux 17.10";
				break;
			case XCC_OS_VERSION_LINUX_1804:
				vSystemString += "Linux 18.04";
				break;
			case XCC_OS_VERSION_LINUX_1810:
				vSystemString += "Linux 18.10";
				break;
			case XCC_OS_VERSION_LINUX_1904:
				vSystemString += "Linux 19.04";
				break;
			case XCC_OS_VERSION_LINUX_1910:
				vSystemString += "Linux 19.10";
				break;
			case XCC_OS_VERSION_LINUX_2004:
				vSystemString += "Linux 2004";
				break;
			case XCC_OS_VERSION_LINUX_2010:
				vSystemString += "Linux 2010";
				break;
			case XCC_OS_VERSION_LINUX_2104:
				vSystemString += "Linux 2104";
				break;
			case XCC_OS_VERSION_LINUX_2110:
				vSystemString += "Linux 2110";
				break;
#endif
#if defined(XCC_SYSTEM_DARWIN)
			case XCC_OS_VERSION_MACOS_1012:
				vSystemString += "10.12 Sierra";
				break;
			case XCC_OS_VERSION_MACOS_1013:
				vSystemString += "10.13 High Sierra";
				break;
			case XCC_OS_VERSION_MACOS_1014:
				vSystemString += "10.14 Mojave";
				break;
			case XCC_OS_VERSION_MACOS_1015:
				vSystemString += "10.15 Catalina";
				break;
			case XCC_OS_VERSION_MACOS_1100:
				vSystemString += "11.00 Big Sur";
				break;
			case XCC_OS_VERSION_MACOS_1101:
				vSystemString += "11.01 Big Sur";
				break;
#endif
			default:
				vSystemString += "Unknown";
				break;
		}
		//拼接编译版本
		vSystemString += " (build ";
		vSystemString += XSystem::buildVersion();
		vSystemString += ")";

		//拼接系统位数
		if(XSystem::is_64bit())
		{
			vSystemString += ",64-Bit";
		}
		else
		{
			vSystemString += ",32-Bit";
		}
		_StaticNativeString = vSystemString;
	}
	return _StaticNativeString;
}

// Native Build Version
XString XSystem::buildVersion() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vBuildVersion = XString();
	auto		vModule = XLibrary::dllOpen("ntdll.dll");
	if(vModule)
	{
		auto		vValue1 = static_cast<long>(0);
		auto		vValue2 = static_cast<long>(0);
		auto		vValue3 = static_cast<long>(0);
		typedef void(WINAPI* LP_GetVersionNumbers)(long*, long*, long*);
		auto		GetVersionNumbers = (LP_GetVersionNumbers)XLibrary::dllFind(vModule, "RtlGetNtVersionNumbers");
		if(GetVersionNumbers)
		{
			GetVersionNumbers(&vValue1, &vValue2, &vValue3);
			vBuildVersion = XString::format("%ld", vValue3 & 0xFFFF);

		}
		XLibrary::dllClose(vModule);
	}
	return vBuildVersion;
#endif
#if defined(XCC_SYSTEM_LINUX)
	return XString("Unknown");
#endif
#if defined(XCC_SYSTEM_DARWIN)
	return XString("Unknown");
#endif
}



// [get] 系统唯一ID
XString XSystem::machineId() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
#if defined(XCC_SYSTEM_WINDOWS)
		HKEY		vKey = nullptr;
		LSTATUS		vStatus = ERROR_SUCCESS;
		if(XSystem::is_64bit())
		{
			vStatus = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", NULL, KEY_READ | KEY_WOW64_64KEY, &vKey);
		}
		else
		{
			vStatus = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", NULL, KEY_READ, &vKey);
		}
		if(ERROR_SUCCESS == vStatus)
		{
			wchar_t		vBuffer[MAX_PATH] = { 0 };
			DWORD		vLength = MAX_PATH;
			DWORD		vType = REG_SZ;
			x_posix_memset(vBuffer, 0, sizeof(wchar_t) * MAX_PATH);
			LSTATUS		vQuery = RegQueryValueExW(vKey, L"MachineGuid", nullptr, &vType, (byte*)vBuffer, &vLength);
			if(vQuery == ERROR_SUCCESS)
			{
				static_object_example = XUuid::fromString(XString::fromWString(vBuffer)).toString(XUuid::WithoutBraces);
			}
			RegCloseKey(vKey);
		}
#endif
#if defined(XCC_SYSTEM_LINUX)
		const char*	vCommandMachineId = "cat /etc/machine-id";
		XString		vCommandResult = nullptr;
		XShell::run(vCommandMachineId, [&](const XString& _Output)->bool
		{
			vCommandResult += _Output;
			return true;
		});
		if(vCommandResult.size() == 32)
		{
			static_object_example = XUuid::fromString(vCommandResult).toString(XUuid::WithoutBraces);
		}
#endif
#if defined(XCC_SYSTEM_DARWIN)
		const char*	vCommandKernUuid = "sysctl kern.uuid";
		XString		vCommandResult = nullptr;
		XShell::run(vCommandKernUuid, [&](const XString& _Output)->bool
		{
			vCommandResult += _Output;
			return true;
		});

		auto		vFindBegin = x_posix_strlen("kern.uuid: ");
		if(vCommandResult.size() == vFindBegin + 36)
		{
			static_object_example = XUuid::fromString(vCommandResult).toString(XUuid::WithoutBraces);
		}
#endif
	}
	return {};
}

// [get] 硬盘ID
XString XSystem::diskID() noexcept
{
	static char				_StaticDiskID[XCC_PATH_MAX] = { 0 };
	if(0 == x_posix_strlen(_StaticDiskID))
	{
#if defined(XCC_SYSTEM_WINDOWS)
		auto		vHandle = ::CreateFileA(R"(\\.\PhysicalDrive0)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if(!vHandle)
		{
			vHandle = ::CreateFileA(R"(\\.\Scsi0)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		}
		if(vHandle)
		{
			DWORD			dwBytesReturned = 0;
			GETVERSIONINPARAMS	gVersionParsams;
			x_posix_memset(&gVersionParsams, 0, sizeof(GETVERSIONINPARAMS));

			if(DeviceIoControl(vHandle, SMART_GET_VERSION, nullptr, 0, &gVersionParsams, sizeof(GETVERSIONINPARAMS), &dwBytesReturned, nullptr))
			{
				if(dwBytesReturned != 0 && gVersionParsams.bIDEDeviceMap > 0)
				{
					SENDCMDINPARAMS		scip;
					x_posix_memset(&scip, 0, sizeof(SENDCMDINPARAMS));
					scip.cBufferSize = IDENTIFY_BUFFER_SIZE;
					scip.irDriveRegs.bSectorCountReg = 1;
					scip.irDriveRegs.bSectorNumberReg = 1;
					scip.irDriveRegs.bDriveHeadReg = 0xA0;
					scip.irDriveRegs.bCommandReg = 0xEC;

					BYTE btBuffer[1024] = { 0 };

					if(DeviceIoControl(vHandle, SMART_RCV_DRIVE_DATA, &scip, sizeof(SENDCMDINPARAMS), btBuffer, 1024, &dwBytesReturned, nullptr))
					{
						//序列号的开始位置,具体请参考SENDCMDOUTPARAMS与IDSECTOR结构
						auto		vSerialPos = 0x24;
						auto		vLocalPos = 0;
						while(btBuffer[vSerialPos] < 128)
						{
							_StaticDiskID[vLocalPos++] = btBuffer[vSerialPos++];
						}
					}
				}
			}
			::CloseHandle(vHandle);
		}
#endif
#if defined(XCC_SYSTEM_LINUX)
#endif
#if defined(XCC_SYSTEM_DARWIN)
		//system_profiler SPSerialATADataType | grep Serial
		XShell::run("system_profiler SPSerialATADataType | grep Serial", [&](const XString& _Output)->bool
		{
			auto		vSPSerialATADataType = _Output;
			vSPSerialATADataType.remove("Serial number:");
			vSPSerialATADataType.remove("Serial Number:");
			vSPSerialATADataType = vSPSerialATADataType.simplified();
			if(vSPSerialATADataType.exist())
			{
				x_posix_strcpy(_StaticDiskID, vSPSerialATADataType.data());
			}
			return true;
		});
#endif
	}
	return {_StaticDiskID};
}

// [get] 当前计算机唯一的字符串
XString XSystem::uniqueId() noexcept
{
	static XString		static_object_example = nullptr;
	if(static_object_example.empty())
	{
		auto		vTempOnlyString = XSystem::nativeString();
		vTempOnlyString += XString::format(" DISK[%s]", XSystem::diskID().data());
		vTempOnlyString += XString::format(" HOST-ID[%016lld]", (x_int64_t)x_posix_gethostid());
		vTempOnlyString += XString::format(" MACHINE-ID[%s]", XSystem::machineId().data());

		static_object_example = XString::fromBytes(XHash::hash(vTempOnlyString.toBytes(), XHash::MD5).toHex().toUpper());
	}
	return static_object_example;
}
