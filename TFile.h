/***************************************                                                           
     文件名：TFile.h
     作者：苏青羽
     功能描述：存取文件的相关属性与提供各种常用方法
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

//linux下使用vim保存文件时会在文件末尾自动添加换行符，且这个换行符在编辑模式下是看不的
//这个问题暂时没有好的解决方法，在使用时需要特别注意该问题

#ifndef _TFILE_H_
#define _TFILE_H_

#include "TPreDef.h"
#include "TPathTool.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class TFile
{
	public:  //默认方法

		//TFile支持string构造初始化以及赋值操作
		TFile();
		
		~TFile();

		TFile( const TFile &other );

		TFile( const string other );

		TFile & operator=( const TFile &other );

		TFile & operator=( const string other );

	public:  //实现方法

		//以mode参数打开文件，mode参数类型与fopen一致，可百度查询
		int Open( const char *mode );
		int Open( const string &mode )
		{
			return Open(mode.c_str());
		}

		//关闭文件
		void Close();

		//刷新缓冲区，将数据从缓冲区写入文件
		void Flush();

		//获取文件路径
		string GetFilePath();

		//获取文件指针
		FILE * GetFilePoint();

		//获取文件打开模式
		string GetFileOpenMode();

		//清空文件内容，不创建新文件
		int Clear();

		//从文件读取指定的字节数，如果未给定或为负则读取所有。
		int Read( string &res, long size = -1 );

		//从文件读取整行，包括 "\n" 字符。
		//如果指定了一个非负数的参数，则返回指定大小的字节数，包括 "\n" 字符。
		int ReadLine( string &res, long size = -1 );

		//读取所有行(直到结束符 EOF)并将结果存储到字符串数组中
		int ReadLines(vector<string> &res);

		//以二进制方式从文件读取指定的字节数，如果未给定或为负则读取所有。
		//第一参数为读取后数据要放入的地址
		//第二参数为操作后实际读取到的字节数
		//第三参数为打算读取的字节数
		//该方法并不安全，请慎用该方法！！！
		int ReadB( void *res, long &readsize, long size = -1 );
		
		//将字符串写入文件
		//第二参数是否开启缓冲，即数据立即写入文件还是写入缓冲区，默认立即写入
		int Write( const char *str, bool buffer = false );
		int Write( const string &str, bool buffer = false )
		{
			return Write(str.c_str(), buffer );
		}

		//将字符串数组写入文件中
		//写入过程不会自动换行，若想换行请在数组中字符串末尾加入换行符
		//第二参数是否开启缓冲，即数据立即写入文件还是写入缓冲区，默认立即写入
		int WriteLines( vector<string> &strs, bool buffer = false );

		//将二进制数据写入文件,必须指定写入字节大小,若为负数返回失败
		//第二参数是否开启缓冲，即数据立即写入文件还是写入缓冲区，默认立即写入
		int WriteB( void *data, long size, bool buffer = false );

		//移动文件读取指针到指定位置
		//offset为开始的偏移量，也就是代表需要移动偏移的字节数。
		//whence为可选参数，默认值为 0。给 offset 定义一个参数，表示要从哪个位置开始偏移；0 代表从文件开头开始算起，1 代表从当前位置开始算起，2 代表从文件末尾算起。
		int Seek( long offset, int whence = 0 );

		//返回文件当前位置。
		//文件位置从0开始，到EOF结束，共EOF-0个字节
		int Tell( long &res );

		//从文件的首行首字符开始截断，截断文件为 size 个字符。无 size 表示从当前位置截断；截断之后后面的所有字符被删除。
		int Truncate( long size = -1 );
	
		//获取文件大小，以字节为单位，失败返回-1
		int GetSize();

		//获取文件创建时间，失败返回默认时间
		TTime GetCTime();

		//获取文件访问时间，失败返回默认时间
		TTime GetATime();

		//获取文件修改时间，失败返回默认时间
		TTime GetMTime();

		//获取文件权限,失败返回-1
		int GetMode();

		//设置文件权限，失败返回false
		bool SetMode( int mode );

		//获取文件名，包括扩展名
		string GetFileAllName();

		//获取文件名，不包括扩展名
		string GetFileName();

		//获取文件扩展名
		string GetFileExt();

		//获取文件所在目录
		string GetFileSuper();

		//文件是否可读
		bool IsReadable();

		//文件是否可写
		bool IsWriteable();

		//文件是否可执行
		bool IsExecuteable();

	private:
		FILE *m_FilePoint;  //文件指针
		string m_FilePath;  //文件路径
		string m_FileOpenMode;  //文件打开模式
};

#endif