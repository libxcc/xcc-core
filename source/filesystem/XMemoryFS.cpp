#include <xcc-core/filesystem/XMemoryFS.h>



// 内存文件系统
struct XMemoryFSPrivate
{
	// 根目录
	XString					rootPath;
};



// constructor
XMemoryFS::XMemoryFS() noexcept
{
	d_ptr = new(std::nothrow) XMemoryFSPrivate();
}

// constructor
XMemoryFS::XMemoryFS(XMemoryFSPrivate* _Private) noexcept
{
	d_ptr = _Private;
}

// constructor
XMemoryFS::XMemoryFS(const XString& _RootDir) noexcept
{
	d_ptr = new(std::nothrow) XMemoryFSPrivate();

	d_ptr->rootPath = _RootDir;
}

// constructor
XMemoryFS::XMemoryFS(const XMemoryFS& _Right) noexcept
{
	d_ptr = new(std::nothrow) XMemoryFSPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// constructor
XMemoryFS::XMemoryFS(XMemoryFS&& _Right) noexcept
{
	d_ptr = new(std::nothrow) XMemoryFSPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// destructor
XMemoryFS::~XMemoryFS() noexcept
{
	delete d_ptr;
}



// operator override =
XMemoryFS& XMemoryFS::operator=(const XMemoryFS& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}

// operator override =
XMemoryFS& XMemoryFS::operator=(XMemoryFS&& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}
