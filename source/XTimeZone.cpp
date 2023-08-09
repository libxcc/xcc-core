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



// [set] 当前区域
void XTimeZone::setRegion(XTimeZone::Region _Region) noexcept
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
