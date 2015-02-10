#include "posix_time.h"

namespace svhttp
{

date_time::date_time()
{
	struct tm tmp;
	time_t timep;
	time(&timep);
	
#if _MSC_VER >= 1400
	localtime_s(&tmp,&timep);
#else
	tmp = *(localtime(&timep));
#endif

	nyear = tmp.tm_year + 1900;
	nmonth = tmp.tm_mon + 1;
	nday = tmp.tm_mday;
	
	nhour = tmp.tm_hour;
	nminuter = tmp.tm_min;
	nseconds = tmp.tm_sec;

#if _WIN32
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	nmilliseconds = wtm.wMilliseconds;
#else
	nmilliseconds = 0;
#endif

	nweekday = tmp.tm_wday; //get_day_of_week(nyear, nmonth, nday);
	nyearday = tmp.tm_yday;
};

date_time::~date_time()
{
}

// [yyyy-mm-dd hh:mi:ss.ms] msec: 毫秒级 (true: 返回时间字符精确到毫秒)
std::string date_time::to_string(bool msec /*= false*/)
{
	char time_str[64] = {0};
	if (msec)
	{
		sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d.%03d", nyear, nmonth, nday, nhour, nminuter, nseconds, nmilliseconds);
	}
	else
	{
		sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", nyear, nmonth, nday, nhour, nminuter, nseconds);
	}

	return std::string(time_str);
}
// [yyyy-mm-dd]
std::string date_time::to_date_string()
{
	char date_str[64] = {0};
	sprintf(date_str, "%04d-%02d-%02d", nyear, nmonth, nday);
	
	return std::string(date_str);
}

// [hh:mi:ss.ms] msec: 毫秒级 (true: 返回时间字符精确到毫秒)
std::string date_time::to_time_string(bool msec /*= false*/)
{
	char time_str[64] = {0};
	if (msec)
	{
		sprintf(time_str, "%02d:%02d:%02d.%03d", nhour, nminuter, nseconds, nmilliseconds);
	}
	else
	{
		sprintf(time_str, "%02d:%02d:%02d", nhour, nminuter, nseconds);
	}
	return std::string(time_str);
}

/**
 *	set operator
 */
void date_time::set_year(const unsigned int& val)
{
	nyear = val;
}
void date_time::set_month(const unsigned int& val)
{
	nmonth = val;
}
void date_time::set_day(const unsigned int& val)
{
	nday = val;
}
void date_time::set_hour(const unsigned int& val)
{
	nhour = val;
}
void date_time::set_minuter(const unsigned int& val)
{
	nminuter = val;
}
void date_time::set_seconds(const unsigned int& val)
{
	nseconds = val;
}
void date_time::set_milli_seconds(const unsigned int& val)
{
	nmilliseconds = val;
}

/**
 *	get operator
 */
unsigned int date_time::get_year()
{
	return nyear;
}
unsigned int date_time::get_month()
{
	return nmonth;
}
unsigned int date_time::get_day()
{
	return nday;
}
unsigned int date_time::get_hour()
{
	return nhour;
}
unsigned int date_time::get_minuter()
{
	return nminuter;
}
unsigned int date_time::get_seconds()
{
	return nseconds;
}
unsigned int date_time::get_milli_seconds()
{
	return nmilliseconds;
}

// 获取date_time日期对应的星期x <nweekday: days since Sunday - [0,6] >
std::string date_time::get_week_day_abbr()
{
	const std::string weeks[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	return weeks[nweekday];
}
// 获取date_time日期对应的月份 英文缩写
std::string date_time::get_month_abbr()
{
	const std::string months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	return months[nmonth-1];
}

// days since January 1 - [0,365]
unsigned int date_time::get_year_day()
{
	//return nyearday;
	return get_day_of_year(nyear, nmonth, nday);
}

// 计算出某天 前/后 idays 天的日期
void date_time::add_any_days(int idays)
{
	if (idays == 0)
	{
		return;
	}
	
	if (idays < 0)
	{
		for (int i = 0; i < abs(idays); i++)
		{
			nday--;
			if (nday == 0)
			{
				nmonth--;
				if (nmonth == 0)
				{
					nyear--;
					nmonth = 12;
				}
				nday = get_day_of_month(nyear, nmonth);
			}
		}
	}
	if (idays > 0)
	{
		for (int i = 0; i < idays; i++)
		{
			nday++;
			if (nday > get_day_of_month(nyear,nmonth) )
			{
				nmonth++;
				nday = 1;
				if (nmonth >12)
				{
					nyear++;
					nmonth = 1;
				}
			}
		}
	}
}

// 蔡勒公式: 计算指定日期对应的星期几 since Sunday - [0,6]
unsigned int date_time::get_day_of_week( const unsigned int& iyear, const unsigned int& imonth, const unsigned int& iday )
{
	int iweek = 0;
	unsigned int y = 0, c = 0, m = 0, d = 0;
	
	if ( imonth == 1 || imonth == 2 )
	{
		c = ( iyear - 1 ) / 100;
		y = ( iyear - 1 ) % 100;
		m = imonth + 12;
		d = iday;
	}
	else
	{
		c = iyear / 100;
		y = iyear % 100;
		m = imonth;
		d = iday;
	}
	
	iweek = y + y / 4 + c / 4 - 2 * c + 26 * ( m + 1 ) / 10 + d - 1;    //蔡勒公式
	iweek = iweek >= 0 ? ( iweek % 7 ) : ( iweek % 7 + 7 );    // iweek 为负时取模
	
	return (unsigned)iweek;
}
// 计算某月的天数
unsigned int date_time::get_day_of_month(const unsigned int& iyear, const unsigned int& imonth)
{
	unsigned iday = 0;
	if (imonth == 1 || 
		imonth == 3 || 
		imonth == 5 || 
		imonth == 7 || 
		imonth == 8 || 
		imonth == 10 || 
		imonth == 12 ){
		iday = 31;
	}
	if (imonth == 4 || 
		imonth == 6 || 
		imonth == 9 || 
		imonth == 11 ){
		iday = 30;
	}
	if(imonth == 2){	//二月要分闰年/平年
		iday = (unsigned int)( ((iyear%4 == 0) && (iyear%100 != 0 ||iyear%400 == 0)) ? 29:28 );
	}
	return iday;
}

// days since January 1 - [0,365]
unsigned int date_time::get_day_of_year(const unsigned int& iyear, const unsigned int& imonth, const unsigned int& iday)
{
	unsigned int ndays = 0;
	for (unsigned int idx = 0; idx < imonth-1; ++idx)
	{
		ndays += get_day_of_month(iyear, idx+1);
	}
	ndays += iday-1;
	
	return ndays;
}

int date_time::get_diff_date( date_time date1, date_time date2,const unsigned int& itype /*= 0*/)	
{
	int nret = 0;
	int compare_ret = compare_date(date1, date2);
	if ( compare_ret == 0)
	{
		return 0;
	}
	
	if (itype == 1)
	{
		nret = (date2.nmonth - date1.nmonth) + 12 * (date2.nyear - date1.nyear);
		if (date2.nday >= date1.nday)
		{
			if (get_day_of_month(date1.nyear, date1.nmonth) == date1.nday &&
				get_day_of_month(date2.nyear, date2.nmonth) >= date2.nday )
			{
			}
			else
			{
				nret++;
			}
		}
		return nret;
	}

	//取相差天数
	int ndays = 0;
	if (compare_ret < 0)
	{
		while (! (date1.nyear == date2.nyear && date1.nmonth == date2.nmonth && date1.nday == date2.nday) )
		{
			date1.add_any_days(1);
			ndays++;
		}
	}
	else
	{
		while (! (date1.nyear == date2.nyear && date1.nmonth == date2.nmonth && date1.nday == date2.nday) )
		{
			date1.add_any_days(-1);
			ndays--;
		}
	}

	//设置返回值
	switch(itype)
	{
	case 2:
		nret = ndays/365;
		break;
	case 0:
		nret = ndays;
		break;
	}

	return nret;
}

int date_time::compare_date(const date_time& date1, const date_time& date2 )
{
	int compare_ret = 0;

	if (date1.nyear == date2.nyear && date1.nmonth == date2.nmonth && date1.nday == date2.nday)
	{
		return 0;
	}
	
	if (date1.nyear < date2.nyear)
	{
		compare_ret = -1;
	}
	else if (date1.nyear > date2.nyear)
	{
		compare_ret = 1;
	}
	else if (date1.nyear == date2.nyear)
	{
		if (date1.nmonth < date2.nmonth)
		{
			compare_ret = -1;
		}
		else if (date1.nmonth > date2.nmonth)
		{
			compare_ret = 1;
		}
		else if (date1.nmonth == date2.nmonth)
		{
			if (date1.nday < date2.nday)
			{
				compare_ret = -1;
			}
			else if (date1.nday > date2.nday)
			{
				compare_ret = 1;
			}
			else if (date1.nday == date2.nday)
			{
				compare_ret = 0;
			}
		}
	}

	return compare_ret;
}



} // @end namespace svhttp
