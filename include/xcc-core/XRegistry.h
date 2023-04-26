#ifndef			_XCC_CORE_REGISTRY_H_
#define			_XCC_CORE_REGISTRY_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 注册表操作 - 跨平台宏定义
#if !defined(XCC_SYSTEM_WINDOWS)
#define HKEY_CLASSES_ROOT                   (( HKEY ) (ULONG_PTR)((LONG)0x80000000) )
#define HKEY_CURRENT_USER                   (( HKEY ) (ULONG_PTR)((LONG)0x80000001) )
#define HKEY_LOCAL_MACHINE                  (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )
#define HKEY_USERS                          (( HKEY ) (ULONG_PTR)((LONG)0x80000003) )
#define HKEY_PERFORMANCE_DATA               (( HKEY ) (ULONG_PTR)((LONG)0x80000004) )
#endif

// 注册表操作 - 跨平台宏定义
#if !defined(XCC_SYSTEM_WINDOWS)
#define REG_NONE                    ( 0ul ) // No value type
#define REG_SZ                      ( 1ul ) // Unicode nul terminated string
#define REG_BINARY                  ( 3ul ) // Free form binary
#define REG_DWORD                   ( 4ul ) // 32-bit number
#define REG_LINK                    ( 6ul ) // Symbolic Link (unicode)
#define REG_MULTI_SZ                ( 7ul ) // Multiple Unicode strings
#define REG_QWORD                   ( 11ul ) // 64-bit number
#endif

// 注册表操作类的私有数据
struct XRegistryPrivate;
typedef struct XRegistryPrivate			XRegistryPrivate;

// 注册表操作类: 仅限Win32
class _XCOREAPI_ XRegistry
{
private:
	XRegistryPrivate*			d_ptr;			// 私有数据

public:
	// constructor
	XRegistry() noexcept;

	// constructor
	XRegistry(HKEY _Root, const XString& _Path, xcc::OpenMode _Mode) noexcept;

	// constructor
	XRegistry(const XRegistry& _InfoOther) noexcept;

	// constructor
	XRegistry(XRegistry&& _InfoOther) noexcept;

	// destructor
	virtual ~XRegistry() noexcept;

public:
	// [get] 是否存在注册表项
	static bool exist(HKEY _Root, const XString& _Path) noexcept;

	// [get] 是否存在注册表项
	virtual bool exist(const XString& _Path) noexcept final;

public:
	// [opt] 打开
	virtual bool open(xcc::OpenMode _Mode) noexcept final;

	// [opt] 打开
	virtual bool open(HKEY _Root, const XString& _Path, xcc::OpenMode _Mode) noexcept final;

	// [opt] 关闭
	virtual void close() noexcept final;

public:
	// [get] 是否打开
	virtual bool isOpen() const noexcept final;

public:
	// [set] 字符串
	virtual bool setItemString(const XString& _Name, const XString& _Value) noexcept final;

	// [set] DWORD
	virtual bool setItemDword(const XString& _Name, DWORD _Value) noexcept final;

	// [get] 字符串
	virtual XString itemString(const XString& _Name) noexcept final;

	// [get] DWORD
	virtual DWORD itemDword(const XString& _Name) noexcept final;

	// [opt] 删除键
	virtual bool remove(const XString& _Name) noexcept final;
};

#endif
