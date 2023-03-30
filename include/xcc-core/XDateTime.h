#ifndef			_XCC_CORE_DATETIME_H_
#define			_XCC_CORE_DATETIME_H_

#include <xcc-core/header.h>
#include <xcc-core/XString.h>
#include <xcc-core/XTimeZone.h>


// 日期时间操作
class _XCOREAPI_ XDateTime
{
public:
	// 与TM结构相对偏移
	static constexpr x_int32_t			tm_offset_year		= 1900;			// 偏移的年数
	static constexpr x_int32_t			tm_offset_month		= 1;			// 偏移的月数
	static constexpr x_int32_t			tm_offset_day		= 0;			// 偏移的天数

public:
	// 秒数定值
	static constexpr x_time_type			second_year		= 31536000;		// 一年的秒数
	static constexpr x_time_type			second_week		= 604800;		// 一周的秒数
	static constexpr x_time_type			second_day		= 86400;		// 一天的秒数
	static constexpr x_time_type			second_hour		= 3600;			// 一小时的秒数
	static constexpr x_time_type			second_minute		= 60;			// 一分钟的秒数

public:
	// 毫秒数定值
	static constexpr x_time_type			millisecond_year	= 31536000000;		// 一年的毫秒数
	static constexpr x_time_type			millisecond_week	= 604800000;		// 一周的毫秒数
	static constexpr x_time_type			millisecond_day		= 86400000;		// 一天的毫秒数
	static constexpr x_time_type			millisecond_hour	= 3600000;		// 一小时的毫秒数
	static constexpr x_time_type			millisecond_minute	= 60000;		// 一分钟的毫秒数
	static constexpr x_time_type			millisecond_second	= 1000;			// 一秒的毫秒数

public:
	// 天数定值
	static constexpr x_time_type			days_in_year		= 365;			// 一年的天数
	static constexpr x_time_type			days_in_month[]		= {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0
	};												// 月份的天数
	static constexpr x_time_type			days_in_week		= 7;			// 一周的天数

public:
	// 月份名称
	static constexpr const char*			month_name_long[]	= {
		"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", nullptr
	};												// 长名称
	static constexpr const char*			month_name_short[]	= {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec", nullptr
	};												// 短名称

public:
	// 星期名称
	static constexpr const char*			week_name_long[]	= {
		"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday", nullptr
	};												// 长名称
	static constexpr const char*			week_name_short[]	= {
		"Mon", "Tue", "Wed", "Thur", "Fri", "Sat", "Sun", nullptr
	};												// 短名称

private:
	// 当前毫秒数
	x_time_type					memberMillisecond;

public:
	// constructors
	XDateTime() noexcept;

	// constructors Year[1900-9999] Month[1-12] Day[1-31]
	XDateTime(x_int32_t _Year, x_int32_t _Month, x_int32_t _Day, x_int32_t _Hour, x_int32_t _Minute, x_int32_t _Second, x_int32_t _Millisecond) noexcept;

	// constructors
	XDateTime(const XDateTime& _Info) noexcept;

	// destructor
	virtual ~XDateTime() noexcept;

public:
	// override operator =
	XDateTime& operator = (const XDateTime& _Other) noexcept;

public:
	// 当前时间日期
	static XDateTime currentDateTime(const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

	// 当前毫秒数
	static x_time_type currentMillisecond(const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

	// 当前秒数
	static x_time_type currentSecond(const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

public:
	// 从毫秒数格式化
	static XDateTime fromMillisecond(x_time_type _Millisecond, const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

	// 从秒数格式化
	static XDateTime fromSecond(x_time_type _Second, const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

public:
	// 是否为闰年
	static bool yearIsLeap(x_time_type _Year) noexcept;

	// 周数开始偏移
	static x_time_type weekStartOffset(x_time_type _Year) noexcept;

public:
	// [cnv] 转换为毫秒
	virtual x_time_type toMillisecond() const noexcept final;

	// [cnv] 转换为秒
	virtual x_time_type toSecond() const noexcept final;

	// [cnv] 转换为字符串
	virtual XString toString(const XString& _Format) const noexcept final;

public:
	// [cnv] 转换到年份
	virtual x_time_type convertToYear() const noexcept final;

	// [cnv] 转换到月份
	virtual x_time_type convertToMonth() const noexcept final;

	// [cnv] 转换到天数
	virtual x_time_type convertToDay() const noexcept final;

public:
	// [cnv] 截断到年份
	virtual XDateTime& truncateToYear() noexcept final;

	// [cnv] 截断到月份
	virtual XDateTime& truncateToMonth() noexcept final;

	// [cnv] 截断到天
	virtual XDateTime& truncateToDay() noexcept final;

	// [cnv] 截断到小时
	virtual XDateTime& truncateToHour() noexcept final;

	// [cnv] 截断到分
	virtual XDateTime& truncateToMinute() noexcept final;

	// [cnv] 截断到秒
	virtual XDateTime& truncateToSecond() noexcept final;

public:
	// [set] 增加年份
	virtual XDateTime& addYear(x_int32_t _Year) noexcept final;

	// [set] 增加天
	virtual XDateTime& addDay(x_int32_t _Day) noexcept final;

	// [set] 增加小时
	virtual XDateTime& addHour(x_int32_t _Hour) noexcept final;

	// [set] 增加分
	virtual XDateTime& addMinute(x_int32_t _Minute) noexcept final;

	// [set] 增加秒
	virtual XDateTime& addSecond(x_int32_t _Second) noexcept final;

	// [set] 增加毫秒
	virtual XDateTime& addMillisecond(x_int32_t _Millisecond) noexcept final;
};

#endif
