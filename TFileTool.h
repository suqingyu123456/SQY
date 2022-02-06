/***************************************                                                           
     文件名：TFileTool.h
     作者：苏青羽
     功能描述：提供关于文件操作的常用方法
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

#ifndef _TFILETOOL_H_
#define _TFILETOOL_H_

#include <string>
#include "TPathTool.h"
#include "TFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/io.h>
#include <unistd.h>
using std::string;

class TFileTool
{
    public:  //默认方法
    
        TFileTool() = delete;

        ~TFileTool() = delete;

        TFileTool( const TFileTool& ) = delete;

        TFileTool & operator=( const TFileTool& ) = delete;

    public:

		//创建文件，文件的上级目录必须存在，否则不能创建
		//第二参数为文件存在时是否覆盖原文件或同名目录，默认为不覆盖
		//第三参数为设置权限，默认为644
		static int MakeFile( const char *filepath, bool cover = false, int mode = 644 );
        static int MakeFile( const string &filepath, bool cover = false, int mode = 644 )
        {
        	return MakeFile(filepath.c_str(), cover, mode);
        }

		//创建文件，文件的上级目录不存在时，连同目录一起创建
		//第二参数为文件存在时是否覆盖原文件或同名目录，默认为不覆盖
		//第三参数为设置权限，默认为644
		static int MakeFullFile( const char *filepath, bool cover = false, int mode = 644 );
        static int MakeFullFile( const string &filepath, bool cover = false, int mode = 644 )
        {
        	return MakeFullFile(filepath.c_str(), cover, mode);
        }

		//删除文件
		static int DelFile( const char *filepath );
        static int DelFile( const string &filepath )
        {
        	return DelFile(filepath.c_str());
        }

		//复制文件
		//目标文件的上级目录不存在时会被创建
		//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
		static int CopyFile( const char *srcfilepath, const char *dstfilepath, bool cover = false);
        static int CopyFile( const string &srcfilepath, const char *dstfilepath, bool cover = false)
        {
        	return CopyFile( srcfilepath.c_str(), dstfilepath, cover );
        }
		static int CopyFile( const char *srcfilepath, const string &dstfilepath, bool cover = false)
		{
        	return CopyFile( srcfilepath, dstfilepath.c_str(), cover );
        }
		static int CopyFile( const string &srcfilepath, const string &dstfilepath, bool cover = false)
		{
        	return CopyFile( srcfilepath.c_str(), dstfilepath.c_str(), cover );
        }

		//移动文件，源文件与目标文件的文件名必须一致
		//目标文件的上级目录不存在时，目录会被创建
		//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
		static int MoveFile( const char *srcfilepath, const char *dstfilepath, bool cover = false );
        static int MoveFile( const string &srcfilepath, const char *dstfilepath, bool cover = false )
        {
        	return MoveFile( srcfilepath.c_str(), dstfilepath, cover );
        }
		static int MoveFile( const char *srcfilepath, const string &dstfilepath, bool cover = false )
		{
        	return MoveFile( srcfilepath, dstfilepath.c_str(), cover );
        }
		static int MoveFile( const string &srcfilepath, const string &dstfilepath, bool cover = false )
		{
        	return MoveFile( srcfilepath.c_str(), dstfilepath.c_str(), cover );
        }

		//重命名文件，源文件与目标文件的上级目录必须一致，即只能在同级目录下改名
		//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
		static int RenameFile( const char *srcfilepath, const char *dstfilepath, bool cover = false );
        static int RenameFile( const string &srcfilepath, const char *dstfilepath, bool cover = false )
        {
        	return RenameFile( srcfilepath.c_str(), dstfilepath, cover );
        }
		static int RenameFile( const char *srcfilepath, const string &dstfilepath, bool cover = false )
		{
        	return RenameFile( srcfilepath, dstfilepath.c_str(), cover );
        }
		static int RenameFile( const string &srcfilepath, const string &dstfilepath, bool cover = false )
		{
        	return RenameFile( srcfilepath.c_str(), dstfilepath.c_str(), cover );
        }

		//移动且更改文件名
		//目标文件的上级目录不存在时，目录会被创建
		//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
		static int MoveAndRenameFile( const char *srcfilepath, const char *dstfilepath, bool cover = false );
        static int MoveAndRenameFile( const string &srcfilepath, const char *dstfilepath, bool cover = false )
        {
        	return MoveAndRenameFile( srcfilepath.c_str(), dstfilepath, cover );
        }
		static int MoveAndRenameFile( const char *srcfilepath, const string &dstfilepath, bool cover = false )
		{
        	return MoveAndRenameFile( srcfilepath, dstfilepath.c_str(), cover );
        }
		static int MoveAndRenameFile( const string &srcfilepath, const string &dstfilepath, bool cover = false )
		{
        	return MoveAndRenameFile( srcfilepath.c_str(), dstfilepath.c_str(), cover );
        }

		//分离文件名与拓展名，以自定义类型StrPair返回
		static int SplitExt( StrPair &res, const char *filepath );
		static int SplitExt( StrPair &res, const string &filepath )
		{
        	return SplitExt(res, filepath.c_str());
        }

		//判断文件是否存在
		static bool IsExist( const char *filepath );
        static bool IsExist( const string &filepath )
        {
        	return IsExist(filepath.c_str());
        }

		//判断文件是否存在且可读
        static bool IsReadable( const char *filepath );
        static bool IsReadable( const string &filepath )
        {
        	return IsReadable(filepath.c_str());
        }

		//判断文件是否存在且可写
        static bool IsWriteable( const char *filepath );
        static bool IsWriteable( const string &filepath )
        {
        	return IsWriteable(filepath.c_str());
        }

		//判断文件是否存在且可执行
        static bool IsExecuteable( const char *filepath );
        static bool IsExecuteable( const string &filepath )
        {
        	return IsExecuteable(filepath.c_str());
        }

		//获取文件名，包括扩展名
		static int GetFileAllName( string &res, const char *filepath );
        static int GetFileAllName( string &res, const string &filepath )
        {
        	return GetFileAllName(res, filepath.c_str());
        }

		//获取文件名，不包括扩展名
		static int GetFileName( string &res, const char *filepath );
        static int GetFileName( string &res, const string &filepath )
        {
        	return GetFileName(res, filepath.c_str());
        }

		//获取文件扩展名，没有扩展名返回空对象
		static int GetFileExt( string &res, const char *filepath );
        static int GetFileExt( string &res, const string &filepath )
        {
        	return GetFileExt( res, filepath.c_str());
        }

		//获取文件所在目录
		static int GetFileSuper( string &res, const char *filepath );
        static int GetFileSuper( string &res, const string &filepath )
        {
        	return GetFileSuper( res, filepath.c_str());
        }

		//获取文件大小
		static int GetFileSize( int &res, const char *filepath );
		static int GetFileSize( int &res, const string &filepath )
		{
			return GetFileSize( res, filepath.c_str());
		}

		//获取文件权限
		static int GetFileMode( int &res, const char *filepath );
		static int GetFileMode( int &res, const string &filepath )
		{
			return GetFileMode( res, filepath.c_str() );
		}

		//设置文件权限
		static int SetFileMode( const char *filepath, int mode);
		static int SetFileMode( const string &filepath, int mode )
		{
			return SetFileMode( filepath.c_str(), mode );
		}

		//获取文件创建时间
		static int GetFileCTime( TTime &res, const char *filepath );
		static int GetFileCTime( TTime &res, const string &filepath )
		{
			return GetFileCTime(res, filepath.c_str());
		}
		
		//获取文件访问时间
		static int GetFileATime( TTime &res, const char *filepath );
		static int GetFileATime( TTime &res, const string &filepath )
		{
			return GetFileATime(res, filepath.c_str());
		}

		//获取文件修改时间
		static int GetFileMTime( TTime &res, const char *filepath );
		static int GetFileMTime( TTime &res, const string &filepath )
		{
			return GetFileMTime(res, filepath.c_str());
		}
		
};

#endif
