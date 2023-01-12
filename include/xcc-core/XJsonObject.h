#ifndef			_XCC_CORE_JSON_OBJECT_H_
#define			_XCC_CORE_JSON_OBJECT_H_

#include <xcc-core/header.h>


// Json: 前置声明
class XJsonValue;
typedef struct XJsonObjectPrivate		XJsonObjectPrivate;

// Json: 对象
class _XCOREAPI_ XJsonObjectT
{
private:
	// 私有数据
	XJsonObjectPrivate*			memberData;

public:
	// constructor
	XJsonObjectT() noexcept;

	// constructor
	explicit XJsonObjectT(XJsonObjectPrivate* _PrivateData) noexcept;

	// constructor
	XJsonObjectT(const XJsonObjectT &_Object) noexcept;

	// constructor
	XJsonObjectT(XJsonObjectT &&_Object) noexcept;

	// destructor
	virtual ~XJsonObjectT() noexcept;

public:
	// operator overload =
	XJsonObjectT& operator=(const XJsonObjectT &_Object) noexcept;

	// operator overload =
	XJsonObjectT& operator=(XJsonObjectT &&_Object) noexcept;

public:
	// operator overload []
	XJsonValue& operator[](const XString& _Key);

	// operator overload []
	const XJsonValue& operator[](const XString& _Key) const;

public:
	// 子项数量
	virtual Xcc::size_t size() const noexcept final;

	// 是否不存在子项
	virtual bool empty() const noexcept final;

	// 是否存在子项
	virtual bool exist() const noexcept final;

public:
	// 插入至指定下标
	virtual void insert(const XString& _Key, bool _Status) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::int8_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::uint8_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::int16_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::uint16_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::int32_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::uint32_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::int64_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, Xcc::uint64_t _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, float _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, double _Number) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, const XString& _String) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, const XJsonValue& _JsonValue) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, const XJsonArray& _JsonArray) noexcept final;

	// 插入至指定下标
	virtual void insert(const XString& _Key, const XJsonObjectT& _JsonObject) noexcept final;

public:
	// 替换指定下标
	virtual void replace(const XString& _Key, bool _Status) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::int8_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::uint8_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::int16_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::uint16_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::int32_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::uint32_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::int64_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, Xcc::uint64_t _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, float _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, double _Number) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, const XString& _String) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, const XJsonValue& _JsonValue) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, const XJsonArray& _JsonArray) noexcept final;

	// 替换指定下标
	virtual void replace(const XString& _Key, const XJsonObjectT& _JsonObject) noexcept final;

public:
	// 删除指定下标
	virtual void remove(const XString& _Key) noexcept final;

public:
	// 分离指定下标
	virtual XJsonValue detach(const XString& _Key) noexcept final;
};

#endif
