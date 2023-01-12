#ifndef			_XCC_CORE_TIME_ZONE_H_
#define			_XCC_CORE_TIME_ZONE_H_

#include <xcc-core/header.h>


// 时区
class _XCOREAPI_ XTimeZone
{
public:
	// http://www.loglogo.com/front/countryCode/
	typedef enum Region
	{
		UTC	= 0,			// 默认为UTC >> 0:00
		A0000	= UTC,			// +00:00
		A0100	= 100,			// +01:00
		A0200	= 200,			// +02:00
		A0300	= 300,			// +03:00
		A0400	= 400,			// +04:00
		A0500	= 500,			// +05:00
		A0600	= 600,			// +06:00
		A0700	= 700,			// +07:00
		A0800	= 800,			// +08:00
		A0900	= 900,			// +09:00
		A1000	= 1000,			// +10:00
		A1100	= 1100,			// +11:00
		A1200	= 1200,			// +12:00
		S0000	= UTC,			// -00:00
		S0100	= -100,			// -01:00
		S0200	= -200,			// -02:00
		S0300	= -300,			// -03:00
		S0400	= -400,			// -04:00
		S0500	= -500,			// -05:00
		S0600	= -600,			// -06:00
		S0700	= -700,			// -07:00
		S0800	= -800,			// -08:00
		S0900	= -900,			// -09:00
		S1000	= -1000,		// -10:00
		S1100	= -1100,		// -11:00
		S1200	= -1200,		// -12:00
		ZH_CN	= A0800,		// 中国，中华人民共和国
		ZH_HK	= A0800,		// 中国，香港特别行政区
	}Region;

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
	// override operator =
	XTimeZone& operator = (XTimeZone::Region _Region) noexcept;

	// override operator =
	XTimeZone& operator = (const XTimeZone& _Other) noexcept;

public:
	// [set] 当前区域
	virtual void setRegion(XTimeZone::Region _Region) noexcept final;

	// [get] 当前区域
	virtual XTimeZone::Region region() const noexcept final;

public:
	// [cnv] 转化为毫秒
	virtual Xcc::time_type toMillisecond() const noexcept;

	// [cnv] 转化为秒
	virtual Xcc::time_type toSecond() const noexcept;

public:
	// [cnv] 转化为毫秒
	static Xcc::time_type toMillisecond(XTimeZone::Region _Region) noexcept;

	// [cnv] 转化为秒
	static Xcc::time_type toSecond(XTimeZone::Region _Region) noexcept;
};

#endif
