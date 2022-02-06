/***************************************                                                           
     文件名：TDir.h
     作者：苏青羽
     功能描述：存取目录的相关属性与提供各种常用方法
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

#ifndef _TDIR_H_
#define _TDIR_H_

#include "TPreDef.h"
#include "TPathTool.h"
#include <string>
#include <vector>
#include <dirent.h>
using std::string;
using std::vector;


class TDir
{
	public:
		//TDir支持string构造初始化以及赋值操作
		TDir();
				
		~TDir();
	
		TDir( const TDir &other );
	
		TDir( const string other );
	
		TDir & operator=( const TDir &other );
	
		TDir & operator=( const string other );

	public:

		//打开目录
		int Open();

		//关闭目录
		void Close();

		//重新打开目录
		int Reopen();

		//列出目录下文件，包括目录和文件，但不包括子目录下的次级文件
		//第二参数为是否显示隐藏文件，默认不显示
		//第三参数为正则表达式的匹配模式，默认为空
		int List( vector<string> &res, bool display = false, const string pattern = "" );

		//只列出目录下文件，不包括目录，也不包括子目录下的次级文件
		//第二参数为是否显示隐藏文件，默认不显示
		//第三参数为正则表达式的匹配模式，默认为空
		int ListFile( vector<string> &res, bool display = false, const string pattern = "" );

		//只列出目录下目录，不包括文件，也不包括子目录下的次级文件
		//第二参数为是否显示隐藏文件，默认不显示
		//第三参数为正则表达式的匹配模式，默认为空
		int ListDir( vector<string> &res, bool display = false, const string pattern = "" );

		//列出目录下所有文件，包括目录和文件，包括子目录下的次级文件
		//第二参数为是否显示隐藏文件，默认不显示
		//该方法若设置匹配模式会有歧义，因此不能设置匹配模式
		int ListFull( vector<string> &res, bool display = false);

		//列出目录下所有文件，不包括目录，包括子目录下的次级文件
		//第二参数为是否显示隐藏文件，默认不显示
		//第三参数为正则表达式的匹配模式，默认为空
		int ListFullFile( vector<string> &res, bool display = false, const string pattern = "" );

		//列出目录下所有子目录，不包括文件，包括子目录下的次级目录
		//第二参数为是否显示隐藏文件，默认不显示
		//第三参数为正则表达式的匹配模式，默认为空
		int ListFullDir( vector<string> &res, bool display = false, const string pattern = "" );

		//获取目录路径
		string GetDirPath();

		//获取目录指针
		DIR * GetDirPoint();

		//获取目录大小，以字节为单位，失败返回-1
		int GetSize();

		//获取目录创建时间，失败返回默认时间
		TTime GetCTime();

		//获取目录访问时间，失败返回默认时间
		TTime GetATime();

		//获取目录修改时间，失败返回默认时间
		TTime GetMTime();

		//获取目录权限,失败返回-1
		int GetMode();

		//设置目录权限，失败返回false
		bool SetMode( int mode );

		//获取目录名
		string GetDirName();

		//获取目录所在目录
		string GetDirSuper();

		//目录是否可读
		bool IsReadable();

		//目录是否可写
		bool IsWriteable();

		//目录是否可执行
		bool IsExecuteable();

	private:
		
		//ListFullDir实现方法，内含递归调用
		int _ListFullDir( vector<string> &res, bool display = false, const string pattern = "" );

		//ListFullFile实现方法，内含递归调用
		int _ListFullFile( vector<string> &res, bool display = false, const string pattern = "" );

		//ListFull实现方法，内含递归调用
		int _ListFull( vector<string> &res, bool display = false);

	private:
		string m_DirPath;
		DIR *m_DirPoint;
		
};


#endif

