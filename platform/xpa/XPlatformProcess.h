#ifndef			_XCC_CORE_PLATFORM_PROCESS_H_
#define			_XCC_CORE_PLATFORM_PROCESS_H_

#include <xcc-core/xcc.h>



// 从句柄读取输出
void x_process_read_output_from_handle(HANDLE _Handle, const std::function<bool(const XString& _Output)>& _Lambda) noexcept;

// 从句柄读取输出
void x_process_read_output_from_file(FILE* _Handle, const std::function<bool(const XString& _Output)>& _Lambda) noexcept;

// 从句柄读取输出
void x_process_read_output_from_descriptor(int _Handle, const std::function<bool(const XString& _Output)>& _Lambda) noexcept;



// 进程参数转换
char** x_process_param_string_to_argv(const char* _Argv) noexcept;

// 进程参数转换
char* x_process_param_argv_to_string(char** _Argv) noexcept;

// 进程参数释放
void x_process_param_free(char** _Argv) noexcept;



// XPA: 进程信息
struct XPrivateProcessData
{
	x_uint64_t			id = 0;		// 进程ID
	XString				name;		// 进程名
	XString				path;		// 进程所在路径
	std::list<XString>		args;		// 启动参数列表
};



// XPA: 遍历本机进程数据
int XPA_ProcessList(const std::function<bool(const XProcessInfo& _ProcessInfo)>& _Lambda) noexcept;

// XPA: 根据进程名称结束进程
int XPA_ProcessTerminateByName(const XString& _ProcessName) noexcept;

// XPA: 根据进程ID结束进程
int XPA_ProcessTerminateById(x_uint64_t _ProcessID) noexcept;



// XPA: 内存参数
typedef struct XPrivateProcessRunMemory
{
	XByteArray			memory;		// 要运行的内存
	XString				args;		// 运行参数
	XString				directory;	// 运行目录
	XString				host;		// 要附加的主机程序
}XPrivateProcessRunMemory;

// XPA: 从内存中运行进程
int XPA_ProcessRunByMemory(const XPrivateProcessRunMemory& _Context, const std::function<bool(const XString& _Output)>& _Lambda) noexcept;



#endif
