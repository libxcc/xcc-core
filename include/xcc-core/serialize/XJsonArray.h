#ifndef			_XCC_CORE_JSON_ARRAY_H_
#define			_XCC_CORE_JSON_ARRAY_H_

#include <xcc-core/header.h>


// Json: 前置声明
class XJsonValue;
class XJsonPrivate;
typedef struct XJsonArrayPrivate		XJsonArrayPrivate;
typedef struct XJsonArrayIterPrivate		XJsonArrayIterPrivate;

// Json: 数组
class _XCOREAPI_ XJsonArray
{
private:
	friend class XJsonPrivate;

public:
	// 前置声明
	class iterator;
	class const_iterator;

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
	// operator override =
	XJsonArray& operator=(const XJsonArray &_Object) noexcept;

	// operator override =
	XJsonArray& operator=(XJsonArray &&_Object) noexcept;

public:
	// operator override []
	XJsonValue& operator[](x_size_t _Index);

	// operator override []
	const XJsonValue& operator[](x_size_t _Index) const;

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
	// 插入至最前
	virtual void prepend(const XJsonValue& _JsonValue) noexcept final;

	// 插入至末尾
	virtual void append(const XJsonValue& _JsonValue) noexcept final;

	// 插入至指定下标
	virtual void insert(x_size_t _Index, const XJsonValue& _JsonValue) noexcept final;

	// 替换指定下标
	virtual void replace(x_size_t _Index, const XJsonValue& _JsonValue) noexcept final;

	// 删除指定下标
	virtual void remove(x_size_t _Index) noexcept final;

	// 分离指定下标
	virtual XJsonValue detach(x_size_t _Index) noexcept final;

public:
	// 查找迭代器
	virtual iterator find(x_size_t _Index) noexcept final;

	// 查找迭代器
	virtual const_iterator find(x_size_t _Index) const noexcept final;

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
		friend class XJsonArray;

	private:
		// 私有数据
		XJsonArrayIterPrivate*			memberData;

	public:
		// constructor
		iterator() noexcept;

		// constructor
		explicit iterator(XJsonArrayIterPrivate* _PrivateData) noexcept;

		// constructor
		iterator(const iterator& _Object) noexcept;

		// constructor
		iterator(iterator&& _Object) noexcept;

		// destructor
		virtual ~iterator() noexcept;

	public:
		// operator override =
		iterator& operator =(const iterator& _Object) noexcept;

		// operator override =
		iterator& operator =(iterator&& _Object) noexcept;

	public:
		// operator override =
		bool operator ==(const iterator& _Object) noexcept;

		// operator override =
		bool operator !=(const iterator& _Object) noexcept;

	public:
		// operator override ++
		iterator& operator ++() noexcept;

		// operator override ++
		iterator operator ++() const noexcept;

		// operator override --
		iterator& operator --() noexcept;

		// operator override --
		iterator operator --() const noexcept;

		// operator override +
		iterator operator +(x_ssize_t _Size) const noexcept;

		// operator override -
		iterator operator -(x_ssize_t _Size) const noexcept;

		// operator override +=
		iterator& operator +=(x_ssize_t _Size) noexcept;

		// operator override -=
		iterator& operator -=(x_ssize_t _Size) noexcept;

	public:
		// operator override *
		virtual XJsonValue& operator *() final;

		// operator override *
		virtual const XJsonValue& operator *() const final;

	public:
		// 是否有效
		virtual bool isValid() const noexcept final;

		// 下标
		virtual x_size_t index() const noexcept final;

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
		friend class XJsonArray;

	private:
		// 私有数据
		XJsonArrayIterPrivate*			memberData;

	public:
		// constructor
		const_iterator() noexcept;

		// constructor
		explicit const_iterator(XJsonArrayIterPrivate* _PrivateData) noexcept;

		// constructor
		const_iterator(const const_iterator& _Object) noexcept;

		// constructor
		const_iterator(const_iterator&& _Object) noexcept;

		// destructor
		virtual ~const_iterator() noexcept;

	public:
		// operator override =
		const_iterator& operator =(const const_iterator& _Object) noexcept;

		// operator override =
		const_iterator& operator =(const_iterator&& _Object) noexcept;

	public:
		// operator override =
		bool operator ==(const const_iterator& _Object) noexcept;

		// operator override =
		bool operator !=(const const_iterator& _Object) noexcept;

	public:
		// operator override ++
		const_iterator& operator ++() noexcept;

		// operator override ++
		const_iterator operator ++() const noexcept;

		// operator override --
		const_iterator& operator --() noexcept;

		// operator override --
		const_iterator operator --() const noexcept;

		// operator override +
		const_iterator operator +(x_ssize_t _Size) const noexcept;

		// operator override -
		const_iterator operator -(x_ssize_t _Size) const noexcept;

		// operator override +=
		const_iterator& operator +=(x_ssize_t _Size) noexcept;

		// operator override -=
		const_iterator& operator -=(x_ssize_t _Size) noexcept;

	public:
		// operator override *
		virtual const XJsonValue& operator *() const final;

	public:
		// 是否有效
		virtual bool isValid() const noexcept final;

		// 下标
		virtual x_size_t index() const noexcept final;

		// Value
		virtual const XJsonValue& value() const final;
	};
};

#endif
