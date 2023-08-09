#ifndef			_XCC_CORE_FS_STANDARD_PATH_H_
#define			_XCC_CORE_FS_STANDARD_PATH_H_

#include <xcc-core/container/XString.h>


// FS: 标准路径获取集
class _XCOREAPI_ XStandardPath
{
public:
	// 路径类型枚举
	typedef enum StandardLocation
	{
		HomeLocation				= 0,		// 用户的主目录
		DesktopLocation				= 1,		// 用户的桌面目录
		DocumentsLocation			= 2,		// 用户的文档目录
		DownloadLocation			= 3,		// 用户的下载目录

		AppConfigLocation			= 10,		// 应用程序的配置目录
		AppDataLocation				= 11,		// 应用程序的数据目录
		AppCacheLocation			= 12,		// 应用程序的缓存目录
		AppTempLocation				= 13,		// 应用程序的临时目录
		AppEnvLocation				= 19,		// 应用程序的ENV目录

		SystemConfigLocation			= 20,		// 系统提供的配置目录
		SystemDataLocation			= 21,		// 系统提供的数据目录
		SystemCacheLocation			= 22,		// 系统提供的缓存目录
		SystemTempLocation			= 23,		// 系统提供的临时目录
		SystemAutorunLocation			= 29		// 系统提供的自启动目录
	}StandardLocation;

public:
	// constructor
	XStandardPath() noexcept;

	// destructor
	virtual ~XStandardPath() noexcept;

public:
	// 用户的主目录
	static XString homeLocation() noexcept;

	// 用户的桌面目录
	static XString desktopLocation() noexcept;

	// 用户的文档目录
	static XString documentsLocation() noexcept;

	// 用户的下载目录
	static XString downloadLocation() noexcept;

public:
	// 应用程序XCC目录名称 - com.libxcc.[app]
	static XString appStandardName() noexcept;

	// 应用程序的配置目录
	static XString appConfigLocation() noexcept;

	// 应用程序的数据目录
	static XString appDataLocation() noexcept;

	// 应用程序的缓存目录
	static XString appCacheLocation() noexcept;

	// 应用程序的临时目录
	static XString appTempLocation() noexcept;

	// 应用程序的Env目录
	static XString appEnvLocation() noexcept;

public:
	// 系统提供的配置目录
	static XString systemConfigLocation() noexcept;

	// 系统提供的数据目录
	static XString systemDataLocation() noexcept;

	// 系统提供的缓存目录
	static XString systemCacheLocation() noexcept;

	// 系统提供的临时目录
	static XString systemTempLocation() noexcept;

	// 系统提供的自启动目录
	static XString systemAutorunLocation() noexcept;

public:
	// 返回指定类型的目录位置
	static XString standardLocations(XStandardPath::StandardLocation _Type) noexcept;
};

#endif
