#ifndef			_XCC_CORE_FOLDER_H_
#define			_XCC_CORE_FOLDER_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 文件系统 - 目录操作
struct XFolderPrivate;
typedef struct XFolderPrivate			XFolderPrivate;
class _XCOREAPI_ XFolder
{
private:
	// Private structures
	XFolderPrivate*				d_ptr;

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
};

#endif
