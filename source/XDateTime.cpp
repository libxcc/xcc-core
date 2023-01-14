#include <xcc-core/XDateTime.h>



// 毫秒转TM结构
static struct tm* __xcall__ x_datetime_millisecond_to_tm(xcc::time_type _Millisecond) noexcept
{
	time_t 		vObjectSec = _Millisecond / XDateTime::millisecond_second;
	return gmtime(&vObjectSec);
}

// 当前毫秒数
static xcc::time_type __xcall__ x_datetime_current_millisecond_utc() noexcept
{
#if defined(XCC_SYSTEM_ANDROID)
	/*
	struct timeval	tv{};
	gettimeofday(&tv);
	std::int64_t	vMillisecond = tv.tv_sec;
	vMillisecond *= 1000;
	vMillisecond += tv.tv_usec / 1000;
	return vMillisecond;
	*/
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
#else
	struct timeb		vRawTime{};
	ftime(&vRawTime);
	return vRawTime.time * 1000LL + vRawTime.millitm;
#endif
};



// constructors
XDateTime::XDateTime() noexcept
{
	memberMillisecond = x_datetime_current_millisecond_utc();
}

// constructors Year[1970-9999] Month[1-12] Day[1-31]
XDateTime::XDateTime(xcc::int32_t _Year, xcc::int32_t _Month, xcc::int32_t _Day, xcc::int32_t _Hour, xcc::int32_t _Minute, xcc::int32_t _Second, xcc::int32_t _Millisecond) noexcept
{
	struct tm	vTime{};
	vTime.tm_year = _Year - XDateTime::tm_offset_year;
	vTime.tm_mon = _Month - XDateTime::tm_offset_month;
	vTime.tm_mday = _Day - XDateTime::tm_offset_day;
	vTime.tm_hour = _Hour;
	vTime.tm_min = _Minute;
	vTime.tm_sec = _Second;
	auto		vSecondTM = mktime(&vTime);

	memberMillisecond = vSecondTM * XDateTime::millisecond_second;
	memberMillisecond += _Millisecond;
}

// constructors
XDateTime::XDateTime(const XDateTime& _Info) noexcept = default;

// destructor
XDateTime::~XDateTime() noexcept = default;



// override operator =
XDateTime& XDateTime::operator = (const XDateTime& _Other) noexcept = default;



// 当前时间日期
XDateTime XDateTime::currentDateTime(const XTimeZone& _TimeZone) noexcept
{
	XDateTime	vDateTime;
	vDateTime.memberMillisecond += _TimeZone.toMillisecond();
	return vDateTime;
}

// 当前毫秒数
xcc::time_type XDateTime::currentMillisecond(const XTimeZone& _TimeZone) noexcept
{
	return XDateTime::currentDateTime(_TimeZone).toMillisecond();
}

// 当前秒数
xcc::time_type XDateTime::currentSecond(const XTimeZone& _TimeZone) noexcept
{
	return XDateTime::currentDateTime(_TimeZone).toSecond();
}




// 从毫秒数格式化
XDateTime XDateTime::fromMillisecond(xcc::time_type _Millisecond, const XTimeZone& _TimeZone) noexcept
{
	XDateTime	vDateTime;
	vDateTime.memberMillisecond = _Millisecond + _TimeZone.toMillisecond();
	return vDateTime;
}

// 从秒数格式化
XDateTime XDateTime::fromSecond(xcc::time_type _Second, const XTimeZone& _TimeZone) noexcept
{
	return XDateTime::fromMillisecond(_Second * XDateTime::millisecond_second, _TimeZone);
}



// 是否为闰年
bool XDateTime::yearIsLeap(xcc::time_type _Year) noexcept
{
	return (_Year % 4 == 0 && _Year % 100 != 0) || (_Year % 400 == 0);
}

// 周数开始偏移
xcc::time_type XDateTime::weekStartOffset(xcc::time_type _Year) noexcept
{
	// 一周起始定为周一
	static const xcc::time_type 	week_offset_1901 = -6;
	if(_Year < 1901)
	{
		return 0;
	}

	xcc::time_type			vDays = week_offset_1901;
	for(xcc::time_type _Index = 1901; _Index < _Year; ++_Index)
	{
		vDays += XDateTime::days_in_year;
		vDays += XDateTime::yearIsLeap(_Index) ? 1 : 0;
	}
	auto		vOffset = vDays % XDateTime::days_in_week;
	return vOffset;
}



// [cnv] 转换为毫秒
xcc::time_type XDateTime::toMillisecond() const noexcept
{
	return memberMillisecond;
}

// [cnv] 转换为秒
xcc::time_type XDateTime::toSecond() const noexcept
{
	return memberMillisecond / XDateTime::millisecond_second;
}

// [cnv] 转换为字符串
XString XDateTime::toString(const XString& _Format) const noexcept
{
	XCC_UNUSED(_Format);

	auto		vMillisecond = this->toMillisecond();
	auto		vTM = x_datetime_millisecond_to_tm(vMillisecond);

	return XString::format("%04d-%02d-%02dT%02d:%02d:%02d.%03dZ",
			       vTM->tm_year + XDateTime::tm_offset_year,
			       vTM->tm_mon + XDateTime::tm_offset_month,
			       vTM->tm_mday + XDateTime::tm_offset_day,
			       vTM->tm_hour, vTM->tm_min, vTM->tm_sec, vMillisecond % XDateTime::millisecond_second);
}

// [cnv] 转换到年份
xcc::time_type XDateTime::convertToYear() const noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	return vTM->tm_year + XDateTime::tm_offset_year;
}

// [cnv] 转换到月份
xcc::time_type XDateTime::convertToMonth() const noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	return vTM->tm_mon + XDateTime::tm_offset_month;
}

// [cnv] 转换到天数
xcc::time_type XDateTime::convertToDay() const noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	return vTM->tm_yday + XDateTime::tm_offset_day;
}



// [cnv] 截断到年份
XDateTime& XDateTime::truncateToYear() noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	*this = XDateTime(vTM->tm_year + XDateTime::tm_offset_year, 0 + XDateTime::tm_offset_month, 1 + XDateTime::tm_offset_day, 0, 0, 0, 0);
	return *this;
}

// [cnv] 截断到月份
XDateTime& XDateTime::truncateToMonth() noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	*this = XDateTime(vTM->tm_year + XDateTime::tm_offset_year, vTM->tm_mon + XDateTime::tm_offset_month, 1 + XDateTime::tm_offset_day, 0, 0, 0, 0);
	return *this;
}

// [cnv] 截断到天
XDateTime& XDateTime::truncateToDay() noexcept
{
	memberMillisecond -= (memberMillisecond % XDateTime::millisecond_day);
	return *this;
}

// [cnv] 截断到小时
XDateTime& XDateTime::truncateToHour() noexcept
{
	memberMillisecond -= (memberMillisecond % XDateTime::millisecond_hour);
	return *this;
}

// [cnv] 截断到分
XDateTime& XDateTime::truncateToMinute() noexcept
{
	memberMillisecond -= (memberMillisecond % XDateTime::millisecond_minute);
	return *this;
}

// [cnv] 截断到秒
XDateTime& XDateTime::truncateToSecond() noexcept
{
	memberMillisecond -= (memberMillisecond % XDateTime::millisecond_second);
	return *this;
}



// [set] 增加年份
XDateTime& XDateTime::addYear(xcc::int32_t _Year) noexcept
{
	auto		vCurrentYear = XDateTime::convertToYear();
	if(_Year > 0)
	{
		for(xcc::int32_t vIndex = 1; vIndex <= _Year; ++vIndex)
		{
			memberMillisecond += XDateTime::millisecond_year;
			memberMillisecond += XDateTime::yearIsLeap(vCurrentYear + vIndex) ? XDateTime::millisecond_day : 0;
		}
	}
	else if(_Year < 0)
	{
		for(xcc::int32_t vIndex = -1; vIndex >= _Year; --vIndex)
		{
			memberMillisecond -= XDateTime::millisecond_year;
			memberMillisecond -= XDateTime::yearIsLeap(vCurrentYear + vIndex) ? XDateTime::millisecond_day : 0;
		}
	}
	return *this;
}

// [set] 增加天
XDateTime& XDateTime::addDay(xcc::int32_t _Day) noexcept
{
	memberMillisecond += (_Day * XDateTime::millisecond_day);
	return *this;
}

// [set] 增加小时
XDateTime& XDateTime::addHour(xcc::int32_t _Hour) noexcept
{
	memberMillisecond += (_Hour * XDateTime::millisecond_hour);
	return *this;
}

// [set] 增加分
XDateTime& XDateTime::addMinute(xcc::int32_t _Minute) noexcept
{
	memberMillisecond += (_Minute * XDateTime::millisecond_minute);
	return *this;
}

// [set] 增加秒
XDateTime& XDateTime::addSecond(xcc::int32_t _Second) noexcept
{
	memberMillisecond += (_Second * XDateTime::millisecond_second);
	return *this;
}

// [set] 增加毫秒
XDateTime& XDateTime::addMillisecond(xcc::int32_t _Millisecond) noexcept
{
	memberMillisecond += _Millisecond;
	return *this;
}
