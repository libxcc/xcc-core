#ifndef			_XCC_CORE_JSON_DOCUMENT_H_
#define			_XCC_CORE_JSON_DOCUMENT_H_

#include <xcc-core/header.h>
#include <xcc-core/XByteArray.h>
#include <xcc-core/XString.h>
#include <xcc-core/XJsonValue.h>


// Json: 文档处理
class _XCOREAPI_ XJsonDocument
{
public:
	// constructor
	XJsonDocument() noexcept;

	// constructor
	XJsonDocument(const XJsonDocument& _Object) noexcept;

	// constructor
	XJsonDocument(XJsonDocument&& _Object) noexcept;

	// destructor
	virtual ~XJsonDocument() noexcept;

public:
	// operator overload =
	XJsonDocument& operator = (const XJsonDocument& _Object) noexcept;

	// operator overload =
	XJsonDocument& operator = (XJsonDocument&& _Object) noexcept;

public:
	// [fmt] 转换为字节数组
	static XByteArray toBytes(const XJsonValue& _JsonValue) noexcept;

	// [fmt] 转换为字符串
	static XString toString(const XJsonValue& _JsonValue) noexcept;

	// [fmt] 保存为文件
	static bool toFile(const XJsonValue& _JsonValue, const XString& _FilePath) noexcept;

public:
	// [fmt] 从字节数组格式化
	static XJsonValue fromBytes(const char* _JsonBytes, Xcc::size_t _JsonLength) noexcept;

	// [fmt] 从字节数组格式化
	static XJsonValue fromBytes(const XByteArray& _JsonBytes) noexcept;

	// [fmt] 从字符串格式化
	static XJsonValue fromString(const XString& _JsonString) noexcept;

	// [fmt] 从文件加载
	static XJsonValue fromFile(const XString& _FilePath) noexcept;
};

#endif
