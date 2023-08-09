#ifndef			_XCC_CORE_SYSTEM_H_
#define			_XCC_CORE_SYSTEM_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>

#define			XCC_OS_VERSION_UNKNOWN					0x00000000

#define			XCC_OS_VERSION_WINDOWS_VISTA				0x00060000
#define			XCC_OS_VERSION_WINDOWS_2008				0x00060001
#define			XCC_OS_VERSION_WINDOWS_7				0x00060100
#define			XCC_OS_VERSION_WINDOWS_2008_R2				0x00060101
#define			XCC_OS_VERSION_WINDOWS_8				0x00060200
#define			XCC_OS_VERSION_WINDOWS_2012				0x00060201
#define			XCC_OS_VERSION_WINDOWS_8_1				0x00060300
#define			XCC_OS_VERSION_WINDOWS_2012_R2				0x00060301
#define			XCC_OS_VERSION_WINDOWS_10				0x00100000
#define			XCC_OS_VERSION_WINDOWS_2016				0x00100001

#define			XCC_OS_VERSION_LINUX_1404				0x00140400
#define			XCC_OS_VERSION_LINUX_1410				0x00141000
#define			XCC_OS_VERSION_LINUX_1504				0x00150400
#define			XCC_OS_VERSION_LINUX_1510				0x00151000
#define			XCC_OS_VERSION_LINUX_1604				0x00160400
#define			XCC_OS_VERSION_LINUX_1610				0x00161000
#define			XCC_OS_VERSION_LINUX_1704				0x00170400
#define			XCC_OS_VERSION_LINUX_1710				0x00171000
#define			XCC_OS_VERSION_LINUX_1804				0x00180400
#define			XCC_OS_VERSION_LINUX_1810				0x00181000
#define			XCC_OS_VERSION_LINUX_1904				0x00190400
#define			XCC_OS_VERSION_LINUX_1910				0x00191000
#define			XCC_OS_VERSION_LINUX_2004				0x00200400
#define			XCC_OS_VERSION_LINUX_2010				0x00201000
#define			XCC_OS_VERSION_LINUX_2104				0x00210400
#define			XCC_OS_VERSION_LINUX_2110				0x00211000

#define			XCC_OS_VERSION_MACOS_1012				0x00101200
#define			XCC_OS_VERSION_MACOS_1013				0x00101300
#define			XCC_OS_VERSION_MACOS_1014				0x00101400
#define			XCC_OS_VERSION_MACOS_1015				0x00101500
#define			XCC_OS_VERSION_MACOS_1100				0x00110000
#define			XCC_OS_VERSION_MACOS_1101				0x00110100

// Xanadu Class system
class _XCOREAPI_ XSystem
{
public:
	XSystem() noexcept;

	virtual ~XSystem() noexcept;

public:
	// The current version of the operating system
	static x_int64_t SystemVersion() noexcept;

	// The name of the user who is now logged in
	static XString currentUser() noexcept;

	// Computer name
	static XString hostName() noexcept;

	// Native system String
	static XString nativeString() noexcept;

public:
	// [get] 系统唯一ID
	static XString machineId() noexcept;

	// [get] 硬盘ID
	static XString diskID() noexcept;

	// [get] 当前计算机唯一的字符串
	static XString uniqueId() noexcept;
};

#endif
