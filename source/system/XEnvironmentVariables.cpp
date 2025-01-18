#include <xcc-core/system/XEnvironmentVariables.h>
#if defined(XCC_SYSTEM_WINDOWS)
#else
extern char**		environ;
#endif


// constructor
XEnvironmentVariables::XEnvironmentVariables() noexcept = default;

// destructor
XEnvironmentVariables::~XEnvironmentVariables() noexcept = default;



// 获取环境变量列表
std::map<XString, XString> XEnvironmentVariables::envs() noexcept
{
	auto		vEnvs = std::map<XString, XString>();
#if defined(XCC_SYSTEM_WINDOWS)
	auto		vTextA = GetEnvironmentStringsW();
	if(nullptr == vTextA)
	{
		return {};
	}
	auto		vTextS = vTextA;
	while (*vTextS != L'\0')
	{
		auto		vEnv = XString::fromWString(vTextS);
		auto		vFind = vEnv.find("=");
		if(vFind != XString::npos)
		{
			vEnvs.insert(std::map<XString, XString>::value_type(vEnv.left(vFind), vEnv.mid(vFind + 1)));
		}
		vTextS += x_posix_wcslen(vTextS) + 1;
	}
	FreeEnvironmentStringsW(vTextA);
#else
	auto		vTextS = environ;
	while (nullptr != vTextS && nullptr != *vTextS)
	{
		auto		vEnv = XString(*vTextS);
		auto		vFind = vEnv.find("=");
		if(vFind != XString::npos)
		{
			vEnvs.insert(std::map<XString, XString>::value_type(vEnv.left(vFind), vEnv.mid(vFind + 1)));
		}
		++vTextS;
	}
#endif
	return vEnvs;
}

// 检查指定Key的环境变量是否不存在
bool XEnvironmentVariables::empty(const XString& _Key) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	char		vBuffer[XCC_SIZE_KB] = {0};
	DWORD 		vSizeEnv = GetEnvironmentVariableA(_Key.data(), vBuffer, XCC_SIZE_KB);
	if(vSizeEnv == 0)
	{
		return GetLastError() == ERROR_ENVVAR_NOT_FOUND;
	}
	return false;
#else
	return nullptr == getenv(_Key.data());
#endif
}

// 检查指定Key的环境变量是否存在
bool XEnvironmentVariables::exist(const XString& _Key) noexcept
{
	return !XEnvironmentVariables::empty(_Key);
}

// 设置指定Key的环境变量值
bool XEnvironmentVariables::set(const XString& _Key, const XString& _Value) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	if(SetEnvironmentVariableA(_Key.data(), _Value.data()))
	{
		return true;
	}
	else
	{
		return 0 == GetLastError();
	}
#else
	return 0 == setenv(_Key.data(), _Value.data(), 1);
#endif
}

// 移除指定Key的环境变量值
bool XEnvironmentVariables::unset(const XString& _Key) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	if(SetEnvironmentVariableA(_Key.data(), nullptr))
	{
		return true;
	}
	else
	{
		return 0 == GetLastError();
	}
#else
	return 0 == unsetenv(_Key.data());
#endif
}

// 获取指定Key的环境变量值
XString XEnvironmentVariables::get(const XString& _Key) noexcept
{
#if defined(XCC_SYSTEM_WINDOWS)
	char		vBuffer[XCC_SIZE_KB] = {0};
	DWORD 		vSizeEnv = GetEnvironmentVariableA(_Key.data(), vBuffer, XCC_SIZE_KB);
	if(vSizeEnv != 0)
	{
		vBuffer[vSizeEnv] = 0;
	}
	return vBuffer;
#else
	return getenv(_Key.data());
#endif
}

// 匹配指定Key的环境变量是否存在指定的数据
bool XEnvironmentVariables::match(const XString& _Key, const XString& _Value) noexcept
{
	auto		vEnv = XEnvironmentVariables::get(_Key);
	return _Value == vEnv;
}
