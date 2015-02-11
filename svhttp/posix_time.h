#ifndef _time_posix_hpp_svhttp_
#define _time_posix_hpp_svhttp_

/**	Copyright (c) 2015-20XX jianyaoy. all rights reserved.
 *	datetime deal class. 
 *	Author: jianyaoy@gmail.com
 *  $Date:	2015-02-11 00:00:00 +0800 $
 */

#include <string>

namespace svhttp
{

class date_time
{
public:
	// constructor
	date_time();
	date_time(std::string date_str);
	date_time(const char* szdate_str);
	date_time(long sec);	// ��������Date���� <1970��1��1�����������>
	// destructor
	~date_time();

	// locale ʱ��
	// [yyyy-mm-dd hh:mi:ss.ms] msec: ���뼶 (true: ����ʱ���ַ���ȷ������)
	std::string to_string(bool msec = false);
	// [yyyy-mm-dd] ��ȡ�����ַ���
	std::string to_date_string();
	// [hh:mi:ss.ms] msec: ���뼶 (true: ����ʱ���ַ���ȷ������)
	std::string to_time_string(bool msec = false);

	// utc ʱ��
	// utcʱ�����ں�ʱ�� [yyyy-mm-dd hh:mi:ss.ms] msec: ���뼶 (true: ����ʱ���ַ���ȷ������)
	std::string to_utc_string(bool msec = false);
	// utc���� [yyyy-mm-dd] ��ȡ�����ַ���
	std::string to_utc_date_string();
	// utcʱ�� [hh:mi:ss.ms] msec: ���뼶 (true: ����ʱ���ַ���ȷ������)
	std::string to_utc_time_string(bool msec = false);

	// ����1970��1��1�������������
	long get_time();
	// ���õ�ǰdatetime�еĺ��� <tip: ��ǰֻ����windows��Ч>
	void reset_milliseconds();

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

	// ��ȡdate_time���ڶ�Ӧ������x Ӣ����д <nweekday: days since Sunday - [0,6] >
	std::string get_week_day_abbr();
	// ��ȡdate_time���ڶ�Ӧ���·� Ӣ����д
	std::string get_month_abbr();
	// days since January 1 - [0,365]
	unsigned int get_year_day();
	// add idays (+/-)
	void add_any_days(int idays);

	/**
	 *	static operation
	 */
	// ����Ƚ���������������/��/�� <type��{0: ��; 1: ��;  2: ��}>
	static int get_diff_date( date_time date1, date_time date2,const unsigned int& itype = 0);
	//  ���ڱȽ� (return 1:0:-1)
	static int compare_date(const date_time& date1, const date_time& date2);

	// ʹ�ò��չ�ʽ: ����ָ�����ڶ�Ӧ������x since Sunday - [0,6]
	static unsigned int get_day_of_week(const unsigned int& iyear, const unsigned int& imonth, const unsigned int& iday );
	// ����ĳ�µ�����
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
	// ����������֮��(�����������)
	int operator - (date_time& b)
	{
		return get_diff_date(b, *this);
	}

	date_time& operator += ( const int& v)
	{
		add_any_days(v);
		return *this;
	}
	date_time& operator -= ( const int& v)
	{
		add_any_days(-v);
		return *this;
	}
	date_time& operator ++ ()
	{
		add_any_days(1);
		return *this;
	}
	date_time& operator ++ (int)
	{
		add_any_days(1);
		return *this;
	}
	date_time& operator -- ()
	{
		add_any_days(-1);
		return *this;
	}
	date_time& operator -- (int)
	{
		add_any_days(-1);
		return *this;
	}

	/**
	 *	���ڴ�С�Ƚϲ�����
	 */
	bool operator > ( const date_time& b)
	{
		return compare_date(*this, b) > 0;
	}
 	bool operator >=( const date_time& b)
 	{
 		return compare_date(*this, b) >= 0;
 	}
 	bool operator < ( const date_time& b)
 	{
 		return compare_date(*this, b) < 0;
 	}
 	bool operator <=( const date_time& b)
 	{
 		return compare_date(*this, b) <= 0;
 	}
 	bool operator ==( const date_time& b)
 	{
 		return compare_date(*this, b) == 0;
 	}
 	bool operator !=( const date_time& b)
 	{
 		return compare_date(*this, b) != 0;
 	}

private:
	// Ҫ���õ����ں�ʱ����GMTʱ��1970��1��1����ҹ֮��������� // utc�� �Ƿ�ת��Ϊutcʱ��
	void set_time(long sec, bool utc = false);

	// date_time ȫ�ָ�ʽ��(date_time��ʼ��)
	void date_global_format();
	// reformat: �Ƿ���������ʽ������
	// date_str format: [yyyy-mm-dd hh:mi:ss.ms] 
	void parse_date_string(const std::string& date_str, bool reformat = false);

	unsigned int nyear;
	unsigned int nmonth;
	unsigned int nday;

	unsigned int nhour;
	unsigned int nminuter;
	unsigned int nseconds;
	unsigned int nmilliseconds;

	unsigned int nisdst;	//����ʱ��ǰ�Ƿ���Ч
	unsigned int nweekday;	// day of week.
	unsigned int nyearday;	// day of year.

};

} // @end namespace svhttp


#endif
