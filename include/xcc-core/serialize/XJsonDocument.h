#ifndef			_XCC_CORE_JSON_DOCUMENT_H_
#define			_XCC_CORE_JSON_DOCUMENT_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XByteArray.h>
#include <xcc-core/container/XString.h>
#include <xcc-core/serialize/XJsonValue.h>


// Json: 文档处理
class _XCOREAPI_ XJsonDocument
{
public:
	// Json格式
	typedef enum JsonFormat
	{
		Compact			= 0,
		Indented		= 1
	}JsonFormat;

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
	// operator =
	XJsonDocument& operator = (const XJsonDocument& _Object) noexcept;

	// operator =
	XJsonDocument& operator = (XJsonDocument&& _Object) noexcept;

public:
	// [fmt] 转换为字节数组
	static XByteArray toBytes(const XJsonValue& _JsonValue, XJsonDocument::JsonFormat _JsonFormat = XJsonDocument::Compact) noexcept;

	// [fmt] 转换为字符串
	static XString toString(const XJsonValue& _JsonValue, XJsonDocument::JsonFormat _JsonFormat = XJsonDocument::Compact) noexcept;

	// [fmt] 保存为文件
	static bool toFile(const XJsonValue& _JsonValue, const XString& _FilePath, XJsonDocument::JsonFormat _JsonFormat = XJsonDocument::Compact) noexcept;

public:
	// [fmt] 从字节数组格式化
	static XJsonValue fromBytes(const char* _JsonBytes, x_size_t _JsonLength) noexcept;

	// [fmt] 从字节数组格式化
	static XJsonValue fromBytes(const XByteArray& _JsonBytes) noexcept;

	// [fmt] 从字符串格式化
	static XJsonValue fromString(const XString& _JsonString) noexcept;

	// [fmt] 从文件加载
	static XJsonValue fromFile(const XString& _FilePath) noexcept;
};

#endif
