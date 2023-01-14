#ifndef			_XCC_CORE_JSON_VALUE_H_
#define			_XCC_CORE_JSON_VALUE_H_

#include <xcc-core/header.h>
#include <xcc-core/XString.h>


// Json: 前置声明
class XJsonArray;
class XJsonObject;
class XJsonDocument;
class XJsonPrivate;
typedef struct XJsonValuePrivate		XJsonValuePrivate;

// Json: 值
class _XCOREAPI_ XJsonValue
{
private:
	friend class XJsonPrivate;
	friend class XJsonDocument;

public:
	// 值类型枚举
	typedef enum Type
	{
		Null				= 0,
		Boolean				= 1,
		Number				= 2,
		String				= 3,
		Array				= 4,
		Object				= 5
	}Type;

private:
	// 私有数据
	XJsonValuePrivate*			memberData;

public:
	// constructor
	XJsonValue() noexcept;

	// constructor
	explicit XJsonValue(XJsonValuePrivate* _PrivateData) noexcept;

	// constructor
	XJsonValue(const XJsonValue& _Object) noexcept;

	// constructor
	XJsonValue(XJsonValue&& _Object) noexcept;

	// destructor
	virtual ~XJsonValue() noexcept;

public:
	// constructor
	XJsonValue(bool _Status) noexcept;

	// constructor
	XJsonValue(xcc::int8_t _Number) noexcept;

	// constructor
	XJsonValue(xcc::uint8_t _Number) noexcept;

	// constructor
	XJsonValue(xcc::int16_t _Number) noexcept;

	// constructor
	XJsonValue(xcc::uint16_t _Number) noexcept;

	// constructor
	XJsonValue(xcc::int32_t _Number) noexcept;

	// constructor
	XJsonValue(xcc::uint32_t _Number) noexcept;

	// constructor
	XJsonValue(xcc::int64_t _Number) noexcept;

	// constructor
	XJsonValue(xcc::uint64_t _Number) noexcept;

	// constructor
	XJsonValue(float _Number) noexcept;

	// constructor
	XJsonValue(double _Number) noexcept;

	// constructor
	XJsonValue(const char* _String) noexcept;

	// constructor
	XJsonValue(const XString& _String) noexcept;

	// constructor
	XJsonValue(const XJsonArray& _JsonArray) noexcept;

	// constructor
	XJsonValue(const XJsonObject& _JsonObject) noexcept;

public:
	// operator overload =
	XJsonValue& operator = (const XJsonValue& _Object) noexcept;

	// operator overload =
	XJsonValue& operator = (XJsonValue&& _Object) noexcept;

public:
	// 设置为空值
	virtual void clear() noexcept;

public:
	// 创建一个空值
	static XJsonValue null() noexcept;

	// 创建一个布尔值
	static XJsonValue boolean(bool _Status) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::int8_t _Number) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::uint8_t _Number) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::int16_t _Number) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::uint16_t _Number) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::int32_t _Number) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::uint32_t _Number) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::int64_t _Number) noexcept;

	// 创建一个整型
	static XJsonValue number(xcc::uint64_t _Number) noexcept;

	// 创建一个单精度浮点数
	static XJsonValue number(float _Number) noexcept;

	// 创建一个双精度浮点数
	static XJsonValue number(double _Number) noexcept;

	// 创建一个字符串
	static XJsonValue string(const char* _String) noexcept;

	// 创建一个字符串
	static XJsonValue string(const XString& _String) noexcept;

	// 创建一个数组
	static XJsonValue array(const XJsonArray& _JsonArray) noexcept;

	// 创建一个对象
	static XJsonValue object(const XJsonObject& _JsonObject) noexcept;

public:
	// 当前值类型
	virtual Type type() const noexcept final;

public:
	// 检查是否为空值
	virtual bool isNull() const noexcept final;

	// 检查是否为布尔值
	virtual bool isBoolean() const noexcept final;

	// 检查是否为数值
	virtual bool isNumber() const noexcept final;

	// 检查是否为整型
	virtual bool isInteger() const noexcept final;

	// 检查是否为浮点数
	virtual bool isDouble() const noexcept final;

	// 检查是否为字符串
	virtual bool isString() const noexcept final;

	// 检查是否为数组
	virtual bool isArray() const noexcept final;

	// 检查是否为对象
	virtual bool isObject() const noexcept final;

public:
	// 转换为布尔值
	virtual bool toBoolean() const noexcept final;

	// 转换为有符号整型
	virtual xcc::int64_t toLLong() const noexcept final;

	// 转换为无符号整型
	virtual xcc::uint64_t toULLong() const noexcept final;

	// 转换为单精度浮点数
	virtual float toFloat() const noexcept final;

	// 转换为双精度浮点数
	virtual double toDouble() const noexcept final;

	// 转换为字符串
	virtual XString toString() const noexcept final;

	// 转换为数组
	virtual XJsonArray toArray() const noexcept final;

	// 转换为对象
	virtual XJsonObject toObject() const noexcept final;
};

#endif
