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

	// [yyyy-mm-dd hh:mi:ss.ms] msec: ���뼶 (true: ����ʱ���ַ���ȷ������)
	std::string to_string(bool msec = false);
	// [yyyy-mm-dd] ��ȡ�����ַ���
	std::string to_date_string();
	// [hh:mi:ss.ms] msec: ���뼶 (true: ����ʱ���ַ���ȷ������)
	std::string to_time_string(bool msec = false);

	// ��ȡdate_time���ڶ�Ӧ������x <nweekday: days since Sunday - [0,6] >
	std::string get_week_day();
	// ��ȡdate_time���ڶ�Ӧ���·� Ӣ����д
	std::string get_month();
	// days since January 1 - [0,365]
	unsigned int get_year_day();
	// add idays (+/-)
	void add_any_days(int idays);

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
	 *	���ڴ�С�Ƚϲ�����
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

	/**
	 *	static operation
	 */
	// �Ƚ���������������/��/�� <type��{0: ��; 1: ��;  2: ��}>
	static int get_diff_date( date_time date1, date_time date2,const unsigned int& itype = 0);
	//  ���ڱȽ�
	static int compare_date(const date_time& date1, const date_time& date2);

	// ���չ�ʽ: ����ָ�����ڶ�Ӧ�����ڼ� since Sunday - [0,6]
	static unsigned int get_day_of_week(const unsigned int& iyear, const unsigned int& imonth, const unsigned int& iday );
	// ����ĳ�µ�����
	static unsigned int get_day_of_month(const unsigned int& iyear, const unsigned int& imonth);
	// days since January 1 - [0,365]
	static unsigned int get_day_of_year(const unsigned int& iyear, const unsigned int& imonth, const unsigned int& iday);

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
