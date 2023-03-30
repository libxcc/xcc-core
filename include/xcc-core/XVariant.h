#ifndef			_XCC_CORE_VARIANT_H_
#define			_XCC_CORE_VARIANT_H_

#include <xcc-core/header.h>
#include <xcc-core/XByteArray.h>
#include <xcc-core/XString.h>

// Xanadu Class Variant
class _XCOREAPI_ XVariant
{
public:
	typedef enum value_type
	{
		VariantInvalid				= 0,
		VariantBoolean				= 1,
		VariantChar				= 2,
		VariantUChar				= 3,
		VariantShort				= 4,
		VariantUShort				= 5,
		VariantInteger				= 6,
		VariantUInteger				= 7,
		VariantLong				= 8,
		VariantULong				= 9,
		VariantLLong				= 10,
		VariantULLong				= 11,
		VariantFloat				= 12,
		VariantDouble				= 13,
		VariantString				= 14,
		VariantByteArray			= 15
	}value_type;

private:
	typedef union data_type
	{
		bool					_VBool;
		x_int64_t				_VInt64S;
		x_uint64_t				_VInt64U;
		double 				_VDouble;
		void*					_VObject;
		x_int64_t				_VNull;
	}data_type;

	data_type					_variant_data;
	value_type					_variant_type;

public:
	// constructor
	XVariant() noexcept;

	// constructor
	XVariant(value_type _Type) noexcept;

	// constructor
	XVariant(bool _Value) noexcept;

	// constructor
	XVariant(x_int8_t _Value) noexcept;

	// constructor
	XVariant(x_uint8_t _Value) noexcept;

	// constructor
	XVariant(x_int16_t _Value) noexcept;

	// constructor
	XVariant(x_uint16_t _Value) noexcept;

	// constructor
	XVariant(x_int32_t _Value) noexcept;

	// constructor
	XVariant(x_uint32_t _Value) noexcept;

	// constructor
	XVariant(long _Value) noexcept;

	// constructor
	XVariant(unsigned long _Value) noexcept;

	// constructor
	XVariant(x_int64_t _Value) noexcept;

	// constructor
	XVariant(x_uint64_t _Value) noexcept;

	// constructor
	XVariant(float _Value) noexcept;

	// constructor
	XVariant(double _Value) noexcept;

	// constructor
	XVariant(const char* _Value) noexcept;

	// constructor
	XVariant(const XString& _Value) noexcept;

	// constructor
	XVariant(const XByteArray& _Value) noexcept;

	// constructor
	XVariant(const XVariant& _Other) noexcept;

	// constructor
	XVariant(XVariant&& _Other) noexcept;

	// destructor
	virtual ~XVariant() noexcept;

public:
	// operator overload =
	XVariant& operator = (bool _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_int8_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_uint8_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_int16_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_uint16_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_int32_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_uint32_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (long _Value) noexcept;

	// operator overload =
	XVariant& operator = (unsigned long _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_int64_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (x_uint64_t _Value) noexcept;

	// operator overload =
	XVariant& operator = (float _Value) noexcept;

	// operator overload =
	XVariant& operator = (double _Value) noexcept;

	// operator overload =
	XVariant& operator = (const char* _Value) noexcept;

	// operator overload =
	XVariant& operator = (const XString& _Value) noexcept;

	// operator overload =
	XVariant& operator = (const XByteArray& _Value) noexcept;

	// operator overload =
	XVariant& operator = (const XVariant& _Other) noexcept;

	// operator overload =
	XVariant& operator = (XVariant&& _Other) noexcept;

private:
	// Data copy
	virtual void _copy(const XVariant& _Other) noexcept final;

public:
	// Get the current type
	virtual value_type type() const noexcept final;

	// Clear current content
	virtual void clear() noexcept final;

public:
	// Convert to bool
	virtual bool toBool() const noexcept final;

	// Convert to x_int8_t
	virtual x_int8_t toChar() const noexcept final;

	// Convert to x_uint8_t
	virtual x_uint8_t toUChar() const noexcept final;

	// Convert to x_int16_t
	virtual x_int16_t toShort() const noexcept final;

	// Convert to x_uint16_t
	virtual x_uint16_t toUShort() const noexcept final;

	// Convert to x_int32_t
	virtual x_int32_t toInt() const noexcept final;

	// Convert to x_uint32_t
	virtual x_uint32_t toUInt() const noexcept final;

	// Convert to long
	virtual long toLong() const noexcept final;

	// Convert to unsigned long
	virtual unsigned long toULong() const noexcept final;

	// Convert to x_int64_t
	virtual x_int64_t toLLong() const noexcept final;

	// Convert to x_uint64_t
	virtual x_uint64_t toULLong() const noexcept final;

	// Convert to float
	virtual float toFloat() const noexcept final;

	// Convert to double
	virtual double toDouble() const noexcept final;

	// Convert to XString
	virtual XString toString() const noexcept final;

	// Convert to XByteArray
	virtual XByteArray toBytes() const noexcept final;
};

#endif
