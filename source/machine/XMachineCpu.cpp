#include <source/machine/XMachineCpuPrivate.h>



// constructor
XMachineCpu::XMachineCpu() noexcept
{
	d_ptr = new(std::nothrow) XMachineCpuPrivate();
	XMachineCpuPrivate::init(d_ptr);
}

// constructor
XMachineCpu::XMachineCpu(XMachineCpuPrivate* _Private) noexcept
{
	d_ptr = _Private;
}

// constructor
XMachineCpu::XMachineCpu(const XMachineCpu& _Right) noexcept
{
	d_ptr = new(std::nothrow) XMachineCpuPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// constructor
XMachineCpu::XMachineCpu(XMachineCpu&& _Right) noexcept
{
	d_ptr = new(std::nothrow) XMachineCpuPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// destructor
XMachineCpu::~XMachineCpu() noexcept
{
	delete d_ptr;
}



// operator override =
XMachineCpu& XMachineCpu::operator = (const XMachineCpu& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}

// operator override =
XMachineCpu& XMachineCpu::operator = (XMachineCpu&& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}



// [get] 指令集
XString XMachineCpu::features() const noexcept
{
	return d_ptr->features;
}

// [get] 模型名称
XString XMachineCpu::modelName() const noexcept
{
	return d_ptr->model_name;
}

// [get] 核心数量
x_uint64_t XMachineCpu::coreCount() const noexcept
{
	return d_ptr->core_count;
}

// [get] 线程数量
x_uint64_t XMachineCpu::threadCount() const noexcept
{
	return d_ptr->thread_count;
}

// [get] 物理地址位数
x_uint64_t XMachineCpu::addressBitsPhysical() const noexcept
{
	return d_ptr->address_bits_physical;
}

// [get] 虚拟地址位数
x_uint64_t XMachineCpu::addressBitsVirtual() const noexcept
{
	return d_ptr->address_bits_virtual;
}

// [get] 供应商ID
XString XMachineCpu::vendorId() const noexcept
{
	return d_ptr->vendor_id;
}

// [get] 缓存对齐字节
x_uint64_t XMachineCpu::cacheAlignment() const noexcept
{
	return d_ptr->cache_alignment;
}

// [get] 缓存字节
x_uint64_t XMachineCpu::cacheSize() const noexcept
{
	return d_ptr->cache_size_bit;
}

// [get] Cpu频率
x_uint64_t XMachineCpu::frequency() const noexcept
{
	return d_ptr->frequency;
}
