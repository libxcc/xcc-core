#include <xcc-core/serialize/XCryptoHash.h>
#include <source/serialize/XHashMD5.h>
#include <source/serialize/XHashSHA1.h>
#include <source/serialize/XHashSHA2.h>
#include <source/serialize/XHashCRC32.h>
#include <source/serialize/XHashCRC64_ECMA.h>



// Private structures
struct XCryptoHashPrivate
{
	XCryptoHash::AlgorithmType		algorithm;
	void*					context;
	XByteArray				hash;

	// constructor
	XCryptoHashPrivate()
	{
		this->algorithm = XCryptoHash::CRC32;
		this->context = nullptr;
	}
};

// constructor
XCryptoHash::XCryptoHash(AlgorithmType _Algorithm) noexcept
{
	d_ptr = new(std::nothrow) XCryptoHashPrivate();

	d_ptr->algorithm = _Algorithm;
	switch (d_ptr->algorithm)
	{
		case XCryptoHash::MD5:
			d_ptr->context = new(std::nothrow) XCC_CORE_MD5_CONTEXT();
			break;
		case XCryptoHash::SHA1:
			d_ptr->context = new(std::nothrow) XCC_CORE_SHA1_CONTEXT();
			break;
		case XCryptoHash::CRC32:
			d_ptr->context = new(std::nothrow) XCC_CORE_CRC32_CONTEXT();
			break;
		case XCryptoHash::CRC64_ECMA:
			d_ptr->context = new(std::nothrow) XCC_CORE_CRC64_CONTEXT();
			break;
	}

	this->reset();
}

// destructor
XCryptoHash::~XCryptoHash() noexcept
{
	switch (d_ptr->algorithm)
	{
		case XCryptoHash::MD5:
			delete (XCC_CORE_MD5_CONTEXT*)(d_ptr->context);
			break;
		case XCryptoHash::SHA1:
			delete (XCC_CORE_SHA1_CONTEXT*)(d_ptr->context);
			break;
		case XCryptoHash::CRC32:
			delete (XCC_CORE_CRC32_CONTEXT*)(d_ptr->context);
			break;
		case XCryptoHash::CRC64_ECMA:
			delete (XCC_CORE_CRC64_CONTEXT*)(d_ptr->context);
			break;
	}

	delete d_ptr;
}




// Empty data
void XCryptoHash::reset() noexcept
{
	switch (d_ptr->algorithm)
	{
		case XCryptoHash::MD5:
			xcc::MD5_Init((XCC_CORE_MD5_CONTEXT*)(d_ptr->context));
			break;
		case XCryptoHash::SHA1:
			xcc::sha1_begin((XCC_CORE_SHA1_CONTEXT*)(d_ptr->context));
			break;
		case XCryptoHash::CRC32:
			xcc::CRC32_Init((XCC_CORE_CRC32_CONTEXT*)(d_ptr->context));
			break;
		case XCryptoHash::CRC64_ECMA:
			xcc::CRC64_Init((XCC_CORE_CRC64_CONTEXT*)(d_ptr->context));
			break;
	}
}

// Add Data
void XCryptoHash::append(const void* _Data, x_uint64_t _Length) noexcept
{
	switch (d_ptr->algorithm)
	{
		case MD5:
			xcc::MD5_Update((XCC_CORE_MD5_CONTEXT*)(d_ptr->context), _Data, _Length);
			break;
		case SHA1:
			xcc::sha1_hash((const unsigned char*)_Data, _Length, (XCC_CORE_SHA1_CONTEXT*)(d_ptr->context));
			break;
		case CRC32:
			xcc::CRC32_Update((XCC_CORE_CRC32_CONTEXT*)(d_ptr->context), _Data, _Length);
			break;
		case CRC64_ECMA:
			xcc::CRC64_Update((XCC_CORE_CRC64_CONTEXT*)(d_ptr->context), _Data, _Length);
			break;
	}
}

// Add Data
void XCryptoHash::append(const XByteArray& _Bytes) noexcept
{
	XCryptoHash::append(_Bytes.data(), _Bytes.size());
}

// View the results
XByteArray XCryptoHash::result() const noexcept
{
	if (d_ptr->hash.empty())
	{
		unsigned char 		vResult[128] = {0};
		switch (d_ptr->algorithm)
		{
			case MD5:
				xcc::MD5_Final(vResult, (XCC_CORE_MD5_CONTEXT*)(d_ptr->context));
				break;
			case SHA1:
				xcc::sha1_end(vResult, (XCC_CORE_SHA1_CONTEXT*)(d_ptr->context));
				break;
			case CRC32:
				xcc::CRC32_Final(vResult, (XCC_CORE_CRC32_CONTEXT*)(d_ptr->context));
				break;
			case CRC64_ECMA:
				xcc::CRC64_Final(vResult, (XCC_CORE_CRC64_CONTEXT*)(d_ptr->context));
				break;
		}
		d_ptr->hash = XByteArray((const char*)vResult, XCryptoHash::hashLength(d_ptr->algorithm));
	}
	return d_ptr->hash;
}






// Static method: Calculates the hash value
XByteArray XCryptoHash::hashBytes(const void* _Buffer, x_uint64_t _Length, AlgorithmType _Algorithm) noexcept
{
	auto		vClass = XCryptoHash(_Algorithm);
	vClass.reset();
	vClass.append(_Buffer, _Length);
	return vClass.result();
}

// Static method: Calculates the hash value
XByteArray XCryptoHash::hashBytes(const XByteArray& _Bytes, AlgorithmType _Algorithm) noexcept
{
	return XCryptoHash::hashBytes(_Bytes.data(), _Bytes.size(), _Algorithm);
}

// Static method: Calculates the hash value
XByteArray XCryptoHash::hashString(const XString& _String, AlgorithmType _Algorithm) noexcept
{
	return XCryptoHash::hashBytes(_String.data(), _String.size(), _Algorithm);
}

// Static method: Calculates the hash value
XByteArray XCryptoHash::hashFile(const XString& _File, AlgorithmType _Algorithm) noexcept
{
	auto		vClass = XCryptoHash(_Algorithm);
	auto		vResult = XByteArray();
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vHandle = x_posix_wfopen(_File.toWString().data(), L"rb");
#else
	auto		vHandle = x_posix_fopen(_File.data(), "rb");
#endif
	if (vHandle)
	{
		auto		vMemory = new(std::nothrow) char[XCC_SIZE_MB];
		if(vMemory)
		{
			vClass.reset();
			while(!x_posix_feof(vHandle))
			{
				auto		vSync = x_posix_fread(vMemory, 1, XCC_SIZE_MB, vHandle);
				if(vSync <= 0)
				{
					break;
				}
				vClass.append(vMemory, vSync);
			};
			vResult = vClass.result();
			XCC_DELETE_ARR(vMemory);
		}
		x_posix_fclose(vHandle);
	}
	return vResult;
}

// Static method: Gets the result length of the specified algorithm
x_size_t XCryptoHash::hashLength(AlgorithmType _Algorithm) noexcept
{
	switch (_Algorithm)
	{
		case XCryptoHash::MD5:
			return 16;
		case XCryptoHash::SHA1:
			return 20;
		case XCryptoHash::CRC32:
			return 4;
		case XCryptoHash::CRC64_ECMA:
			return 8;
		default:
			return 0;
	}
}
