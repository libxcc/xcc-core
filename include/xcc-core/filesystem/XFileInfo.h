#ifndef			_XCC_CORE_FILE_INFO_H_
#define			_XCC_CORE_FILE_INFO_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 文件详情
struct XFileInfoPrivate;
typedef struct XFileInfoPrivate			XFileInfoPrivate;
class _XCOREAPI_ XFileInfo
{
private:
	// Private structures
	XFileInfoPrivate*			d_ptr;

public:
	// constructor
	XFileInfo() noexcept;

	// constructor
	explicit XFileInfo(XFileInfoPrivate* _Private) noexcept;

	// constructor
	explicit XFileInfo(const XString& _Path) noexcept;

	// constructor
	XFileInfo(const XFileInfo& _Right) noexcept;

	// constructor
	XFileInfo(XFileInfo&& _Right) noexcept;

	// destructor
	virtual ~XFileInfo() noexcept;

public:
	// operator override =
	XFileInfo& operator=(const XFileInfo& _Right) noexcept;

	// operator override =
	XFileInfo& operator=(XFileInfo&& _Right) noexcept;

public:
	// [fmt] 转换至本机路径
	static XString pathToNative(const XString& _Path) noexcept;

	// [fmt] 转换至通用路径
	static XString pathToCommon(const XString& _Path) noexcept;

	// [fmt] 路径修复\/:*?"<>|
	static XString pathRepairInvalid(const XString& _Path) noexcept;

	// [fmt] 路径包装空格
	static XString pathRepairSpace(const XString& _Path) noexcept;

	// [opt] 检查名称是否符合规范
	static bool isCorrect(const XString& _FileName) noexcept;

public:
	// [static] 检查路径是否存在
	static bool exist(const XString& _Path) noexcept;

	// [static] 检查路径是否为目录
	static bool isDir(const XString& _Path) noexcept;

	// [static] 检查路径是否为文件
	static bool isFile(const XString& _Path) noexcept;

	// [static] 获取文件大小
	static x_uint64_t size(const XString& _Path) noexcept;

	// [static] 删除指定路径
	static bool remove(const XString& _Path) noexcept;

	// [static] 重命名指定路径
	static bool rename(const XString& _PathOld, const XString& _PathNew) noexcept;

public:
	// 清理所有状态标记
	virtual void clear() const noexcept final;

	// 检查路径是否存在
	virtual bool exist() const noexcept final;

	// 检查路径是否为目录
	virtual bool isDir() const noexcept final;

	// 检查路径是否为文件
	virtual bool isFile() const noexcept final;

	// 获取文件大小
	virtual x_uint64_t size() const noexcept final;

public:
	// 路径的父目录
	virtual XString dirPath() const noexcept final;

	// 路径的绝对路径
	virtual XString filePath() const noexcept final;

	// 路径的名称
	virtual XString fileName() const noexcept final;

	// 路径不带后缀的名称
	virtual XString stem() const noexcept final;

	// 返回文件的扩展名
	virtual XString extension() const noexcept final;

	// 返回文件的扩展名
	virtual XString suffix() const noexcept final;
};

#endif
