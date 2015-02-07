#ifndef _svhttp_posix_time_hpp_
#define _svhttp_posix_time_hpp_

#include <string>
#include <time.h>

#if _WIN32
#include <windows.h>
#endif

// /D "_CRT_SECURE_NO_WARNINGS" 
// #if _MSC_VER > 1400
// #define sprintf sprintf_s
// #endif

namespace svhttp
{
	/* 获取当前时间，返回时间字符串格式:[yyyy-mm-dd hh24:mi:ss]
	 * time_type = 1，返回时间格式: [hh24:mi:ss]
	 * time_type = 2, 返回日期格式: [yyyy-mm-dd]
	 * time_type = 3, 返回时间格式: [hh24:mi:ss.ms]
	 */
	inline std::string get_now_time(int time_type = 0)
	{
		struct tm tmp;    
		time_t timep;
		time(&timep);

#if _MSC_VER >= 1400
		localtime_s(&tmp,&timep);
#else
		tmp = *(localtime(&timep));
#endif

		char time[64] = {0};

		if (time_type == 1)
		{
			sprintf(time, "%02d:%02d:%02d", tmp.tm_hour,tmp.tm_min, tmp.tm_sec);
		}
		else if (time_type == 2)
		{
			sprintf(time, "%04d-%02d-%02d", tmp.tm_year + 1900,tmp.tm_mon + 1,tmp.tm_mday);
		}
		else if (time_type == 3)
		{
#if _WIN32
			SYSTEMTIME wtm;
			GetLocalTime(&wtm);
			sprintf(time, "%02d:%02d:%02d.%03d", wtm.wHour, wtm.wMinute, wtm.wSecond, wtm.wMilliseconds);
#else
			sprintf(time, "%02d:%02d:%02d", tmp.tm_hour,tmp.tm_min, tmp.tm_sec);
#endif
		}
		else
		{
			sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d", tmp.tm_year + 1900,tmp.tm_mon + 1,
				tmp.tm_mday, tmp.tm_hour,tmp.tm_min, tmp.tm_sec);
		}

		return std::string(time);
	}
	
} //@ end namespace svhttp.

#endif