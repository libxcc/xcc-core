#ifndef			_XCC_CORE_FOLDER_PRIVATE_H_
#define			_XCC_CORE_FOLDER_PRIVATE_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>



// File Information Enumeration
typedef enum XFolderStatus
{
	FILE_EXIST				= 1 << 0,
	FILE_SIZE				= 1 << 1,
	FILE_TYPE				= 1 << 2
}XFolderStatus;

// File Information Private Data
struct XFolderPrivate
{
	XString					path;			// 全路径
	bool 					exist;			// 是否存在

	// constructor
	XFolderPrivate()
	{
		this->exist = false;
	}
};


#endif
