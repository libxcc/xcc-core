﻿/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD5 Message-Digest Algorithm (RFC 1321).
 */

#ifndef			_XCC_CORE_HASH_MD5_H_
#define			_XCC_CORE_HASH_MD5_H_

#include <xcc-core/header.h>

// MD5 context
typedef struct _XCC_CORE_MD5_CONTEXT
{
	std::uint32_t			lo;
	std::uint32_t			hi;
	std::uint32_t			a;
	std::uint32_t			b;
	std::uint32_t			c;
	std::uint32_t			d;
	unsigned char			buffer[64];
	std::uint32_t			block[16];
}XCC_CORE_MD5_CONTEXT;

namespace xcc
{
	// MD5 initialization
	_XCOREAPI_ void __xcall__ MD5_Init(XCC_CORE_MD5_CONTEXT* _Context) noexcept;

	// MD5 updates the data
	_XCOREAPI_ void __xcall__ MD5_Update(XCC_CORE_MD5_CONTEXT* _Context, const void* _Data, std::uint64_t _Size) noexcept;

	// MD5 retrieves the results
	_XCOREAPI_ void __xcall__ MD5_Final(unsigned char* _Result, XCC_CORE_MD5_CONTEXT* _Context) noexcept;
};

#endif // _XCC_CORE_HASH_MD5_H_
