#ifndef			_XCC_CORE_DYNAMIC_LIBRARY_H_
#define			_XCC_CORE_DYNAMIC_LIBRARY_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 定义Windows下不存在的动态库加载标志
#if defined(XCC_SYSTEM_WINDOWS) && !defined(RTLD_LOCAL)
#define		RTLD_LOCAL		0
#define		RTLD_LAZY		1
#define		RTLD_NOW		2
#define		RTLD_GLOBAL		4
#define		RTLD_NODELETE		8
#define		RTLD_NOLOAD		16
#define		RTLD_DEEPBIND		32
#endif

// 动态链接库加载处理
class _XCOREAPI_ XDynamicLibrary
{
public:
	using				handle_type = HANDLE;
	using				func_type = void*;

public:
	// constructor
	XDynamicLibrary() noexcept;

	// destructor
	virtual ~XDynamicLibrary() noexcept;

public:
	// 打开动态链接库文件
	static handle_type open(const XString& _FileName) noexcept;

	// 以指定标记打开动态链接库文件
	static handle_type open(const XString& _FileName, int _Flags) noexcept;

	// 在动态链接库句柄中查找函数
	static func_type find(handle_type _Handle, const XString& _FuncName) noexcept;

	// 关闭动态链接库句柄
	static void close(handle_type _Handle) noexcept;

	// 最近一次的错误消息
	static XString error() noexcept;
};


#endif
