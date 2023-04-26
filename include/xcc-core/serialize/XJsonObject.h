#ifndef			_XCC_CORE_JSON_OBJECT_H_
#define			_XCC_CORE_JSON_OBJECT_H_

#include <xcc-core/header.h>


// Json: 前置声明
class XJsonValue;
class XJsonPrivate;
typedef struct XJsonObjectPrivate		XJsonObjectPrivate;
typedef struct XJsonObjectIterPrivate		XJsonObjectIterPrivate;

// Json: 对象
class _XCOREAPI_ XJsonObject
{
private:
	friend class XJsonPrivate;

public:
	// 前置声明
	class iterator;
	class const_iterator;

private:
	// 私有数据
	XJsonObjectPrivate*			memberData;

public:
	// constructor
	XJsonObject() noexcept;

	// constructor
	explicit XJsonObject(XJsonObjectPrivate* _PrivateData) noexcept;

	// constructor
	XJsonObject(const XJsonObject &_Object) noexcept;

	// constructor
	XJsonObject(XJsonObject &&_Object) noexcept;

	// destructor
	virtual ~XJsonObject() noexcept;

public:
	// operator =
	XJsonObject& operator=(const XJsonObject &_Object) noexcept;

	// operator =
	XJsonObject& operator=(XJsonObject &&_Object) noexcept;

public:
	// operator []
	XJsonValue& operator[](const XString& _Key);

	// operator []
	const XJsonValue& operator[](const XString& _Key) const;

public:
	// 子项数量
	virtual x_size_t size() const noexcept final;

	// 是否不存在子项
	virtual bool empty() const noexcept final;

	// 是否存在子项
	virtual bool exist() const noexcept final;

	// 清空
	virtual void clear() noexcept final;

public:
	// 正向迭代器起点
	virtual iterator begin() noexcept final;

	// 正向迭代器终点
	virtual iterator end() noexcept final;

	// 正向只读迭代器起点
	virtual const_iterator begin() const noexcept final;

	// 正向只读迭代器终点
	virtual const_iterator end() const noexcept final;

public:
	// 插入至指定键
	virtual void insert(const XString& _Key, const XJsonValue& _JsonValue) noexcept final;

	// 替换指定键
	virtual void replace(const XString& _Key, const XJsonValue& _JsonValue) noexcept final;

	// 删除指定键
	virtual void remove(const XString& _Key) noexcept final;

	// 分离指定键
	virtual XJsonValue detach(const XString& _Key) noexcept final;

public:
	// 查找迭代器
	virtual iterator find(const XString& _Key) noexcept final;

	// 查找迭代器
	virtual const_iterator find(const XString& _Key) const noexcept final;

	// 删除迭代器
	virtual iterator remove(iterator _Iterator) noexcept final;

	// 删除迭代器
	virtual const_iterator remove(const_iterator _Iterator) noexcept final;

	// 分离迭代器
	virtual XJsonValue detach(iterator _Iterator) noexcept final;

	// 分离迭代器
	virtual XJsonValue detach(const_iterator _Iterator) noexcept final;

public:
	// 迭代器
	class _XCOREAPI_ iterator
	{
	private:
		friend class XJsonObject;

	private:
		// 私有数据
		XJsonObjectIterPrivate*			memberData;

	public:
		// constructor
		iterator() noexcept;

		// constructor
		explicit iterator(XJsonObjectIterPrivate* _PrivateData) noexcept;

		// constructor
		iterator(const iterator& _Object) noexcept;

		// constructor
		iterator(iterator&& _Object) noexcept;

		// destructor
		virtual ~iterator() noexcept;

	public:
		// operator =
		iterator& operator =(const iterator& _Object) noexcept;

		// operator =
		iterator& operator =(iterator&& _Object) noexcept;

	public:
		// operator =
		bool operator ==(const iterator& _Object) noexcept;

		// operator =
		bool operator !=(const iterator& _Object) noexcept;

	public:
		// operator ++
		iterator& operator ++() noexcept;

		// operator ++
		iterator operator ++() const noexcept;

		// operator --
		iterator& operator --() noexcept;

		// operator --
		iterator operator --() const noexcept;

		// operator +
		iterator operator +(x_ssize_t _Size) const noexcept;

		// operator -
		iterator operator -(x_ssize_t _Size) const noexcept;

		// operator +=
		iterator& operator +=(x_ssize_t _Size) noexcept;

		// operator -=
		iterator& operator -=(x_ssize_t _Size) noexcept;

	public:
		// operator *
		virtual XJsonValue& operator *() final;

		// operator *
		virtual const XJsonValue& operator *() const final;

	public:
		// 是否有效
		virtual bool isValid() const noexcept final;

		// Key
		virtual const XString& key() const final;

		// Value
		virtual XJsonValue& value() final;

		// Value
		virtual const XJsonValue& value() const final;
	};

public:
	// 只读迭代器
	class _XCOREAPI_ const_iterator
	{
	private:
		friend class XJsonObject;

	private:
		// 私有数据
		XJsonObjectIterPrivate*			memberData;

	public:
		// constructor
		const_iterator() noexcept;

		// constructor
		explicit const_iterator(XJsonObjectIterPrivate* _PrivateData) noexcept;

		// constructor
		const_iterator(const const_iterator& _Object) noexcept;

		// constructor
		const_iterator(const_iterator&& _Object) noexcept;

		// destructor
		virtual ~const_iterator() noexcept;

	public:
		// operator =
		const_iterator& operator =(const const_iterator& _Object) noexcept;

		// operator =
		const_iterator& operator =(const_iterator&& _Object) noexcept;

	public:
		// operator =
		bool operator ==(const const_iterator& _Object) noexcept;

		// operator =
		bool operator !=(const const_iterator& _Object) noexcept;

	public:
		// operator ++
		const_iterator& operator ++() noexcept;

		// operator ++
		const_iterator operator ++() const noexcept;

		// operator --
		const_iterator& operator --() noexcept;

		// operator --
		const_iterator operator --() const noexcept;

		// operator +
		const_iterator operator +(x_ssize_t _Size) const noexcept;

		// operator -
		const_iterator operator -(x_ssize_t _Size) const noexcept;

		// operator +=
		const_iterator& operator +=(x_ssize_t _Size) noexcept;

		// operator -=
		const_iterator& operator -=(x_ssize_t _Size) noexcept;

	public:
		// operator *
		virtual const XJsonValue& operator *() const final;

	public:
		// 是否有效
		virtual bool isValid() const noexcept final;

		// Key
		virtual const XString& key() const final;

		// Value
		virtual const XJsonValue& value() const final;
	};
};

#endif
