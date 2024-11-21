#ifndef			_XCC_CORE_MEMORY_FS_H_
#define			_XCC_CORE_MEMORY_FS_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 内存文件系统
struct XMemoryFSPrivate;
typedef struct XMemoryFSPrivate			XMemoryFSPrivate;
class _XCOREAPI_ XMemoryFS
{
private:
	// Private structures
	XMemoryFSPrivate*			d_ptr;

public:
	// constructor
	XMemoryFS() noexcept;

	// constructor
	explicit XMemoryFS(XMemoryFSPrivate* _Private) noexcept;

	// constructor
	explicit XMemoryFS(const XString& _RootDir) noexcept;

	// constructor
	XMemoryFS(const XMemoryFS& _Right) noexcept;

	// constructor
	XMemoryFS(XMemoryFS&& _Right) noexcept;

	// destructor
	virtual ~XMemoryFS() noexcept;

public:
	// operator override =
	XMemoryFS& operator=(const XMemoryFS& _Right) noexcept;

	// operator override =
	XMemoryFS& operator=(XMemoryFS&& _Right) noexcept;
};

#endif
