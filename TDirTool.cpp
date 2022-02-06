#include "TDirTool.h"
#include "TFileTool.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//创建目录，目录的上级目录必须存在，否则不能创建
//第二参数为目录存在时是否覆盖原目录或同名文件，默认为不覆盖
//第三参数为设置权限，默认为755
int TDirTool::MakeDir( const char *dirpath, bool cover, int mode )
{
	if ( TPathTool::IsExist(dirpath) == true )  //路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsDir(dirpath) )  //是否为目录
			{
				id = DelFullDir(dirpath);  //删除原目录
			}
			else
			{
				id = TFileTool::DelFile(dirpath);  //删除同名文件
			}
			
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else
		{
			return PATH_ALREADY_EXIST;
		}
	}

	//以给定权限创建目录
	//由于mkdir创建目录时，权限受制于系统的umask，所以先以默认权限创建
	if ( mkdir(dirpath, 0755) != 0 )
	{
		return FAIL;
	}

	return TPathTool::SetMode(dirpath, mode);  //更改目录权限
}

//创建目录，目录的上级目录不存在时，连同上级目录一起创建
//第二参数为目录存在时是否覆盖原目录或同名文件，默认为不覆盖
//第三参数为设置权限，默认为755
int TDirTool::MakeFullDir( const char *dirpath, bool cover, int mode )
{
	return _MakeFullDir(dirpath, cover, mode ); //调用实现函数
}

//MakeFullDir的实现函数，内函递归调用
int TDirTool::_MakeFullDir( const char *dirpath, bool cover, int mode )
{
	string super;
	GetDirSuper(super, dirpath);  //获取上级目录
	int id;
	if ( IsExist(super) == false )
	{	
		//上级目录不存在，递归创建上级目录
		id = _MakeFullDir(super.c_str(), cover, mode);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	//创建目录
	id = MakeDir( dirpath, cover, mode );
	if ( id != SUCCESS )
	{
		return id;
	}
	
	return SUCCESS;
	
}

//删除目录，目录必须为空目录才可删除
//linux目录下只存在.与..两个隐藏目录才算空目录
int TDirTool::DelDir( const char *dirpath )
{
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(dirpath)  == false )  //是否为目录
	{
		return PATH_NOT_DIR;
	}
	
	if ( rmdir(dirpath) != 0 )  //调用库函数rmdir
	{
		return FAIL;
	}
	
	return SUCCESS;
}

//删除目录，目录不为空则会递归删除目录下所有文件与目录
int TDirTool::DelFullDir( const char *dirpath )
{
	TDir dir(dirpath);  
	vector<string> dirlist;
	int id;
	id = dir.ListFile( dirlist, true ); //获得目录下所有文件，包括隐藏文件
	if ( id != SUCCESS )
	{
		return id;
	}

	for ( size_t i = 0; i < dirlist.size(); i++ )
	{	
		//删除所有文件
		id = TFileTool::DelFile(dirlist[i]);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	id = dir.ListDir( dirlist, true );  //获取目录下的子目录，包括隐藏目录
	if ( id != SUCCESS )
	{
		return id;
	}

	//获取隐藏目录时，前两个目录总是为.和..
	//这两个目录为当前目录与上级目录，不能进入递归删除，否则会有错误
	for ( size_t i = 2; i < dirlist.size(); i++ )
	{	
		//递归调用删除目录
		id = DelFullDir(dirlist[i]);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	//目录已清空，删除空目录
	id = DelDir(dirpath);
	if ( id != SUCCESS )
	{
		return id;
	}

	return SUCCESS;
}

//复制目录,目标目录的上级目录不存在时会被创建
//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
int TDirTool::CopyDir( const char *srcdirpath, const char *dstdirpath, bool cover )
{
	return _CopyDir(srcdirpath, dstdirpath, cover); 
}

//CopyDir的实现函数，内涵递归调用
int TDirTool::_CopyDir( const char *srcdirpath, const char *dstdirpath, bool cover )
{
	if ( TPathTool::IsExist(srcdirpath) == false )  //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}
	
	if ( TPathTool::IsDir(srcdirpath)  == false )  //是否为目录
	{
		return PATH_NOT_DIR;
	}
	
	if ( TPathTool::IsExist(dstdirpath) == true )  //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsDir(dstdirpath) )  //是否为目录
			{
				id = DelFullDir(dstdirpath);  //删除原目录
			}
			else
			{
				id = TFileTool::DelFile(dstdirpath);  //删除同名文件
			}
			
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else
		{
			return PATH_ALREADY_EXIST;
		}
	}

	int id = 0;
	string tmpdir = dstdirpath;
	tmpdir += ".tmp";  //复制时先将内容复制为临时目录
	
	id = MakeFullDir(tmpdir, cover);  //创建临时目录
	if ( id != SUCCESS )	
	{
		return id;
	}

	TDir src(srcdirpath);
	vector<string> dirlist;
	string filename, dirname;
	src.ListFile(dirlist, true);  //获取源目录下所有文件

	//复制目录下所有文件
	for ( size_t i = 0; i < dirlist.size(); i++ )
	{
		TFileTool::GetFileAllName(filename, dirlist[i]);
		TPathTool::JointPath(filename, tmpdir, filename);
		id = TFileTool::CopyFile(dirlist[i], filename);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	src.ListDir(dirlist, true);  //获取源目录下子目录

	//复制目录下所有目录
	for ( size_t i = 2; i < dirlist.size(); i++ )
	{
		TDirTool::GetDirName(dirname, dirlist[i]);
		TPathTool::JointPath(dirname, tmpdir, dirname);
		id = _CopyDir( dirlist[i].c_str(), dirname.c_str() );
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	RenameDir( tmpdir, dstdirpath ); //更改目录名

	return SUCCESS;
}

//重命名目录，源目录与目标目录的上级目录必须一致，即只能在同级目录下改名
//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
int TDirTool::RenameDir( const char *srcdirpath, const char *dstdirpath, bool cover )
{
	if ( strcmp( srcdirpath, dstdirpath ) == 0 )  //源路径与目标路径是否一致
	{
		return SUCCESS;
	}
	
	if ( TPathTool::IsExist(srcdirpath) == false )  //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(srcdirpath)  == false )  //源路径是否为目录
	{
		return PATH_NOT_DIR;
	}

	if ( TPathTool::CompareSuper( srcdirpath, dstdirpath ) == false )  //比较上级目录是否一致
	{
		return PATH_NOT_SAME_SUPER;
	}

	if ( TPathTool::IsExist(dstdirpath) == true )   //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsDir(dstdirpath) )  //是否为目录
			{
				id = DelFullDir(dstdirpath);  //删除原目录
			}
			else
			{
				id = TFileTool::DelFile(dstdirpath);  //删除同名文件
			}
			
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else
		{
			return PATH_ALREADY_EXIST;
		}
	}

	//调用TPathTool重命名方法
	int id;
	id = TPathTool::RenamePath(srcdirpath, dstdirpath);
	if ( id != SUCCESS )
	{
		return id;
	}

	return SUCCESS;
}

//移动目录，源目录与目标目录的目录名必须一致
//目标目录的上级目录不存在时，目录会被创建
//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
int TDirTool::MoveDir( const char *srcdirpath, const char *dstdirpath, bool cover )
{
	if ( strcmp( srcdirpath, dstdirpath ) == 0 )  //源路径与目标路径是否一致
	{
		return SUCCESS;
	}
	
	if ( TPathTool::IsExist(srcdirpath) == false )  //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(srcdirpath)  == false )  //源路径是否为目录
	{
		return PATH_NOT_DIR;
	}

	if ( TPathTool::CompareBase( srcdirpath, dstdirpath ) == false )  //比较目录名是否一致
	{
		return PATH_NOT_SAME_DIRNAME;
	}

	if ( TPathTool::IsExist(dstdirpath) == true )   //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsDir(dstdirpath) )  //是否为目录
			{
				id = DelFullDir(dstdirpath);  //删除原目录
			}
			else
			{
				id = TFileTool::DelFile(dstdirpath);  //删除同名文件
			}
			
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else
		{
			return PATH_ALREADY_EXIST;
		}
	}

	int id;

	//目标目录的上级目录不存在时，目录会被创建
	string supername;
	GetDirSuper(supername, dstdirpath);
	if ( TDirTool::IsExist(supername) == false ) //上级目录不存在，创建上级目录
	{
		id = TDirTool::MakeFullDir(supername);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	//调用TPathTool重命名方法
	id = TPathTool::RenamePath(srcdirpath, dstdirpath);
	if ( id != SUCCESS )
	{
		return id;
	}
	
	return SUCCESS;
}

//移动且更改目录名
//目标目录的上级目录不存在时，目录会被创建
//第三参数为目标目录存在时是否覆盖目标目录或同名文件，默认为不覆盖
int TDirTool::MoveAndRenameDir( const char *srcdirpath, const char *dstdirpath, bool cover )
{
	if ( strcmp( srcdirpath, dstdirpath ) == 0 )  //源路径与目标路径是否一致
	{
		return SUCCESS;
	}
	
	if ( TPathTool::IsExist(srcdirpath) == false )  //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(srcdirpath)  == false )  //源路径是否为目录
	{
		return PATH_NOT_DIR;
	}

	if ( TPathTool::IsExist(dstdirpath) == true )   //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsDir(dstdirpath) )  //是否为目录
			{
				id = DelFullDir(dstdirpath);  //删除原目录
			}
			else
			{
				id = TFileTool::DelFile(dstdirpath);  //删除同名文件
			}
			
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else
		{
			return PATH_ALREADY_EXIST;
		}
	}

	int id;

	//目标目录的上级目录不存在时，目录会被创建
	string supername;
	GetDirSuper(supername, dstdirpath);
	if ( TDirTool::IsExist(supername) == false ) //上级目录不存在，创建上级目录
	{
		id = TDirTool::MakeFullDir(supername);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	//调用TPathTool重命名方法
	id = TPathTool::RenamePath(srcdirpath, dstdirpath);
	if ( id != SUCCESS )
	{
		return id;
	}
	
	return SUCCESS;
}

//判断目录是否存在
bool TDirTool::IsExist( const char *dirpath )
{
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return false;
	}
	return TPathTool::IsDir(dirpath);  //是否为目录
}

//判断目录是否存在且可读
bool TDirTool::IsReadable( const char *dirpath )
{
	if ( IsExist(dirpath) == false )  //判断目录是否存在
	{
		return false;
	}	
	return TPathTool::IsReadable(dirpath);  //判断是否可读
}

//判断目录是否存在且可写
bool TDirTool::IsWriteable( const char *dirpath )
{
	if ( IsExist(dirpath) == false )  //判断目录是否存在
	{
		return false;
	}	
	return TPathTool::IsWriteable(dirpath);  //判断是否可读
}

//判断目录是否存在且可执行
bool TDirTool::IsExecuteable( const char *dirpath )
{
	if ( IsExist(dirpath) == false )  //判断目录是否存在
	{
		return false;
	}	
	return TPathTool::IsExecuteable(dirpath);  //判断是否可读
}

//获取目录名
int TDirTool::GetDirName( string &res, const char *dirpath )
{
	return TPathTool::GetBaseName(res, dirpath);  //调用TPathTool方法
}

//获取上级目录
int TDirTool::GetDirSuper( string &res, const char *dirpath )
{
	return TPathTool::GetSuperName(res, dirpath);  //调用TPathTool方法
}

//获取目录大小
int TDirTool::GetDirSize( int &res, const char *dirpath )
{
	res = -1; //设置默认值
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(dirpath) == false )  //该路径是否为目录
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetSize(res, dirpath);  //调用TPathTool方法
}

//获取目录权限
int TDirTool::GetDirMode( int &res, const char *dirpath )
{
	res = -1;  //设置默认值
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(dirpath) == false )  //该路径是否为目录
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetMode(res, dirpath);  //调用TPathTool方法
}

//设置目录权限
int TDirTool::SetDirMode( const char *dirpath, int mode )
{
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(dirpath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::SetMode(dirpath, mode);  //调用TPathTool方法
}

//获取目录创建时间
int TDirTool::GetDirCTime( TTime &res, const char *dirpath )
{
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(dirpath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetCreateTime(res, dirpath);  //调用TPathTool方法
}

//获取目录访问时间
int TDirTool::GetDirATime( TTime &res, const char *dirpath )
{
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(dirpath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}
 
	return TPathTool::GetAccessTime(res, dirpath);  //调用TPathTool方法
}

//获取目录修改时间
int TDirTool::GetDirMTime( TTime &res, const char *dirpath )
{
	if ( TPathTool::IsExist(dirpath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(dirpath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetModifyTime(res, dirpath);  //调用TPathTool方法
}