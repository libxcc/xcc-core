#ifndef			_XCC_CORE_TIME_ZONE_H_
#define			_XCC_CORE_TIME_ZONE_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// 时区
class _XCOREAPI_ XTimeZone
{
public:
	// http://www.loglogo.com/front/countryCode/
	typedef enum Region
	{
		UTC		= 0,			// 默认为UTC >> 0:00
		A0000		= UTC,			// +00:00
		A0100		= 100,			// +01:00
		A0200		= 200,			// +02:00
		A0300		= 300,			// +03:00
		A0400		= 400,			// +04:00
		A0500		= 500,			// +05:00
		A0600		= 600,			// +06:00
		A0700		= 700,			// +07:00
		A0800		= 800,			// +08:00
		A0900		= 900,			// +09:00
		A1000		= 1000,			// +10:00
		A1100		= 1100,			// +11:00
		A1200		= 1200,			// +12:00
		S0000		= UTC,			// -00:00
		S0100		= -100,			// -01:00
		S0200		= -200,			// -02:00
		S0300		= -300,			// -03:00
		S0400		= -400,			// -04:00
		S0500		= -500,			// -05:00
		S0600		= -600,			// -06:00
		S0700		= -700,			// -07:00
		S0800		= -800,			// -08:00
		S0900		= -900,			// -09:00
		S1000		= -1000,		// -10:00
		S1100		= -1100,		// -11:00
		S1200		= -1200,		// -12:00
		LOCAL		= -1,			// 本地时区
	}Region;

public:
	// 时区列表
	static constexpr const char*			zone_name_link[]	= {
		"+0000", "+0100", "+0200", "+0300", "+0400", "+0500", "+0600", "+0700", "+0800", "+0900", "+1000", "+1100", "+1200",
		"-0000", "-0100", "-0200", "-0300", "-0400", "-0500", "-0600", "-0700", "-0800", "-0900", "-1000", "-1100", "-1200",
		nullptr
	};

	static constexpr const char*			zone_name_div[]	= {
		"+00:00", "+01:00", "+02:00", "+03:00", "+04:00", "+05:00", "+06:00", "+07:00", "+08:00", "+09:00", "+10:00", "+11:00", "+12:00",
		"-00:00", "-01:00", "-02:00", "-03:00", "-04:00", "-05:00", "-06:00", "-07:00", "-08:00", "-09:00", "-10:00", "-11:00", "-12:00",
		nullptr
	};

	static constexpr Region				zone_name_enum[]	= {
		A0000, A0100, A0200, A0300, A0400, A0500, A0600, A0700, A0800, A0900, A1000, A1100, A1200,
		S0000, S0100, S0200, S0300, S0400, S0500, S0600, S0700, S0800, S0900, S1000, S1100, S1200,
		LOCAL
	};

private:
	// 区域
	Region					memberRegion;

public:
	// constructors
	XTimeZone() noexcept;

	// constructors
	XTimeZone(XTimeZone::Region _Region) noexcept; // NOLINT(google-explicit-constructor)

	// constructors
	XTimeZone(const XTimeZone& _Other) noexcept;

	// destructor
	virtual ~XTimeZone() noexcept;

public:
	// operator override =
	XTimeZone& operator = (XTimeZone::Region _Region) noexcept;

	// operator override =
	XTimeZone& operator = (const XTimeZone& _Other) noexcept;

public:
	// [fmt] 从字符串格式化 (-0800)(+0800)
	static XTimeZone fromString(const XString& _Format) noexcept;

	// [fmt] 从字符串格式化 (-0800)(+0800)
	virtual XString toString() const noexcept final;

public:
	// [set] 当前区域
	virtual void region(XTimeZone::Region _Region) noexcept final;

	// [get] 当前区域
	virtual XTimeZone::Region region() const noexcept final;

public:
	// [cnv] 转化为毫秒
	virtual x_time_type toMillisecond() const noexcept;

	// [cnv] 转化为秒
	virtual x_time_type toSecond() const noexcept;

public:
	// [cnv] 转化为毫秒
	static x_time_type toMillisecond(XTimeZone::Region _Region) noexcept;

	// [cnv] 转化为秒
	static x_time_type toSecond(XTimeZone::Region _Region) noexcept;
};

#endif
