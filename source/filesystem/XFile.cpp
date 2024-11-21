#include <xcc-core/filesystem/XFile.h>
#include <xcc-core/filesystem/XFileInfo.h>
#include <xcc-core/serialize/XBase64.h>
#include <xcc-core/filesystem/XFolder.h>



// [api] 打开文件
FILE* XFile::fopen(const XString& _FilePath, const XString& _Mode) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_FilePath);
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vFlags = XString(_Mode).toWString();
	auto		vHandle = x_posix_wfopen(vFilePath.toWString().data(), vFlags.data());
#else
	auto		vHandle = x_posix_fopen(vFilePath.data(), _Mode.data());
#endif
	return vHandle;
}

// [api] 跳转至指定位置
x_int64_t XFile::fseek(FILE* _Handle, x_int64_t _Offset, int _Origin) noexcept
{
	if(nullptr == _Handle)
	{
		return 0;
	}
	return x_posix_fseek64(_Handle, _Offset, _Origin);
}

// [api] 取出当前偏移
x_int64_t XFile::ftell(FILE* _Handle) noexcept
{
	if(nullptr == _Handle)
	{
		return 0;
	}
	return x_posix_ftell(_Handle);
}

// [api] 是否已到文件末尾
bool XFile::feof(FILE* _Handle) noexcept
{
	if(nullptr == _Handle)
	{
		return true;
	}
	return x_posix_feof(_Handle);
}

// [api] 读取指定长度的数据
bool XFile::fread(FILE* _Handle, void* _Buffer, x_uint64_t _Length) noexcept
{
	if(nullptr == _Handle || nullptr == _Buffer || 0 == _Length)
	{
		return false;
	}

	uint64_t 	vAlready = 0;
	size_t 		vSync = 0;

	while(vAlready < _Length)
	{
		vSync = x_posix_fread((char*)(_Buffer) + vAlready, 1, static_cast<size_t>(_Length - vAlready), _Handle);
		if(vSync <= 0)
		{
			break;
		}
		vAlready += vSync;
	}
	return vAlready == _Length;
}

// [api] 读取一行数据
XByteArray XFile::freadLine(FILE* _Handle) noexcept
{
	if(nullptr == _Handle)
	{
		return {};
	}

	auto		vBytes = XByteArray();
	auto		vBuffer = new(std::nothrow) char[XCC_SIZE_MB];
	if(vBuffer)
	{
		if(x_posix_fgets(vBuffer, XCC_SIZE_MB - 2, _Handle))
		{
			vBytes = XByteArray(vBuffer, x_posix_strlen(vBuffer));
		}
		XCC_DELETE_ARR(vBuffer);
	}
	return vBytes;
}

// [api] 读取指定项的数据
x_uint64_t XFile::fread(void* _Buffer, x_uint64_t _Size, x_uint64_t _Count, FILE* _Handle) noexcept
{
	if(nullptr == _Handle || nullptr == _Buffer || 0 == _Size || 0 == _Count)
	{
		return 0;
	}
	return x_posix_fread(_Buffer, static_cast<size_t>(_Size), static_cast<size_t>(_Count), _Handle);
}

// [api] 写入所有数据
bool XFile::fwrite(FILE* _Handle, const void* _Buffer, x_uint64_t _Length) noexcept
{
	if(nullptr == _Handle || nullptr == _Buffer || 0 == _Length)
	{
		return false;
	}

	uint64_t 	vAlready = 0;
	size_t 		vSync = 0;

	while(vAlready < _Length)
	{
		vSync = x_posix_fwrite((const char*)(_Buffer) + vAlready, 1, static_cast<size_t>(_Length - vAlready), _Handle);
		if(vSync <= 0)
		{
			break;
		}
		vAlready += vSync;
	}
	return vAlready == _Length;
}

// [api] 写入所有数据
bool XFile::fwrite(FILE* _Handle, const XByteArray& _Buffer) noexcept
{
	if(nullptr == _Handle || nullptr == _Buffer)
	{
		return false;
	}
	return XFile::fwrite(_Handle, _Buffer.data(), _Buffer.size());
}

// [api] 写入指定项的数据
x_uint64_t XFile::fwrite(const void* _Buffer, x_uint64_t _Size, x_uint64_t _Count, FILE* _Handle) noexcept
{
	if(nullptr == _Handle || nullptr == _Buffer || 0 == _Size || 0 == _Count)
	{
		return 0;
	}
	return x_posix_fwrite(_Buffer, static_cast<size_t>(_Size), static_cast<size_t>(_Count), _Handle);
}

// [api] 刷新流
bool XFile::fflush(FILE* _Handle) noexcept
{
	if(nullptr == _Handle)
	{
		return false;
	}
	return 0 == x_posix_fflush(_Handle);
}

// [api] 关闭文件
bool XFile::fclose(FILE* _Handle) noexcept
{
	if(nullptr == _Handle)
	{
		return false;
	}
	return 0 == x_posix_fclose(_Handle);
}



// [opt] 文件大小
x_uint64_t XFile::fsize(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);
	return x_posix_fsize(vFilePath.data());
}

// [opt] 检查路径是否存在
bool XFile::exist(const XString& _Path) noexcept
{
	auto		vFilePath = XFileInfo::pathToNative(_Path);
#if defined(XCC_SYSTEM_WINDOWS)
	if(0 == x_posix_waccess(vFilePath.toWString().data(), F_OK))
	{
		return !XFolder::exist(_Path);
	}
#else
	if(0 == x_posix_access(vFilePath.data(), F_OK))
	{
		return !XFolder::exist(_Path);
	}
#endif
	return false;
}

// [opt] 重命名
bool XFile::rename(const XString& _SrcPath, const XString& _TagPath) noexcept
{
	XFile::remove(_TagPath);

	auto		vSrcPath = XFileInfo::pathToNative(_SrcPath);
	auto		vTagPath = XFileInfo::pathToNative(_TagPath);
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vSync = x_posix_wrename(vSrcPath.toWString().data(), vTagPath.toWString().data());
#else
	auto		vSync = x_posix_rename(vSrcPath.data(), vTagPath.data());
#endif
	if(0 == vSync)
	{
		return true;
	}
	else
	{
#if defined(XCC_SYSTEM_WINDOWS)
		if(::MoveFileW(vSrcPath.toWString().data(), vSrcPath.toWString().data()))
		{
			return true;
		}
#endif
		return false;
	}
}

// [opt] 删除
bool XFile::remove(const XString& _Path) noexcept
{
	if (XFile::exist(_Path))
	{
		auto		vFilePath = XFileInfo::pathToNative(_Path);
#if defined(XCC_SYSTEM_WINDOWS)
		auto		vRemove = x_posix_wremove(vFilePath.toWString().data());
#else
		auto		vRemove = x_posix_remove(vFilePath.data());
#endif
		if(0 == vRemove)
		{
			return true;
		}
		else
		{
#if defined(XCC_SYSTEM_WINDOWS)
			if(::DeleteFileW(vFilePath.toWString().data()))
			{
				return true;
			}
#endif
			auto		vFileXrm = vFilePath + XFile::rm_suffix;
#if defined(XCC_SYSTEM_WINDOWS)
			auto		vSync = x_posix_wrename(vFilePath.toWString().data(), vFileXrm.toWString().data());
#else
			auto		vSync = x_posix_rename(vFilePath.data(), vFileXrm.data());
#endif
			return vSync;
		}
	}
	return true;
}

// [opt] 复制
bool XFile::copy(const XString& _SrcPath, const XString& _TagPath, FuncFileTransfer _Lambda) noexcept
{
	auto		vSync = false;
	auto		vBytes = XByteArray(XCC_SIZE_MB, '\0');
	auto		vFileSize = XFile::fsize(_SrcPath);
	auto		vSyncSize = 0ULL;
	auto		vSrcHandle = XFile::fopen(_SrcPath, "rb");
	auto		vTagHandle = XFile::fopen(_TagPath, "wb");
	if(vFileSize == 0)
	{
		vSync = true;
	}
	else if(vSrcHandle && vTagHandle && vBytes.size() == XCC_SIZE_MB)
	{
		while(vFileSize > vSyncSize)
		{
			auto		vReadSize = ((vFileSize - vSyncSize) > XCC_SIZE_MB) ? XCC_SIZE_MB : (vFileSize - vSyncSize);
			if(!XFile::fread(vSrcHandle, vBytes.data(), vReadSize))
			{
				break;
			}
			vSyncSize += vReadSize;
			if(!XFile::fwrite(vTagHandle, vBytes.data(), vReadSize))
			{
				break;
			}
			if(_Lambda && !_Lambda(vSyncSize, vFileSize))
			{
				break;
			}
			if(vFileSize == vSyncSize)
			{
				vSync = true;
				break;
			}
		};
	}
#if !defined(XCC_SYSTEM_WINDOWS)
	// When successful, we need copy permissions
	if(vSync)
	{

	}
#endif
	XFile::fclose(vSrcHandle);
	XFile::fclose(vTagHandle);
#if defined(XCC_SYSTEM_WINDOWS)
	if(!vSync)
	{
		auto		vSrcPath = XFileInfo::pathToNative(_SrcPath);
		auto		vTagPath = XFileInfo::pathToNative(_TagPath);
		vSync = CopyFileW(vSrcPath.toWString().data(), vTagPath.toWString().data(), FALSE);
	}
#endif
	return vSync;
}

// [opt] 移动
bool XFile::move(const XString& _SrcPath, const XString& _TagPath, FuncFileTransfer _Lambda) noexcept
{
	if(XFile::rename(_SrcPath, _TagPath))
	{
		return true;
	}
	else
	{
		if(XFile::copy(_SrcPath, _TagPath, _Lambda))
		{
			XFile::remove(_SrcPath);
			return true;
		}
	}
	return false;
}



// [opt] 将文件内容读取至字节数组
XByteArray XFile::toBytes(const XString& _Path) noexcept
{
	auto		vSize = XFile::fsize(_Path);
	auto		vBytes = XByteArray(static_cast<x_size_t>(vSize), '\0');
	auto		vHandle = XFile::fopen(_Path, "rb");
	if(vHandle && vSize > 0 && vBytes.size() == vSize)
	{
		XFile::fread(vHandle, vBytes.data(), vSize);
	}
	XFile::fclose(vHandle);
	return vBytes;
}

// [opt] 将字节数组写入至文件内容
bool XFile::fromBytes(const XString& _Path, const void* _Bytes, x_size_t _Length) noexcept
{
	auto		vSync = false;
	auto		vHandle = XFile::fopen(_Path, "wb");
	if(vHandle)
	{
		if(_Bytes && _Length > 0)
		{
			vSync = XFile::fwrite(vHandle, _Bytes, _Length);
		}
		else
		{
			vSync = true;
		}
		XFile::fclose(vHandle);
	}
	return vSync;
}

// [opt] 将字节数组写入至文件内容
bool XFile::fromBytes(const XString& _Path, const XByteArray& _Bytes) noexcept
{
	return XFile::fromBytes(_Path, _Bytes.data(), _Bytes.size());
}

// [opt] 将文件内容读取至字节数组并编码Base64
XByteArray XFile::toBase64(const XString& _Path) noexcept
{
	return XFile::toBytes(_Path).toBase64();
}

// [opt] 将字节数组Base64解码并写入至文件内容
bool XFile::fromBase64(const XString& _Path, const void* _Base64, x_size_t _Length) noexcept
{
	return XFile::fromBytes(_Path, XBase64::decode(_Base64, _Length));
}

// [opt] 将字节数组Base64解码并写入至文件内容
bool XFile::fromBase64(const XString& _Path, const XByteArray& _Base64) noexcept
{
	return XFile::fromBase64(_Path, _Base64.data(), _Base64.size());
}
