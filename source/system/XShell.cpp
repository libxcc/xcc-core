#include <xcc-core/system/XShell.h>
#include <xcc-core/sync/XProcess.h>
#include <platform/xpa/XPlatformProcess.h>


// constructor
XShell::XShell() noexcept = default;

// destructor
XShell::~XShell() noexcept = default;



// Async run shell
int XShell::run(const XString& _Shell) noexcept
{
	XCC_CHECK_RETURN(_Shell.exist(), -1);

#if defined(XCC_SYSTEM_WINDOWS)
	return static_cast<int>(XProcess::execute("cmd.exe", _Shell));
#else
#if defined(XCC_SYSTEM_ARM)
	return static_cast<int>(XProcess::execute("/bin/sh", XString("-c ") + _Shell));
#else
	return static_cast<int>(XProcess::execute("/bin/bash", XString("-c ") + _Shell));
#endif
#endif
}

// Sync run shell
int XShell::run(const XString& _Shell, const std::function<bool(const XString& _Output)>& _Lambda) noexcept
{
	XCC_CHECK_RETURN(_Shell.exist(), -1);

#if defined(XCC_SYSTEM_WINDOWS)
	auto			vShell = _Shell.toWString();
	SECURITY_ATTRIBUTES	sa;
	HANDLE			vRHandle = nullptr;
	HANDLE			vWHandle = nullptr;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;

	// 创建匿名管道
	if (::CreatePipe(&vRHandle, &vWHandle, &sa, 0))
	{
		STARTUPINFOW		si;
		PROCESS_INFORMATION	pi;
		x_posix_memset(&si, 0, sizeof(STARTUPINFOW));
		si.cb = sizeof(STARTUPINFO);
		::GetStartupInfoW(&si);
		si.hStdError = vWHandle;
		si.hStdOutput = vWHandle;
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		auto		vShellW = _Shell.toWString();
		auto		vCommandLine = x_posix_wcsdup(vShellW.data());
		if (::CreateProcessW(nullptr, vCommandLine, nullptr, nullptr, TRUE, NULL, nullptr, nullptr, &si, &pi))
		{
			::CloseHandle(vWHandle);
			x_process_read_output_from_handle(vRHandle, _Lambda);
			::CloseHandle(vRHandle);

			// 等待进程结束
			::WaitForSingleObject(pi.hProcess, INFINITE);
			auto		vExitCode = static_cast<DWORD>(STILL_ACTIVE);
			::GetExitCodeProcess(pi.hProcess, &vExitCode);
			// ::CloseHandle(pi.hProcess);
			x_posix_free(vCommandLine);
			return (int)vExitCode;
		}
		else
		{
			x_posix_free(vCommandLine);
			return (int)GetLastError();
		}
	}
#else
	auto		vHandle = x_posix_popen(_Shell.data(), "r");
	if(vHandle)
	{
		x_process_read_output_from_file(vHandle, _Lambda);
		return x_posix_pclose(vHandle);
	}
#endif
	return -1;
}

// Native system
int XShell::system(const XString& _Command) noexcept
{
	XCC_CHECK_RETURN(_Command.exist(), -1);
#if defined(XCC_SYSTEM_WINDOWS)
	return x_posix_wsystem(_Command.toWString().data());
#else
	return x_posix_system(_Command.toUString().data());
#endif
}



// 运行命令行并返回结果
XString XShell::terminalCommand(const XString& _Command) noexcept
{
	XString		vTempResult;
	XShell::run(_Command, [&](const XString& _Output)->bool
	{
		vTempResult += _Output;
		return true;
	});
	return vTempResult;
}
