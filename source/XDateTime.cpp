#include <xcc-core/XDateTime.h>



// 毫秒转TM结构
static struct tm* __xcall__ x_datetime_millisecond_to_tm(x_time_type _Millisecond) noexcept
{
	time_t 		vObjectSec = _Millisecond / XDateTime::millisecond_second;
	return gmtime(&vObjectSec);
}

// 当前毫秒数
static x_time_type __xcall__ x_datetime_current_millisecond_utc() noexcept
{
#if defined(XCC_SYSTEM_ANDROID)
	/*
	struct timeval	tv{};
	gettimeofday(&tv);
	x_int64_t	vMillisecond = tv.tv_sec;
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
}

// 检查相同字符数量
static int __xcall__ xcc_datetime_repeat_count(const XString& _String) noexcept
{
	if(_String.empty())
	{
		return 0;
	}

	auto		vChar = _String[0];
	auto		vCount = _String.size();
	auto		vIndex = XString::size_type(1);
	while(vIndex < vCount)
	{
		if(vChar != _String[vIndex])
		{
			break;
		}
		++vIndex;
	}
	return (int)vIndex;
}



// constructors
XDateTime::XDateTime() noexcept
{
	memberMillisecond = x_datetime_current_millisecond_utc();
}

// constructors Year[1970-9999] Month[1-12] Day[1-31]
XDateTime::XDateTime(x_int32_t _Year, x_int32_t _Month, x_int32_t _Day, x_int32_t _Hour, x_int32_t _Minute, x_int32_t _Second, x_int32_t _Millisecond) noexcept
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



// operator override =
XDateTime& XDateTime::operator = (const XDateTime& _Other) noexcept = default;



// [get] 当前时间日期
XDateTime XDateTime::currentDateTime(const XTimeZone& _TimeZone) noexcept
{
	XDateTime	vDateTime;
	vDateTime.memberMillisecond += _TimeZone.toMillisecond();
	return vDateTime;
}

// [get] 当前毫秒数
x_time_type XDateTime::currentMillisecond(const XTimeZone& _TimeZone) noexcept
{
	return XDateTime::currentDateTime(_TimeZone).toMillisecond();
}

// [get] 当前秒数
x_time_type XDateTime::currentSecond(const XTimeZone& _TimeZone) noexcept
{
	return XDateTime::currentDateTime(_TimeZone).toSecond();
}




// [fmt] 从毫秒数格式化
XDateTime XDateTime::fromMillisecond(x_time_type _Millisecond, const XTimeZone& _TimeZone) noexcept
{
	XDateTime	vDateTime;
	vDateTime.memberMillisecond = _Millisecond + _TimeZone.toMillisecond();
	return vDateTime;
}

// [fmt] 从秒数格式化
XDateTime XDateTime::fromSecond(x_time_type _Second, const XTimeZone& _TimeZone) noexcept
{
	return XDateTime::fromMillisecond(_Second * XDateTime::millisecond_second, _TimeZone);
}



// [get] 是否为闰年
bool XDateTime::yearIsLeap(x_time_type _Year) noexcept
{
	return (_Year % 4 == 0 && _Year % 100 != 0) || (_Year % 400 == 0);
}

// [get] 周数开始偏移
x_time_type XDateTime::weekStartOffset(x_time_type _Year) noexcept
{
	// 一周起始定为周一
	static const x_time_type 	week_offset_1901 = -6;
	if(_Year < 1900)
	{
		return 0;
	}

	x_time_type			vDays = week_offset_1901;
	for(x_time_type _Index = 1901; _Index < _Year; ++_Index)
	{
		vDays += XDateTime::days_in_year;
		vDays += XDateTime::yearIsLeap(_Index) ? 1 : 0;
	}
	auto		vOffset = vDays % XDateTime::days_in_week;
	return vOffset;
}



// [cnv] 转换为毫秒
x_time_type XDateTime::toMillisecond() const noexcept
{
	return memberMillisecond;
}

// [cnv] 转换为秒
x_time_type XDateTime::toSecond() const noexcept
{
	return memberMillisecond / XDateTime::millisecond_second;
}

// [cnv] 转换为字符串
XString XDateTime::toString(const XString& _Format) const noexcept
{
	auto		vMillisecond = this->toMillisecond();
	auto		vTM = x_datetime_millisecond_to_tm(vMillisecond);
	if(vTM == nullptr)
	{
		return "null";
	}
	auto		value_year	= vTM->tm_year + XDateTime::tm_offset_year;
	auto		value_month	= vTM->tm_mon + XDateTime::tm_offset_month;
	auto		value_day	= vTM->tm_mday + XDateTime::tm_offset_day;
	auto		value_hour	= vTM->tm_hour;
	auto		value_min	= vTM->tm_min;
	auto		value_sec	= vTM->tm_sec;
	auto		value_ms	= vMillisecond % XDateTime::millisecond_second;
	auto		value_week	= (vTM->tm_wday + 6) % 7;

	// 检查数据范围
	if(value_month <= 0 || value_month > 12)
	{
		return "null";
	}

	XString		vFormatDT = nullptr;
	auto		vFormatCount = _Format.size();
	auto		vFormatIndex = XString::size_type(0);
	while(vFormatIndex < vFormatCount)
	{
		auto		vChar = _Format[vFormatIndex];
		auto		vSameAmount = xcc_datetime_repeat_count(_Format.mid(vFormatIndex));
		switch (vChar)
		{
			case 'y':
				switch (vSameAmount)
				{
					case 4:
						vFormatDT << XString::format("%04d", value_year).right(vSameAmount);
						vFormatIndex += vSameAmount;
						break;
					case 2:
						vFormatDT << XString::format("%02d", value_year).right(vSameAmount);
						vFormatIndex += vSameAmount;
						break;
					default:
						vFormatDT << vChar;
						vFormatIndex += 1;
						break;
				}
				break;
			case 'M':
				switch (vSameAmount)
				{
					case 4:
						vFormatDT << month_name_long[value_month - 1];
						vFormatIndex += vSameAmount;
						break;
					case 3:
						vFormatDT << month_name_short[value_month - 1];
						vFormatIndex += vSameAmount;
						break;
					case 2:
						vFormatDT << XString::format("%02d", value_month).right(vSameAmount);
						vFormatIndex += vSameAmount;
						break;
					case 1:
						vFormatDT << XString::format("%d", value_month);
						vFormatIndex += vSameAmount;
						break;
					default:
						vFormatDT << vChar;
						vFormatIndex += 1;
						break;
				}
				break;
			case 'd':
				switch (vSameAmount)
				{
					case 4:
						vFormatDT << week_name_long[value_week];
						vFormatIndex += vSameAmount;
						break;
					case 3:
						vFormatDT << week_name_short[value_week];
						vFormatIndex += vSameAmount;
						break;
					case 2:
						vFormatDT << XString::format("%02d", value_day).right(vSameAmount);
						vFormatIndex += vSameAmount;
						break;
					case 1:
						vFormatDT << XString::format("%d", value_day).right(vSameAmount);
						vFormatIndex += vSameAmount;
						break;
					default:
						vFormatDT << vChar;
						vFormatIndex += 1;
						break;
				}
				break;
			case 'h':
				switch (vSameAmount)
				{
					case 2:
						vFormatDT << XString::format("%02d", value_hour);
						vFormatIndex += vSameAmount;
						break;
					case 1:
						vFormatDT << XString::format("%d", value_hour);
						vFormatIndex += vSameAmount;
						break;
					default:
						vFormatDT << vChar;
						vFormatIndex += 1;
						break;
				}
				break;
			case 'm':
				switch (vSameAmount)
				{
					case 2:
						vFormatDT << XString::format("%02d", value_min);
						vFormatIndex += vSameAmount;
						break;
					case 1:
						vFormatDT << XString::format("%d", value_min);
						vFormatIndex += vSameAmount;
						break;
					default:
						vFormatDT << vChar;
						vFormatIndex += 1;
						break;
				}
				break;
			case 's':
				switch (vSameAmount)
				{
					case 2:
						vFormatDT << XString::format("%02d", value_sec);
						vFormatIndex += vSameAmount;
						break;
					case 1:
						vFormatDT << XString::format("%d", value_sec);
						vFormatIndex += vSameAmount;
						break;
					default:
						vFormatDT << vChar;
						vFormatIndex += 1;
						break;
				}
				break;
			case 'z':
				switch (vSameAmount)
				{
					case 3:
					case 2:
					case 1:
						vFormatDT << XString::format("%03d", value_ms).left(vSameAmount);
						vFormatIndex += vSameAmount;
						break;
					default:
						vFormatDT << vChar;
						vFormatIndex += 1;
						break;
				}
				break;
			default:
				vFormatDT << vChar;
				vFormatIndex += 1;
				break;
		}
	}
	return vFormatDT;
}

// [cnv] 转换到年份
x_time_type XDateTime::convertToYear() const noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	if(vTM == nullptr)
	{
		return 0;
	}
	return vTM->tm_year + XDateTime::tm_offset_year;
}

// [cnv] 转换到月份
x_time_type XDateTime::convertToMonth() const noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	if(vTM == nullptr)
	{
		return 0;
	}
	return vTM->tm_mon + XDateTime::tm_offset_month;
}

// [cnv] 转换到天数
x_time_type XDateTime::convertToDay() const noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	if(vTM == nullptr)
	{
		return 0;
	}
	return vTM->tm_yday + XDateTime::tm_offset_day;
}



// [cnv] 截断到年份
XDateTime& XDateTime::truncateToYear() noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	if(vTM)
	{
		*this = XDateTime(vTM->tm_year + XDateTime::tm_offset_year, 0 + XDateTime::tm_offset_month, 1 + XDateTime::tm_offset_day, 0, 0, 0, 0);
	}
	return *this;
}

// [cnv] 截断到月份
XDateTime& XDateTime::truncateToMonth() noexcept
{
	auto		vTM = x_datetime_millisecond_to_tm(memberMillisecond);
	if(vTM)
	{
		*this = XDateTime(vTM->tm_year + XDateTime::tm_offset_year, vTM->tm_mon + XDateTime::tm_offset_month, 1 + XDateTime::tm_offset_day, 0, 0, 0, 0);
	}
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
XDateTime& XDateTime::addYear(x_int32_t _Year) noexcept
{
	auto		vCurrentYear = XDateTime::convertToYear();
	if(_Year > 0)
	{
		for(x_int32_t vIndex = 1; vIndex <= _Year; ++vIndex)
		{
			memberMillisecond += XDateTime::millisecond_year;
			memberMillisecond += XDateTime::yearIsLeap(vCurrentYear + vIndex) ? XDateTime::millisecond_day : 0;
		}
	}
	else if(_Year < 0)
	{
		for(x_int32_t vIndex = -1; vIndex >= _Year; --vIndex)
		{
			memberMillisecond -= XDateTime::millisecond_year;
			memberMillisecond -= XDateTime::yearIsLeap(vCurrentYear + vIndex) ? XDateTime::millisecond_day : 0;
		}
	}
	return *this;
}

// [set] 增加天
XDateTime& XDateTime::addDay(x_int32_t _Day) noexcept
{
	memberMillisecond += (_Day * XDateTime::millisecond_day);
	return *this;
}

// [set] 增加小时
XDateTime& XDateTime::addHour(x_int32_t _Hour) noexcept
{
	memberMillisecond += (_Hour * XDateTime::millisecond_hour);
	return *this;
}

// [set] 增加分
XDateTime& XDateTime::addMinute(x_int32_t _Minute) noexcept
{
	memberMillisecond += (_Minute * XDateTime::millisecond_minute);
	return *this;
}

// [set] 增加秒
XDateTime& XDateTime::addSecond(x_int32_t _Second) noexcept
{
	memberMillisecond += (_Second * XDateTime::millisecond_second);
	return *this;
}

// [set] 增加毫秒
XDateTime& XDateTime::addMillisecond(x_int32_t _Millisecond) noexcept
{
	memberMillisecond += _Millisecond;
	return *this;
}
