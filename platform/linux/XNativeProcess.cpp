#include <platform/xpa/XPlatformProcess.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/memfd.h>



// XPA: 遍历本机进程数据
int XPA_ProcessList(const std::function<bool(const XProcessInfo& _ProcessInfo)>& _Lambda) noexcept
{
	auto		vSync = XFileSystem::directory_traverse("/proc", [&](const XFileSystem::path& _Path)->bool
	{
		if(XFileSystem::path::isDirectory(_Path))
		{
			auto		vExepath = XString::format("/proc/%ls/exe", _Path.fileName().data());
			auto		vApplication = vExepath.toUString();
			char		vDirectory[XCC_PATH_MAX] = { 0 };
			auto		vCount = readlink(vApplication.data(), vDirectory, XCC_PATH_MAX);
			if(0 <= vCount || vCount <= XCC_PATH_MAX)
			{
				if(x_posix_strlen(vDirectory))
				{
					auto		vName = x_posix_strrchr(vDirectory, '/') + 1;
					if(vName && x_posix_strlen(vName))
					{
						auto		vPrivate = new(std::nothrow) XPrivateProcessData();
						if(vPrivate == nullptr)
						{
							return false;
						}
						vPrivate->id = _Path.fileName().toLLong();
						vPrivate->name = XString::fromUString(vName);
						if(!_Lambda(XProcessInfo(vPrivate)))
						{
							return false;
						}
					}
				}
			}
		}
		return true;
	});
	return vSync ? 0 : -1;
}

// XPA: 根据进程ID结束进程
int XPA_ProcessTerminateById(x_uint64_t _ProcessID) noexcept
{
	auto		vError = x_posix_kill(_ProcessID, 9);
	return vError;
}



// XPA: 从内存中运行进程
int XPA_ProcessRunByMemory(const XPrivateProcessRunMemory& _Context, const std::function<bool(const XString& _Output)>& _Lambda) noexcept
{
	auto		AppMemory = _Context.memory.data();
	auto		AppLength = _Context.memory.size();
	auto		RunParameter = x_process_param_string_to_argv(_Context.args.data());
	auto		RunDirectory = _Context.directory;
	char		HostPath[256] = "\0";
	int		HostHandle = 0;
	pid_t		AppProcessID = x_posix_getpid();
	pid_t		AppChild = 0;
	int		AppSync[2] = {0};
	int		vAppStatus = -1;
	char		RMA_NAME[128] = {0};

	// 使用系统调用创建内存句柄
	x_posix_sprintf(RMA_NAME, "rma-%lld", (long long)x_time_system_millisecond());
	HostHandle = syscall(SYS_memfd_create, RMA_NAME, MFD_CLOEXEC);
	// x_posix_write(HostHandle, AppMemory, AppLength);
	uint64_t 	vAlready = 0;
	size_t 		vSync = 0;

	while(vAlready < AppLength)
	{
		vSync = x_posix_write(HostHandle, AppMemory + vAlready, AppLength - vAlready);
		if(vSync <= 0)
		{
			break;
		}
		vAlready += vSync;
	}
	if(vAlready != AppLength)
	{
		XLOG_DEBUG("CORE", u8"memory write error");
		return -1;
	}

	x_posix_sprintf(HostPath,"/proc/%d/fd/%d", AppProcessID, HostHandle);

	// 设置环境变量
	auto		vLoadDirectory = XString(x_posix_getenv("LD_LIBRARY_PATH"));
	vLoadDirectory += ":";
	vLoadDirectory += RunDirectory;
	x_posix_setenv("LD_LIBRARY_PATH", vLoadDirectory.data(), 1);

	auto		vRunParam0 = RunParameter[0];
	RunParameter[0] = HostPath;
	auto		vRunShell = x_process_param_argv_to_string(RunParameter);
	RunParameter[0] = vRunParam0;

	auto		vHandle = x_posix_popen(vRunShell, "r");
	if(vHandle)
	{
		x_process_read_output_from_file(vHandle, _Lambda);
		vAppStatus = x_posix_pclose(vHandle);
		x_posix_close(HostHandle);
		return vAppStatus;
	}

	pipe(AppSync);

	AppChild = fork();
	if (AppChild == 0)
	{
		x_posix_close(AppSync[0]);
		x_posix_close(1);
		x_posix_close(2);
		x_posix_dup2(AppSync[1], 1);
		x_posix_dup2(AppSync[1], 2);

		x_posix_chdir(RunDirectory.data());
		execve(HostPath, RunParameter, nullptr);
		return 0;
	}
	else if (AppChild == -1)
	{
		x_posix_close(HostHandle);
		return -1;
	}
	else
	{
		x_posix_close(AppSync[1]);
		x_process_read_output_from_descriptor(AppSync[0], _Lambda);
		x_posix_close(AppSync[0]);
		waitpid(AppChild, &vAppStatus, 0);
		x_posix_close(HostHandle);
		return vAppStatus;
	}
}
