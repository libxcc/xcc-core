#ifndef			_XCC_CORE_PROCESS_H_
#define			_XCC_CORE_PROCESS_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>
#include <xcc-core/sync/XProcessInfo.h>


// class XProcessRunMemoryInfo
class XProcessRunMemoryInfo
{
public:
	XByteArray				runMemory;
	XString					runArgs;
	XString					runDirectory;
	XString					runHostApp;
};

// Process
class _XCOREAPI_ XProcess
{
public:
	// constructor
	XProcess() noexcept;

	// destructor
	virtual ~XProcess() noexcept;

public:
	// 指定延时(毫秒)后退出
	static void delay_exit(x_uint32_t _Millisecond, int _Code) noexcept;

public:
	// 终止指定名称的进程
	static bool kill(const XString& _ProcessName) noexcept;

	// 终止指定进程ID的进程
	static bool kill(x_uint64_t _ProcessID) noexcept;

	// 终止除自己外的其它同名进程
	static bool killSameName() noexcept;

public:
	// Gets the current process ID
	static x_uint64_t currentProcessID() noexcept;

public:
	// Traverses the list of currently running processes
	static bool traverse(const std::function<bool(const XProcessInfo& _Info)>& _Lambda) noexcept;

public:
	// Does the specified process name exist
	static bool isExist(const XString& _ProcessName) noexcept;

	// How many instances of the specified process name
	static x_uint64_t number(const XString& _ProcessName) noexcept;

public:
	// 运行并等待进程
	static x_int64_t execute(const XString& _Application, const XString& _Param = XString(), const XString& _Directory = XString(), bool _Wait = true, bool _UI = false) noexcept;

	// Loads and executes new child processes.
	static int execds(const char* _Application, const char* _Directory, const char* _Param) noexcept;

	// 在内存中运行进程
	static int run_memory_app(const XProcessRunMemoryInfo& _MemoryInfo, const std::function<bool(const XString& _Output)>& _Lambda = nullptr) noexcept;
};

#endif
