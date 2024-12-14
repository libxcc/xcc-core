#include <xcc-core/filesystem/XFileCache.h>
#include <xcc-core/filesystem/XFolder.h>
#include <xcc-core/filesystem/XStandardPath.h>
#include <xcc-core/XUuid.h>



// constructor
XFileCache::XFileCache() noexcept = default;

// destructor
XFileCache::~XFileCache() noexcept = default;



// [val] 根目录
static XString			static_root_dir = nullptr;

// [set] 根目录
void XFileCache::rootDir(const XString& _RootDir) noexcept
{
	static_root_dir = _RootDir;
	XFolder::dirCreate(static_root_dir);
}

// [get] 根目录
XString XFileCache::rootDir() noexcept
{
	if(nullptr == static_root_dir)
	{
		static_root_dir = XStandardPath::appCacheLocation();
		XFolder::dirCreate(static_root_dir);
	}
	return static_root_dir;
}



// [get] 随机一个缓存目录路径
XString XFileCache::randomDir() noexcept
{
	while (true)
	{
		auto		vTempPath = XFileCache::rootDir() + "/" + XUuid::createUuid().toString(XUuid::WithoutBraces);
		if(XFileInfo::exist(vTempPath))
		{
			continue;
		}
		XFolder::dirCreate(vTempPath);
		return vTempPath;
	}
}

// [get] 随机一个缓存文件路径
XString XFileCache::randomFile() noexcept
{
	while (true)
	{
		auto		vTempPath = XFileCache::rootDir() + "/" + XUuid::createUuid().toString(XUuid::WithoutBraces);
		if(XFileInfo::exist(vTempPath))
		{
			continue;
		}
		return vTempPath;
	}
}
