/***************************************                                                           
     文件名：TDirTool.h
     作者：苏青羽
     功能描述：提供关于目录操作的常用方法
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

#ifndef _TDIRTOOL_H_
#define _TDIRTOOL_H_

#include <string>
#include "TDir.h"
using std::string;

class TDirTool
{
	public:  //默认方法
        TDirTool() = delete;

        ~TDirTool() = delete;

        TDirTool( const TDirTool& ) = delete;

        TDirTool & operator=( const TDirTool& ) = delete;

	public:  //实现方法

		//创建目录，目录的上级目录必须存在，否则不能创建
		//第二参数为文件存在时是否覆盖原目录或同名文件，默认为不覆盖
		//第三参数为设置权限，默认为755
		static int MakeDir( const char *dirpath, bool cover = false, int mode = 755 );
        static int MakeDir( const string &dirpath, bool cover = false, int mode = 755 )
        {
        	return MakeDir(dirpath.c_str(), cover, mode);
        }

		//创建目录，目录的上级目录不存在时，连同上级目录一起创建
		//第二参数为目录存在时是否覆盖原目录或同名文件，默认为不覆盖
		//第二参数为设置权限，默认为755
		static int MakeFullDir( const char *dirpath, bool cover = false, int mode = 755 );
        static int MakeFullDir( const string &dirpath, bool cover = false, int mode = 755 )
        {
        	return MakeFullDir(dirpath.c_str(), cover, mode);
        }

		//删除目录，目录必须为空目录才可删除
		//linux目录下只存在.与..两个隐藏目录才算空目录
		static int DelDir( const char *dirpath );
        static int DelDir( const string &dirpath )
        {
        	return DelDir(dirpath.c_str());
        }

		//删除目录，目录不为空则会递归删除目录下所有文件与目录
		static int DelFullDir( const char *dirpath );
        static int DelFullDir( const string &dirpath )
        {
        	return DelFullDir(dirpath.c_str());
        }

		//复制目录,目标目录的上级目录不存在时会被创建
		//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
		static int CopyDir( const char *srcdirpath, const char *dstdirpath, bool cover = false);
        static int CopyDir( const string &srcdirpath, const char *dstdirpath, bool cover = false)
        {
        	return CopyDir( srcdirpath.c_str(), dstdirpath, cover );
        }
		static int CopyDir( const char *srcdirpath, const string &dstdirpath, bool cover = false)
		{
        	return CopyDir( srcdirpath, dstdirpath.c_str(), cover );
        }
		static int CopyDir( const string &srcdirpath, const string &dstdirpath, bool cover = false)
		{
        	return CopyDir( srcdirpath.c_str(), dstdirpath.c_str(), cover );
        }

		//重命名目录，源目录与目标目录的上级目录必须一致，即只能在同级目录下改名
		//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
		static int RenameDir( const char *srcdirpath, const char *dstdirpath, bool cover = false );
        static int RenameDir( const string &srcdirpath, const char *dstdirpath, bool cover = false )
        {
        	return RenameDir( srcdirpath.c_str(), dstdirpath, cover );
        }
		static int RenameDir( const char *srcdirpath, const string &dstdirpath, bool cover = false )
		{
        	return RenameDir( srcdirpath, dstdirpath.c_str(), cover );
        }
		static int RenameDir( const string &srcdirpath, const string &dstdirpath, bool cover = false )
		{
        	return RenameDir( srcdirpath.c_str(), dstdirpath.c_str(), cover );
        }

		
		//移动目录，源目录与目标目录的目录名必须一致
		//目标目录的上级目录不存在时，目录会被创建
		//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
		static int MoveDir( const char *srcdirpath, const char *dstdirpath, bool cover = false );
		static int MoveDir( const string &srcdirpath, const char *dstdirpath, bool cover = false )
		{
			return MoveDir( srcdirpath.c_str(), dstdirpath, cover );
		}
		static int MoveDir( const char *srcdirpath, const string &dstdirpath, bool cover = false )
		{
			return MoveDir( srcdirpath, dstdirpath.c_str(), cover );
		}
		static int MoveDir( const string &srcdirpath, const string &dstdirpath, bool cover = false )
		{
			return MoveDir( srcdirpath.c_str(), dstdirpath.c_str(), cover );
		}

		//移动且更改目录名
		//目标目录的上级目录不存在时，目录会被创建
		//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
		static int MoveAndRenameDir( const char *srcdirpath, const char *dstdirpath, bool cover = false );
		static int MoveAndRenameDir( const string &srcdirpath, const char *dstdirpath, bool cover = false )
		{
			return MoveAndRenameDir( srcdirpath.c_str(), dstdirpath, cover );
		}
		static int MoveAndRenameDir( const char *srcdirpath, const string &dstdirpath, bool cover = false )
		{
			return MoveAndRenameDir( srcdirpath, dstdirpath.c_str(), cover );
		}
		static int MoveAndRenameDir( const string &srcdirpath, const string &dstdirpath, bool cover = false )
		{
			return MoveAndRenameDir( srcdirpath.c_str(), dstdirpath.c_str(), cover );
		}

		//判断目录是否存在
		static bool IsExist( const char *dirpath );
        static bool IsExist( const string &dirpath )
        {
        	return IsExist(dirpath.c_str());
        }

		//判断目录是否存在且可读
        static bool IsReadable( const char *dirpath );
        static bool IsReadable( const string &dirpath )
        {
        	return IsReadable(dirpath.c_str());
        }

		//判断目录是否存在且可写
        static bool IsWriteable( const char *dirpath );
        static bool IsWriteable( const string &dirpath )
        {
        	return IsWriteable(dirpath.c_str());
        }

		//判断目录是否存在且可执行
        static bool IsExecuteable( const char *dirpath );
        static bool IsExecuteable( const string &dirpath )
        {
        	return IsExecuteable(dirpath.c_str());
        }

		//获取目录名
		static int GetDirName( string &res, const char *dirpath );
        static int GetDirName( string &res, const string &dirpath )
        {
        	return GetDirName(res, dirpath.c_str());
        }

		//获取上级目录
		static int GetDirSuper( string &res, const char *dirpath );
        static int GetDirSuper( string &res, const string &dirpath )
        {
        	return GetDirSuper( res, dirpath.c_str());
        }

		//获取目录大小
		static int GetDirSize( int &res, const char *dirpath );
		static int GetDirSize( int &res, const string &dirpath )
		{
			return GetDirSize( res, dirpath.c_str());
		}

		//获取目录权限
		static int GetDirMode( int &res, const char *dirpath );
		static int GetDirMode( int &res, const string &dirpath )
		{
			return GetDirMode( res, dirpath.c_str() );
		}

		//设置目录权限
		static int SetDirMode( const char *dirpath, int mode);
		static int SetDirMode( const string &dirpath, int mode )
		{
			return SetDirMode( dirpath.c_str(), mode );
		}

		//获取目录创建时间
		static int GetDirCTime( TTime &res, const char *dirpath );
		static int GetDirCTime( TTime &res, const string &dirpath )
		{
			return GetDirCTime(res, dirpath.c_str());
		}
		
		//获取目录访问时间
		static int GetDirATime( TTime &res, const char *dirpath );
		static int GetDirATime( TTime &res, const string &dirpath )
		{
			return GetDirATime(res, dirpath.c_str());
		}

		//获取目录修改时间
		static int GetDirMTime( TTime &res, const char *dirpath );
		static int GetDirMTime( TTime &res, const string &dirpath )
		{
			return GetDirMTime(res, dirpath.c_str());
		}

	private:
	
		//MakeFullDir的实现函数，内函递归调用
		static int _MakeFullDir( const char *dirpath, bool cover = false, int mode = 755 );
	
		//CopyDir的实现函数，内涵递归调用
		static int _CopyDir( const char *srcdirpath, const char *dstdirpath, bool cover = false);
};

#endif

