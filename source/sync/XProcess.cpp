#include <xcc-core/sync/XProcess.h>
#include <xcc-core/filesystem/XFileSystem.h>
#include <platform/xpa/XPlatformProcess.h>


// constructor
XProcess::XProcess() noexcept = default;

// destructor
XProcess::~XProcess() noexcept = default;



// 指定延时(毫秒)后退出
static void xcc_core_process_exit_by_delay(x_uint32_t _Millisecond, int _Code) noexcept
{
	x_posix_msleep(_Millisecond);
	x_posix_exit(_Code);
}

// 指定延时(毫秒)后退出
void XProcess::delay_exit(x_uint32_t _Millisecond, int _Code) noexcept
{
	std::thread(xcc_core_process_exit_by_delay, _Millisecond, _Code).detach();
}



// Kills the process by the specified name
bool XProcess::kill(const XString& _ProcessName) noexcept
{
	if(_ProcessName.empty())
	{
		return false;
	}

	auto		vSync = XPA_ProcessTerminateByName(_ProcessName);
	return 0 == vSync;
}

// Kill the process with the specified process ID
bool XProcess::kill(x_uint64_t _ProcessID) noexcept
{
	auto		vSync = XPA_ProcessTerminateById(_ProcessID);
	return 0 == vSync;
}



// Gets the current process ID
x_uint64_t XProcess::currentProcessID() noexcept
{
	return static_cast<x_uint64_t>(x_posix_getpid());
}



// Traverses the list of currently running processes
bool XProcess::traverse(const std::function<bool(const XProcessInfo& _Info)>& _Lambda) noexcept
{
	XCC_CHECK_RETURN(_Lambda, false);

	auto		vSync = XPA_ProcessList(_Lambda);
	return 0 == vSync;
}



// Does the specified process name exist
bool XProcess::isExist(const XString& _ProcessName) noexcept
{
	if(_ProcessName.empty())
	{
		return false;
	}

	if(XProcess::number(_ProcessName) > 0)
	{
		return true;
	}
	return false;
}

// How many instances of the specified process name
x_uint64_t XProcess::number(const XString& _ProcessName) noexcept
{
	if(_ProcessName.empty())
	{
		return 0;
	}

	auto		vNumber = 0;
	XPA_ProcessList([&](const XProcessInfo& _ProcessInfo)->bool
	{
		if(_ProcessInfo.name() == _ProcessName)
		{
			++vNumber;
		}
		return true;
	});
	return vNumber;
}

// 运行并等待进程
x_int64_t XProcess::execute(const XString& _Application, const XString& _Param, const XString& _Directory, bool _Wait, bool _UI) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	x_int64_t		vSync = STATUS_INVALID_HANDLE;
	SHELLEXECUTEINFOW	vInfoShell = {sizeof(SHELLEXECUTEINFOW) };
	if (_UI)
	{
		vInfoShell.fMask = SEE_MASK_NOCLOSEPROCESS;
	}
	else
	{
		vInfoShell.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
	}
	auto		vWApplication = _Application.toWString();
	auto		vWParam = _Param.toWString();
	auto		vWDirectory = _Directory.toWString();
	vInfoShell.lpFile = vWApplication.data();
	vInfoShell.lpParameters = vWParam.data();
	vInfoShell.lpDirectory = vWDirectory.data();
	vInfoShell.nShow = _UI ? SW_SHOW : SW_HIDE;
	if(ShellExecuteExW(&vInfoShell))
	{
		auto	vHandle = vInfoShell.hProcess;
		if(vHandle)
		{
			if(_Wait)
			{
				WaitForSingleObject(vHandle, INFINITE);
				DWORD		vExitCode = STILL_ACTIVE;
				GetExitCodeProcess(vHandle, &vExitCode);
				CloseHandle(vHandle);

				vSync = (int)vExitCode;
			}
			else
			{
				vSync = 0;
			}
		}
	}
	else
	{
		// STATUS_INVALID_HANDLE
		vSync = GetLastError();
	}
	return vSync;
#else
	auto		vSync = fork();
	if(vSync > 0)
	{
		if(_Wait)
		{
			int 	vStatus = 0;
			::waitpid(vSync, &vStatus, 0);
			if(WIFEXITED(vStatus))
			{
				return WEXITSTATUS(vStatus);
			}
			else
			{
				return WTERMSIG(vStatus);
			}
		}
		return 0;
	}
	else if(vSync == 0)
	{
		if(-1 == XProcess::execds(_Application.data(), _Directory.data(), _Param.data()))
		{
			x_posix_exit(0);
		}
	}
	return -1;
#endif
}

// Loads and executes new child processes.
int XProcess::execds(const char* _Application, const char* _Directory, const char* _Param) noexcept
{
	auto		vSync = -1;
	auto		vExecParam = std::string(_Application ? _Application : "") + " " + std::string(_Param ? _Param : "");
	auto		vArgs = x_process_param_string_to_argv(vExecParam.data());

	// 我们需要改变工作目录
	x_posix_chdir(_Directory);

	// 在这里调用exec系列函数
#if defined(XCC_SYSTEM_WINDOWS)
	vSync = ::_execv(_Application, vArgs);
#else
	vSync = ::execv(_Application, vArgs);
#endif

	x_process_param_free(vArgs);
	return vSync;
}

// 在内存中运行进程
int XProcess::run_memory_app(const XProcessRunMemoryInfo& _MemoryInfo, const std::function<bool(const XString& _Output)>& _Lambda) noexcept
{
	auto		vRunStatus = 0;
	auto		vContext = XPrivateProcessRunMemory();

	vContext.memory = _MemoryInfo.runMemory;
	vContext.args = _MemoryInfo.runArgs;
	vContext.directory = _MemoryInfo.runDirectory;
	vContext.host = _MemoryInfo.runHostApp;

	vRunStatus = XPA_ProcessRunByMemory(vContext, _Lambda);
	return vRunStatus;
}
