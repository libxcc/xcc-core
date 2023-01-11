#ifndef			_XCC_CORE_DATETIME_H_
#define			_XCC_CORE_DATETIME_H_

#include <xcc-core/header.h>
#include <xcc-core/XString.h>
#include <xcc-core/XTimeZone.h>


// 日期时间操作
class _XCOREAPI_ XDateTime
{
public:
	// 秒数定值
	static constexpr Xcc::time_type			second_year		= 31536000;		// 一年的秒数
	static constexpr Xcc::time_type			second_week		= 604800;		// 一周的秒数
	static constexpr Xcc::time_type			second_day		= 86400;		// 一天的秒数
	static constexpr Xcc::time_type			second_hour		= 3600;			// 一小时的秒数
	static constexpr Xcc::time_type			second_minute		= 60;			// 一分钟的秒数

public:
	// 毫秒数定值
	static constexpr Xcc::time_type			millisecond_year	= 31536000000;		// 一年的毫秒数
	static constexpr Xcc::time_type			millisecond_week	= 604800000;		// 一周的毫秒数
	static constexpr Xcc::time_type			millisecond_day		= 86400000;		// 一天的毫秒数
	static constexpr Xcc::time_type			millisecond_hour	= 3600000;		// 一小时的毫秒数
	static constexpr Xcc::time_type			millisecond_minute	= 60000;		// 一分钟的毫秒数
	static constexpr Xcc::time_type			millisecond_second	= 1000;			// 一秒的毫秒数

public:
	// 与TM结构相对偏移
	static constexpr Xcc::int32_t			tm_offset_year		= 1900;			// 偏移的年数
	static constexpr Xcc::int32_t			tm_offset_month		= 1;			// 偏移的月数
	static constexpr Xcc::int32_t			tm_offset_day		= 0;			// 偏移的天数

public:
	// 月份名称
	static constexpr const char*			month_Jan		= "January";		// 一月的名称
	static constexpr const char*			month_Feb		= "February";		// 二月的名称
	static constexpr const char*			month_Mar		= "March";		// 三月的名称
	static constexpr const char*			month_Apr		= "April";		// 四月的名称
	static constexpr const char*			month_May		= "May";		// 五月的名称
	static constexpr const char*			month_Jun		= "June";		// 六月的名称
	static constexpr const char*			month_Jul		= "July";		// 七月的名称
	static constexpr const char*			month_Aug		= "August";		// 八月的名称
	static constexpr const char*			month_Sept		= "September";		// 九月的名称
	static constexpr const char*			month_Oct		= "October";		// 十月的名称
	static constexpr const char*			month_Nov		= "November";		// 十一月的名称
	static constexpr const char*			month_Dec		= "December";		// 十二月的名称
	static constexpr const char*			month_name[]		= {
		month_Jan, month_Feb, month_Mar, month_Apr, month_May, month_Jun, month_Jul, month_Aug, month_Sept, month_Oct, month_Nov, month_Dec, nullptr
	};

public:
	// 年的天数
	static constexpr Xcc::time_type			days_in_year		= 365;			// 一年的天数

	// 月份的天数
	static constexpr Xcc::time_type			days_in_Jan		= 31;			// 一月的天数
	static constexpr Xcc::time_type			days_in_Feb		= 28;			// 二月的天数
	static constexpr Xcc::time_type			days_in_Mar		= 31;			// 三月的天数
	static constexpr Xcc::time_type			days_in_Apr		= 30;			// 四月的天数
	static constexpr Xcc::time_type			days_in_May		= 31;			// 五月的天数
	static constexpr Xcc::time_type			days_in_Jun		= 30;			// 六月的天数
	static constexpr Xcc::time_type			days_in_Jul		= 31;			// 七月的天数
	static constexpr Xcc::time_type			days_in_Aug		= 31;			// 八月的天数
	static constexpr Xcc::time_type			days_in_Sept		= 30;			// 九月的天数
	static constexpr Xcc::time_type			days_in_Oct		= 31;			// 十月的天数
	static constexpr Xcc::time_type			days_in_Nov		= 30;			// 十一月的天数
	static constexpr Xcc::time_type			days_in_Dec		= 31;			// 十二月的天数
	static constexpr Xcc::time_type			days_month[]		= {
		days_in_Jan, days_in_Feb, days_in_Mar, days_in_Apr, days_in_May, days_in_Jun, days_in_Jul, days_in_Aug, days_in_Sept, days_in_Oct, days_in_Nov, days_in_Dec, 0
	};

	// 周的天数
	static constexpr Xcc::time_type			days_in_week		= 7;			// 一周的天数

public:
	// 星期名称
	static constexpr const char*			week_Mon		= "Monday";		// 周一的名称
	static constexpr const char*			week_Tue		= "Tuesday";		// 周二的名称
	static constexpr const char*			week_Wed		= "Wednesday";		// 周三的名称
	static constexpr const char*			week_Thur		= "Thursday";		// 周四的名称
	static constexpr const char*			week_Fri		= "Friday";		// 周五的名称
	static constexpr const char*			week_Sat		= "Saturday";		// 周六的名称
	static constexpr const char*			week_Sun		= "Sunday";		// 周日的名称
	static constexpr const char*			week_name[]		= {
		week_Mon, week_Tue, week_Wed, week_Thur, week_Fri, week_Sat, week_Sun, nullptr
	};

private:
	// 当前毫秒数
	Xcc::time_type					memberMillisecond;

public:
	// constructors
	XDateTime() noexcept;

	// constructors Year[1900-9999] Month[1-12] Day[1-31]
	XDateTime(Xcc::int32_t _Year, Xcc::int32_t _Month, Xcc::int32_t _Day, Xcc::int32_t _Hour, Xcc::int32_t _Minute, Xcc::int32_t _Second, Xcc::int32_t _Millisecond) noexcept;

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
	static Xcc::time_type currentMillisecond(const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

	// 当前秒数
	static Xcc::time_type currentSecond(const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

public:
	// 从毫秒数格式化
	static XDateTime fromMillisecond(Xcc::time_type _Millisecond, const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

	// 从秒数格式化
	static XDateTime fromSecond(Xcc::time_type _Second, const XTimeZone& _TimeZone = XTimeZone::UTC) noexcept;

public:
	// 是否为闰年
	static bool yearIsLeap(Xcc::time_type _Year) noexcept;

	// 周数开始偏移
	static Xcc::time_type weekStartOffset(Xcc::time_type _Year) noexcept;

public:
	// [cnv] 转换为毫秒
	virtual Xcc::time_type toMillisecond() const noexcept final;

	// [cnv] 转换为秒
	virtual Xcc::time_type toSecond() const noexcept final;

	// [cnv] 转换为字符串
	virtual XString toString(const XString& _Format) const noexcept final;

public:
	// [cnv] 转换到年份
	virtual Xcc::time_type convertToYear() const noexcept final;

	// [cnv] 转换到月份
	virtual Xcc::time_type convertToMonth() const noexcept final;

	// [cnv] 转换到天数
	virtual Xcc::time_type convertToDay() const noexcept final;

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
	virtual XDateTime& addYear(Xcc::int32_t _Year) noexcept final;

	// [set] 增加天
	virtual XDateTime& addDay(Xcc::int32_t _Day) noexcept final;

	// [set] 增加小时
	virtual XDateTime& addHour(Xcc::int32_t _Hour) noexcept final;

	// [set] 增加分
	virtual XDateTime& addMinute(Xcc::int32_t _Minute) noexcept final;

	// [set] 增加秒
	virtual XDateTime& addSecond(Xcc::int32_t _Second) noexcept final;

	// [set] 增加毫秒
	virtual XDateTime& addMillisecond(Xcc::int32_t _Millisecond) noexcept final;
};

#endif
