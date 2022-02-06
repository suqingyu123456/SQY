#include "TFileTool.h"
#include "TDirTool.h"

const int MAXSIZE = 1024; //字符串大小

//创建文件，文件的上级目录必须存在，否则不能创建
//第二参数为文件存在时是否覆盖原文件或同名目录，默认为不覆盖
//第三参数为设置权限，默认为644
int TFileTool::MakeFile( const char *filepath, bool cover, int mode )
{
	if ( TPathTool::IsExist(filepath) == true )  //路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsFile(filepath) )  //是否为文件
			{
				id = DelFile(filepath);  //删除原文件
			}
			else
			{
				id = TDirTool::DelFullDir(filepath);  //删除同名目录
			}
			
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else
		{
			return PATH_ALREADY_EXIST;  //文件已存在
		}
	}

	//以给定权限创建文件
	//由于open创建文件时权限受制于系统的umask，所以先以默认权限创建
	int fd = open( filepath, O_CREAT, S_IRUSR|S_IWUSR );
	if ( fd == -1 )
	{
		return FAIL;
	}
	close(fd);

	return TPathTool::SetMode(filepath, mode);  //更改文件权限
}

//创建文件，文件的上级目录不存在时，连同目录一起创建
//第二参数为文件存在时是否覆盖原文件或同名目录，默认为不覆盖
//第三参数为设置权限，默认为644
int TFileTool::MakeFullFile( const char *filepath, bool cover, int mode )
{
	if ( TPathTool::IsExist(filepath) == true )  //路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsFile(filepath) )  //是否为文件
			{
				id = DelFile(filepath);  //删除原文件
			}
			else
			{
				id = TDirTool::DelFullDir(filepath);  //删除同名目录
			}
			
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else
		{
			return PATH_ALREADY_EXIST;  //文件已存在
		}
	}

	string filesuper;
	int id = GetFileSuper(filesuper, filepath);  //获取文件上级目录
	if ( id != SUCCESS )
	{
		return id;
	}

	if ( TDirTool::IsExist(filesuper) == false ) //上级目录不存在，创建上级目录
	{
		id = TDirTool::MakeFullDir(filesuper);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	//以给定权限创建文件
	//由于open创建文件时，权限受制于系统的umask，所以先以默认权限创建
	int fd = open( filepath, O_CREAT, S_IRUSR|S_IWUSR );
	if ( fd == -1 )
	{
		return FAIL;
	}
	close(fd);

	return TPathTool::SetMode(filepath, mode);  //更改文件权限
}


//删除文件
int TFileTool::DelFile( const char *filepath )
{
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(filepath)  == false )  //是否为文件
	{
		return PATH_NOT_FILE;
	}
	
	if ( remove(filepath) != 0 )  //调用库函数remove
	{
		return FAIL;
	}
	
	return SUCCESS;
}

//复制文件
//目标文件的上级目录不存在时会被创建
//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
int TFileTool::CopyFile( const char *srcfilepath, const char *dstfilepath, bool cover )
{
	if ( TPathTool::IsExist(srcfilepath) == false )  //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}
	
	if ( TPathTool::IsFile(srcfilepath)  == false )  //是否为文件
	{
		return PATH_NOT_FILE;
	}
	
	if ( TPathTool::IsExist(dstfilepath) == true )  //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsFile(dstfilepath) )  //是否为文件
			{
				id = DelFile(dstfilepath);  //删除原文件
			}
			else
			{
				id = TDirTool::DelFullDir(dstfilepath);  //删除同名目录
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
	string tempfile = dstfilepath;
	tempfile += ".tmp";  //复制时先将内容复制为临时文件
	
	id = MakeFullFile(tempfile, cover);  //创建临时文件
	if ( id != SUCCESS )
	{
		return id;
	}

	TFile src(srcfilepath);  //源文件
	TFile tmp(tempfile);  //临时文件

	id = src.Open("r");  //只读打开源文件
	if ( id != SUCCESS )
	{
		return id;
	}

	id = tmp.Open("w");  //只写打开临时文件
	if ( id != SUCCESS )
	{
		return id;
	}

	//调用TFile方法，循环复制文件内容，支持文本文件与二进制文件复制
	char data[MAXSIZE] = {'\0'};
	long size = 0;
	while (1)
	{
		memset( data, 0, sizeof(data) );
		id = src.ReadB( data, size, MAXSIZE );
		if ( id != SUCCESS )
		{
			return id;
		}
		
		if ( size <= 0 )
		{
			break;
		}
		
		id = tmp.WriteB(data, size);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	RenameFile(tempfile, dstfilepath);  //将临时文件重命名为目标文件

	return SUCCESS;
}

//移动文件，源文件与目标文件的文件名必须一致
//目标文件的上级目录不存在时，目录会被创建
//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
int TFileTool::MoveFile( const char *srcfilepath, const char *dstfilepath, bool cover )
{
	if ( strcmp( srcfilepath, dstfilepath ) == 0 )  //源路径与目标路径是否一致
	{
		return SUCCESS;
	}

	if ( TPathTool::IsExist(srcfilepath) == false )  //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(srcfilepath)  == false )  //源路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	if ( TPathTool::CompareBase( srcfilepath, dstfilepath ) == false )  //比较文件名是否一致
	{
		return PATH_NOT_SAME_FILENAME;
	}

	if ( TPathTool::IsExist(dstfilepath) == true )  //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsFile(dstfilepath) )  //是否为文件
			{
				id = DelFile(dstfilepath);  //删除原文件
			}
			else
			{
				id = TDirTool::DelFullDir(dstfilepath);  //删除同名目录
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

	//目标文件的上级目录不存在时，目录会被创建
	string supername;
	GetFileSuper(supername, dstfilepath);
	if ( TDirTool::IsExist(supername) == false ) //上级目录不存在，创建上级目录
	{
		id = TDirTool::MakeFullDir(supername);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	//调用TPathTool重命名方法
	id = TPathTool::RenamePath(srcfilepath, dstfilepath);
	if ( id != SUCCESS )
	{
		return id;
	}
	
	return SUCCESS;
}

//重命名文件，源文件与目标文件的上级目录必须一致，即只能在同级目录下改名
//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
int TFileTool::RenameFile( const char *srcfilepath, const char *dstfilepath, bool cover )
{
	if ( strcmp( srcfilepath, dstfilepath ) == 0 )  //源路径与目标路径是否一致
	{
		return SUCCESS;
	}
	
	if ( TPathTool::IsExist(srcfilepath) == false )  //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(srcfilepath)  == false )  //源路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	if ( TPathTool::CompareSuper( srcfilepath, dstfilepath ) == false )  //比较上级目录是否一致
	{
		return PATH_NOT_SAME_SUPER;
	}

	if ( TPathTool::IsExist(dstfilepath) == true )   //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsFile(dstfilepath) )  //是否为文件
			{
				id = DelFile(dstfilepath);  //删除原文件
			}
			else
			{
				id = TDirTool::DelFullDir(dstfilepath);  //删除同名目录
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
	id = TPathTool::RenamePath(srcfilepath, dstfilepath);
	if ( id != SUCCESS )
	{
		return id;
	}

	return SUCCESS;
}

//移动且更改文件名
//第三参数为目标文件存在时是否覆盖目标文件或同名目录，默认为不覆盖
int TFileTool::MoveAndRenameFile( const char *srcfilepath, const char *dstfilepath, bool cover )
{
	if ( strcmp( srcfilepath, dstfilepath ) == 0 )  //源路径与目标路径是否一致
	{
		return SUCCESS;
	}
	
	if ( TPathTool::IsExist(srcfilepath) == false )   //源路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(srcfilepath)  == false )  //源路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	if ( TPathTool::IsExist(dstfilepath) == true )  //目标路径是否存在
	{
		if ( cover == true )  //是否覆盖
		{
			int id;
			if ( TPathTool::IsFile(dstfilepath) )  //是否为文件
			{
				id = DelFile(dstfilepath);  //删除原文件
			}
			else
			{
				id = TDirTool::DelFullDir(dstfilepath);  //删除同名目录
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

	//目标文件的上级目录不存在时，目录会被创建
	string supername;
	GetFileSuper(supername, dstfilepath);
	if ( TDirTool::IsExist(supername) == false ) //上级目录不存在，创建上级目录
	{
		id = TDirTool::MakeFullDir(supername);
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	//调用TPathTool重命名方法
	id = TPathTool::RenamePath(srcfilepath, dstfilepath);
	if ( id != SUCCESS )
	{
		return id;
	}

	return SUCCESS;
}

//分离文件名与拓展名，以自定义类型StrPair返回
int TFileTool::SplitExt( StrPair &res, const char *filepath )
{
	string filename;
	int id = 0;
	id = TPathTool::GetBaseName(filename, filepath);  //获取文件名
	if ( id != SUCCESS )
	{
		return id;
	}

	//判断是否有扩展名
	if ( filename.find(".") == string::npos )
	{
		res.first = filename;
		return SUCCESS;
	}

	//切割文件名，获取文件名与扩展名
	id = TStrTool::CutStr(res, filename, ".", -1);
	if ( id != SUCCESS )
	{
		return id;
	}
	
	return SUCCESS;
}

//判断文件是否存在
bool TFileTool::IsExist( const char *filepath )
{
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return false;
	}
	return TPathTool::IsFile(filepath);  //是否为文件
}

//判断文件是否可读
bool TFileTool::IsReadable( const char *filepath )
{
	if ( IsExist(filepath) == false )  //判断文件是否存在
	{
		return false;
	}	
	return TPathTool::IsReadable(filepath);  //判断是否可读
}


bool TFileTool::IsWriteable( const char *filepath )
{
	if ( IsExist(filepath) == false )  //判断文件是否存在
	{
		return false;
	}	
	return TPathTool::IsWriteable(filepath);  //判断是否可写
}

bool TFileTool::IsExecuteable( const char *filepath )
{
	if ( IsExist(filepath) == false )  //判断文件是否存在
	{
		return false;
	}	
	return TPathTool::IsExecuteable(filepath);  //判断文件是否可执行
}

//获取文件名，包括扩展名
int TFileTool::GetFileAllName( string &res, const char *filepath )
{
	return TPathTool::GetBaseName(res, filepath);  //调用TPathTool方法
}

//获取文件名，不包括扩展名
int TFileTool::GetFileName( string &res, const char *filepath )
{
	StrPair filename;
	int id = SplitExt(filename, filepath);  //分离文件名与拓展名
	if ( id != SUCCESS )
	{
		return id;
	}

	res = filename.first; //返回文件名
	
	return SUCCESS;
}

//获取文件扩展名，没有扩展名返回空对象
int TFileTool::GetFileExt( string &res, const char *filepath )
{
	StrPair filename;
	int id = SplitExt(filename, filepath);  //分离文件名与拓展名
	if ( id != SUCCESS )
	{
		return id;
	}

	res = filename.second;  //放回扩展名
	
	return SUCCESS;
}

//获取文件所在目录
int TFileTool::GetFileSuper( string &res, const char *filepath )
{
	return TPathTool::GetSuperName(res, filepath);  //调用TPathTool方法
}

//获取文件大小
int TFileTool::GetFileSize( int &res, const char *filepath )
{
	res = -1; //设置默认值
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(filepath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetSize(res, filepath);
}

//获取文件权限
int TFileTool::GetFileMode( int &res, const char *filepath )
{
	res = -1;  //设置默认值
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(filepath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetMode(res, filepath);
}

int TFileTool::SetFileMode( const char *filepath, int mode )
{
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(filepath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::SetMode(filepath, mode);
}

int TFileTool::GetFileCTime( TTime &res, const char *filepath )
{
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(filepath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetCreateTime(res, filepath);
}

int TFileTool::GetFileATime( TTime &res, const char *filepath )
{
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(filepath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetAccessTime(res, filepath);
}

int TFileTool::GetFileMTime( TTime &res, const char *filepath )
{
	if ( TPathTool::IsExist(filepath) == false )  //路径是否存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsFile(filepath) == false )  //该路径是否为文件
	{
		return PATH_NOT_FILE;
	}

	return TPathTool::GetModifyTime(res, filepath);
}

