#include <xcc-core/XTimeZone.h>
#include <xcc-core/XDateTime.h>


// constructors
XTimeZone::XTimeZone() noexcept
{
	memberRegion = XTimeZone::UTC;
}

// constructors
XTimeZone::XTimeZone(XTimeZone::Region _Region) noexcept
{
	memberRegion = _Region;
}

// constructors
XTimeZone::XTimeZone(const XTimeZone& _Other) noexcept = default;

// destructor
XTimeZone::~XTimeZone() noexcept = default;



// operator override =
XTimeZone& XTimeZone::operator = (XTimeZone::Region _Region) noexcept
{
	memberRegion = _Region;
	return *this;
}

// operator override =
XTimeZone& XTimeZone::operator = (const XTimeZone& _Other) noexcept = default;



// [fmt] 从字符串格式化 (-0800)(+0800)
XTimeZone XTimeZone::fromString(const XString& _Format) noexcept
{
	// 格式化数据
	if(nullptr == _Format)
	{
		return XTimeZone::UTC;
	}
	auto		vFormat = _Format;
	if('+' != vFormat[0] && '-' != vFormat[0])
	{
		vFormat.insert(0, '+');
	}

	// 检查是否存在分割符
	if(XString::npos == vFormat.find(":"))
	{
		for(x_size_t vIndex = 0; zone_name_link[vIndex]; ++vIndex)
		{
			if(vFormat == zone_name_link[vIndex])
			{
				return zone_name_enum[vIndex];
			}
		}
	}
	else
	{
		for(x_size_t vIndex = 0; zone_name_div[vIndex]; ++vIndex)
		{
			if(vFormat == zone_name_div[vIndex])
			{
				return zone_name_enum[vIndex];
			}
		}
	}
	return XTimeZone::UTC;
}

// [fmt] 从字符串格式化 (-0800)(+0800)
XString XTimeZone::toString() const noexcept
{
	for(x_size_t vIndex = 0; LOCAL != zone_name_enum[vIndex]; ++vIndex)
	{
		if(memberRegion == zone_name_enum[vIndex])
		{
			return zone_name_link[vIndex];
		}
	}
	return zone_name_link[0];
}



// [set] 当前区域
void XTimeZone::region(XTimeZone::Region _Region) noexcept
{
	memberRegion = _Region;
}

// [get] 当前区域
XTimeZone::Region XTimeZone::region() const noexcept
{
	return memberRegion;
}



// [cnv] 转化为毫秒
x_time_type XTimeZone::toMillisecond() const noexcept
{
	return XTimeZone::toMillisecond(memberRegion);
}

// [cnv] 转化为秒
x_time_type XTimeZone::toSecond() const noexcept
{
	return XTimeZone::toSecond(memberRegion);
}



// [cnv] 转化为毫秒
x_time_type XTimeZone::toMillisecond(XTimeZone::Region _Region) noexcept
{
	return XTimeZone::toSecond(_Region) * 1000;
}

// [cnv] 转化为秒
x_time_type XTimeZone::toSecond(XTimeZone::Region _Region) noexcept
{
	// 特定时区转换
	if(XTimeZone::LOCAL == _Region)
	{
		auto		vTimeNow = time(nullptr);
		auto		vOffsetSeconds = difftime(mktime(localtime(&vTimeNow)), mktime(gmtime(&vTimeNow)));
		return (x_time_type)vOffsetSeconds;
	}

	// 其它时区转换
	auto		vHour = _Region / 100;
	auto		vMinute = _Region % 100;

	if(vHour == 0)
	{
		if(vMinute == 0)
		{
			return 0;
		}
		else if(vMinute > 0)
		{
			return vMinute * XDateTime::second_minute;
		}
		else
		{
			return -(vMinute * XDateTime::second_minute);
		}
	}
	else if(vHour > 0)
	{
		return (vHour * XDateTime::second_hour) + (vMinute * XDateTime::second_minute);
	}
	else
	{
		return -((vHour * XDateTime::second_hour) + (vMinute * XDateTime::second_minute));
	}
}
