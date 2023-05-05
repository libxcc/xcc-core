#ifndef			_XCC_CORE_NATIVE_SOFTWARE_H_
#define			_XCC_CORE_NATIVE_SOFTWARE_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// Native software details
struct XNativeSoftwareDescPrivate;
typedef struct XNativeSoftwareDescPrivate	XNativeSoftwareDescPrivate;
class _XCOREAPI_ XNativeSoftwareDesc
{
private:
	// Private structures
	XNativeSoftwareDescPrivate*		d_ptr;

public:
	// constructor
	XNativeSoftwareDesc() noexcept;

	// constructor
	explicit XNativeSoftwareDesc(XNativeSoftwareDescPrivate* _Private) noexcept;

	// constructor
	XNativeSoftwareDesc(const XNativeSoftwareDesc& _Right) noexcept;

	// constructor
	XNativeSoftwareDesc(XNativeSoftwareDesc&& _Right) noexcept;

	// destructor
	virtual ~XNativeSoftwareDesc() noexcept;

public:
	// operator =
	XNativeSoftwareDesc& operator=(const XNativeSoftwareDesc& _Right) noexcept;

	// operator =
	XNativeSoftwareDesc& operator=(XNativeSoftwareDesc&& _Right) noexcept;

public:
	// [get] 是否32位
	virtual bool isX86() const noexcept final;

	// [get] 是否64位
	virtual bool isX64() const noexcept final;

	// [get] 标签
	virtual const XString& label() const noexcept final;

	// [get] 名称
	virtual const XString& displayName() const noexcept final;

	// [get] 版本
	virtual const XString& displayVersion() const noexcept final;

	// [get] 安装位置
	virtual const XString& installLocation() const noexcept final;

	// [get] 卸载命令行
	virtual const XString& uninstallString() const noexcept final;
};


// The operating class of software installed on the local computer
class _XCOREAPI_ XNativeSoftware
{
private:
	XCC_DISABLE_COPY(XNativeSoftware);
	XCC_DISABLE_MOVE(XNativeSoftware);

public:
	// constructor
	XNativeSoftware() noexcept;

	// destructor
	virtual ~XNativeSoftware() noexcept;

public:
	// Enumerate the list of native software
	static bool softwareEnum(const std::function<bool(const XNativeSoftwareDesc& _InfoSoftware)>& _Lambda) noexcept;
};

#endif
