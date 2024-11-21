#include <xcc-core/system/XSystem.h>
#include <xcc-core/system/XDynamicLibrary.h>
#include <xcc-core/system/XOperatingSystem.h>
#if defined(XCC_SYSTEM_WINDOWS)
#include <UserEnv.h>
#else
#include <sys/utsname.h>
#include <xcc-core/system/XShell.h>
#endif



// constructor
XSystem::XSystem() noexcept = default;

// destructor
XSystem::~XSystem() noexcept = default;



// The current version of the operating system
x_int64_t XSystem::SystemVersion() noexcept
{
	//static system version value
	static x_int64_t		static_system_version = 0;
	if(!static_system_version)
	{
#if defined(XCC_SYSTEM_WINDOWS)
		typedef void(WINAPI* _Function_GetVersionNumbers)(long*, long*, long*);
		auto		vVersion_1 = static_cast<long>(0);
		auto		vVersion_2 = static_cast<long>(0);
		auto		vVersion_3 = static_cast<long>(0);
		auto		vIsServer = XOperatingSystem::isServerVersion();
		auto		vModule = XDynamicLibrary::open("ntdll.dll");
		if(vModule)
		{
			auto		GetVersionNumbers = (_Function_GetVersionNumbers)XDynamicLibrary::find(vModule, "RtlGetNtVersionNumbers");
			if(GetVersionNumbers)
			{
				GetVersionNumbers(&vVersion_1, &vVersion_2, &vVersion_3);
				switch(vVersion_1)
				{
					case 10:
						static_system_version = vIsServer ? XCC_OS_VERSION_WINDOWS_2016 : XCC_OS_VERSION_WINDOWS_10;
						break;
					case 6:
						switch(vVersion_2)
						{
							case 0:
								static_system_version = vIsServer ? XCC_OS_VERSION_WINDOWS_2008 : XCC_OS_VERSION_WINDOWS_VISTA;
								break;
							case 1:
								static_system_version = vIsServer ? XCC_OS_VERSION_WINDOWS_2008_R2 : XCC_OS_VERSION_WINDOWS_7;
								break;
							case 2:
								static_system_version = vIsServer ? XCC_OS_VERSION_WINDOWS_2012 : XCC_OS_VERSION_WINDOWS_8;
								break;
							case 3:
								static_system_version = vIsServer ? XCC_OS_VERSION_WINDOWS_2012_R2 : XCC_OS_VERSION_WINDOWS_8_1;
								break;
							default:
								break;
						}
						break;
					default:
						static_system_version = XCC_OS_VERSION_UNKNOWN;
						break;
				}
			}
			XDynamicLibrary::close(vModule);
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
					static_system_version = XCC_OS_VERSION_MACOS_1012;
					break;
				case 13:
					static_system_version = XCC_OS_VERSION_MACOS_1013;
					break;
				case 14:
					static_system_version = XCC_OS_VERSION_MACOS_1014;
					break;
				case 15:
					static_system_version = XCC_OS_VERSION_MACOS_1015;
					break;
				case 16:
					static_system_version = XCC_OS_VERSION_MACOS_1100;
					break;
				default:
					static_system_version = XCC_OS_VERSION_UNKNOWN;
					break;
			}
		}
#endif
	}
	return static_system_version;
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
		vSystemString += XOperatingSystem::buildVersion();
		vSystemString += ")";

		//拼接系统位数
		if(XOperatingSystem::isBit64())
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
