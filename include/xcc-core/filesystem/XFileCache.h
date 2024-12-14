#ifndef			_XCC_CORE_FILE_CACHE_H_
#define			_XCC_CORE_FILE_CACHE_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// XCC - 文件缓存
class _XCOREAPI_ XFileCache
{
public:
	// constructor
	XFileCache() noexcept;

	// destructor
	virtual ~XFileCache() noexcept;

public:
	// [set] 根目录
	static void rootDir(const XString& _RootDir) noexcept;

	// [get] 根目录
	static XString rootDir() noexcept;

public:
	// [get] 随机一个缓存目录路径
	static XString randomDir() noexcept;

	// [get] 随机一个缓存文件路径
	static XString randomFile() noexcept;
};

#endif
