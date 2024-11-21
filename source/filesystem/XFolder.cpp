#include <xcc-core/filesystem/XFolder.h>
#include <xcc-core/filesystem/XFileInfo.h>
#include <source/filesystem/XFolderPrivate.h>
#include <xcc-core/filesystem/XFile.h>


// constructor
XFolder::XFolder() noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();
}

// constructor
XFolder::XFolder(XFolderPrivate* _Private) noexcept
{
	d_ptr = _Private;
}

// constructor
XFolder::XFolder(const XString& _Path) noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();

	d_ptr->path = XFileInfo::pathToCommon(_Path);
}

// constructor
XFolder::XFolder(const XFolder& _Right) noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// constructor
XFolder::XFolder(XFolder&& _Right) noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// destructor
XFolder::~XFolder() noexcept
{
	delete d_ptr;
}



// operator override =
XFolder& XFolder::operator=(const XFolder& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}

// operator override =
XFolder& XFolder::operator=(XFolder&& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}



// [get] 路径
XString XFolder::path() const noexcept
{
	return d_ptr->path;
}



// [opt] 子级路径列表
std::list<XString> XFolder::entryList(Filters _Filters) const noexcept
{
	return XFolder::entryList(d_ptr->path, _Filters);
}

// [opt] 子级信息列表
std::list<XFileInfo> XFolder::entryInfoList(Filters _Filters) const noexcept
{
	return XFolder::entryInfoList(d_ptr->path, _Filters);
}

// [opt] 子级路径列表
std::list<XString> XFolder::entryList(const XString& _DirPath, Filters _Filters) noexcept
{
	auto		vRevPath = XFileInfo::pathToCommon(_DirPath);
	auto		vPathList = std::list<XString>();
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vDirWin32 = vRevPath.toWString() + L"/*.*";
	auto		vFileInfo = _wfinddata64_t();
	auto		vHandle = _wfindfirst64(vDirWin32.data(), &vFileInfo);
	if(vHandle == -1L)
	{
		return {};
	}
	do
	{
		auto		vFullPath = vRevPath + L'/' + XString::fromWString(vFileInfo.name);
		if(_Filters == NoFilter)
		{
			vPathList.push_back(vFullPath);
		}
		else
		{
			if(0 == x_posix_wcscmp(vFileInfo.name, L".") && (_Filters & XFolder::NoDot))
			{
			}
			else if(0 == x_posix_wcscmp(vFileInfo.name, L"..") && (_Filters & XFolder::NoDotDot))
			{
			}
			else if(XFileInfo::isDir(vFullPath) && (_Filters & XFolder::Dirs))
			{
				vPathList.push_back(vFullPath);
			}
			else if(XFileInfo::isFile(vFullPath) && (_Filters & XFolder::Files))
			{
				vPathList.push_back(vFullPath);
			}
		}
	}while(_wfindnext64(vHandle, &vFileInfo) == 0);
	_findclose(vHandle);
#else
	auto 		vHandle = ::opendir(vRevPath.data());
	if(nullptr == vHandle)
	{
		return {};
	}
	auto		vDirEntry = ::readdir(vHandle);
	while(vDirEntry)
	{
		auto		vFullPath = vRevPath + '/' + vDirEntry->d_name;
		if(_Filters == NoFilter)
		{
			vPathList.push_back(vFullPath);
		}
		else
		{
			if(0 == x_posix_strcmp(vDirEntry->d_name, ".") && (_Filters & XFolder::NoDot))
			{
			}
			else if(0 == x_posix_strcmp(vDirEntry->d_name, "..") && (_Filters & XFolder::NoDotDot))
			{
			}
			else if(XFileInfo::isDir(vFullPath) && (_Filters & XFolder::Dirs))
			{
				vPathList.push_back(vFullPath);
			}
			else if(XFileInfo::isFile(vFullPath) && (_Filters & XFolder::Files))
			{
				vPathList.push_back(vFullPath);
			}
		}
		vDirEntry = ::readdir(vHandle);
	}
	::closedir(vHandle);
#endif
	return vPathList;
}

// [opt] 子级信息列表
std::list<XFileInfo> XFolder::entryInfoList(const XString& _DirPath, Filters _Filters) noexcept
{
	auto		vPathList = XFolder::entryList(_DirPath, _Filters);
	auto		vInfoList = std::list<XFileInfo>();
	for(const auto & vIterator : vPathList)
	{
		vInfoList.emplace_back(vIterator);
	}
	return vInfoList;
}



// [opt] 目录是否存在
bool XFolder::exist(const XString& _DirPath) noexcept
{
	auto		vDirPath = XFileInfo::pathToCommon(_DirPath);
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vAttrib = ::GetFileAttributesW(vDirPath.toWString().data());
	if(INVALID_FILE_ATTRIBUTES != vAttrib && vAttrib & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
#else
	x_file_stat_t	vFileStatus{};
	if(0 == x_fs_path_stat(vDirPath.data(), &vFileStatus))
	{
		return S_ISDIR(vFileStatus.st_mode);
	}
#endif
	return false;
}



// [opt] 创建目录
bool XFolder::mkdir(const XString& _DirName, int _Mode) const noexcept
{
	return XFolder::mkpath(d_ptr->path + "/" + _DirName, _Mode);
}

// [opt] 移除目录
bool XFolder::rmdir(const XString& _DirName) const noexcept
{
	return XFolder::rmpath(d_ptr->path + "/" + _DirName);
}

// [opt] 创建路径
bool XFolder::mkpath(const XString& _DirPath, int _Mode) noexcept
{
	auto		vRevPath = XFileInfo::pathToCommon(_DirPath);
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vSync = x_posix_wmkdir_r(vRevPath.toWString().data(), _Mode);
#else
	auto		vSync = x_posix_mkdir_r(vRevPath.data(), _Mode);
#endif
	return 0 == vSync;
}

// [opt] 移除路径
bool XFolder::rmpath(const XString& _DirPath) noexcept
{
	// 检查路径属性
	if(!XFileInfo::isDir(_DirPath))
	{
		return false;
	}

	// 首先删除子项
	auto		vInfoList = XFolder::entryInfoList(_DirPath, XFolder::DefFilter);
	auto		vSyncN = 0u;
	for(const auto & vIterator : vInfoList)
	{
		bool		vSyncB = false;
		if(vIterator.isDir())
		{
			vSyncB = XFolder::rmpath(vIterator.filePath());
		}
		else
		{
			vSyncB = XFileInfo::remove(vIterator.filePath());
		}
		vSyncN += vSyncB ? 1 : 0;
	}
	if(vSyncN != vInfoList.size())
	{
		return false;
	}

	// 删除本身
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vSyncB = x_posix_wrmdir(_DirPath.toWString().data());
#else
	auto		vSyncB = x_posix_rmdir(_DirPath.data());
#endif
	return 0 == vSyncB;
}



// DIR - 递归计算目录的大小
x_uint64_t XFolder::dirSize(const XString& _DirPath) noexcept
{
	x_uint64_t	vDirSize = 0;
	auto		vInfoArray = XFolder::entryInfoList(_DirPath, XFolder::DefFilter);
	for(const auto & vIterator : vInfoArray)
	{
		if(vIterator.isFile())
		{
			vDirSize += vIterator.size();
		}
		else
		{
			vDirSize += XFolder::dirSize(vIterator.filePath());
		}
	}
	return vDirSize;
}

// DIR - 递归计算目录的子项
x_uint64_t XFolder::dirCount(const XString& _DirPath) noexcept
{
	x_uint64_t	vDirCount = 0;
	auto		vInfoArray = XFolder::entryInfoList(_DirPath, XFolder::DefFilter);
	for(const auto & vIterator : vInfoArray)
	{
		if(vIterator.isFile())
		{
			++vDirCount;
		}
		else
		{
			vDirCount += XFolder::dirCount(vIterator.filePath());
		}
	}
	return vDirCount;
}

// DIR - 检查目录是否存在
bool XFolder::dirExist(const XString& _DirPath) noexcept
{
	return XFolder::exist(_DirPath);
}

// DIR - 递归创建目录
bool XFolder::dirCreate(const XString& _DirPath, int _Mode) noexcept
{
	return XFolder::mkpath(_DirPath, _Mode);
}

// DIR - 将目录和层次结构复制到其他位置
bool XFolder::dirCopy(const XString& _SrcPath, const XString& _TagPath, FuncDirTransfer2 _Lambda) noexcept
{
	XFolder::mkpath(_TagPath);

	auto		vSyncB = true;
	auto		vInfoArray = XFolder::entryInfoList(_SrcPath, XFolder::DefFilter);
	for(const auto & vIterator : vInfoArray)
	{
		auto		vTarFull = _TagPath + XFolder::dir_native_separator +  vIterator.fileName();
		if(vIterator.isFile())
		{
			vSyncB = XFile::copy(vIterator.filePath(), vTarFull, nullptr);
		}
		else
		{
			XFolder::mkpath(vTarFull);
			vSyncB = XFolder::dirCopy(vIterator.filePath(), vTarFull, _Lambda);
		}
		if(!vSyncB)
		{
			break;
		}
	}
	return vSyncB;
}

// DIR - 将目录和层次结构移动到其他位置
bool XFolder::dirMove(const XString& _SrcPath, const XString& _TagPath, FuncDirTransfer2 _Lambda) noexcept
{
	auto		vSyncC = XFolder::dirCopy(_SrcPath, _TagPath, _Lambda);
	if(vSyncC)
	{
		vSyncC = XFolder::dirRemove(_SrcPath, nullptr);
	}
	return vSyncC;
}

// DIR - 递归删除目录
bool XFolder::dirRemove(const XString& _DirPath, FuncDirTransfer1 _Lambda) noexcept
{
	auto		vSyncB = true;
	auto		vInfoArray = XFolder::entryInfoList(_DirPath, XFolder::DefFilter);
	for(const auto & vIterator : vInfoArray)
	{
		if(vIterator.isFile())
		{
			vSyncB = XFile::remove(vIterator.filePath());
		}
		else
		{
			vSyncB = XFolder::dirRemove(vIterator.filePath(), _Lambda);
		}
		if(!vSyncB)
		{
			break;
		}
	}

	if(vSyncB && XFolder::dirExist(_DirPath))
	{
		auto		vRevPath = XFileInfo::pathToCommon(_DirPath);
#if defined(XCC_SYSTEM_WINDOWS)
		auto		vRemove = x_posix_wrmdir(vRevPath.toWString().data());
#else
		auto		vRemove = x_posix_rmdir(vRevPath.data());
#endif
		vSyncB = (0 == vRemove);
	}
	return vSyncB;
}
