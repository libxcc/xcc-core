#ifndef			_XCC_CORE_UUID_H_
#define			_XCC_CORE_UUID_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


/// class XUuid
class _XCOREAPI_ XUuid
{
public:
	// 输出类型
	typedef enum STRING_FORMAT
	{
		// {00000000-0000-0000-0000-000000000000}	38-Size
		WithBraces			= 38,

		// 00000000-0000-0000-0000-000000000000		36-Size
		WithoutBraces			= 36,

		// 00000000000000000000000000000000		32-Size
		HexDigitsOnly			= 32
	}STRING_FORMAT;

private:
	x_uint32_t			_uuid_data_1;
	x_uint16_t			_uuid_data_2;
	x_uint16_t			_uuid_data_3;
	x_uint8_t			_uuid_data_4[8];

public:
	// constructor
	XUuid() noexcept;

	// constructor
	XUuid(x_uint32_t _D1, x_uint16_t _D2, x_uint16_t _D3, x_uint8_t _D41, x_uint8_t _D42, x_uint8_t _D43, x_uint8_t _D44, x_uint8_t _D45, x_uint8_t _D46, x_uint8_t _D47, x_uint8_t _D48) noexcept;

	// constructor
	explicit XUuid(const char* _UUID) noexcept;

	// constructor
	explicit XUuid(const XString& _UUID) noexcept;

	// constructor
	XUuid(const XUuid& _Object) noexcept;

	// constructor
	XUuid(XUuid&& _Object) noexcept;

	// destructor
	virtual ~XUuid() noexcept;

public:
	// operator =
	XUuid& operator = (const XUuid& _Object) noexcept;

	// operator =
	XUuid& operator = (XUuid&& _Object) noexcept;

private:
	// 从字符串格式化
	virtual bool formatString(const XString& _UUID) noexcept final;

public:
	// 更换一个新的UUID
	virtual void update() noexcept final;

public:
	// 创建一个UUID
	static XUuid createUuid() noexcept;

public:
	// 从字符串格式化
	static XUuid fromString(const XString& _UUID) noexcept;

	// 转换为字符串
	virtual XString toString() const noexcept final;

	// 转换为字符串
	virtual XString toString(STRING_FORMAT _Format) const noexcept final;
};

#endif
