#ifndef			_XCC_CORE_MACHINE_CPU_PRIVATE_H_
#define			_XCC_CORE_MACHINE_CPU_PRIVATE_H_

#include <xcc-core/XMachineCpu.h>


// Core: 主机CPU信息
struct XMachineCpuPrivate
{
	XString				features;			// 指令集
	XString				model_name;			// 模型名称
	x_uint64_t			core_count = 0;			// 核心数量
	x_uint64_t			thread_count = 0;		// 线程数量
	x_uint64_t			address_bits_physical = 0;	// 物理地址位数
	x_uint64_t 			address_bits_virtual = 0;	// 虚拟地址位数
	XString				vendor_id;			// 供应商ID
	x_uint64_t 			cache_alignment = 0;		// 缓存对齐字节
	x_uint64_t 			cache_size_bit = 0;		// 缓存字节
	x_uint64_t 			frequency = 0;			// cpu频率

public:
	// [set] 初始化对象
	static bool init(XMachineCpuPrivate* _Private) noexcept;
};

#endif
