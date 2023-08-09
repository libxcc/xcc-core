#ifndef			_XCC_CORE_PROCESS_INFO_H_
#define			_XCC_CORE_PROCESS_INFO_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 进程信息
struct XPrivateProcessData;
typedef struct XPrivateProcessData		XPrivateProcessData;
class _XCOREAPI_ XProcessInfo
{
private:
	// 私有数据
	XPrivateProcessData*			d_ptr;

public:
	// constructor
	explicit XProcessInfo(XPrivateProcessData* _Private) noexcept;

public:
	// constructor
	XProcessInfo() noexcept;

	// constructor
	XProcessInfo(const XProcessInfo& _Right) noexcept;

	// destructor
	virtual ~XProcessInfo() noexcept;

public:
	// operator override =
	XProcessInfo& operator = (const XProcessInfo& _Right) noexcept;

public:
	// [get] 进程ID
	virtual x_uint64_t pid() const noexcept final;

	// [get] 进程名称
	virtual XString name() const noexcept final;

	// [get] 进程路径
	virtual XString path() const noexcept final;

	// [get] 进程参数
	virtual std::list<XString> args() const noexcept final;
};

#endif
