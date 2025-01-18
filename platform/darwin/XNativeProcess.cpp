#include <platform/xpa/XPlatformProcess.h>
#include <libproc.h>



// XPA: 遍历本机进程数据
int XPA_ProcessList(const std::function<bool(const XProcessInfo& _ProcessInfo)>& _Lambda) noexcept
{
	auto		vError = -1;
	auto		vProcessNumber = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0) * 2;
	if(vProcessNumber)
	{
		auto	vProcessArray = (pid_t*)x_posix_malloc(sizeof(pid_t) * vProcessNumber);
		if(vProcessArray)
		{
			vProcessNumber = proc_listpids(PROC_ALL_PIDS, 0, vProcessArray, sizeof(pid_t) * vProcessNumber);
			if(vProcessNumber)
			{
				vError = 0;
				for(auto vIndex = 0; vIndex < vProcessNumber; ++vIndex)
				{
					pid_t           vProcessID = vProcessArray[vIndex];
					if(vProcessID == 0)
					{
						continue;
					}
					char			vProcessNAME[1024] = { 0 };
					char			vProcessPATH[2048] = { 0 };
					//struct proc_bsdinfo     vProcessINFO;
					proc_name(vProcessID, vProcessNAME, 1024);
					proc_pidpath(vProcessID, vProcessPATH, 2048);
					//proc_pidinfo(vProcessID, PROC_PIDTBSDINFO, 0, &vProcessINFO, PROC_PIDTBSDINFO_SIZE);
					auto		vPrivate = new(std::nothrow) XPrivateProcessData();
					if(nullptr == vPrivate)
					{
						vError = -1;
						break;
					}
					vPrivate->id = vProcessID;
					vPrivate->name = XString::fromUString(vProcessNAME);
					vPrivate->path = XString::fromUString(vProcessPATH);
					if(!_Lambda(XProcessInfo(vPrivate)))
					{
						break;
					}
				}
			}
			x_posix_free(vProcessArray);
		}
	}
	return vError;
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
	XCC_UNUSED(_Context);
	XCC_UNUSED(_Lambda);
	return -1;
}
