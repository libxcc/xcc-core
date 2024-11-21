#ifndef			_XCC_CORE_FOLDER_H_
#define			_XCC_CORE_FOLDER_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>
#include <xcc-core/filesystem/XFileInfo.h>


// 文件系统 - 目录操作
struct XFolderPrivate;
typedef struct XFolderPrivate			XFolderPrivate;
class _XCOREAPI_ XFolder
{
public:
	// 类型 - 目录传输
	using						FuncDirTransfer1 = const std::function<bool(const XString& _FilePath)>&;
	using						FuncDirTransfer2 = const std::function<bool(const XString& _SrcPath, const XString& _TagPath)>&;

public:
	// 本机路径分割符
#if defined(XCC_SYSTEM_WINDOWS)
	static constexpr const char			dir_native_separator = '\\';
#else
	static constexpr const char			dir_native_separator = '/';
#endif

	// 通用路径分割符
	static constexpr const char			dir_common_separator = '/';

private:
	// Private structures
	XFolderPrivate*				d_ptr;

public:
	// 过滤器
	typedef enum Filter
	{
		Dirs				= 0x001,
		Files				= 0x002,
		NoDot				= 0x2000,
		NoDotDot			= 0x4000,
		NoDotAndDotDot			= NoDot | NoDotDot,
		DefFilter			= Dirs | Files | NoDotAndDotDot,
		NoFilter			= -1
	}Filter;
	// Q_DECLARE_FLAGS(Filters, Filter);
	typedef int 				Filters;

public:
	// constructor
	XFolder() noexcept;

	// constructor
	explicit XFolder(XFolderPrivate* _Private) noexcept;

	// constructor
	explicit XFolder(const XString& _Path) noexcept;

	// constructor
	XFolder(const XFolder& _Right) noexcept;

	// constructor
	XFolder(XFolder&& _Right) noexcept;

	// destructor
	virtual ~XFolder() noexcept;

public:
	// operator override =
	XFolder& operator=(const XFolder& _Right) noexcept;

	// operator override =
	XFolder& operator=(XFolder&& _Right) noexcept;

public:
	// [get] 路径
	virtual XString path() const noexcept final;

public:
	// [opt] 子级路径列表
	virtual std::list<XString> entryList(Filters _Filters = NoFilter) const noexcept final;

	// [opt] 子级信息列表
	virtual std::list<XFileInfo> entryInfoList(Filters _Filters = NoFilter) const noexcept final;

	// [opt] 子级路径列表
	static std::list<XString> entryList(const XString& _DirPath, Filters _Filters = NoFilter) noexcept;

	// [opt] 子级信息列表
	static std::list<XFileInfo> entryInfoList(const XString& _DirPath, Filters _Filters = NoFilter) noexcept;

public:
	// [opt] 目录是否存在
	static bool exist(const XString& _DirPath) noexcept;

public:
	// [opt] 创建目录
	virtual bool mkdir(const XString& _DirName, int _Mode = 0755) const noexcept final;

	// [opt] 移除目录
	virtual bool rmdir(const XString& _DirName) const noexcept final;

	// [opt] 创建路径
	static bool mkpath(const XString& _DirPath, int _Mode = 0755) noexcept;

	// [opt] 移除路径
	static bool rmpath(const XString& _DirPath) noexcept;

public:
	// DIR - 递归计算目录的大小
	static x_uint64_t dirSize(const XString& _DirPath) noexcept;

	// DIR - 递归计算目录的子项
	static x_uint64_t dirCount(const XString& _DirPath) noexcept;

	// DIR - 检查目录是否存在
	static bool dirExist(const XString& _DirPath) noexcept;

	// DIR - 递归创建目录
	static bool dirCreate(const XString& _DirPath, int _Mode = 0755) noexcept;

	// DIR - 将目录和层次结构复制到其他位置
	static bool dirCopy(const XString& _SrcPath, const XString& _TagPath, FuncDirTransfer2 _Lambda = nullptr) noexcept;

	// DIR - 将目录和层次结构移动到其他位置
	static bool dirMove(const XString& _SrcPath, const XString& _TagPath, FuncDirTransfer2 _Lambda = nullptr) noexcept;

	// DIR - 递归删除目录
	static bool dirRemove(const XString& _DirPath, FuncDirTransfer1 _Lambda = nullptr) noexcept;
};

#endif
