#include <xcc-core/system/XRegistry.h>



// 注册表操作类的私有数据
struct XRegistryPrivate
{
	HKEY			handle{};
	x_uint64_t 		reference{};

	HKEY 			root{};
	XString			path;
};



// constructor
XRegistry::XRegistry() noexcept
{
	d_ptr = new(std::nothrow) XRegistryPrivate();
	d_ptr->reference = 1;
}

// constructor
XRegistry::XRegistry(HKEY _Root, const XString& _Path, xcc::OpenMode _Mode) noexcept
{
	d_ptr = new(std::nothrow) XRegistryPrivate();
	d_ptr->reference = 1;

	d_ptr->root = _Root;
	d_ptr->path = _Path;

	this->open(_Mode);
}

// constructor
XRegistry::XRegistry(const XRegistry& _InfoOther) noexcept
{
	this->d_ptr = _InfoOther.d_ptr;
	++this->d_ptr->reference;
}

// constructor
XRegistry::XRegistry(XRegistry&& _InfoOther) noexcept
{
	this->d_ptr = _InfoOther.d_ptr;
	_InfoOther.d_ptr = nullptr;
}

// destructor
XRegistry::~XRegistry() noexcept
{
	if(d_ptr->reference == 1)
	{
		this->close();
		delete d_ptr;
	}
	else
	{
		--d_ptr->reference;
	}
}



// [get] 是否存在注册表项
bool XRegistry::exist(HKEY _Root, const XString& _Path) noexcept
{
	if(nullptr == _Root || _Path.empty())
	{
		return false;
	}
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPathW = _Path.toWString();
	HKEY 		vHandle = nullptr;
	auto		vStatus = RegOpenKeyExW(_Root, vPathW.data(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &vHandle);
	if(ERROR_SUCCESS == vStatus)
	{
		RegCloseKey(vHandle);
		return true;
	}
#else
	XCC_UNUSED(_Root);
	XCC_UNUSED(_Path);
#endif
	return false;
}

// [get] 是否存在注册表项
bool XRegistry::exist(const XString& _Path) noexcept
{
	return XRegistry::exist(d_ptr->handle, _Path);
}



// [opt] 打开
bool XRegistry::open(xcc::OpenMode _Mode) noexcept
{
	if(nullptr == d_ptr->root || d_ptr->path.empty())
	{
		return false;
	}

#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPath = d_ptr->path.toWString();
	auto		vFlagCreate = _Mode & xcc::WriteOnly;
	HKEY 		vHandle = nullptr;
	LSTATUS		vStatus = 0;

	if(vFlagCreate)
	{
		DWORD 		vDisposition = 0;
		vStatus = RegCreateKeyExW(d_ptr->root, vPath.data(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, nullptr, &vHandle, &vDisposition);
	}
	else
	{
		vStatus = RegOpenKeyExW(d_ptr->root, vPath.data(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &vHandle);
	}

	if(ERROR_SUCCESS == vStatus)
	{
		d_ptr->handle = vHandle;
	}
#else
	XCC_UNUSED(_Mode);
#endif
	return false;
}

// [opt] 打开
bool XRegistry::open(HKEY _Root, const XString& _Path, xcc::OpenMode _Mode) noexcept
{
	d_ptr->root = _Root;
	d_ptr->path = _Path;

	return this->open(_Mode);
}

// [opt] 关闭
void XRegistry::close() noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	RegCloseKey(d_ptr->handle);
#endif
}



// [get] 是否打开
bool XRegistry::isOpen() const noexcept
{
	return d_ptr->handle;
}



// [set] 字符串
bool XRegistry::setItemString(const XString& _Name, const XString& _Value) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vNameW = _Name.toWString();
	auto		vValue = _Value.toWString();

	auto		vStatus = RegSetValueExW(d_ptr->handle, vNameW.data(), 0, REG_SZ, (const BYTE*)vValue.data(), (DWORD)((vValue.size() + 1) * sizeof(wchar_t)));
	return ERROR_SUCCESS == vStatus;
#else
	XCC_UNUSED(_Name);
	XCC_UNUSED(_Value);
	return false;
#endif
}

// [set] 字符串
bool XRegistry::setItemDword(const XString& _Name, DWORD _Value) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vNameW = _Name.toWString();

	auto		vStatus = RegSetValueExW(d_ptr->handle, vNameW.data(), 0, REG_DWORD, (const BYTE*)&_Value, sizeof(DWORD));
	return ERROR_SUCCESS == vStatus;
#else
	XCC_UNUSED(_Name);
	XCC_UNUSED(_Value);
	return false;
#endif
}

// [get] 字符串
XString XRegistry::itemString(const XString& _Name) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vNameW = _Name.toWString();
	wchar_t 	vBuffer[XCC_PATH_MAX] = {0};
	DWORD		vSize = XCC_PATH_MAX;
	DWORD		vType = REG_SZ;
	auto		vStatus = RegQueryValueExW(d_ptr->handle, vNameW.data(), nullptr, &vType, (BYTE*)vBuffer, &vSize);
	if(ERROR_SUCCESS == vStatus)
	{
		return XString::fromWString(vBuffer);
	}
#else
	XCC_UNUSED(_Name);
#endif
	return {};
}

// [get] 字符串
DWORD XRegistry::itemDword(const XString& _Name) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vNameW = _Name.toWString();
	DWORD 		vValue = 0;
	DWORD		vSize = sizeof(DWORD);
	DWORD		vType = REG_DWORD;
	auto		vStatus = RegQueryValueExW(d_ptr->handle, vNameW.data(), nullptr, &vType, (BYTE*)&vValue, &vSize);
	if(ERROR_SUCCESS == vStatus)
	{
		return vValue;
	}
#else
	XCC_UNUSED(_Name);
#endif
	return 0;
}

// [opt] 删除键
bool XRegistry::remove(const XString& _Name) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vPathW = _Name.toWString();

	// auto		vStatus = SHDeleteKeyW(_Handle, _Path.data());
	auto		vStatus = RegDeleteTreeW(d_ptr->handle, vPathW.data());
	return ERROR_SUCCESS == vStatus;
#else
	XCC_UNUSED(_Name);
	return false;
#endif
}
