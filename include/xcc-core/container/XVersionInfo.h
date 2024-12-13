#ifndef			_XCC_CORE_VERSION_INFO_H_
#define			_XCC_CORE_VERSION_INFO_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// XCC - 版本信息
struct PKVersionInfo;
typedef struct PKVersionInfo			PKVersionInfo;
class _XCOREAPI_ XVersionInfo
{
public:
	// 版本格式
	typedef enum VersionFormat
	{
		// 自动匹配
		VersionFormatAuto		= 0,

		// 三段式版本号
		VersionFormatV3			= 3,

		// 四段式版本号
		VersionFormatV4			= 4
	}VersionFormat;

private:
	// 私有数据
	PKVersionInfo*				d_ptr;

public:
	// constructor
	XVersionInfo() noexcept;

	// constructor
	XVersionInfo(x_uint32_t _VerNum) noexcept;

	// constructor
	XVersionInfo(const char* _VerStr) noexcept;

	// constructor
	XVersionInfo(const XString& _VerStr) noexcept;

	// constructor
	XVersionInfo(const XVersionInfo& _Right) noexcept;

	// destructor
	virtual ~XVersionInfo() noexcept;

public:
	// operator override =
	XVersionInfo& operator = (x_uint32_t _VerNum) noexcept;

	// operator override =
	XVersionInfo& operator = (const char* _VerStr) noexcept;

	// operator override =
	XVersionInfo& operator = (const XString& _VerStr) noexcept;

	// operator override =
	XVersionInfo& operator = (const XVersionInfo& _Right) noexcept;

public:
	// operator override ==
	bool operator == (x_uint32_t _VerNum) const noexcept;

	// operator override ==
	bool operator == (const char* _VerStr) const noexcept;

	// operator override ==
	bool operator == (const XString& _VerStr) const noexcept;

	// operator override ==
	bool operator == (const XVersionInfo& _Right) const noexcept;

	// operator override !=
	bool operator != (x_uint32_t _VerNum) const noexcept;

	// operator override !=
	bool operator != (const char* _VerStr) const noexcept;

	// operator override !=
	bool operator != (const XString& _VerStr) const noexcept;

	// operator override !=
	bool operator != (const XVersionInfo& _Right) const noexcept;

	// operator override <
	bool operator < (x_uint32_t _VerNum) const noexcept;

	// operator override <
	bool operator < (const char* _VerStr) const noexcept;

	// operator override <
	bool operator < (const XString& _VerStr) const noexcept;

	// operator override <
	bool operator < (const XVersionInfo& _Right) const noexcept;

	// operator override >
	bool operator > (x_uint32_t _VerNum) const noexcept;

	// operator override >
	bool operator > (const char* _VerStr) const noexcept;

	// operator override >
	bool operator > (const XString& _VerStr) const noexcept;

	// operator override >
	bool operator > (const XVersionInfo& _Right) const noexcept;

	// operator override <=
	bool operator <= (x_uint32_t _VerNum) const noexcept;

	// operator override <=
	bool operator <= (const char* _VerStr) const noexcept;

	// operator override <=
	bool operator <= (const XString& _VerStr) const noexcept;

	// operator override <=
	bool operator <= (const XVersionInfo& _Right) const noexcept;

	// operator override >=
	bool operator >= (x_uint32_t _VerNum) const noexcept;

	// operator override >=
	bool operator >= (const char* _VerStr) const noexcept;

	// operator override >=
	bool operator >= (const XString& _VerStr) const noexcept;

	// operator override >=
	bool operator >= (const XVersionInfo& _Right) const noexcept;

public:
	// [opt] 是否为空
	virtual bool empty() const noexcept final;

	// [opt] 是否有值
	virtual bool exist() const noexcept final;

public:
	// [fmt] 从整数格式化
	static XVersionInfo fromNumber(x_uint32_t _VerNum, VersionFormat _VF = VersionFormatAuto) noexcept;

	// [fmt] 从字符串格式化
	static XVersionInfo fromString(const XString& _VerStr, VersionFormat _VF = VersionFormatAuto) noexcept;

public:
	// [cnv] 转换为整数
	virtual x_uint32_t toNumber() const noexcept final;

	// [cnv] 转换为字符串
	virtual XString toString() const noexcept final;

	// [cnv] 转换为整数
	virtual x_uint32_t toNumber(VersionFormat _VF) const noexcept final;

	// [cnv] 转换为字符串
	virtual XString toString(VersionFormat _VF) const noexcept final;

public:
	// [cmp] 检查是否与指定数据相同
	virtual int compare(x_uint32_t _VerNum) const noexcept final;

	// [cmp] 检查是否与指定数据相同
	virtual int compare(const char* _VerStr) const noexcept final;

	// [cmp] 检查是否与指定数据相同
	virtual int compare(const XString& _VerStr) const noexcept final;

	// [cmp] 检查是否与指定数据相同
	virtual int compare(const XVersionInfo& _VerInfo) const noexcept final;

	// [cmp] 检查是否与指定数据相同
	static int compare(const XVersionInfo& _VerInfoL, const XVersionInfo& _VerInfoR) noexcept;

public:
	// [set] 主版本号
	virtual void major(x_uint32_t _Major) noexcept final;

	// [get] 主版本号
	virtual x_uint32_t major() const noexcept final;

	// [set] 次版本号
	virtual void minor(x_uint32_t _Minor) noexcept final;

	// [get] 次版本号
	virtual x_uint32_t minor() const noexcept final;

	// [set] 补丁号
	virtual void patch(x_uint32_t _Patch) noexcept final;

	// [get] 补丁号
	virtual x_uint32_t patch() const noexcept final;

	// [set] 构建号
	virtual void build(x_uint32_t _Build) noexcept final;

	// [get] 构建号
	virtual x_uint32_t build() const noexcept final;
};



// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (const char* _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (const XString& _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (const char* _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (const XString& _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator <
_XCOREAPI_ bool __xcall__ operator < (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator <
_XCOREAPI_ bool __xcall__ operator < (const char* _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator <
_XCOREAPI_ bool __xcall__ operator < (const XString& _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator >
_XCOREAPI_ bool __xcall__ operator > (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator >
_XCOREAPI_ bool __xcall__ operator > (const char* _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator >
_XCOREAPI_ bool __xcall__ operator > (const XString& _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator <=
_XCOREAPI_ bool __xcall__ operator <= (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator <=
_XCOREAPI_ bool __xcall__ operator <= (const char* _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator <=
_XCOREAPI_ bool __xcall__ operator <= (const XString& _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator >=
_XCOREAPI_ bool __xcall__ operator >= (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator >=
_XCOREAPI_ bool __xcall__ operator >= (const char* _VerStr, const XVersionInfo& _Object) noexcept;

// Syntax sugar: operator >=
_XCOREAPI_ bool __xcall__ operator >= (const XString& _VerStr, const XVersionInfo& _Object) noexcept;



#endif
