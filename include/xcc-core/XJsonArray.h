#ifndef			_XCC_CORE_JSON_ARRAY_H_
#define			_XCC_CORE_JSON_ARRAY_H_

#include <xcc-core/header.h>


// Json: 前置声明
class XJsonValue;
typedef struct XJsonArrayPrivate		XJsonArrayPrivate;

// Json: 数组
class _XCOREAPI_ XJsonArray
{
private:
	// 私有数据
	XJsonArrayPrivate*			memberData;

public:
	// constructor
	XJsonArray() noexcept;

	// constructor
	explicit XJsonArray(XJsonArrayPrivate* _PrivateData) noexcept;

	// constructor
	XJsonArray(const XJsonArray &_Object) noexcept;

	// constructor
	XJsonArray(XJsonArray &&_Object) noexcept;

	// destructor
	virtual ~XJsonArray() noexcept;

public:
	// operator overload =
	XJsonArray& operator=(const XJsonArray &_Object) noexcept;

	// operator overload =
	XJsonArray& operator=(XJsonArray &&_Object) noexcept;

public:
	// operator overload []
	XJsonValue& operator[](Xcc::size_t _Index);

	// operator overload []
	const XJsonValue& operator[](Xcc::size_t _Index) const;

public:
	// 子项数量
	virtual Xcc::size_t size() const noexcept final;

	// 是否不存在子项
	virtual bool empty() const noexcept final;

	// 是否存在子项
	virtual bool exist() const noexcept final;

public:
	// 插入至最前
	virtual void prepend(bool _Status) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::int8_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::uint8_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::int16_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::uint16_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::int32_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::uint32_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::int64_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(Xcc::uint64_t _Number) noexcept final;

	// 插入至最前
	virtual void prepend(float _Number) noexcept final;

	// 插入至最前
	virtual void prepend(double _Number) noexcept final;

	// 插入至最前
	virtual void prepend(const XString& _String) noexcept final;

	// 插入至最前
	virtual void prepend(const XJsonValue& _JsonValue) noexcept final;

	// 插入至最前
	virtual void prepend(const XJsonArray& _JsonArray) noexcept final;

	// 插入至最前
	virtual void prepend(const XJsonObjectT& _JsonObject) noexcept final;

public:
	// 插入至末尾
	virtual void append(bool _Status) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::int8_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::uint8_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::int16_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::uint16_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::int32_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::uint32_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::int64_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(Xcc::uint64_t _Number) noexcept final;

	// 插入至末尾
	virtual void append(float _Number) noexcept final;

	// 插入至末尾
	virtual void append(double _Number) noexcept final;

	// 插入至末尾
	virtual void append(const XString& _String) noexcept final;

	// 插入至末尾
	virtual void append(const XJsonValue& _JsonValue) noexcept final;

	// 插入至末尾
	virtual void append(const XJsonArray& _JsonArray) noexcept final;

	// 插入至末尾
	virtual void append(const XJsonObjectT& _JsonObject) noexcept final;

public:
	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, bool _Status) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::int8_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::uint8_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::int16_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::uint16_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::int32_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::uint32_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::int64_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, Xcc::uint64_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, float _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, double _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, const XString& _String) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, const XJsonValue& _JsonValue) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, const XJsonArray& _JsonArray) noexcept final;

	// 插入至指定下标
	virtual void insert(Xcc::size_t _Index, const XJsonObjectT& _JsonObject) noexcept final;

public:
	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, bool _Status) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::int8_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::uint8_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::int16_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::uint16_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::int32_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::uint32_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::int64_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, Xcc::uint64_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, float _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, double _Number) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, const XString& _String) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, const XJsonValue& _JsonValue) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, const XJsonArray& _JsonArray) noexcept final;

	// 替换指定下标
	virtual void replace(Xcc::size_t _Index, const XJsonObjectT& _JsonObject) noexcept final;

public:
	// 删除指定下标
	virtual void remove(Xcc::size_t _Index) noexcept final;

public:
	// 分离指定下标
	virtual XJsonValue detach(Xcc::size_t _Index) noexcept final;
};

#endif
