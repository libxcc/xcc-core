#ifndef			_XCC_CORE_JSON_JSON_H_
#define			_XCC_CORE_JSON_JSON_H_

#include <xcc-core/header.h>
#include <xcc-core/XByteArray.h>
#include <xcc-core/XString.h>

// Json objects from the Xanadu family
// JSON TYPE
#define	XJson_False		0
#define XJson_True		1
#define XJson_NULL		2
#define XJson_Int		3
#define XJson_Double		4
#define XJson_String		5
#define XJson_Array		6
#define XJson_Object		7

#define XJson_IsReference	256

// XCC_JSON_INFO结构
typedef struct _XCC_CORE_JSON_INFO
{
	struct _XCC_CORE_JSON_INFO*	prev;		// next/prev允许您遍历数组/对象链。或者，使用GetArraySize/GetArrayItem/GetObjectItem
	struct _XCC_CORE_JSON_INFO*	next;		// next/prev允许您遍历数组/对象链。或者，使用GetArraySize/GetArrayItem/GetObjectItem
	struct _XCC_CORE_JSON_INFO*	child;		// 数组或对象项将有一个子指针指向数组/对象中的链表
	int				type;		// 项目的类型
	char*				valuestring;	// 字符串，如果type==XJson_String
	unsigned long long		valueint;	// 整型，如果type==XJson_Number
	double				valuedouble;	// 浮点数，如果type==XJson_Number
	int				sign;		// valueint的符号，1（无符号），-1（有符号）
	char*				name;		// 项的名称字符串，如果此项是对象的子项，或位于对象的子项列表中
}XCC_JSON_INFO;


// 提供一个JSON块，这将返回一个可以查询的XCC_JSON_INFO对象。完成后请调用XJson_Delete
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_parse(const char* value) noexcept;

// 将XCC_JSON_INFO实体呈现为文本以进行传输/存储。完成后释放字符
_XCOREAPI_ char* __xcall__ XJson_Print(XCC_JSON_INFO* item) noexcept;

// 将XCC_JSON_INFO实体呈现为文本，以便传输/存储，而无需任何格式。完成后释放字符
_XCOREAPI_ char* __xcall__ XJson_PrintUnformatted(XCC_JSON_INFO* item) noexcept;

// 删除XCC_JSON_INFO实体及其所有子实体
_XCOREAPI_ void __xcall__ XJson_remove(XCC_JSON_INFO* _Json) noexcept;


// 返回数组（或对象）中的项数
_XCOREAPI_ int __xcall__ XJson_GetArraySize(XCC_JSON_INFO* array) noexcept;


// 从数组“array”中检索项目编号“item”。如果不成功，则返回NULL
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_GetArrayItem(XCC_JSON_INFO* array, int item) noexcept;


// 从对象获取项“name”。不区分大小写
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_GetObjectItem(XCC_JSON_INFO* object, const char* string) noexcept;


// 用于分析失败的原因。这将返回一个指向解析错误的指针。当XJson_Parse（）返回0时定义
_XCOREAPI_ const char* __xcall__ XJson_GetErrorPtr() noexcept;


// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateNull() noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateTrue() noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateFalse() noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateBool(int b) noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateDouble(double num, int sign) noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateInt(unsigned long long num, int sign) noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateString(const char* string) noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateArray() noexcept;

// 这些调用创建适当类型的XCC_JSON_INFO项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateObject() noexcept;


// 创建int数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateIntArray(int* numbers, int sign, int count) noexcept;

// 创建float数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateFloatArray(float* numbers, int count) noexcept;

// 创建double数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateDoubleArray(double* numbers, int count) noexcept;

// 创建const char*数组
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_CreateStringArray(const char** strings, int count) noexcept;


// 将项附加到指定的数组/对象
_XCOREAPI_ void __xcall__ XJson_AddItemToArray(XCC_JSON_INFO* array, XCC_JSON_INFO* item) noexcept;

// 将项附加到指定的数组/对象
_XCOREAPI_ void __xcall__ XJson_AddItemToArrayHead(XCC_JSON_INFO* array, XCC_JSON_INFO* item) noexcept;

// 将项附加到指定的数组/对象
_XCOREAPI_ void __xcall__ XJson_AddItemToObject(XCC_JSON_INFO* object, const char* string, XCC_JSON_INFO* item) noexcept;


// 将对项的引用附加到指定的数组/对象。如果要将现有的XCC_JSON_INFO添加到新的XCC_JSON_INFO，但不希望损坏现有的XCC_JSON_INFO，请使用此选项
_XCOREAPI_ void __xcall__ XJson_AddItemReferenceToArray(XCC_JSON_INFO* array, XCC_JSON_INFO* item) noexcept;

// 将对项的引用附加到指定的数组/对象。如果要将现有的XCC_JSON_INFO添加到新的XCC_JSON_INFO，但不希望损坏现有的XCC_JSON_INFO，请使用此选项
_XCOREAPI_ void __xcall__ XJson_AddItemReferenceToObject(XCC_JSON_INFO* object, const char* string, XCC_JSON_INFO* item) noexcept;


// 从数组/对象中移除/取消匹配项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_DetachItemFromArray(XCC_JSON_INFO* array, int which) noexcept;

// 从数组/对象中移除/取消匹配项
_XCOREAPI_ void __xcall__ XJson_DeleteItemFromArray(XCC_JSON_INFO* array, int which) noexcept;

// 从数组/对象中移除/取消匹配项
_XCOREAPI_ XCC_JSON_INFO* __xcall__ XJson_DetachItemFromObject(XCC_JSON_INFO* object, const char* string) noexcept;

// 从数组/对象中移除/取消匹配项
_XCOREAPI_ void __xcall__ XJson_DeleteItemFromObject(XCC_JSON_INFO* object, const char* string) noexcept;


// 更新数组项
_XCOREAPI_ void __xcall__ XJson_ReplaceItemInArray(XCC_JSON_INFO* array, int which, XCC_JSON_INFO* newitem) noexcept;

// 更新数组项
_XCOREAPI_ void __xcall__ XJson_ReplaceItemInObject(XCC_JSON_INFO* object, const char* string, XCC_JSON_INFO* newitem) noexcept;



#define XJson_AddNullToObject(_Object,_Name)			XJson_AddItemToObject(_Object, _Name, XJson_CreateNull())
#define XJson_AddTrueToObject(_Object,_Name)			XJson_AddItemToObject(_Object, _Name, XJson_CreateTrue())
#define XJson_AddFalseToObject(_Object,_Name)			XJson_AddItemToObject(_Object, _Name, XJson_CreateFalse())
#define XJson_AddNumberToObject(_Object,_Name,_Number)		XJson_AddItemToObject(_Object, _Name, XJson_CreateInt(_Number))
#define XJson_AddStringToObject(_Object,_Name,_String)		XJson_AddItemToObject(_Object, _Name, XJson_CreateString(_String))




// Xanadu Class Json
class _XCOREAPI_ XJsonPosix
{
private:
	// constructor
	XJsonPosix(XCC_JSON_INFO* _JsonData) noexcept;

public:
	// constructor
	XJsonPosix() noexcept;

	// constructor
	XJsonPosix(const UString& _JsonString) noexcept;

	// constructor
	XJsonPosix(const XByteArray& _JsonBytes) noexcept;

	// constructor
	XJsonPosix(const XString& _JsonString) noexcept;

	// constructor
	XJsonPosix(const XJsonPosix* _JsonObject) noexcept;

	// constructor
	XJsonPosix(const XJsonPosix& _JsonObject) noexcept;

	// destructor
	virtual ~XJsonPosix() noexcept;

public:
	// [opt] operators overload =
	XJsonPosix& operator=(const XJsonPosix& _Value) noexcept;

	// [opt] operators overload ==
	virtual bool operator==(const XJsonPosix& _Value) const noexcept;

public:
	// [fmt] from string
	virtual bool parse(const char* _Bytes) noexcept;

	// [fmt] from string
	virtual bool parse(const UString& _JsonString) noexcept;

	// [fmt] from bytes
	virtual bool parse(const XByteArray& _JsonBytes) noexcept;

	// [fmt] from string
	virtual bool parse(const XString& _JsonString) noexcept;

public:
	virtual void clear() noexcept;

	// [get] 是否为空
	virtual bool empty() const noexcept;

	// [get] 是否有值
	virtual bool exist() const noexcept;

	virtual bool isArray() const noexcept;

	virtual XString toString() const noexcept;

	virtual XString toFormattedString() const noexcept;

	virtual const XString& getErrorMessage() const noexcept;

public:
	virtual bool appendEmptySubObject(const UString& _Key) noexcept;

	virtual bool appendEmptySubArray(const UString& _Key) noexcept;

	virtual XJsonPosix& operator[](const UString& _Key) noexcept;

	virtual UString operator()(const UString& _Key) const noexcept;

private:
	// [get] 获取指定Key的子项
	virtual XCC_JSON_INFO* _get(const UString& _Key) const noexcept;

public:
	// [get] 遍历
	virtual bool traverse(const std::function<void(const UString& _Key)>& _Lambda) const noexcept;

public:
	// [get] 获取指定Key的值 Object
	virtual bool get(const UString& _Key, XJsonPosix& _Value) const noexcept;

	// [get] 获取指定Key的值 String
	virtual bool get(const UString& _Key, UString& _Value) const noexcept;

	// [get] 获取指定Key的值 Bytes
	virtual bool get(const UString& _Key, XByteArray& _Value) const noexcept;

	// [get] 获取指定Key的值 String
	virtual bool get(const UString& _Key, XString& _Value) const noexcept;

	// [get] 获取指定Key的值 bool
	virtual bool get(const UString& _Key, bool& _Value) const noexcept;

	// [get] 获取指定Key的值 char
	virtual bool get(const UString& _Key, char& _Value) const noexcept;

	// [get] 获取指定Key的值 unsigned char
	virtual bool get(const UString& _Key, unsigned char& _Value) const noexcept;

	// [get] 获取指定Key的值 short
	virtual bool get(const UString& _Key, short& _Value) const noexcept;

	// [get] 获取指定Key的值 unsigned short
	virtual bool get(const UString& _Key, unsigned short& _Value) const noexcept;

	// [get] 获取指定Key的值 int
	virtual bool get(const UString& _Key, int& _Value) const noexcept;

	// [get] 获取指定Key的值 unsigned int
	virtual bool get(const UString& _Key, unsigned int& _Value) const noexcept;

	// [get] 获取指定Key的值 long
	virtual bool get(const UString& _Key, long& _Value) const noexcept;

	// [get] 获取指定Key的值 unsigned long
	virtual bool get(const UString& _Key, unsigned long& _Value) const noexcept;

	// [get] 获取指定Key的值 long long
	virtual bool get(const UString& _Key, long long& _Value) const noexcept;

	// [get] 获取指定Key的值 unsigned long long
	virtual bool get(const UString& _Key, unsigned long long& _Value) const noexcept;

	// [get] 获取指定Key的值 float
	virtual bool get(const UString& _Key, float& _Value) const noexcept;

	// [get] 获取指定Key的值 double
	virtual bool get(const UString& _Key, double& _Value) const noexcept;

public:
	// [set] 添加子项 Object
	virtual bool append(const UString& _Key, const XJsonPosix& _Value) noexcept;

	// [set] 添加子项 String
	virtual bool append(const UString& _Key, const UString& _Value) noexcept;

	// [set] 添加子项 XByteArray
	virtual bool append(const UString& _Key, const XByteArray& _Value) noexcept;

	// [set] 添加子项 XString
	virtual bool append(const UString& _Key, const XString& _Value) noexcept;

	// [set] 添加子项 bool
	virtual bool append(const UString& _Key, bool _Value) noexcept;

	// [set] 添加子项 char
	virtual bool append(const UString& _Key, char _Value) noexcept;

	// [set] 添加子项 unsigned char
	virtual bool append(const UString& _Key, unsigned char _Value) noexcept;

	// [set] 添加子项 short
	virtual bool append(const UString& _Key, short _Value) noexcept;

	// [set] 添加子项 unsigned short
	virtual bool append(const UString& _Key, unsigned short _Value) noexcept;

	// [set] 添加子项 int
	virtual bool append(const UString& _Key, int _Value) noexcept;

	// [set] 添加子项 unsigned int
	virtual bool append(const UString& _Key, unsigned int _Value) noexcept;

	// [set] 添加子项 long
	virtual bool append(const UString& _Key, long _Value) noexcept;

	// [set] 添加子项 unsigned long
	virtual bool append(const UString& _Key, unsigned long _Value) noexcept;

	// [set] 添加子项 long long
	virtual bool append(const UString& _Key, long long _Value) noexcept;

	// [set] 添加子项 unsigned long long
	virtual bool append(const UString& _Key, unsigned long long _Value) noexcept;

	// [set] 添加子项 float
	virtual bool append(const UString& _Key, float _Value) noexcept;

	// [set] 添加子项 double
	virtual bool append(const UString& _Key, double _Value) noexcept;

public:
	// [set] 移除子项
	virtual bool remove(const UString& _Key) noexcept;

public:
	// [set] 替换子项 Object
	virtual bool replace(const UString& _Key, const XJsonPosix& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, const UString& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, const XByteArray& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, const XString& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, bool _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, char _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, unsigned char _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, short _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, unsigned short _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, int _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, unsigned int _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, unsigned long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, long long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, unsigned long long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, float _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(const UString& _Key, double _Value) noexcept;

public:
	// [get] 获取子项 Object
	virtual XJsonPosix toObject(const UString& _Key) const noexcept;

	// [get] 获取子项 String
	virtual UString toString(const UString& _Key) const noexcept;

	// [get] 获取子项 Bytes
	virtual XByteArray toBytes(const UString& _Key) const noexcept;

	// [get] 获取子项 XString
	virtual XString toXString(const UString& _Key) const noexcept;

	// [get] 获取子项 bool
	virtual bool toBool(const UString& _Key) const noexcept;

	// [get] 获取子项 char
	virtual char toChar(const UString& _Key) const noexcept;

	// [get] 获取子项 unsigned char
	virtual unsigned char toUChar(const UString& _Key) const noexcept;

	// [get] 获取子项 short
	virtual short toShort(const UString& _Key) const noexcept;

	// [get] 获取子项 unsigned short
	virtual unsigned short toUShort(const UString& _Key) const noexcept;

	// [get] 获取子项 int
	virtual int toInt(const UString& _Key) const noexcept;

	// [get] 获取子项 unsigned int
	virtual unsigned int toUInt(const UString& _Key) const noexcept;

	// [get] 获取子项 long
	virtual long toLong(const UString& _Key) const noexcept;

	// [get] 获取子项 unsigned long
	virtual unsigned long toULong(const UString& _Key) const noexcept;

	// [get] 获取子项 long long
	virtual long long toLLong(const UString& _Key) const noexcept;

	// [get] 获取子项 unsigned long long
	virtual unsigned long long toULLong(const UString& _Key) const noexcept;

	// [get] 获取子项 float
	virtual float toFloat(const UString& _Key) const noexcept;

	// [get] 获取子项 double
	virtual double toDouble(const UString& _Key) const noexcept;

public:
	// [get] 获取数组大小
	virtual int arraySize() const noexcept;

	// [get] 根据下标获取子项
	virtual XJsonPosix& operator[](unsigned int _Index) noexcept;

	// [get] 根据下标获取子项
	virtual UString operator()(unsigned int _Index) const noexcept;

private:
	// [get] 根据下标获取子项
	virtual XCC_JSON_INFO* _get(int _Index) const noexcept;

public:
	// [get] 根据下标获取子项
	virtual bool get(int _Index, XJsonPosix& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, UString& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, XByteArray& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, XString& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, bool& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, char& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, unsigned char& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, short& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, unsigned short& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, int& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, unsigned int& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, long& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, unsigned long& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, long long& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, unsigned long long& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, float& _Value) const noexcept;

	// [get] 根据下标获取子项
	virtual bool get(int _Index, double& _Value) const noexcept;

public:
	// [set] 在数组开头添加子项
	virtual bool prepend(const XJsonPosix& _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(const UString& _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(const XByteArray& _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(const XString& _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(bool _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(char _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(unsigned char _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(short _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(unsigned short _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(int _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(unsigned int _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(long _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(unsigned long _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(long long _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(unsigned long long _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(float _Value) noexcept;

	// [set] 在数组开头添加子项
	virtual bool prepend(double _Value) noexcept;

public:
	// [set] 在数组结尾添加子项
	virtual bool append(const XJsonPosix& _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(const UString& _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(const XByteArray& _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(const XString& _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(bool _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(char _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(unsigned char _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(short _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(unsigned short _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(int _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(unsigned int _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(long _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(unsigned long _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(long long _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(unsigned long long _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(float _Value) noexcept;

	// [set] 在数组结尾添加子项
	virtual bool append(double _Value) noexcept;

public:
	// [set] 移除子项
	virtual bool remove(int _Index) noexcept;

public:
	// [set] 替换子项
	virtual bool replace(int _Index, const XJsonPosix& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, const UString& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, const XByteArray& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, const XString& _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, bool _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, char _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, unsigned char _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, short _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, unsigned short _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, int _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, unsigned int _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, unsigned long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, long long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, unsigned long long _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, float _Value) noexcept;

	// [set] 替换子项
	virtual bool replace(int _Index, double _Value) noexcept;

private:
	XCC_JSON_INFO*					_ThisJsonData;

	XCC_JSON_INFO*					_ThisExternJsonDataRef;

	XString							_ThisErrorMessage;

	std::map<unsigned int, XJsonPosix*>*			_ThisJsonArrayRef;

	std::map<UString, XJsonPosix*>*			_ThisJsonObjectRef;
};

#endif
