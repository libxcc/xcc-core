#ifndef			_XCC_CORE_HEADER_H_
#define			_XCC_CORE_HEADER_H_

#include <xcc-posix/xcc.h>

// Include some C runtime library header files based on the operating system version
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





// Export definition of X-Series core library
#if defined(XCC_CORE_BUILD_STATIC)
#define			_XCOREAPI_
#else
#if defined(XCC_CORE_BUILD_SHARED)
#define			_XCOREAPI_					XCC_COMPILER_API_EXP
#else
#define			_XCOREAPI_					XCC_COMPILER_API_IMP
#endif
#endif





// Macro definition of X-Series
#define			XCC_CHECK_RETURN(_Value, ...)			if(!(_Value)){	return __VA_ARGS__;	}
#define			XCC_DELETE_ARR(_Value)				if(_Value){ delete[] _Value;_Value = nullptr;}
#define			XCC_DELETE_PTR(_Value)				if(_Value){ delete _Value;	_Value = nullptr;}
#define			XCC_ASSERT(_Condition)				assert(_Condition)





/// namespace X-Series
namespace Xcc
{
	// Case Sensitivity enum
	typedef enum CaseSensitivity
	{
		// NO
		CaseInsensitive			= 0,

		// YES
		CaseSensitive
	}CaseSensitivity;
}



#endif
