#include "TPathTool.h"
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>


const int MAXSIZE = 1024;  //字符串最大大小

//判断是否为绝对路径
bool TPathTool::IsAbs( const char *path )
{
	//仅判断路径非空与首字符是否为'/'
	return ( !TStrTool::IsEmptyStr(path) ) && ( (*path) == '/' );
}

//判断是否为相对路径
bool TPathTool::IsRel( const char *path )
{
	//仅判断路径非空与首字符是否为'/'
	return ( !TStrTool::IsEmptyStr(path) ) && ( !IsAbs(path) );
}

//将相对路径转化为绝对路径
int TPathTool::AbsPath( string &res, const char *path )
{
	if ( IsAbs(path) == true )
	{
		return SUCCESS;  //path已经是绝对路径了
	}

	if ( IsExist(path) == false )
	{
		return PATH_NOT_EXIST; //路径不存在，无法转化
	}
	
	char abspath[MAXSIZE] = {'\0'};
	if ( realpath(path,abspath) == NULL ) //调用库函数realpath转化，失败返回FAIL
	{
		return FAIL;
	}
	
	res = abspath;
	return SUCCESS;
}

//判断该路径指向的文件或目录是否存在
bool TPathTool::IsExist( const char *path  )
{
    if ( TStrTool::IsEmptyStr(path) == true )  //判断路径非空
    {
        return false;
    }
    if ( access(path, F_OK) == 0 )  //调用库函数access判断
    {
        return true;
    }
    return false;
}

//判断该路径指向的文件或目录是否可读
bool TPathTool::IsReadable( const char *path  )
{
    if ( access(path, R_OK) == 0 )  //调用库函数access判断
    {
        return true;
    }
    return false;
}


//判断该路径指向的文件或目录是否可写
bool TPathTool::IsWriteable( const char *path  )
{

    if ( access(path, W_OK) == 0 )  //调用库函数access判断
    {
        return true;
    }
    return false;
}


//判断该路径指向的文件或目录是否可执行
bool TPathTool::IsExecuteable( const char *path  )
{

    if ( access(path, X_OK) == 0 )  //调用库函数access判断
    {
        return true;
    }

    return false;
}


//判断该路径是否为文件
bool TPathTool::IsFile( const char *path  )
{

    struct stat st;
    stat(path, &st);  //调用库函数stat获取信息

    if ( S_ISREG(st.st_mode) == true )
    {
        return true;
    }
    return false;

}


//判断该路径是否为目录
bool TPathTool::IsDir( const char *path  )
{

    struct stat st;
    stat( path, &st );  //调用库函数stat获取信息

    if ( S_ISDIR(st.st_mode) == true )
    {
        return true;
    }
    return false;
}

//获取该路径指向的文件的文件名
int TPathTool::GetBaseName( string &res, const char *path )
{
	if ( strchr(path, '/') == NULL ) //判断路径是否有目录存在
	{
		res = path;
		return SUCCESS;
	}

	//切割路径，以最后一个'/'为切割点，获取文件名
    StrPair pathpair;
	int strerror = TStrTool::CutStr(pathpair, path, "/", -1 ); 
	if ( strerror != SUCCESS )
	{
		return strerror;
	}
	
	res = pathpair.second;
	return SUCCESS;
}

//获取该路径指向的文件的上级目录
int TPathTool::GetSuperName( string &res, const char *path )
{
	if ( strchr(path, '/') == NULL )  //判断路径是否有目录存在
	{
		res = ".";  //该路径上级目录为当前目录
		return SUCCESS;
	}

	//切割路径，以最后一个'/'为切割点，获取目录名
	StrPair pathpair;
	int strerror = TStrTool::CutStr(pathpair, path, "/", -1 );
	if ( strerror != SUCCESS )
	{
		return strerror;
	}

	if ( pathpair.first.empty() == true )
	{
		res = "/";
	}
	else
	{
		res = pathpair.first;
	}
	return SUCCESS;
}


//组合两个路径，返回组合后的新路径
int TPathTool::JointPath( string &res, const char *leftpath, const char *rightpath )
{
	//判断左右路径非空
    if ( TStrTool::IsEmptyStr(leftpath) == true )
    {
        res = rightpath;
		return SUCCESS;
    }
	if ( TStrTool::IsEmptyStr(rightpath) == true )
	{
		res = leftpath;
		return SUCCESS;
	}

	//组合路径，不管左右路径如何，连接处都会且只会存在一个'/'作为连接符
	string temp = leftpath;
	if ( res[ res.size() - 1 ] != '/' && (*rightpath) != '/' )
	{
		temp += '/';
	}
	else if ( res[ res.size() - 1 ] == '/' && (*rightpath) == '/' )
	{
		rightpath++;
	}
	temp += rightpath;

	res = temp;

	return SUCCESS;
}

//将路径分隔，返回字符串数组
int TPathTool::SplitPath( vector<string> &res, const char *path )
{
	int id;
	id = TStrTool::SplitStr( res, path, "/" );  //调用TStrTool的SplitStr方法
	
	if ( id != SUCCESS )
	{
		return id;
	}

	if ( IsAbs(path) == true )
	{
		res[0] = "/";
	}

	return SUCCESS;
}

//切割路径
//cutindex切割点，默认为0, 0为左边第一个'/'，自左向右递增，可为负数，自右向左递减
//请看以下用例
//CutStr(res, "root/program/test" ) = root 与 program/test
//CutStr(res, "root/program/test", 1 ) = root/program 与 test
//CutStr(res, "root/program/test", -1 ) = root/program 与 test
//CutStr(res, "root/program/test", -2) = root 与 program/test
int TPathTool::CutPath( StrPair &res, const char *path, int cutindex)
{
	//调用TStrTool的CutStr方法
    return TStrTool::CutStr( res, path, "/", cutindex );
}

//获取路径所指向的文件或目录的大小，单位为字节
int TPathTool::GetSize( int &res, const char *path )
{
	res = -1;  //设置大小默认值
	if ( IsExist(path) == false )  //判断路径存在
	{
		return PATH_NOT_EXIST;
	}
	
	if ( IsReadable(path) == false )  //判断路径可读
	{
		return PATH_NOT_READ;
	}
	
    struct stat st;
    stat(path, &st);  //调用库函数stat获取信息
	res = st.st_size;
	
    return SUCCESS;
}

//获取路径所指向的文件或目录的创建时间
int TPathTool::GetCreateTime( TTime &res, const char *path )
{
	if ( IsExist(path) == false )  //判断路径存在
	{
		return PATH_NOT_EXIST;
	}

	if ( IsReadable(path) == false )  //判断路径可读
	{
		return PATH_NOT_READ;
	}
	
	struct stat st;
    stat(path, &st);  //调用库函数stat获取信息
	res.SetTime(st.st_ctime);

	return SUCCESS;
}

//获取路径所指向的文件或目录的最后访问时间
int TPathTool::GetAccessTime( TTime &res, const char *path )
{
	if ( IsExist(path) == false )  //判断路径存在
	{
		return PATH_NOT_EXIST;
	}

	if ( IsReadable(path) == false )  //判断路径可读
	{
		return PATH_NOT_READ;
	}
	
	struct stat st;
    stat(path, &st);  //调用库函数stat获取信息
	res.SetTime(st.st_atime);

	return SUCCESS;
}

//获取路径所指向的文件或目录的最后修改时间
int TPathTool::GetModifyTime( TTime &res, const char *path )
{
	if ( IsExist(path) == false )  //判断路径存在
	{
		return PATH_NOT_EXIST;
	}

	if ( IsReadable(path) == false )  //判断路径可读
	{
		return PATH_NOT_READ;
	}
	
	struct stat st;
    stat(path, &st);  //调用库函数stat获取信息
	res.SetTime(st.st_mtime);

	return SUCCESS;
}

//获取路径所指向的文件或目录的权限
//以linux数字格式返回权限，即000~777
int TPathTool::GetMode( int &res, const char *path )
{
	res = -1;  //设置权限默认值
	if ( IsExist(path) == false )  //判断路径存在
	{
		return PATH_NOT_EXIST;
	}

	if ( IsReadable(path) == false )  //判断路径可读
	{
		return PATH_NOT_READ;
	}
	
	struct stat st;
    stat(path, &st);  //调用库函数stat获取信息

	//将模式从8进制转换为10进制
	st.st_mode %= 512;

	int user = st.st_mode / 64;
	int group = st.st_mode / 8 % 8;
	int others = st.st_mode % 8;
	res = user * 10 * 10 + group * 10 + others;

	return SUCCESS;
}

//设置路径所指向的文件或目录的权限
int TPathTool::SetMode( const char *path, int mode )
{
	if ( TPathTool::IsExist(path) == false )  //判断路径存在
	{
		return PATH_NOT_EXIST;
	}

	if ( mode > 777 || mode < 000 ) //判断模式是否有效
	{
		return PATH_INVALID_MODE;
	}

	//将模式从10进制转换为8进制
	int user = mode / 100;
	int group = mode / 10 % 10;
	int others = mode % 10;
	mode = user * 8 * 8 + group * 8 + others;

	if( chmod( path, mode ) != 0 )  //调用库函数chmod
	{
		return FAIL;
	}

	return SUCCESS;
}

//重命名旧路径所指向的文件或目录
//目录更改相当于移动文件
int TPathTool::RenamePath( const char *oldpath, const char *newpath )
{
	if ( IsExist(oldpath) == false )  //判断旧路径是否存在
	{
		return PATH_NOT_EXIST;
	}
	
	if ( IsReadable(oldpath) == false )  //判断旧路径是否可读
	{
		return PATH_NOT_READ;
	}

	if ( IsExist(newpath) == true )  //判断新路径是否存在
	{
		return PATH_ALREADY_EXIST;
	}

	if ( rename(oldpath, newpath) != 0 )  //调用库函数rename
	{
		return FAIL;
	}

	return SUCCESS;
}

bool TPathTool::CompareBase( const char *lpath, const char *rpath )
{
	string lbasename, rbasename;

	//调用TPathTool方法
	TPathTool::GetBaseName(lbasename, lpath);
	TPathTool::GetBaseName(rbasename, rpath);

	if ( lbasename != rbasename )
	{
		return false;
	}
	
	return true;
}

bool TPathTool::CompareSuper( const char *lpath, const char *rpath )
{
	string lsuper, rsuper;
	
	//调用TPathTool方法
	TPathTool::GetSuperName(lsuper, lpath);
	TPathTool::GetSuperName(rsuper, rpath);

	TPathTool::AbsPath(lsuper, lsuper);
	TPathTool::AbsPath(rsuper, rsuper);

	if ( lsuper != rsuper )
	{
		return false;
	}

	return true;
}

