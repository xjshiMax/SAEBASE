//2018-12-13
//封装常用的获取时间和时间戳的函数
/*
1.获取当前时间戳 s(ms,100ns)
2.获取当前时间字符串，任意格式输出
3.根据时间戳获取时间字符串
4.根据时间串获取时间戳
5.分别获取年月日
6.时间类型比较 重载 = == + - 
//系统使用的时间变量
*time_t		长整形（64位）用来存放utc时间戳
*struct tm  结构体 
struct tm {
int tm_sec;  秒–取值区间为[0,59] 
int tm_min;  分 - 取值区间为[0,59] 
int tm_hour;  时 - 取值区间为[0,23] 
int tm_mday;  一个月中的日期 - 取值区间为[1,31] 
int tm_mon;  月份（从一月开始，0代表一月） - 取值区间为[0,11] 
int tm_year; 年份，其值从1900开始 
int tm_wday; 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 
*struct timeval   对应函数gettimeofday
{
	time_t tv_sec;
	suseconds_t tv_usec;
}
*struct timespec	这个可以用在epoll的超时
{
	time_t tv_sec;
	long tv_nsec;
}
*/

#if defined(WIN32)
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#else
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <string>
#endif
using namespace std;
//64bit 类型变量
#define LONGLONG long int
#define ULONGLONG unsigned long int
#define s_100ns   10000000
namespace SAEBASE{
namespace HelpTime
{
	inline static string  getCurrentTime()
	{
#ifdef WIN32
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		char strtime[64]={0};
		sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
		return strtime;


#else
		struct tm *tblock;
		time_t timer = time(NULL);
		char strtime[64]={0};
		tblock = localtime(&timer);
		sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02d",tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
		return strtime;
#endif
	}

	inline static string getLogFileName()
	{
#ifdef WIN32
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		char strtime[64]={0};
		sprintf(strtime,"%04d-%02d-%02d",sys.wYear,sys.wMonth,sys.wDay);
		return strtime;


#else
		time_t t=time(NULL);
#endif
	}
//获取当前时间秒数。
#ifdef WIN32
	time_t SystemTimeToTimet(SYSTEMTIME st)
	{
		FILETIME ft;
		SystemTimeToFileTime( &st, &ft );

		LONGLONG nLL;
		ULARGE_INTEGER ui;
		ui.LowPart = ft.dwLowDateTime;
		ui.HighPart = ft.dwHighDateTime;
		nLL = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;
		time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
		return pt;
	}
#endif
}
/*
class xTimevar
{
public:
	xTimevar();
	xTimevar(ULONGLONG timestamp=0)
	{
		if(timestamp==0)
		{
			m_timestamp_s = time(NULL);
			
			struct timeval tv;
			gettimeofday(&tv,NULL);
			m_timestamp_ms = tv.tv_sec*1000+tv.tv_usec/1000;
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME,&ts);
			m_timestamp_100ns=ts.tv_sec*s_100ns+ts.ntv_sec*100;
		}
	}
	xTimevar(struct tm &structime);
	ULONGLONG getTimeStamp()
	{
		m_timestamp_s = time(NULL);
		return m_timestamp_s;
	}
	 ULONGLONG getTimeStamp(struct tm &Inputtime)
	{
		return mktime(&Inputtime);
	}
	//通过时间戳获取结构体时间，为0表示获取当前时间。
	const struct tm &getstructTime(ULONGLONG timestamp=0)
	{
		time_t temp=(time_t)m_timestamp_s;
			if(timestamp!=0)
			{
				temp=timestamp;
			}
		return  *localtime(&temp);
	}
	string GetTimeFormat(const char* lptimeformat)
	{

	}
	bool operator ==(xTimevar);
	//返回时间差值，秒数
	ULONGLONG operator -(xTimevar& secondTime);
	//赋值
	xTimevar operator =(xTimevar&secondTime);
private:
	std::string strtime;
	ULONGLONG m_timestamp_s;
	ULONGLONG m_timestamp_ms;
	ULONGLONG m_timestamp_100ns;
	struct tm m_time_struct;
	
};*/

}