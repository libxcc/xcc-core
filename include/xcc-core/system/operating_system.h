#ifndef			_XCC_CORE_SYSTEM_OPERATING_SYSTEM_H_
#define			_XCC_CORE_SYSTEM_OPERATING_SYSTEM_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// operating system
namespace xcc::os
{
	// kernel version
	_XCOREAPI_ XString __xcall__ kernel_version() noexcept;

	// build version
	_XCOREAPI_ XString __xcall__ build_version() noexcept;

	// system version
	_XCOREAPI_ XString __xcall__ system_version() noexcept;

	// release name
	_XCOREAPI_ XString __xcall__ release_name() noexcept;

	// product name
	_XCOREAPI_ XString __xcall__ product_name() noexcept;

	// display name
	_XCOREAPI_ XString __xcall__ display_name() noexcept;
}

// operating system
namespace xcc::os
{
	// Check if it is server version
	_XCOREAPI_ bool __xcall__ is_server_version() noexcept;

	// Check if it is desktop version
	_XCOREAPI_ bool __xcall__ is_desktop_version() noexcept;
}

// operating system
namespace xcc::os
{
	// bits
	_XCOREAPI_ x_uint64_t __xcall__ bits() noexcept;

	// Whether it is a 32-bit system
	_XCOREAPI_ bool __xcall__ is_32bit() noexcept;

	// Whether it is a 32-bit system
	_XCOREAPI_ bool __xcall__ is_64bit() noexcept;
}

#endif
