#include <xcc-core/filesystem/XFileInfo.h>
#include <source/filesystem/XFileInfoPrivate.h>
#include <xcc-core/filesystem/XFile.h>
#include <xcc-core/filesystem/XFolder.h>
#include <xcc-core/filesystem/XStandardPath.h>


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

	d_ptr->path = XFileInfo::pathToCommon(_Path);
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



// operator override =
XFileInfo& XFileInfo::operator=(const XFileInfo& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}

// operator override =
XFileInfo& XFileInfo::operator=(XFileInfo&& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}



// [fmt] 转换至本机路径
XString XFileInfo::pathToNative(const XString& _Path) noexcept
{
	auto		vFilePath = _Path;
#if defined(XCC_SYSTEM_WINDOWS)
	vFilePath.replace('/', '\\');
	vFilePath.replace("\\\\", '\\');
	while(vFilePath.size() > 1 && vFilePath[vFilePath.size() - 1] == '\\')
#else
	vFilePath.replace('\\', '/');
	vFilePath.replace("//", '/');
	while(vFilePath.size() > 1 && vFilePath[vFilePath.size() - 1] == '/')
#endif
	{
		vFilePath.remove(vFilePath.size() - 1, 1);
	}
	return vFilePath;
}

// [fmt] 转换至通用路径
XString XFileInfo::pathToCommon(const XString& _Path) noexcept
{
	auto		vFilePath = _Path;
	vFilePath.replace('\\', '/');
	vFilePath.replace("//", '/');
	while(vFilePath.size() > 1 && vFilePath[vFilePath.size() - 1] == '/')
	{
		vFilePath.remove(vFilePath.size() - 1, 1);
	}
	return vFilePath;
}

// [fmt] 路径修复\/:*?"<>|
XString XFileInfo::pathRepairInvalid(const XString& _Path) noexcept
{
	auto		vRevPath = _Path;
	auto		vPos = vRevPath.rfind('/');
	for(auto vIndex = ((vPos == XString::npos) ? 0 : vPos); vIndex < vRevPath.size(); ++vIndex)
	{
		switch(vRevPath[vIndex])
		{
			//\/:*?"<>|
			case ':':
			case '*':
			case '?':
			case '\"':
			case '<':
			case '>':
			case '|':
				vRevPath[vIndex] = '_';
				break;
			default:
				break;
		}
	}
	return vRevPath;
}

// [fmt] 路径包装空格
XString XFileInfo::pathRepairSpace(const XString& _Path) noexcept
{
	if(_Path.contains(" "))
	{
		return XString("\"") + _Path + XString("\"");
	}
	return _Path;
}

// [opt] 检查名称是否符合规范
bool XFileInfo::isCorrect(const XString& _FileName) noexcept
{
	auto		vFilePath = XStandardPath::appCacheLocation() + "/" + _FileName;
	auto		vHandle = XFile::fopen(vFilePath, "wb");
	if(vHandle)
	{
		XFile::fclose(vHandle);
		XFile::remove(vFilePath);
		return true;
	}
	return false;
}



// [static] 检查路径是否存在
bool XFileInfo::exist(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);
#if defined(XCC_SYSTEM_WINDOWS)
	return 0 == x_posix_waccess(vFilePath.toWString().data(), F_OK);
#else
	return 0 == x_posix_access(vFilePath.data(), F_OK);
#endif
}

// [static] 检查路径是否为目录
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

// [static] 检查路径是否为文件
bool XFileInfo::isFile(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);
	if(XFileInfo::exist(vFilePath.data()))
	{
		return !XFileInfo::isDir(_Path);
	}
	return false;
}

// [static] 获取文件大小
x_uint64_t XFileInfo::size(const XString& _Path) noexcept
{
	if(XFileInfo::isDir(_Path))
	{
		return XFolder::dirSize(_Path);
	}
	else
	{
		return XFile::fsize(_Path);
	}
}

// [static] 删除指定路径
bool XFileInfo::remove(const XString& _Path) noexcept
{
	if(XFileInfo::isDir(_Path))
	{
		return XFolder::dirRemove(_Path, nullptr);
	}
	else
	{
		return XFile::remove(_Path);
	}
}

// [static] 重命名指定路径
bool XFileInfo::rename(const XString& _PathOld, const XString& _PathNew) noexcept
{
	return XFile::rename(_PathOld, _PathNew);
}



// 清理所有状态标记
void XFileInfo::clear() const noexcept
{
	d_ptr->status = 0;
}

// 检查路径是否存在
bool XFileInfo::exist() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_EXIST) == 0)
	{
		d_ptr->exist = XFileInfo::exist(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_EXIST;
	return d_ptr->exist;
}

// 检查路径是否为目录
bool XFileInfo::isDir() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_TYPE) == 0)
	{
		d_ptr->isDir = XFileInfo::isDir(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_TYPE;
	return d_ptr->isDir;
}

// 检查路径是否为文件
bool XFileInfo::isFile() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_TYPE) == 0)
	{
		d_ptr->isDir = XFileInfo::isDir(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_TYPE;
	return !d_ptr->isDir;
}

// 获取文件大小
x_uint64_t XFileInfo::size() const noexcept
{
	if((d_ptr->status & XFileInfoStatus::FILE_SIZE) == 0)
	{
		d_ptr->size = XFileInfo::size(d_ptr->path);
	}
	d_ptr->status &= XFileInfoStatus::FILE_SIZE;
	return d_ptr->size;
}



// 路径的父目录
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

// 路径的绝对路径
XString XFileInfo::filePath() const noexcept
{
	return d_ptr->path;
}

// 路径的名称
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

// 路径不带后缀的名称
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

// 返回文件的扩展名
XString XFileInfo::extension() const noexcept
{
	return this->suffix();
}

// 返回文件的扩展名
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
