/***************************************                                                           
     文件名：TTimeTool.h
     作者：苏青羽
     功能描述：提供时间操作的常用方法
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

#ifndef _TTIMETOOL_H_
#define _TTIMETOOL_H_

#include <ctime>
#include <string>
#include "TTime.h"
#include <sys/time.h>
#include <cstring>
#include <unistd.h>
using std::string;

class TTimeTool
{

	public:

		TTimeTool()
		{
			memset(&timer, 0, sizeof(timer));
		}
		
		~TTimeTool() = default;

		TTimeTool( const TTimeTool &other ) = delete;

		TTimeTool & operator=( const TTimeTool &other ) = delete;

		
	public:

		//获取操作系统时间
		//无参数则放回TTime类，有参数则放回特殊格式字符串
		//格式字符串规则请看TTime类
		static TTime GetSysTime();
		static string GetSysTime( const char *format );
		static string GetSysTime( const string &format )
		{
			return GetSysTime(format.c_str());
		}

		//程序睡眠，以秒为单位
		static void Sleep( int sec )
		{
			sleep(sec);
		}

		//程序睡眠，以微妙为单位，1秒 = 1000000微秒
		static void USleep( int usec )
		{
			usleep(usec);
		}

		//计时器从0开始计时
		void StartTimer();

		//计时器结束计时，结束后计时器关闭，无法获取时间
		void EndTimer();

		//获取计时器时间,以秒为单位，必须先开启定时器才可以获取
		int GetPassSec();

		//获取当前秒过去的微妙时间，必须先开启定时器才可以获取
		int GetPassUSec();

	private:
		struct timeval timer;  //timeval结构体，存储秒与微妙
		
};

#endif

