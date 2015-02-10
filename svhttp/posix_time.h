// test_c1.cpp : Defines the entry point for the console application.
//
#ifndef _time_posix_hpp_
#define _time_posix_hpp_

#include <string>
#include <sstream>
#include <time.h>

#if _WIN32
#include <windows.h>
#endif

namespace svhttp
{

class date_time
{
public:
	date_time();
	~date_time();

	// [yyyy-mm-dd hh:mi:ss.ms] msec: 毫秒级 (true: 返回时间字符精确到毫秒)
	std::string to_string(bool msec = false);
	// [yyyy-mm-dd] 获取日期字符串
	std::string to_date_string();
	// [hh:mi:ss.ms] msec: 毫秒级 (true: 返回时间字符精确到毫秒)
	std::string to_time_string(bool msec = false);

	// set operator
	void set_year(const unsigned int& val);
	void set_month(const unsigned int& val);
	void set_day(const unsigned int& val);
	void set_hour(const unsigned int& val);
	void set_minuter(const unsigned int& val);
	void set_seconds(const unsigned int& val);
	void set_milli_seconds(const unsigned int& val);
	// get operator
	unsigned int get_year();
	unsigned int get_month();
	unsigned int get_day();
	unsigned int get_hour();
	unsigned int get_minuter();
	unsigned int get_seconds();
	unsigned int get_milli_seconds();

	// 获取date_time日期对应的星期x <nweekday: days since Sunday - [0,6] >
	std::string get_week_day_abbr();
	// 获取date_time日期对应的月份 英文缩写
	std::string get_month_abbr();
	// days since January 1 - [0,365]
	unsigned int get_year_day();
	// add idays (+/-)
	void add_any_days(int idays);

	/**
	 *	static operation
	 */
	// 计算比较两个日期相差的年/月/日 <type：{0: 日; 1: 月;  2: 年}>
	static int get_diff_date( date_time date1, date_time date2,const unsigned int& itype = 0);
	//  日期比较 (return 1:0:-1)
	static int compare_date(const date_time& date1, const date_time& date2);

	// 蔡勒公式: 计算指定日期对应的星期几 since Sunday - [0,6]
	static unsigned int get_day_of_week(const unsigned int& iyear, const unsigned int& imonth, const unsigned int& iday );
	// 计算某月的天数
	static unsigned int get_day_of_month(const unsigned int& iyear, const unsigned int& imonth);
	// days since January 1 - [0,365]
	static unsigned int get_day_of_year(const unsigned int& iyear, const unsigned int& imonth, const unsigned int& iday);

	/**
	 *	operator Override
	 */
	friend date_time operator + (date_time& a, const int& v)
	{
		date_time d = a;
		d.add_any_days(v);
		return d;
	}
	friend date_time operator + (const int& v, date_time& a)
	{
		date_time d = a;
		d.add_any_days(v);
		return d;
	}
	friend date_time operator - (date_time& a, const int& v)
	{
		date_time d = a;
		d.add_any_days(-v);
		return d;
	}
	friend date_time operator - (const int& v, date_time& a)
	{
		date_time d = a;
		d.add_any_days(-v);
		return d;
	}

	friend date_time& operator += (date_time& a, const int& v)
	{
		a.add_any_days(v);
		return a;
	}
	friend date_time& operator -= (date_time& a, const int& v)
	{
		a.add_any_days(-v);
		return a;
	}
	friend date_time& operator ++ (date_time& a)
	{
		a.add_any_days(1);
		return a;
	}
	friend date_time& operator -- (date_time& a)
	{
		a.add_any_days(-1);
		return a;
	}

	/**
	 *	日期大小比较操作符
	 */
	friend bool operator > (const date_time& a,const date_time& b)
	{
		return compare_date(a, b) > 0;
	}
 	friend bool operator >=(const date_time& a,const date_time& b)
 	{
 		return compare_date(a, b) >= 0;
 	}
 	friend bool operator < (const date_time& a,const date_time& b)
 	{
 		return compare_date(a, b) < 0;
 	}
 	friend bool operator <=(const date_time& a,const date_time& b)
 	{
 		return compare_date(a, b) <= 0;
 	}
 	friend bool operator ==(const date_time& a,const date_time& b)
 	{
 		return compare_date(a, b) == 0;
 	}
 	friend bool operator !=(const date_time& a,const date_time& b)
 	{
 		return compare_date(a, b) != 0;
 	}

private:
	unsigned int nyear;
	unsigned int nmonth;
	unsigned int nday;

	unsigned int nhour;
	unsigned int nminuter;
	unsigned int nseconds;
	unsigned int nmilliseconds;

	unsigned int nweekday;	// day of week.
	unsigned int nyearday;	// day of year.

};

} // @end namespace svhttp


#endif
