#ifndef			_XCC_CORE_HASH_CRC64_ECMA_H_
#define			_XCC_CORE_HASH_CRC64_ECMA_H_

#include <xcc-core/header.h>

// CRC32 context
typedef struct _XCC_CORE_CRC64_CONTEXT
{
	x_uint64_t			hash;
}XCC_CORE_CRC64_CONTEXT;

namespace xcc
{
	// CRC64 initialization
	_XCOREAPI_ void __xcall__ CRC64_Init(XCC_CORE_CRC64_CONTEXT* _Context) noexcept;

	// CRC64 updates the data
	_XCOREAPI_ void __xcall__ CRC64_Update(XCC_CORE_CRC64_CONTEXT* _Context, const void* _Data, std::uint64_t _Size) noexcept;

	// CRC64 retrieves the results
	_XCOREAPI_ void __xcall__ CRC64_Final(unsigned char* _Result, XCC_CORE_CRC64_CONTEXT* _Context) noexcept;
}

#endif
