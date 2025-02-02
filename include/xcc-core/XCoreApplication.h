﻿#ifndef			_XCC_CORE_CORE_APPLICATION_H_
#define			_XCC_CORE_CORE_APPLICATION_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 核心应用程序的私有数据
struct XCoreApplicationPrivate;
typedef struct XCoreApplicationPrivate		XCoreApplicationPrivate;

// 核心应用程序的基础功能
class _XCOREAPI_ XCoreApplication
{
private:
	// 实例指针
	static XCoreApplication*		d_self;

	// 私有数据
	XCoreApplicationPrivate*		d_ptr;

private:
	XCC_DISABLE_COPY(XCoreApplication);
	XCC_DISABLE_MOVE(XCoreApplication);

public:
	// constructor
	XCoreApplication(int _Argc, char** _Argv) noexcept;

	// destructor
	virtual ~XCoreApplication() noexcept;

public:
	// [get] 当前实例指针
	static XCoreApplication* example() noexcept;

	// [get] 当前应用程序参数列表
	static std::vector<XString> arguments() noexcept;

public:
	// [get] 应用程序当前位数
	static x_uint64_t appCurrentBit() noexcept;

public:
	// [get] 应用程序文件路径
	static XString applicationFilePath() noexcept;

	// [get] 应用程序目录路径
	static XString applicationDirPath() noexcept;

	// [get] 应用程序文件名称
	static XString applicationFileName() noexcept;

	// [get] 应用程序文件除去后缀的名称
	static XString applicationFileStem() noexcept;

public:
	// [set] 应用程序运行目录
	static XString setCurrentDirectory(const XString& _Directory) noexcept;

	// [get] 应用程序运行目录
	static XString currentDirectory() noexcept;
};

#endif
