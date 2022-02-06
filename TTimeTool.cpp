#include "TTimeTool.h"

//获取操作系统时间，以特殊格式返回
string TTimeTool::GetSysTime( const char *format )
{
	TTime systime( time(0) );
	return systime.GetString(format);
}

//获取操作系统时间，以TTime类返回
TTime TTimeTool::GetSysTime()
{ 
	return TTime( time(0) );
}

//计时器从0开始计时
void TTimeTool::StartTimer()
{
	gettimeofday(&timer,0);
}

//计时器结束计时，结束后计时器关闭，无法获取时间
void TTimeTool::EndTimer()
{
	memset(&timer, 0, sizeof(timer));
}

//获取计时器时间,以秒为单位，必须先开启定时器才可以获取
int TTimeTool::GetPassSec()
{
	if ( timer.tv_sec == 0 )
	{
		return 0;
	}
	struct timeval nowtime;
	gettimeofday(&nowtime,0);

	return nowtime.tv_sec - timer.tv_sec;
}

//获取当前秒过去的微妙时间，必须先开启定时器才可以获取
int TTimeTool::GetPassUSec()
{
	if ( timer.tv_usec == 0 )
	{
		return 0;
	}

	struct timeval nowtime;
	gettimeofday(&nowtime,0);
	return nowtime.tv_usec - timer.tv_usec;
}

