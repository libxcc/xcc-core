#include <xcc-core/XException.h>
#include <xcc-core/XCoreApplication.h>
#if defined(XCC_SYSTEM_WINDOWS)
#include <Dbghelp.h>
#endif


// constructor
XException::XException() noexcept = default;

// constructor
XException::XException(const char* _Explain) noexcept
{
	this->_explain = _Explain;
}

// constructor
XException::XException(const XString& _Explain) noexcept
{
	this->_explain = _Explain;
}

// constructor
XException::XException(const XException& _Exception) noexcept = default;

// destructor
XException::~XException() noexcept = default;



// operator override =
XException& XException::operator = (const char* _Explain) noexcept
{
	this->_explain = _Explain;
	return *this;
}

// operator override =
XException& XException::operator = (const XString& _Explain) noexcept
{
	this->_explain = _Explain;
	return *this;
}

// operator override =
XException& XException::operator = (const XException& _Exception) noexcept = default;



// Get exception description
const char* XException::what() const noexcept
{
	return this->_explain.data();
}



#if defined(XCC_SYSTEM_WINDOWS)
// 此函数一旦成功调用，之后对 SetUnhandledExceptionFilter 的调用将无效
static void DisableSetUnhandledExceptionFilter() noexcept
{
	auto		vFunctionAddress = (void*)GetProcAddress(LoadLibraryA("kernel32.dll"), "SetUnhandledExceptionFilter");
	if (vFunctionAddress)
	{
		unsigned char code[16];
		int size = 0;

#if defined(_M_IX86)
		// Code for x86:
		// 33 C0                xor         eax,eax
		// C2 04 00             ret         4
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;
#elif defined(_M_X64)
		// 33 C0                xor         eax,eax
		// C3                   ret
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC3;
#else
#error "此功能仅限于 x86 或 x64!"
#endif

		DWORD dwOldFlag, dwTempFlag;
		VirtualProtect(vFunctionAddress, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), vFunctionAddress, code, size, nullptr);
		VirtualProtect(vFunctionAddress, size, dwOldFlag, &dwTempFlag);
	}
}

// 创建异常文件
static void ApplicationCrashCreateDumpFile(const wchar_t* _DumpFile, EXCEPTION_POINTERS* _Exception) noexcept
{
	// 创建Dump文件
	auto		vHandle = CreateFileW(_DumpFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	// Dump信息
	MINIDUMP_EXCEPTION_INFORMATION		vDumpInfo;
	vDumpInfo.ExceptionPointers = _Exception;
	vDumpInfo.ThreadId = GetCurrentThreadId();
	vDumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), vHandle, MiniDumpWithFullMemory, &vDumpInfo, nullptr, nullptr);

	CloseHandle(vHandle);
}

// 异常回调句柄
static LONG NET_API_FUNCTION ApplicationCrashHandler(EXCEPTION_POINTERS* _Exception)
{
	SYSTEMTIME	vSysTime;
	GetLocalTime(&vSysTime);
	char		vDateTime[64]={NULL};
	x_posix_sprintf(vDateTime,"%4d-%02d-%02d-%02d-%02d-%02d",vSysTime.wYear, vSysTime.wMonth, vSysTime.wDay, vSysTime.wHour, vSysTime.wMinute, vSysTime.wSecond);

	auto		vDirectory = XCoreApplication::applicationDirPath();
	auto		vFileName = XCoreApplication::applicationFileStem();
	auto		vDumpFileX = vDirectory + "/" + vFileName + "-" + vDateTime + ".dmp";
	auto		vDumpFile = vDumpFileX.toWString();
	ApplicationCrashCreateDumpFile(vDumpFile.data(), _Exception);
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

// [opt] 核心转储注册
void XException::coreDumpRegistry() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
	// DisableSetUnhandledExceptionFilter();
#endif
}

// [opt] 核心转储反注册
void XException::coreDumpUnregister() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	SetUnhandledExceptionFilter(nullptr);
#endif
}



// [std] range_error
std::range_error XException::range_error() noexcept
{
	return std::range_error("range_error");
}

// [std] range_error
std::range_error XException::range_error(const char* _Message) noexcept
{
	return std::range_error(_Message ? _Message : "null");
}

// [std] range_error
std::range_error XException::range_error(const std::string& _Message) noexcept
{
	return std::range_error(_Message.empty() ? "null" : _Message);
}

// [std] range_error
std::range_error XException::range_error(const XString& _Message) noexcept
{
	return std::range_error(_Message.empty() ? "null" : _Message.data());
}
