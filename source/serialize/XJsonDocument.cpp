#include <source/serialize/XJsonPrivate.h>
#include <xcc-core/filesystem/XFileSystem.h>


// constructor
XJsonDocument::XJsonDocument() noexcept = default;

// constructor
XJsonDocument::XJsonDocument(const XJsonDocument& _Object) noexcept = default;

// constructor
XJsonDocument::XJsonDocument(XJsonDocument&& _Object) noexcept = default;

// destructor
XJsonDocument::~XJsonDocument() noexcept = default;



// operator override =
XJsonDocument& XJsonDocument::operator = (const XJsonDocument& _Object) noexcept = default;

// operator override =
XJsonDocument& XJsonDocument::operator = (XJsonDocument&& _Object) noexcept = default;



// [fmt] 转换为字节数组
XByteArray XJsonDocument::toBytes(const XJsonValue& _JsonValue, XJsonDocument::JsonFormat _JsonFormat) noexcept
{
	char*		vBytes = nullptr;
	x_size_t 	vLength = 0;
	auto		vStatus = XJsonPrivate::serialize(_JsonValue.memberData, _JsonFormat, &vBytes, &vLength);
	if(vStatus)
	{
		auto		vByteArray = XByteArray(vBytes, vLength);
		XJsonPrivate::free(vBytes);
		return vByteArray;
	}
	return {};
}

// [fmt] 转换为字符串
XString XJsonDocument::toString(const XJsonValue& _JsonValue, XJsonDocument::JsonFormat _JsonFormat) noexcept
{
	char*		vBytes = nullptr;
	x_size_t 	vLength = 0;
	auto		vStatus = XJsonPrivate::serialize(_JsonValue.memberData, _JsonFormat, &vBytes, &vLength);
	if(vStatus)
	{
		auto		vString = XString(vBytes, vLength);
		XJsonPrivate::free(vBytes);
		return vString;
	}
	return {};
}

// [fmt] 保存为文件
bool XJsonDocument::toFile(const XJsonValue& _JsonValue, const XString& _FilePath, XJsonDocument::JsonFormat _JsonFormat) noexcept
{
	auto		vJsonBytes = XJsonDocument::toBytes(_JsonValue, _JsonFormat);
	return XFileSystem::file::fromBytes(_FilePath, vJsonBytes);
}



// [fmt] 从字节数组格式化
XJsonValue XJsonDocument::fromBytes(const char* _JsonBytes, x_size_t _JsonLength) noexcept
{
	auto		vPrivate = static_cast<XJsonValuePrivate*>(nullptr);
	auto		vStatus = XJsonPrivate::deserialize(_JsonBytes, _JsonLength, &vPrivate);
	if(vStatus)
	{
		return XJsonValue(vPrivate);
	}
	return {};
}

// [fmt] 从字节数组格式化
XJsonValue XJsonDocument::fromBytes(const XByteArray& _JsonBytes) noexcept
{
	return XJsonDocument::fromBytes(_JsonBytes.data(), _JsonBytes.size());
}

// [fmt] 从字符串格式化
XJsonValue XJsonDocument::fromString(const XString& _JsonString) noexcept
{
	return XJsonDocument::fromBytes(_JsonString.data(), _JsonString.size());
}

// [fmt] 从文件加载
XJsonValue XJsonDocument::fromFile(const XString& _FilePath) noexcept
{
	auto		vFileBytes = XFileSystem::file::toBytes(_FilePath);
	return XJsonDocument::fromBytes(vFileBytes.data(), vFileBytes.size());
}
