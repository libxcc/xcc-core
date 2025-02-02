﻿#ifndef			_XCC_CORE_SYSTEM_OPERATING_SYSTEM_H_
#define			_XCC_CORE_SYSTEM_OPERATING_SYSTEM_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// operating system
class _XCOREAPI_ XOperatingSystem
{
public:
	// constructor
	XOperatingSystem() noexcept;

	// destructor
	virtual ~XOperatingSystem() noexcept;

public:
	// kernel version
	static XString kernelVersion() noexcept;

	// build version
	static XString buildVersion() noexcept;

	// system version
	static XString systemVersion() noexcept;

	// release name
	static XString releaseName() noexcept;

	// product name
	static XString productName() noexcept;

	// display name
	static XString displayName() noexcept;

public:
	// [get] 系统唯一ID
	static XString machineId() noexcept;

	// [get] 当前计算机唯一的字符串
	static XString uniqueId() noexcept;

public:
	// 检查是否为服务端系统
	static bool isServerVersion() noexcept;

	// 检查是否为桌面系统
	static bool isDesktopVersion() noexcept;

public:
	// 操作系统位数
	static x_uint64_t bits() noexcept;

	// 操作系统是否为32位数
	static bool isBit32() noexcept;

	// 操作系统是否为64位数
	static bool isBit64() noexcept;

public:
	// 本机名称
	static XString hostName() noexcept;

public:
	// 用户名称
	static XString userName() noexcept;

	// 用户主目录
	static XString userHome() noexcept;
};

#endif
