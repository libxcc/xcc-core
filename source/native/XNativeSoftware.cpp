#include <xcc-core/native/XNativeSoftware.h>
#include <xcc-core/system/XSystem.h>


// Native software details
struct XNativeSoftwareDescPrivate
{
	bool					isX64;			// 是否64位
	XString					label;			// 标签
	XString					displayName;		// 名称
	XString					displayVersion;		// 版本
	XString					installLocation;	// 安装位置
	XString					uninstallString;	// 卸载命令行

	// constructor
	XNativeSoftwareDescPrivate()
	{
		this->isX64 = true;
	}
};



// constructor
XNativeSoftwareDesc::XNativeSoftwareDesc() noexcept
{
	d_ptr = new(std::nothrow) XNativeSoftwareDescPrivate();
}

// constructor
XNativeSoftwareDesc::XNativeSoftwareDesc(XNativeSoftwareDescPrivate* _Private) noexcept
{
	d_ptr = _Private;
}

// constructor
XNativeSoftwareDesc::XNativeSoftwareDesc(const XNativeSoftwareDesc& _Right) noexcept
{
	d_ptr = new(std::nothrow) XNativeSoftwareDescPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// constructor
XNativeSoftwareDesc::XNativeSoftwareDesc(XNativeSoftwareDesc&& _Right) noexcept
{
	d_ptr = new(std::nothrow) XNativeSoftwareDescPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// destructor
XNativeSoftwareDesc::~XNativeSoftwareDesc() noexcept
{
	delete d_ptr;
}



// operator =
XNativeSoftwareDesc& XNativeSoftwareDesc::operator=(const XNativeSoftwareDesc& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}

// operator =
XNativeSoftwareDesc& XNativeSoftwareDesc::operator=(XNativeSoftwareDesc&& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}



// [get] 是否32位
bool XNativeSoftwareDesc::isX86() const noexcept
{
	return !d_ptr->isX64;
}

// [get] 是否64位
bool XNativeSoftwareDesc::isX64() const noexcept
{
	return d_ptr->isX64;
}

// [get] 标签
const XString& XNativeSoftwareDesc::label() const noexcept
{
	return d_ptr->label;
}

// [get] 名称
const XString& XNativeSoftwareDesc::displayName() const noexcept
{
	return d_ptr->displayName;
}

// [get] 版本
const XString& XNativeSoftwareDesc::displayVersion() const noexcept
{
	return d_ptr->displayVersion;
}

// [get] 安装位置
const XString& XNativeSoftwareDesc::installLocation() const noexcept
{
	return d_ptr->installLocation;
}

// [get] 卸载命令行
const XString& XNativeSoftwareDesc::uninstallString() const noexcept
{
	return d_ptr->uninstallString;
}





// constructor
XNativeSoftware::XNativeSoftware() noexcept = default;

// destructor
XNativeSoftware::~XNativeSoftware() noexcept = default;



// 获取子项值
static XString __xcall__ NativeSoftwareRegisterKeyValue(HKEY _Key, const WString& _Guid, const WString& _Name) noexcept
{
	auto		vKeyValue = XString();
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vLocalKey = static_cast<HKEY>(nullptr);
	auto		vResult = RegOpenKeyExW(_Key, _Guid.data(), NULL, KEY_READ, &vLocalKey);
	if(vResult == ERROR_SUCCESS)
	{
		wchar_t		vBuffer[MAX_PATH] = { 0 };
		auto		vLength = DWORD(MAX_PATH);
		auto		vType = DWORD(REG_SZ);
		x_posix_memset(vBuffer, 0, sizeof(wchar_t) * MAX_PATH);
		auto		vQuery = RegQueryValueExW(vLocalKey, _Name.data(), nullptr, &vType, (byte*)vBuffer, &vLength);
		if(vQuery == ERROR_SUCCESS)
		{
			vKeyValue = XString::fromWString(vBuffer);
		}
	}
#endif
	return vKeyValue;
}

// Enumerate the list of native software
bool XNativeSoftware::softwareEnum(const std::function<bool(const XNativeSoftwareDesc& _InfoSoftware)>& _Lambda) noexcept
{
	auto		vResult = false;
#if defined(XCC_SYSTEM_WINDOWS)
	XCC_CHECK_RETURN(_Lambda, vResult);
	auto		FunctionEnumItem = [&](HKEY _Key, bool _X64)
	{
		LONG		vResult = 0;
		DWORD		vIndex = 0;
		do
		{
			wchar_t		vBuffer[MAX_PATH] = { 0 };
			DWORD		vLength = MAX_PATH;
			x_posix_memset(vBuffer, 0, sizeof(wchar_t) * MAX_PATH);
			vResult = RegEnumKeyExW(_Key, vIndex, vBuffer, &vLength, nullptr, nullptr, nullptr, nullptr);
			if(vResult == ERROR_SUCCESS)
			{
				auto		vPrivate = new(std::nothrow) XNativeSoftwareDescPrivate();
				if(vPrivate)
				{
					vPrivate->isX64 = _X64;
					vPrivate->label = XString::fromWString(vBuffer);
					vPrivate->installLocation = NativeSoftwareRegisterKeyValue(_Key, vBuffer, L"InstallLocation");
					vPrivate->displayName = NativeSoftwareRegisterKeyValue(_Key, vBuffer, L"DisplayName");
					vPrivate->uninstallString = NativeSoftwareRegisterKeyValue(_Key, vBuffer, L"UninstallString");
					vPrivate->displayVersion = NativeSoftwareRegisterKeyValue(_Key, vBuffer, L"DisplayVersion");
					_Lambda(XNativeSoftwareDesc(vPrivate));
				}
			}
			++vIndex;
		} while(vResult != ERROR_NO_MORE_ITEMS);
	};

	//获取32位
	{
		HKEY		vKey = nullptr;
		LONG		vRegedit = RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall)", NULL, KEY_READ, &vKey);
		if(ERROR_SUCCESS == vRegedit)
		{
			FunctionEnumItem(vKey, false);
			RegCloseKey(vKey);
			vResult = true;
		}
	}
	//获取64位
	if(XSystem::is_64bit())
	{
		HKEY		vKey = nullptr;
		LONG		vRegedit = RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall)", NULL, KEY_READ | KEY_WOW64_64KEY, &vKey);
		if(ERROR_SUCCESS == vRegedit)
		{
			FunctionEnumItem(vKey, true);
			RegCloseKey(vKey);
			vResult = true;
		}
	}
#endif
#if defined(XCC_SYSTEM_LINUX)
#endif
#if defined(XCC_SYSTEM_DARWIN)
	/*
	XFileSystem::DirectoryList("/Applications", [&](const XFileInfo& _Info)->bool
	{
		if(_Info.isDir())
		{
			auto	vUninstall = Dynamic::plist_from_file(_Info.filePath() + "/Contents/Info.plist");
			if(vUninstall)
			{
				auto		vPrivate = new(std::nothrow) XNativeSoftwareDescPrivate();
				if(vInfo)
				{
					vPrivate->isX64 = true;
					vPrivate->label = Dynamic::plist_node_get_string(Dynamic::plist_dict_get_item(vUninstall, "CFBundleExecutable"));
					vPrivate->installLocation = L"";
					vPrivate->displayName = Dynamic::plist_node_get_string(Dynamic::plist_dict_get_item(vUninstall, "CFBundleDisplayName"));
					vPrivate->uninstallString = L"";
					vPrivate->displayVersion = Dynamic::plist_node_get_string(Dynamic::plist_dict_get_item(vUninstall, "CFBundleShortVersionString"));
					_Lambda(XNativeSoftwareDesc(vPrivate));
				}
				Dynamic::plist_free(vUninstall);
			}
		}
		return true;
	});
	*/
#endif
	return vResult;
}
