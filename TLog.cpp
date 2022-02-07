#include "TLog.h"
#include <mutex>
#include "TTimeTool.h"
#include "TFileTool.h"
using std::mutex;

#define MAXSIZE 256 //文件路径最大长度，linux下默认为256

TLog::TLog()
: TFile(), m_MaxLogSize(100), m_MaxLogNum(10), m_Mutex(false)
{
	;
}

TLog::TLog( const TLog &other )
: TFile(other), m_MaxLogSize(other.m_MaxLogSize), m_MaxLogNum(other.m_MaxLogNum), m_Mutex(other.m_Mutex)
{
	;
}


TLog::TLog( const string other )
: TFile(other), m_MaxLogSize(100), m_MaxLogNum(10), m_Mutex(false)
{
	;
}

TLog::TLog( const char *other )
: TFile(other), m_MaxLogSize(100), m_MaxLogNum(10), m_Mutex(false)
{
	;
}

TLog & TLog::operator=( const TLog &other )
{
	m_Mutex = other.m_Mutex;
	m_MaxLogSize = other.m_MaxLogSize;
	m_MaxLogNum = m_MaxLogNum;
	TFile::operator=(other);
	return *this;
}

TLog & TLog::operator=( const string other )
{
	TFile::operator=(other);
	return *this;
}

TLog & TLog::operator=( const char *other )
{
	TFile::operator=(other);
	return *this;
}

//覆盖TFile-Open,日志只能以追加写打开
int TLog::Open()
{
	return TFile::Open("a+");
}

//设置日志最大大小，单位为M
void TLog::SetMaxSize( long size )
{
	m_MaxLogSize = size;
}

//设置日志备份最大数量
void TLog::SetMaxNum( size_t num )
{
	m_MaxLogNum = num;
}

//设置是否加锁写日志
void TLog::SetMutex( bool ismutex )
{
	m_Mutex = ismutex;
}

//备份日志, 备份日志以.back和数字结尾
int TLog::Backup()
{
	Close();  //关闭文件

	static size_t num = 0;

	if ( num < m_MaxLogNum )
	{
		num++;  //备份文件编号
	}

	char backfile[MAXSIZE] = {'\0'};
	sprintf( backfile, "%s.back%zu", GetFilePath().c_str(), num );  //备份文件命名

	TFileTool::RenameFile( GetFilePath(), backfile, true );  //重命名文件

	int id = TFileTool::MakeFile(GetFilePath());  //创建新日志文件

	if ( id != SUCCESS )
	{
		return id;
	}

	return Open();
}

//写入日志，会自动在写入内容前加入时间，并且若是最后一个字符不是换行会自动添加换行
//日志需要实时写入，不打开缓冲模式，自动添加锁
int TLog::WriteLog( const char *str )
{
	if ( GetSize() > m_MaxLogSize * 1024 * 1024 )  //文件已到达最大日志大小，备份文件
	{
		Backup();
	}
	
	static mutex log_lock;
	if ( m_Mutex == true )  //获取锁
	{
		log_lock.lock();
	}
	
	TTime systime = TTimeTool::GetSysTime();  //获取系统时间
	string log = systime.GetString();
	log += ' ';
	log += str;
	
	if ( log[log.size() - 1] != '\n' )  //写入内容中最后若是没有换行，自动添加换行符
	{
		log += '\n';
	}

	int id = TFile::Write(log);  //写入日志文件

	log_lock.unlock(); //释放锁

	return id;
}

//覆盖TFile——Write,不打开缓冲模式，自动添加锁
int TLog::Write( const char *str )
{
	static mutex log_lock;
	if ( m_Mutex == true )  //获取锁
	{
		log_lock.lock();
	}

	int id = TFile::Write(str);  //写入日志文件

	log_lock.unlock();  //释放锁

	return id;
}

//覆盖TFile——Write,不打开缓冲模式，自动添加锁
int TLog::WriteLines( vector<string> &strs )
{
	static mutex log_lock;
	if ( m_Mutex == true )  //获取锁
	{
		log_lock.lock();
	}

	int id = TFile::WriteLines(strs);  //写入日志文件

	log_lock.unlock();  //释放锁

	return id;
}

//覆盖TFile——Write,不打开缓冲模式，自动添加锁
int TLog::WriteB( void *data, long size )
{
	static mutex log_lock;
	if ( m_Mutex == true )  //获取锁
	{
		log_lock.lock();
	}

	int id = TFile::WriteB(data, size);   //写入日志文件

	log_lock.unlock();  //释放锁

	return id;
}

