﻿#ifndef			_XCC_CORE_PROCESS_INFO_H_
#define			_XCC_CORE_PROCESS_INFO_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 进程信息私有块
class XProcessInfoPrivate;

// 进程信息
class _XCOREAPI_ XProcessInfo
{
private:
	XProcessInfoPrivate*			_private_info;

public:
	XProcessInfo() noexcept;

	explicit XProcessInfo(x_uint64_t _ProcessID) noexcept;

	XProcessInfo(x_uint64_t _ProcessID, const XString& _ProcessName) noexcept;

	XProcessInfo(const XProcessInfo& _Other) noexcept;

	virtual ~XProcessInfo() noexcept;

public:
	XProcessInfo& operator = (const XProcessInfo& _Other) noexcept;

public:
	// 获取进程ID
	virtual x_uint64_t getProcessID() const noexcept;

	// 获取进程名称
	virtual XString getProcessName() const noexcept;

public:
	// 结束进程
	virtual bool terminate() const noexcept;
};

#endif
