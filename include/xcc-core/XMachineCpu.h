#ifndef			_XCC_CORE_MACHINE_CPU_H_
#define			_XCC_CORE_MACHINE_CPU_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// Core: 主机CPU信息
struct XMachineCpuPrivate;
typedef struct XMachineCpuPrivate		XMachineCpuPrivate;
class _XCOREAPI_ XMachineCpu
{
private:
	// Private structures
	XMachineCpuPrivate*			d_ptr;

public:
	// constructor
	XMachineCpu() noexcept;

	// constructor
	explicit XMachineCpu(XMachineCpuPrivate* _Private) noexcept;

	// constructor
	XMachineCpu(const XMachineCpu& _Right) noexcept;

	// constructor
	XMachineCpu(XMachineCpu&& _Right) noexcept;

	// destructor
	virtual ~XMachineCpu() noexcept;

public:
	// operator override =
	XMachineCpu& operator = (const XMachineCpu& _Right) noexcept;

	// operator override =
	XMachineCpu& operator = (XMachineCpu&& _Right) noexcept;

public:
	// [get] 指令集
	virtual XString features() const noexcept final;

	// [get] 模型名称
	virtual XString modelName() const noexcept final;

	// [get] 核心数量
	virtual x_uint64_t coreCount() const noexcept final;

	// [get] 线程数量
	virtual x_uint64_t threadCount() const noexcept final;

	// [get] 物理地址位数
	virtual x_uint64_t addressBitsPhysical() const noexcept final;

	// [get] 虚拟地址位数
	virtual x_uint64_t addressBitsVirtual() const noexcept final;

	// [get] 供应商ID
	virtual XString vendorId() const noexcept final;

	// [get] 缓存对齐字节
	virtual x_uint64_t cacheAlignment() const noexcept final;

	// [get] 缓存字节
	virtual x_uint64_t cacheSize() const noexcept final;

	// [get] Cpu频率
	virtual x_uint64_t frequency() const noexcept final;
};

#endif
