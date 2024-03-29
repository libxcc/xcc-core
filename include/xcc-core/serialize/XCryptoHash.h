﻿#ifndef			_XCC_CORE_HASH_H_
#define			_XCC_CORE_HASH_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XByteArray.h>
#include <xcc-core/container/XString.h>


// 计算哈希值的公共类。
// 包含CRC32、MD5、SHA1等常用算法。
struct XCryptoHashPrivate;
typedef struct XCryptoHashPrivate		XCryptoHashPrivate;
class _XCOREAPI_ XCryptoHash
{
public:
	// Algorithm type
	typedef enum AlgorithmType
	{
		MD5				= 0,
		SHA1				= 1,
		CRC32				= 2,
		CRC64_ECMA			= 3
	}AlgorithmType;

private:
	// Private structures
	XCryptoHashPrivate*			d_ptr;

private:
	XCC_DISABLE_COPY(XCryptoHash);
	XCC_DISABLE_MOVE(XCryptoHash);

public:
	// constructor
	explicit XCryptoHash(AlgorithmType _Algorithm) noexcept;

	// destructor
	virtual ~XCryptoHash() noexcept;

public:
	// Empty data
	virtual void reset() noexcept final;

	// Add Data
	virtual void append(const void* _Data, x_uint64_t _Length) noexcept final;

	// Add Data
	virtual void append(const XByteArray& _Bytes) noexcept final;

	// View the results
	virtual XByteArray result() const noexcept final;

public:
	// Static method: Calculates the hash value
	static XByteArray hashBytes(const void* _Buffer, x_uint64_t _Length, AlgorithmType _Algorithm) noexcept;

	// Static method: Calculates the hash value
	static XByteArray hashBytes(const XByteArray& _Bytes, AlgorithmType _Algorithm) noexcept;

	// Static method: Calculates the hash value
	static XByteArray hashString(const XString& _String, AlgorithmType _Algorithm) noexcept;

	// Static method: Calculates the hash value
	static XByteArray hashFile(const XString& _File, AlgorithmType _Algorithm) noexcept;

	// Static method: Gets the result length of the specified algorithm
	static x_size_t hashLength(AlgorithmType _Algorithm) noexcept;
};

#endif
