#ifndef			_XCC_CORE_JSON_PRIVATE_H_
#define			_XCC_CORE_JSON_PRIVATE_H_

#include <xcc-core/XJsonDocument.h>
#include <xcc-core/XJsonValue.h>
#include <xcc-core/XJsonObject.h>
#include <xcc-core/XJsonArray.h>
#include <functional>


// 私有数据: 数组
struct XJsonArrayPrivate
{
	std::vector<XJsonValue>				data;

	// constructor
	XJsonArrayPrivate() noexcept
	{
		this->data.clear();
	}
};

// 私有数据: 数组迭代器
struct XJsonArrayIterPrivate
{
	XJsonArrayPrivate*				array;
	std::vector<XJsonValue>::iterator		iter;

	// constructor
	XJsonArrayIterPrivate() noexcept
	{
		this->array = nullptr;
	}
};

// 哈希表排序结构
typedef struct XJsonKeySort
{
	bool operator()(const XString& lhs, const XString& rhs) const noexcept
	{
		static XString l = lhs;
		static XString r = rhs;

		if(lhs == rhs)
		{
			return false;
		}

		if(l == rhs && r == lhs)
		{
			l = lhs;
			r = rhs;
			return false;
		}

		l = lhs;
		r = rhs;
		return true;
	}
}XJsonKeySort;

// 哈希表类型
// typedef std::map<XString, XJsonValue, XJsonKeySort>	XJsonMapType;
// typedef std::map<XString, XJsonValue>		XJsonMapType;
typedef std::vector<std::pair<XString, XJsonValue>>	XJsonMapType;

// 私有数据: 对象
struct XJsonObjectPrivate
{
	XJsonMapType					data;

	// constructor
	XJsonObjectPrivate() noexcept
	{
		this->data.clear();
	}

public:
	// 大小
	virtual xcc::size_t size() const noexcept final
	{
		return this->data.size();
	}

	// 是否为空
	virtual bool empty() const noexcept final
	{
		return this->data.empty();
	}

	// 清空
	virtual void clear() noexcept final
	{
		this->data.clear();
	}

public:
	// iterator: begin
	virtual XJsonMapType::iterator begin() noexcept final
	{
		return this->data.begin();
	}

	// iterator: begin
	virtual XJsonMapType::iterator end() noexcept final
	{
		return this->data.end();
	}

public:
	// 查找
	virtual XJsonMapType::iterator find(const XString& _Key) noexcept final
	{
		for(auto vIterator = this->data.begin(); vIterator != this->data.end(); ++vIterator)
		{
			if(vIterator->first == _Key)
			{
				return vIterator;
			}
		}
		return this->data.end();
	}

	// 插入
	virtual void insert(const XString& _Key, const XJsonValue& _Value) noexcept final
	{
		// this->data.insert(XJsonMapType::value_type(_Key, _Value));
		// this->data[_Key] = _Value;
		this->data.emplace_back(_Key, _Value);
	}

	// 移除
	virtual XJsonMapType::iterator erase(XJsonMapType::iterator _Iterator) noexcept final
	{
		if(_Iterator != this->data.end())
		{
			return this->data.erase(_Iterator);
		}
		return this->data.end();
	}

	// 查重
	virtual bool duplicate_check() const noexcept final
	{
		std::set<XString>		vTempSet;

		for(const auto & vIterator : this->data)
		{
			if(vTempSet.find(vIterator.first) != vTempSet.end())
			{
				return false;
			}
			vTempSet.insert(std::set<XString>::value_type(vIterator.first));
		}
		return true;
	}
};

// 私有数据: 对象迭代器
struct XJsonObjectIterPrivate
{
	XJsonObjectPrivate*				object;
	XJsonMapType::iterator				iter;

	// constructor
	XJsonObjectIterPrivate() noexcept
	{
		this->object = nullptr;
	}
};

// 私有数据: 值
struct XJsonValuePrivate
{
	XJsonValue::Type				type;

	struct Sign
	{
		bool					s_signed;
		bool					s_integer;
	}sign{};

	union Value
	{
		bool					v_boolean;
		xcc::int64_t 				v_int64;
		xcc::uint64_t 				v_uint64;
		double 					v_double;
		XString*				v_string;
		XJsonArray*				v_array;
		XJsonObject*				v_object;
	}value{};

	// constructor
	XJsonValuePrivate() noexcept
	{
		this->type = XJsonValue::Null;

		this->sign.s_signed = false;
		this->sign.s_integer = false;

		this->value.v_boolean = false;
		this->value.v_int64 = 0;
		this->value.v_uint64 = 0;
		this->value.v_double = 0.0f;
		this->value.v_string = nullptr;
		this->value.v_array = nullptr;
		this->value.v_object = nullptr;
	}
};

// 私有数据: 序列化
typedef struct XJsonSerializePrivate
{
	char*						data;
	xcc::size_t					pos;
	xcc::size_t					size;
	XJsonDocument::JsonFormat			format;

	// constructor
	XJsonSerializePrivate() noexcept
	{
		this->data = nullptr;
		this->pos = 0;
		this->size = 0;
		this->format = XJsonDocument::Indented;
	}
}XJsonSerializePrivate;

// 私有数据: 反序列化
typedef struct XJsonDeserializePrivate
{
	const char*					data;
	xcc::size_t					pos;
	xcc::size_t					size;

	// constructor
	XJsonDeserializePrivate() noexcept
	{
		this->data = nullptr;
		this->pos = 0;
		this->size = 0;
	}
}XJsonDeserializePrivate;


// Json: 私有数据
class XJsonPrivate
{
public:
	// 创建数组
	static XJsonArrayPrivate* newArray() noexcept;

	// 创建数组迭代器
	static XJsonArrayIterPrivate* newArrayIter(XJsonArrayPrivate* _Array, xcc::size_t _Index) noexcept;

	// 创建对象
	static XJsonObjectPrivate* newObject() noexcept;

	// 创建对象迭代器
	static XJsonObjectIterPrivate* newObjectIter(XJsonObjectPrivate* _Object, const XString& _Key) noexcept;

	// 创建值
	static XJsonValuePrivate* newValue() noexcept;

public:
	// 释放数据
	static void free(XJsonArrayPrivate* _Object) noexcept;

	// 释放数据
	static void free(XJsonArrayIterPrivate* _Object) noexcept;

	// 释放数据
	static void free(XJsonObjectPrivate* _Object) noexcept;

	// 释放数据
	static void free(XJsonObjectIterPrivate* _Object) noexcept;

	// 释放数据
	static void free(XJsonValuePrivate* _Object) noexcept;

	// 释放数据
	static void free(char* _Object) noexcept;

public:
	// 复制数据
	static XJsonArrayPrivate* clone(XJsonArrayPrivate* _Src) noexcept;

	// 复制数据
	static XJsonArrayIterPrivate* clone(XJsonArrayIterPrivate* _Src) noexcept;

	// 复制数据
	static XJsonObjectPrivate* clone(XJsonObjectPrivate* _Src) noexcept;

	// 复制数据
	static XJsonObjectIterPrivate* clone(XJsonObjectIterPrivate* _Src) noexcept;

	// 复制数据
	static XJsonValuePrivate* clone(XJsonValuePrivate* _Src) noexcept;

public:
	// 预计长度 - 新行
	static xcc::size_t calc_length_new_line(XJsonDocument::JsonFormat _JsonFormat, xcc::size_t _Layer) noexcept;

	// 预计长度
	static xcc::size_t calc_length(xcc::int64_t _Number) noexcept;

	// 预计长度
	static xcc::size_t calc_length(xcc::uint64_t _Number) noexcept;

	// 预计长度
	static xcc::size_t calc_length(double _Number) noexcept;

	// 预计长度
	static xcc::size_t calc_length(XJsonArrayPrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, xcc::size_t _Layer) noexcept;

	// 预计长度
	static xcc::size_t calc_length(XJsonObjectPrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, xcc::size_t _Layer) noexcept;

	// 预计长度
	static xcc::size_t calc_length(XJsonValuePrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, xcc::size_t _Layer) noexcept;

public:
	// 序列化
	static bool serialize(XJsonValuePrivate* _Src, XJsonDocument::JsonFormat _JsonFormat, char** _Bytes, xcc::size_t* _Size) noexcept;

	// 序列化 - 字符
	static bool serialize_char(XJsonSerializePrivate* _Context, char _Char) noexcept;

	// 序列化 - 新行
	static bool serialize_new_line(XJsonSerializePrivate* _Context, xcc::size_t _Layer) noexcept;

	// 序列化 - 空值
	static bool serialize_null(XJsonSerializePrivate* _Context) noexcept;

	// 序列化 - 布尔
	static bool serialize_boolean(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private) noexcept;

	// 序列化 - 数字
	static bool serialize_number(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private) noexcept;

	// 序列化 - 字符串
	static bool serialize_string(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private) noexcept;

	// 序列化 - 字符串
	static bool serialize_string(XJsonSerializePrivate* _Context, const XString& _String) noexcept;

	// 序列化 - 值
	static bool serialize_value(XJsonSerializePrivate* _Context, XJsonValuePrivate* _Private, xcc::size_t _Layer) noexcept;

	// 序列化 - 数组
	static bool serialize_array(XJsonSerializePrivate* _Context, XJsonArrayPrivate* _Private, xcc::size_t _Layer) noexcept;

	// 序列化 - 对象
	static bool serialize_object(XJsonSerializePrivate* _Context, XJsonObjectPrivate* _Private, xcc::size_t _Layer) noexcept;

public:
	// 反序列化
	static bool deserialize(const char* _Bytes, xcc::size_t _Size, XJsonValuePrivate** _Src) noexcept;

	// 反序列化 - 跳过
	static bool deserialize_skip(XJsonDeserializePrivate* _Context) noexcept;

	// 反序列化 - 数字
	static bool deserialize_number(XJsonDeserializePrivate* _Context, XJsonValuePrivate* _Private) noexcept;

	// 反序列化 - 字符串
	static bool deserialize_string(XJsonDeserializePrivate* _Context, XJsonValuePrivate* _Private) noexcept;

	// 反序列化 - 数组
	static bool deserialize_array(XJsonDeserializePrivate* _Context, XJsonArrayPrivate* _Private) noexcept;

	// 反序列化 - 数组 - 子项
	static bool deserialize_array_item(XJsonDeserializePrivate* _Context, XJsonArrayPrivate* _Private) noexcept;

	// 反序列化 - 对象
	static bool deserialize_object(XJsonDeserializePrivate* _Context, XJsonObjectPrivate* _Private) noexcept;

	// 反序列化 - 对象 - 子项
	static bool deserialize_object_item(XJsonDeserializePrivate* _Context, XJsonObjectPrivate* _Private) noexcept;

	// 反序列化 - 值
	static bool deserialize_value(XJsonDeserializePrivate* _Context, XJsonValuePrivate* _Private) noexcept;
};

#endif
