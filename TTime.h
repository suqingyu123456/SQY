/***************************************                                                           
     文件名：TTime.h
     作者：苏青羽
     功能描述：存取日期与时间的相关属性与提供各种常用方法
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

#ifndef _TTIME_H_
#define _TTIME_H_

#include <ctime>
#include <sys/time.h>
#include <string>
#include "TStrTool.h"
using std::string;

class TTime
{
	public:

		TTime( time_t other = 0)
		: ltime(other){}
		
		~TTime() = default;

		TTime( const TTime &other ) = default;

		TTime & operator=( const TTime &other ) = default;
		
	public:

		//获取time_t类时间
		time_t GetTime() const;

		//设置时间，支持各种参数，包括time_t, TTime, 字符串格式
		bool SetTime( time_t other );
		bool SetTime( TTime &other );
		bool SetTime( const char *other );
		bool SetTime( string &other )
		{
			return SetTime(other.c_str());
		}

		//获取时间属性值，包括年、月、日、小时、分钟、秒、星期、今年的第几天
		int GetYear() const;
		int GetMonth() const;
		int GetDay() const;
		int GetWeakDay() const;
		int GetYearDay() const;
		int GetHour() const;
		int GetMin() const;
		int GetSec() const;

		//提供对时间某一属性的加减，正数值为加，负数值为减
		void AddYear( int year );
		void AddMonth( int month );
		void AddDay( int day );
		void AddHour( int hour );
		void AddMin( int min );
		void Addsec( int sec );
		void AddWeek( int week );

		//获取对应格式时间字符串
		//格式规则：@Y(年), @M(月), @D(日), @h(小时), @m(分钟), @s(秒), @w(星期), @y(今年的第几天)
		//默认为年-月-日 小时:分钟:秒
		string GetString( const char *format = "@Y-@M-@D @h:@m:@s" );
		string GetString( const string &format )
		{
			if ( TStrTool::IsEmptyStr(format) == true )
			{
				return GetString("@Y-@M-@D @h:@m:@s");
			}
			return GetString(format.c_str());
		}

		//判断操作符重载
		bool operator==( const TTime& other )
		{
			return ltime == other.GetTime();
		}
		bool operator!=( const TTime& other )
		{
			return ltime != other.GetTime();
		}
		bool operator>( const TTime& other )
		{
			return ltime > other.GetTime();
		}
		bool operator<( const TTime& other )
		{
			return ltime < other.GetTime();
		}
		bool operator>=( const TTime& other )
		{
			return ltime >= other.GetTime();
		}
		bool operator<=( const TTime& other )
		{
			return ltime <= other.GetTime();
		}

		//加减操作符重载，只支持秒的加减，其他加减其直接调用Add方法
		TTime operator+( int sec )
		{
			return TTime( ltime + sec );
		}
		TTime operator-( int sec )
		{
			return TTime( ltime - sec );
		}
		TTime & operator+=( int sec )
		{
			ltime += sec;
			return *this;
		}
		TTime & operator-=( int sec )
		{
			ltime -= sec;
			return *this;
		}

	private:
		time_t ltime;
		
};

#endif
