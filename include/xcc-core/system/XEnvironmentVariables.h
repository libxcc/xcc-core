#ifndef			_XCC_CORE_ENVIRONMENT_VARIABLES_H_
#define			_XCC_CORE_ENVIRONMENT_VARIABLES_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 环境变量
class _XCOREAPI_ XEnvironmentVariables
{
public:
	// constructor
	XEnvironmentVariables() noexcept;

	// destructor
	virtual ~XEnvironmentVariables() noexcept;

public:
	// 获取环境变量列表
	static std::map<XString, XString> envs() noexcept;

	// 检查指定Key的环境变量是否不存在
	static bool empty(const XString& _Key) noexcept;

	// 检查指定Key的环境变量是否存在
	static bool exist(const XString& _Key) noexcept;

	// 设置指定Key的环境变量值
	static bool set(const XString& _Key, const XString& _Value) noexcept;

	// 移除指定Key的环境变量值
	static bool unset(const XString& _Key) noexcept;

	// 获取指定Key的环境变量值
	static XString get(const XString& _Key) noexcept;

	// 匹配指定Key的环境变量是否存在指定的数据
	static bool match(const XString& _Key, const XString& _Value) noexcept;
};

#endif
