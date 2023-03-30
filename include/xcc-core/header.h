#ifndef			_XCC_CORE_HEADER_H_
#define			_XCC_CORE_HEADER_H_

#include <xcc-posix/xcc.h>

// 根据操作系统版本包含一些C运行时库头文件
#if defined(XCC_SYSTEM_ANDROID)
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif
#include <cerrno>
#include <fcntl.h>
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cfloat>
#include <cwchar>
#include <cctype>
#include <cwctype>
#include <cinttypes>
#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <cstring>
#include <algorithm>
#include <regex>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <functional>
#include <mutex>
#include <thread>
#include <stdexcept>
#include <cassert>
#include <random>
#if defined(XCC_SYSTEM_WINDOWS)
#include <io.h>
#include <direct.h>
#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Shlobj.h>
#else
#if defined(XCC_SYSTEM_DARWIN)
#include <sys/uio.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <objc/objc.h>
#include <objc/message.h>
#include <xlocale.h>
#else
#if defined(XCC_SYSTEM_ANDROID) || defined(XCC_SYSTEM_ARM)
#include <sys/uio.h>
#else
#include <sys/io.h>
#endif
#include <sys/vfs.h>
#include <locale.h>
#endif
#include <wctype.h>
#include <unistd.h>
#include <utime.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/statvfs.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#endif



// X系列核心 - 导出宏
#if defined(XCC_CORE_BUILD_STATIC)
#define			_XCOREAPI_
#else
#if defined(XCC_CORE_BUILD_SHARED)
#define			_XCOREAPI_					XCC_COMPILER_API_EXP
#else
#define			_XCOREAPI_					XCC_COMPILER_API_IMP
#endif
#endif



// X系列核心 - 宏定义
#define			XCC_CORE_LOG_TAG				u8"xcc-core"
#define			XCC_CHECK_RETURN(_Value, ...)			if(!(_Value)){	return __VA_ARGS__;	}
#define			XCC_DELETE_ARR(_Value)				if(_Value){ delete[] _Value;_Value = nullptr;}
#define			XCC_DELETE_PTR(_Value)				if(_Value){ delete _Value;	_Value = nullptr;}
#define			XCC_ASSERT(_Condition)				assert(_Condition)
#define			XCC_DISABLE_COPY(_Class)			_Class(const _Class &) = delete;	_Class &operator=(const _Class &) = delete
#define			XCC_DISABLE_MOVE(_Class)			_Class(_Class &&) = delete;		_Class &operator=(_Class &&) = delete



// X系列核心 - 类型定义
namespace xcc
{
	// 枚举 - 区分大小写
	typedef enum CaseSensitivity
	{
		CaseInsensitive			= 0,			// 不区分
		CaseSensitive			= 1			// 区分
	}CaseSensitivity;

	// 枚举 - 打开模式
	typedef enum OpenMode
	{
		ReadOnly			= 0x0001,		// 只读
		WriteOnly			= 0x0002,		// 只写
		ReadWrite			= ReadOnly | WriteOnly,	// 读写
		Append				= 0x0004,
		Truncate			= 0x0008,
		Text				= 0x0010,
		Unbuffered			= 0x0020,
		NewOnly				= 0x0040,
		ExistingOnly			= 0x0080
	}OpenMode;
}


#endif
