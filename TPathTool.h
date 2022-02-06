/***************************************                                                           
     文件名：TPathTool.h
     作者：苏青羽
     功能描述：提供关于路径操作的常用方法
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/


#ifndef _TPATHTOOL_H_
#define _TPATHTOOL_H_

#include "TStrTool.h"
#include "TTime.h"
#include <string>
#include <utility>
using std::string;

class TPathTool
{
	public:  //默认方法
			
		TPathTool() = delete;
				
		~TPathTool() = delete;
		
		TPathTool( const TPathTool& ) = delete;
		
		TPathTool & operator=( const TPathTool& ) = delete;

    public:  //实现方法
		
		//判断是否为绝对路径
        static bool IsAbs( const char *path );
        static bool IsAbs( const string &path )
        {
        	return IsAbs(path.c_str());
        }

		//判断是否为相对路径
        static bool IsRel( const char *path );
        static bool IsRel( const string &path )
        {
        	return IsRel(path.c_str());
        }

		//将相对路径转化为绝对路径
		static int AbsPath( string &res, const char *path );
		static int AbsPath( string &res, const string &path )
		{
			return AbsPath(res, path.c_str());
		}
		
		//判断该路径指向的文件或目录是否存在
        static bool IsExist( const char *path );
        static bool IsExist( const string &path )
        {
        	return IsExist(path.c_str());
        }

		//判断该路径指向的文件或目录是否可读
        static bool IsReadable( const char *path );
        static bool IsReadable( const string &path )
        {
        	return IsReadable(path.c_str());
        }

		//判断该路径指向的文件或目录是否可写
        static bool IsWriteable( const char *path );
        static bool IsWriteable( const string &path )
        {
        	return IsWriteable(path.c_str());
        }

		//判断该路径指向的文件或目录是否可执行
        static bool IsExecuteable( const char *path );
        static bool IsExecuteable( const string &path )
        {
        	return IsExecuteable(path.c_str());
        }

		//判断该路径是否为文件
        static bool IsFile( const char *path );
        static bool IsFile( const string &path )
        {
        	return IsFile(path.c_str());
        }

		//判断该路径是否为目录
        static bool IsDir( const char *path );
        static bool IsDir( const string &path )
        {
        	return IsDir(path.c_str());
        }

		//获取该路径指向的文件的文件名
        static int GetBaseName( string &res, const char *path );
        static int GetBaseName( string &res, const string &path )
        {
        	return GetBaseName(res, path.c_str());
        }

		//获取该路径指向的文件的上级目录
		static int GetSuperName( string &res, const char *path );
		static int GetSuperName( string &res, const string &path )
		{
        	return GetSuperName(res, path.c_str());
        }

		//组合两个路径，返回组合后的新路径
        static int JointPath( string &res, const char *leftpath, const char *rightpath );
		static int JointPath( string &res, const string &leftpath, const char *rightpath )
		{
			return JointPath(res, leftpath.c_str(), rightpath);
		}
		static int JointPath( string &res, const char *leftpath, const string &rightpath )
		{
			return JointPath(res, leftpath, rightpath.c_str());
		}
        static int JointPath( string &res, const string &leftpath, const string &rightpath )
        {
			return JointPath(res, leftpath.c_str(), rightpath.c_str());
		}

		//将路径分隔，返回字符串数组
        static int SplitPath( vector<string> &res, const char *path );
        static int SplitPath( vector<string> &res, const string &path )
        {
        	return SplitPath(res, path.c_str());
        }

		//切割路径
		//cutindex切割点，默认为0, 0为左边第一个'/'，自左向右递增，可为负数，自右向左递减
		//请看以下用例
		//CutStr(res, "root/program/test" ) = root 与 program/test
		//CutStr(res, "root/program/test", 1 ) = root/program 与 test
		//CutStr(res, "root/program/test", -1 ) = root/program 与 test
		//CutStr(res, "root/program/test", -2) = root 与 program/test
		static int CutPath( StrPair &res, const char *path, int cutindex = 0);
		static int CutPath( StrPair &res, const string &path, int cutindex = 0)
		{
			return CutPath(res, path.c_str(), cutindex);
		}

		//获取路径所指向的文件或目录的大小，单位为字节
		static int GetSize( int &res, const char *path );
		static int GetSize( int &res, const string &path )
		{
        	return GetSize(res, path.c_str());
        }

		//获取路径所指向的文件或目录的创建时间
		static int GetCreateTime( TTime &res, const char *path );
		static int GetCreateTime( TTime &res, const string &path )
		{
        	return GetCreateTime(res, path.c_str());
        }

		//获取路径所指向的文件或目录的最后访问时间
		static int GetAccessTime( TTime &res, const char *path );
		static int GetAccessTime( TTime &res, const string &path )
		{
        	return GetAccessTime(res, path.c_str());
        }

		//获取路径所指向的文件或目录的最后修改时间
		static int GetModifyTime( TTime &res, const char *path );
		static int GetModifyTime( TTime &res, const string &path )
		{
        	return GetModifyTime(res, path.c_str());
        }

		//获取路径所指向的文件或目录的权限
		//以linux数字格式返回权限，即000~777
		static int GetMode( int &res, const char *path );
		static int GetMode( int &res, const string &path )
		{
			return GetMode( res, path.c_str());
		}

		//设置路径所指向的文件或目录的权限
		//以linux数字格式设置权限，即000~777
		static int SetMode( const char *path, int mode );
		static int SetMode( const string &path, int mode )
		{
			return SetMode(path.c_str(), mode);
		}

		//重命名旧路径所指向的文件或目录
		//目录更改相当于移动文件
		static int RenamePath( const char *oldpath, const char *newpath );
        static int RenamePath( const string &oldpath, const char *newpath )
        {
        	return RenamePath( oldpath.c_str(), newpath );
        }
		static int RenamePath( const char *oldpath, const string &newpath )
		{
        	return RenamePath( oldpath, newpath.c_str() );
        }
		static int RenamePath( const string &oldpath, const string &newpath )
		{
        	return RenamePath( oldpath.c_str(), newpath.c_str() );
        }

		//比较文件或目录名是否一致
		static bool CompareBase( const char *lpath, const char *rpath );
        static bool CompareBase( const string &lpath, const char *rpath )
        {
        	return CompareBase( lpath.c_str(), rpath );
        }
		static bool CompareBase( const char *lpath, const string &rpath )
		{
        	return CompareBase( lpath, rpath.c_str() );
        }
		static bool CompareBase( const string &lpath, const string &rpath )
		{
        	return CompareBase( lpath.c_str(), rpath.c_str() );
        }


		//比较上级目录是否一致
		static bool CompareSuper( const char *lpath, const char *rpath );
        static bool CompareSuper( const string &lpath, const char *rpath )
        {
        	return CompareSuper( lpath.c_str(), rpath );
        }
		static bool CompareSuper( const char *lpath, const string &rpath )
		{
        	return CompareSuper( lpath, rpath.c_str() );
        }
		static bool CompareSuper( const string &lpath, const string &rpath )
		{
        	return CompareSuper( lpath.c_str(), rpath.c_str() );
        }
};

#endif
