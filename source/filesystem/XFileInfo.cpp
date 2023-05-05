#include <xcc-core/filesystem/XFileInfo.h>
#include <source/filesystem/XFileInfoPrivate.h>


// constructor
XFileInfo::XFileInfo() noexcept
{
	d_ptr = new(std::nothrow) XFileInfoPrivate();
}

// constructor
XFileInfo::XFileInfo(XFileInfoPrivate* _Private) noexcept
{
	d_ptr = _Private;
}

// constructor
XFileInfo::XFileInfo(const XString& _Path) noexcept
{
	d_ptr = new(std::nothrow) XFileInfoPrivate();

	d_ptr->path = XFileInfo::pathToNative(_Path);
}

// constructor
XFileInfo::XFileInfo(const XFileInfo& _Right) noexcept
{
	d_ptr = new(std::nothrow) XFileInfoPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// constructor
XFileInfo::XFileInfo(XFileInfo&& _Right) noexcept
{
	d_ptr = new(std::nothrow) XFileInfoPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// destructor
XFileInfo::~XFileInfo() noexcept
{
	delete d_ptr;
}



// operator =
XFileInfo& XFileInfo::operator=(const XFileInfo& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}

// operator =
XFileInfo& XFileInfo::operator=(XFileInfo&& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}



// [fmt] Convert path to native format
XString XFileInfo::pathToNative(const XString& _Path) noexcept
{
	auto		vFilePath = _Path;
#if defined(XCC_SYSTEM_WINDOWS)
	vFilePath.replace('/', '\\');
	while(vFilePath.size() > 1 && vFilePath[vFilePath.size() - 1] == '\\')
#else
	vFilePath.replace('\\', '/');
	while(vFilePath.size() > 1 && vFilePath[vFilePath.size() - 1] == '/')
#endif
	{
		vFilePath.remove(vFilePath.size() - 1, 1);
	}
	return vFilePath;
}

// [fmt] Convert path to common format
XString XFileInfo::pathToCommon(const XString& _Path) noexcept
{
	auto		vFilePath = _Path;
	vFilePath.replace('\\', '/');
	while(vFilePath.size() > 1 && vFilePath[vFilePath.size() - 1] == '/')
	{
		vFilePath.remove(vFilePath.size() - 1, 1);
	}
	return vFilePath;
}



// [static] Check if the path exists
bool XFileInfo::exist(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);
#if defined(XCC_SYSTEM_WINDOWS)
	return 0 == x_posix_waccess(vFilePath.toWString().data(), F_OK);
#else
	return 0 == x_posix_access(vFilePath.data(), F_OK);
#endif
}

// [static] checks whether the given path refers to a directory
bool XFileInfo::isDir(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);

#if defined(XCC_SYSTEM_WINDOWS)
	auto		vAttrib = ::GetFileAttributesW(vFilePath.toWString().data());
	if(INVALID_FILE_ATTRIBUTES != vAttrib && vAttrib & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
#else
	x_file_stat_t	vFileStatus{};
	if(0 == x_fs_path_stat(vFilePath.data(), &vFileStatus))
	{
		return S_ISDIR(vFileStatus.st_mode);
	}
#endif
	return false;
}

// [static] checks whether the given path refers to a file
bool XFileInfo::isFile(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);
	if(XFileInfo::exist(vFilePath.data()))
	{
		return !XFileInfo::isDir(_Path);
	}
	return false;
}

// [static] returns the size of a path
x_uint64_t XFileInfo::size(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);
	return x_posix_fsize(vFilePath.data());
}



// clear all status flags
void XFileInfo::clear() const noexcept
{
	d_ptr->status = 0;
}

// Does the file exist
bool XFileInfo::exist() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_EXIST) == 0)
	{
		d_ptr->exist = XFileInfo::exist(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_EXIST;
	return d_ptr->exist;
}

// checks whether the given path refers to a directory
bool XFileInfo::isDir() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_TYPE) == 0)
	{
		d_ptr->isDir = XFileInfo::isDir(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_TYPE;
	return d_ptr->isDir;
}

// checks whether the given path refers to a file
bool XFileInfo::isFile() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_TYPE) == 0)
	{
		d_ptr->isDir = XFileInfo::isDir(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_TYPE;
	return !d_ptr->isDir;
}

// Query file size
x_uint64_t XFileInfo::size() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_SIZE) == 0)
	{
		d_ptr->size = XFileInfo::size(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_SIZE;
	return d_ptr->size;
}



// file parent directory path
XString XFileInfo::dirPath() const noexcept
{
	auto		vPos = d_ptr->path.rfind("/");
	if(vPos == XString::npos)
	{
		return d_ptr->path;
	}
	else
	{
		auto		vDirPath = d_ptr->path.left(vPos);
		return vDirPath;
	}
}

// file absolute path
XString XFileInfo::filePath() const noexcept
{
	return d_ptr->path;
}

// file name
XString XFileInfo::fileName() const noexcept
{
	auto		vPos = d_ptr->path.rfind("/");
	if(vPos == XString::npos)
	{
		return d_ptr->path;
	}
	else
	{
		auto		vFileName = d_ptr->path.right(d_ptr->path.size() - vPos - 1);
		return vFileName;
	}
}

// returns the stem path component
XString XFileInfo::stem() const noexcept
{
	auto		vFileName = this->fileName();
	auto		vPos = vFileName.rfind(".");
	if(vPos == XString::npos)
	{
		return vFileName;
	}
	else
	{
		return vFileName.left(vPos);
	}
}

// returns the file extension path component
XString XFileInfo::extension() const noexcept
{
	return this->suffix();
}

// returns the file extension path component
XString XFileInfo::suffix() const noexcept
{
	auto		vFileName = this->fileName();
	auto		vPos = vFileName.rfind(".");
	if(vPos == XString::npos)
	{
		return {};
	}
	else
	{
		auto		vSuffix = vFileName.right(vFileName.size() - vPos);
		return vSuffix;
	}
}
