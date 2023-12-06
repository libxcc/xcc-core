#ifndef			_XCC_CORE_HASH_CRC32_H_
#define			_XCC_CORE_HASH_CRC32_H_

#include <xcc-core/header.h>

// CRC32 context
typedef struct _XCC_CORE_CRC32_CONTEXT
{
	std::uint32_t			hash;
}XCC_CORE_CRC32_CONTEXT;

namespace xcc
{
	// CRC32 initialization
	_XCOREAPI_ void __xcall__ CRC32_Init(XCC_CORE_CRC32_CONTEXT* _Context) noexcept;

	// CRC32 updates the data
	_XCOREAPI_ void __xcall__ CRC32_Update(XCC_CORE_CRC32_CONTEXT* _Context, const void* _Data, std::uint64_t _Size) noexcept;

	// CRC32 retrieves the results
	_XCOREAPI_ void __xcall__ CRC32_Final(unsigned char* _Result, XCC_CORE_CRC32_CONTEXT* _Context) noexcept;
}

#endif
