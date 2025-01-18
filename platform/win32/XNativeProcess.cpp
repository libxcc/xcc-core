#include <platform/xpa/XPlatformProcess.h>
#include <platform/win32/XNativeNtdll.h>
#include <TlHelp32.h>



// XPA: 遍历本机进程数据
int XPA_ProcessList(const std::function<bool(const XProcessInfo& _ProcessInfo)>& _Lambda) noexcept
{
	int		vError = -1;
	auto		vProcessEntry32 = PROCESSENTRY32W();
	auto		vSnapshotHandle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(vSnapshotHandle != INVALID_HANDLE_VALUE)
	{
		vProcessEntry32.dwSize = sizeof(vProcessEntry32);
		auto		vMore = ::Process32FirstW(vSnapshotHandle, &vProcessEntry32);
		while(vMore)
		{
			auto		vPrivate = new(std::nothrow) XPrivateProcessData();
			if(nullptr == vPrivate)
			{
				break;
			}
			vPrivate->id = vProcessEntry32.th32ProcessID;
			vPrivate->name = XString::fromWString(vProcessEntry32.szExeFile);
			if(!_Lambda(XProcessInfo(vPrivate)))
			{
				break;
			}
			vMore = ::Process32NextW(vSnapshotHandle, &vProcessEntry32);
		}
		::CloseHandle(vSnapshotHandle);
		vError = 0;
	}
	return vError;
}

// XPA: 根据进程ID结束进程
int XPA_ProcessTerminateById(x_uint64_t _ProcessID) noexcept
{
	auto		vSync = true;
	auto		vProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, static_cast<DWORD>(_ProcessID));
	if(vProcess != nullptr)
	{
		vSync = ::TerminateProcess(vProcess, 0);
		::CloseHandle(vProcess);
	}
	return vSync;
}



// 格式化DOS头
static bool pe_format_dos_header(const void* _Memory, x_uint64_t _Length, IMAGE_DOS_HEADER** _DosHeader) noexcept
{
	if(nullptr == _DosHeader || _Length < sizeof(IMAGE_DOS_HEADER))
	{
		return false;
	}

	*_DosHeader = (IMAGE_DOS_HEADER*)_Memory;
	if((*_DosHeader)->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}

	return true;
}

// 格式化NT头
static bool pe_format_nt_header(const void* _Memory, x_uint64_t _Length, const IMAGE_DOS_HEADER* _DosHeader, IMAGE_NT_HEADERS** _NtHeader) noexcept
{
	if(nullptr == _DosHeader || nullptr == _NtHeader || _Length < (_DosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS)))
	{
		return false;
	}

	(*_NtHeader) = PIMAGE_NT_HEADERS(x_size_t(_Memory) + _DosHeader->e_lfanew);
	if((*_NtHeader)->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}

	return true;
}

// 检查PE是否32位
static bool pe_bit_is_32(const IMAGE_NT_HEADERS* _NtHeader) noexcept
{
	if(_NtHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
	{
		return true;
	}
	return false;
}

// 检查PE是否64位
static bool pe_bit_is_64(const IMAGE_NT_HEADERS* _NtHeader) noexcept
{
	if(_NtHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_IA64 || _NtHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
	{
		return true;
	}
	return false;
}

// 获取默认HOST
static const char* pe_run_get_default_host(const IMAGE_NT_HEADERS* _NtHeader) noexcept
{
	if(XOperatingSystem::isBit64() && pe_bit_is_64(_NtHeader))
	{
		return R"(C:\Windows\SysWOW64\cmd.exe)";
	}
	else
	{
		return R"(C:\Windows\System32\cmd.exe)";
	}
}

// 初始化管道
static bool pe_run_memory_init_pipe(STARTUPINFOW* SI, HANDLE* _WHandle, HANDLE* _RHandle) noexcept
{
	SECURITY_ATTRIBUTES	sa;
	HANDLE			vRHandle = nullptr;
	HANDLE			vWHandle = nullptr;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;

	// 创建匿名管道
	if (::CreatePipe(&vRHandle, &vWHandle, &sa, 0))
	{
		x_posix_memset(SI, 0, sizeof(STARTUPINFOW));
		SI->cb = sizeof(STARTUPINFO);
		::GetStartupInfoW(SI);
		SI->hStdError = vWHandle;
		SI->hStdOutput = vWHandle;
		SI->wShowWindow = SW_HIDE;
		SI->dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		*_WHandle = vWHandle;
		*_RHandle = vRHandle;
		return true;
	}
	return false;
}

// XPA: 从内存中运行进程
int XPA_ProcessRunByMemory(const XPrivateProcessRunMemory& _Context, const std::function<bool(const XString& _Output)>& _Lambda) noexcept
{
	auto		Context = static_cast<CONTEXT*>(nullptr);
	auto		DosHeader = static_cast<IMAGE_DOS_HEADER*>(nullptr);
	auto		NtHeader = static_cast<IMAGE_NT_HEADERS*>(nullptr);
	auto		SectionHeader = static_cast<IMAGE_SECTION_HEADER*>(nullptr);
	auto		PI = PROCESS_INFORMATION();
	auto		SI = STARTUPINFOW();
	HANDLE		vRHandle = nullptr;
	HANDLE		vWHandle = nullptr;
	auto		vAppMemory = _Context.memory;

	std::memset(&PI, 0, sizeof(PROCESS_INFORMATION));
	std::memset(&SI, 0, sizeof(STARTUPINFOW));

	do
	{
		// 初始化管道
		if(!pe_run_memory_init_pipe(&SI, &vWHandle, &vRHandle))
		{
			break;
		}

		// 格式化DOS头
		if(!pe_format_dos_header(vAppMemory.data(), vAppMemory.size(), &DosHeader))
		{
			break;
		}

		// 格式化NT头
		if(!pe_format_nt_header(vAppMemory.data(), vAppMemory.size(), DosHeader, &NtHeader))
		{
			break;
		}

		// 为新图像创建一个处于挂起状态的当前进程的新实例
		auto		AppMemory = vAppMemory.data();
		auto		RunParameter = _Context.args.toWString();
		auto		RunParameterW = x_posix_wcsdup(RunParameter.data());
		auto		HostApp = _Context.host.empty() ? pe_run_get_default_host(NtHeader) : _Context.host;
		auto		HostAppW = HostApp.toWString();
		auto		RunDirectory = _Context.directory.toWString();
		if (CreateProcessW(HostAppW.data(), RunParameterW, nullptr, nullptr, TRUE, CREATE_SUSPENDED, nullptr, RunDirectory.data(), &SI, &PI))
		{
			::CloseHandle(vWHandle);

			// 为上下文分配内存
			Context = (CONTEXT*)VirtualAlloc(nullptr, sizeof(CONTEXT), MEM_COMMIT, PAGE_READWRITE);
			Context->ContextFlags = CONTEXT_FULL;
			void*		AppBaseAddr = nullptr;
			void*		HostBaseAddr = nullptr;

			// 如果上下文在线程中
			if(GetThreadContext(PI.hThread, Context))
			{
				// 获取进程基地址
#if defined(_WIN64)
				ntdll::NtReadVirtualMemory(PI.hProcess, (void*)(Context->Rdx + (sizeof(SIZE_T) * 2)), &HostBaseAddr, sizeof(void*), nullptr);
#else
				ntdll::NtReadVirtualMemory(PI.hProcess, (void*)(Context->Ebx + (sizeof(SIZE_T) * 2)), &HostBaseAddr, sizeof(void*), nullptr);
#endif
				if((SIZE_T)HostBaseAddr == NtHeader->OptionalHeader.AddressOfEntryPoint)
				{
					HANDLE		vProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PI.dwProcessId);
					ntdll::NtUnmapViewOfSection(vProcess, HostBaseAddr);
				}

				// AppBaseAddr = VirtualAllocEx(PI.hProcess, (void*)NtHeader->OptionalHeader.ImageBase, NtHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				AppBaseAddr = VirtualAllocEx(PI.hProcess, nullptr, NtHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if(nullptr == AppBaseAddr)
				{
					XLOG_ERROR(nullptr, u8"[ %s : %d ] VirtualAllocEx : ERROR %d", __XFUNCTION__, __XLINE__, (int)GetLastError());
					return false;
				}

				// 写入PE头
				ntdll::NtWriteVirtualMemory(PI.hProcess, AppBaseAddr, AppMemory, NtHeader->OptionalHeader.SizeOfHeaders, nullptr);

				// 写入节点
				for(auto vIndex = 0; vIndex < NtHeader->FileHeader.NumberOfSections; ++vIndex)
				{
					SectionHeader = PIMAGE_SECTION_HEADER(x_size_t(NtHeader) + sizeof(IMAGE_NT_HEADERS) + sizeof(IMAGE_SECTION_HEADER) * vIndex);
					ntdll::NtWriteVirtualMemory(PI.hProcess, LPVOID(x_size_t(AppBaseAddr) + SectionHeader->VirtualAddress), LPVOID(x_size_t(AppMemory) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, nullptr);
				}

				// 替换 PE头 与 入口点地址
#if defined(_WIN64)
				ntdll::NtWriteVirtualMemory(PI.hProcess, LPVOID(Context->Rdx + (sizeof(SIZE_T) * 2)), (const void*)(&AppBaseAddr), sizeof(void*), nullptr);
				Context->Rcx = (SIZE_T)(LPBYTE)AppBaseAddr + NtHeader->OptionalHeader.AddressOfEntryPoint;
#else
				ntdll::NtWriteVirtualMemory(PI.hProcess, LPVOID(Context->Ebx + (sizeof(SIZE_T) * 2)), (const void*)(&AppBaseAddr), sizeof(void*), nullptr);
				Context->Eax = (SIZE_T)(LPBYTE)AppBaseAddr + NtHeader->OptionalHeader.AddressOfEntryPoint;
#endif

				// 设置上下文
				SetThreadContext(PI.hThread, Context);
				// 启动进程/调用 main()
				ResumeThread(PI.hThread);

				// 循环读取输出
				x_process_read_output_from_handle(vRHandle, _Lambda);
				::CloseHandle(vRHandle);

				// 等待进程结束
				::WaitForSingleObject(PI.hProcess, INFINITE);
				auto		vExitCode = static_cast<DWORD>(STILL_ACTIVE);
				::GetExitCodeProcess(PI.hProcess, &vExitCode);
				return (int)vExitCode;
			}
			else
			{
				XLOG_ERROR(nullptr, u8"[ %s : %d ] GetThreadContext : ERROR %d", __XFUNCTION__, __XLINE__, (int)GetLastError());
				ResumeThread(PI.hThread);
			}
		}
	} while (false);
	return -1;
}
