#ifndef			_XCC_CORE_FILE_H_
#define			_XCC_CORE_FILE_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// Core: 文件操作
class _XCOREAPI_ XFile
{
public:
	// 删除后缀
	static constexpr const char*			rm_suffix		= ".xrm";

public:
	// 类型 - 文件传输
	using						FuncFileTransfer = const std::function<bool(x_uint64_t _Progress, x_uint64_t _Total)>&;

public:
	// [api] 打开文件
	static FILE* fopen(const XString& _FilePath, const XString& _Mode) noexcept;

	// [api] 跳转至指定位置
	static x_int64_t fseek(FILE* _Handle, x_int64_t _Offset, int _Origin = SEEK_SET) noexcept;

	// [api] 取出当前偏移
	static x_int64_t ftell(FILE* _Handle) noexcept;

	// [api] 是否已到文件末尾
	static bool feof(FILE* _Handle) noexcept;

	// [api] 读取指定长度的数据
	static bool fread(FILE* _Handle, void* _Buffer, x_uint64_t _Length) noexcept;

	// [api] 读取一行数据
	static XByteArray freadLine(FILE* _Handle) noexcept;

	// [api] 读取指定项的数据
	static x_uint64_t fread(void* _Buffer, x_uint64_t _Size, x_uint64_t _Count, FILE* _Handle) noexcept;

	// [api] 写入所有数据
	static bool fwrite(FILE* _Handle, const void* _Buffer, x_uint64_t _Length) noexcept;

	// [api] 写入所有数据
	static bool fwrite(FILE* _Handle, const XByteArray& _Buffer) noexcept;

	// [api] 写入指定项的数据
	static x_uint64_t fwrite(const void* _Buffer, x_uint64_t _Size, x_uint64_t _Count, FILE* _Handle) noexcept;

	// [api] 刷新流
	static bool fflush(FILE* _Handle) noexcept;

	// [api] 关闭文件
	static bool fclose(FILE* _Handle) noexcept;

public:
	// [opt] 文件大小
	static x_uint64_t fsize(const XString& _Path) noexcept;

	// [opt] 检查路径是否存在
	static bool exist(const XString& _Path) noexcept;

	// [opt] 重命名
	static bool rename(const XString& _SrcPath, const XString& _TagPath) noexcept;

	// [opt] 删除
	static bool remove(const XString& _Path) noexcept;

	// [opt] 复制
	static bool copy(const XString& _SrcPath, const XString& _TagPath, FuncFileTransfer _Lambda = nullptr) noexcept;

	// [opt] 移动
	static bool move(const XString& _SrcPath, const XString& _TagPath, FuncFileTransfer _Lambda = nullptr) noexcept;

public:
	// [opt] 将文件内容读取至字节数组
	static XByteArray toBytes(const XString& _Path) noexcept;

	// [opt] 将字节数组写入至文件内容
	static bool fromBytes(const XString& _Path, const void* _Bytes, x_size_t _Length) noexcept;

	// [opt] 将字节数组写入至文件内容
	static bool fromBytes(const XString& _Path, const XByteArray& _Bytes) noexcept;

	// [opt] 将文件内容读取至字节数组并编码Base64
	static XByteArray toBase64(const XString& _Path) noexcept;

	// [opt] 将字节数组Base64解码并写入至文件内容
	static bool fromBase64(const XString& _Path, const void* _Base64, x_size_t _Length) noexcept;

	// [opt] 将字节数组Base64解码并写入至文件内容
	static bool fromBase64(const XString& _Path, const XByteArray& _Base64) noexcept;
};

#endif
