/***************************************                                                           
     文件名：TLog.h
     作者：苏青羽
     功能描述：存取日志文件的相关属性和提供各种常用方法
     创建日期：2022-02-06
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

#ifndef _TLOG_H_
#define _TLOG_H_

#include "TFile.h"
#include <string>
using std::string;

class TLog
: public TFile
{
	public:  //默认方法

		//TLog支持string构造初始化以及赋值操作以修改文件路径
		TLog();
		
		~TLog() = default;

		TLog( const TLog &other );

		TLog( const string other );

		TLog( const char *other );

		TLog & operator=( const TLog &other );

		TLog & operator=( const string other );

		TLog & operator=( const char *other );

	public:  //实现方法

		//覆盖TFile-Open,日志只能以追加写打开
		int Open();

		//设置日志最大大小，单位为M
		void SetMaxSize( long size );

		//设置日志备份最大数量
		void SetMaxNum( size_t num );

		//设置是否加锁写日志
		void SetMutex( bool ismutex );

		//备份日志, 备份日志以.back和数字结尾
		int Backup();

		//写入日志，会自动在写入内容前加入时间，并且若是最后一个字符不是换行会自动添加换行
		//日志需要实时写入，不打开缓冲模式
		int WriteLog( const char *str );
		int WriteLog( const string &str )
		{
			return WriteLog(str.c_str());
		}

		//覆盖TFile——Write,不打开缓冲模式，自动添加锁
		int Write( const char *str );
		int Write( const string &str )
		{
			return Write(str.c_str() );
		}

		//覆盖TFile——Write,不打开缓冲模式，自动添加锁
		int WriteLines( vector<string> &strs );

		//覆盖TFile——Write,不打开缓冲模式，自动添加锁
		int WriteB( void *data, long size );

	private:
	
		long m_MaxLogSize;  //单个日志最大大小，单位为M， 默认为100
		size_t m_MaxLogNum;  //备份日志最大数量，默认为10
		bool m_Mutex;  //日志写时是否加锁，默认不加锁
};

#endif
