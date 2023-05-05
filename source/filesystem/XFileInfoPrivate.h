#ifndef			_XCC_CORE_FILE_INFO_PRIVATE_H_
#define			_XCC_CORE_FILE_INFO_PRIVATE_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>



// File Information Enumeration
typedef enum XFileInfoStatus
{
	FILE_EXIST				= 1 << 0,
	FILE_SIZE				= 1 << 1,
	FILE_TYPE				= 1 << 2
}XFileInfoStatus;

// File Information Private Data
struct XFileInfoPrivate
{
	x_uint64_t 				status;			// 查找状态
	XString					path;			// 全路径
	bool 					exist;			// 是否存在
	x_uint64_t 				size;			// 文件大小
	bool 					isDir;			// 是否为目录

	// constructor
	XFileInfoPrivate()
	{
		this->status = 0;
		this->exist = false;
		this->size = 0;
		this->isDir = false;
	}
};


#endif
