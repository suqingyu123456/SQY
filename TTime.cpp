#include "TTime.h"
#include <cstring>
using std::to_string;

//获取time_t类时间
time_t TTime::GetTime() const
{
	return ltime;
}

//设置时间，以time_t为参数
bool TTime::SetTime( time_t other )
{
	ltime = other;
	return true;
}

//设置时间，以TTime为参数
bool TTime::SetTime( TTime &other )
{
	ltime = other.GetTime();
	return true;
}

//设置时间，以字符串格式为参数
bool TTime::SetTime( const char *other )
{
	string temp;
	struct tm st_time;
	if ( TStrTool::IsEmptyStr(other) == true )
	{
		return false;
	}
	for ( size_t i = 0; i < strlen(other); i++ )
	{
		if ( other[i] >= '0' && other[i] <= '9' )
		{
			temp += other[i];
		}
	}
	if ( temp.size() != 14 )
	{
		return false;
	}
	st_time.tm_year = stoi(temp.substr(0, 4)) - 1900;
 	st_time.tm_mon = stoi(temp.substr(4, 2)) - 1;
  	st_time.tm_mday = stoi(temp.substr(6, 2));
  	st_time.tm_hour = stoi(temp.substr(8, 2));
  	st_time.tm_min = stoi(temp.substr(10, 2));
  	st_time.tm_sec = stoi(temp.substr(12, 2));
  	st_time.tm_isdst = 0;

	ltime = mktime(&st_time);
	return true;
	
}


//获取年份
int TTime::GetYear() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_year + 1900;
}

//获取月份
int TTime::GetMonth() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_mon + 1;
}

//获取日期
int TTime::GetDay() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_mday;
}

//获取星期
int TTime::GetWeakDay() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_wday;
}


//获取今年的第几天
int TTime::GetYearDay() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_yday;
}

//获取小时
int TTime::GetHour() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_hour;
}

//获取分钟
int TTime::GetMin() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_min;
}

//获取秒
int TTime::GetSec() const
{
	struct tm st_time = *localtime( &ltime );
	return st_time.tm_sec;
}

//加减年份
void TTime::AddYear( int year )
{
	struct tm st_time = *localtime( &ltime );
	st_time.tm_year += year;
	ltime = mktime(&st_time);
}


//加减月份
void TTime::AddMonth( int month )
{
	struct tm st_time = *localtime( &ltime );
	int totalmon = st_time.tm_mon + month + 1;
	if ( totalmon > 12 || totalmon < 1 )
	{
		st_time.tm_year += totalmon / 12;
		totalmon %= 12;
	}
	if ( totalmon > 0 )
	{
		st_time.tm_mon = totalmon - 1;
	}
	else
	{
		st_time.tm_year--;
		st_time.tm_mon = 11 + totalmon;
	}
	ltime = mktime(&st_time);
}

//加减日期
void TTime::AddDay( int day )
{
	ltime += day * 24 * 60 * 60;
}

//加减小时
void TTime::AddHour( int hour )
{
	ltime += hour * 60 * 60;
}

//加减分钟
void TTime::AddMin( int min )
{
	ltime  += min * 60;
}

//加减秒
void TTime::Addsec( int sec )
{
	ltime += sec;
}

//加减星期
void TTime::AddWeek( int week )
{
	AddDay(week * 7);
}


//获取对应格式时间字符串
//格式规则：@Y(年), @M(月), @D(日), @h(小时), @m(分钟), @s(秒), @w(星期), @y(今年的第几天)
//默认为年-月-日 小时:分钟:秒
string TTime::GetString( const char *format )
{
	struct tm st_time = *localtime( &ltime );
	st_time.tm_year += 1900;
  	st_time.tm_mon++;

	string res;
	
	while ( strcmp( format, "\0" ) != 0 )
	{
		if ( (*format) == '@' && *(format + 1) != '\0' )
		{
			switch( *(format + 1) )
			{
				case 'Y':
				{
					res += to_string(st_time.tm_year);
					break;
				}
				case 'M':
				{
					if ( st_time.tm_mon < 10 )
					{
						res += '0';
					}
					res += to_string(st_time.tm_mon);
					break;
				}
				case 'D':
				{
					if ( st_time.tm_mday < 10 )
					{
						res += '0';
					}
					res += to_string(st_time.tm_mday);
					break;
				}
				case 'h':
				{
					if ( st_time.tm_hour < 10 )
					{
						res += '0';
					}
					res += to_string(st_time.tm_hour);
					break;
				}
				case 'm':
				{
					if ( st_time.tm_min < 10 )
					{
						res += '0';
					}
					res += to_string(st_time.tm_min);
					break;
				}
				case 's':
				{
					if ( st_time.tm_sec < 10 )
					{
						res += '0';
					}
					res += to_string(st_time.tm_sec);
					break;
				}
				case 'w':
				{
					res += to_string(st_time.tm_wday);
					break;
				}
				case 'y':
				{
					if ( st_time.tm_yday < 100 )
					{
						res += '0';
					}
					if ( st_time.tm_yday < 10 )
					{
						res += '0';
					}
					res += to_string(st_time.tm_yday);
					break;
				}
				default:
				{
					res += *(format + 1);
				}
			}
			format += 2;
		}
		else
		{
			res += *format;
			format++;
		}
	}

	return res;
}

