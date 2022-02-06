#include "TFile.h"
#include "TStrTool.h"
#include <unistd.h>
#include <cstring>
#include "TFileTool.h"

const int MAXSIZE = 1024;

TFile::TFile()
: m_FilePoint(NULL), m_FilePath(""), m_FileOpenMode("")
{
	;
}

TFile::~TFile()
{
	Close();
}

TFile::TFile( const TFile &other )
: m_FilePoint(NULL), m_FilePath(other.m_FilePath), m_FileOpenMode("")
{
	;
}


TFile::TFile( const string other )
: m_FilePoint(NULL), m_FilePath(other), m_FileOpenMode("")
{
	;
}

TFile & TFile::operator=( const TFile &other )
{
	Close();
	m_FilePath = other.m_FilePath;
	return *this;
}

TFile & TFile::operator=( const string other )
{
	Close();
	m_FilePath = other;
	return *this;
}

//以mode参数打开文件，mode参数类型与fopen一致，可百度查询
int TFile::Open( const char *mode )
{
	if ( m_FilePoint != NULL )  //判断文件是否已打开
	{
		return PATH_ALREADY_OPEN;
	}
	
	if ( TPathTool::IsExist(m_FilePath) == false ) //路径不存在
	{
		if ( (*mode) == 'r' )  //模式为只读，返回错误
		{
			return PATH_NOT_EXIST;
		}
	}
	else  //路径已存在
	{
		if ( TPathTool::IsFile(m_FilePath) == false )  //该路径不是文件
		{
			return PATH_NOT_FILE;
		}
			
		//模式为读写，文件权限不支持读或写，返回错误
		if ( strchr(m_FilePath.c_str(), '+') != NULL && ( IsReadable() == false || IsWriteable() == false ) )
		{
			return PATH_NOT_READ_AND_WRITE;
		}

		//模式为读，文件权限不支持读，返回错误
		if ( strchr(m_FilePath.c_str(), 'r') != NULL && IsReadable() == false )
		{
			return PATH_NOT_READ;
		}

		//模式为写，文件权限不支持写，放回错误
		if ( ( strchr(m_FilePath.c_str(), 'w') != NULL || strchr(m_FilePath.c_str(), 'a' ) != NULL ) && IsWriteable() == false )
		{
			return PATH_NOT_WRITE;
		}
		
	}
	
	m_FilePoint = fopen(m_FilePath.c_str(),mode);  //打开文件
	if ( m_FilePoint == NULL )
	{
		return FAIL;
	}
	m_FileOpenMode = mode;
	
	return SUCCESS;
}

//关闭文件
void TFile::Close()
{
	m_FileOpenMode.clear();
	
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return;
	}

	fclose(m_FilePoint);  //释放资源
	m_FilePoint = NULL;
}

//刷新缓冲区，将数据从缓冲区写入文件
void TFile::Flush()
{
	fflush(m_FilePoint); //调用库函数fflush
}

//获取文件路径
string TFile::GetFilePath()
{
	return m_FilePath;
}

//获取文件指针
FILE* TFile::GetFilePoint()
{
	return m_FilePoint;
}

//获取文件模式
string TFile::GetFileOpenMode()
{
	return m_FileOpenMode;
}

//清空文件内容
int TFile::Clear()
{
	if ( TFileTool::IsExist(m_FilePath) == false )
	{
		return PATH_NOT_EXIST;
	}
	Close();
	int id;
	id = Open("w");
	if ( id != SUCCESS )
	{
		return id;
	}
	Close();
	
	return SUCCESS;
}



//从文件读取指定的字节数，如果未给定或为负则读取所有。
int TFile::Read( string &res, long size )
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	if ( m_FileOpenMode.find("r") == string::npos && m_FileOpenMode.find("+") == string::npos)  //文件模式不可读
	{
		return PATH_NOT_READ;
	}
	
	res.clear();  //清除res
	
	if ( size == 0 )  //size为0，直接返回
	{
		return SUCCESS;
	}

	int filesize = GetSize();  //获取文件大小
	long curpos = 0;  
	Tell(curpos); //获取当前文件指针位置

	if ( curpos >= filesize )  //当前文件指针已到文件末尾，无法继续读取，直接放回
	{
		return SUCCESS;
	}

	if ( size >= filesize - curpos || size < 0 )  //读取字节数大于文件剩余字节或者size为负数，则转为读取剩下所有字节
	{
		size = filesize - curpos;
	}

	size++;  //fgets读取字符串时会在末尾自动添加'\0'，需要将size自增一字节才能读取size字节
	char str[size] = {'\0'};

	//循环读取文件，将字符串进行拼贴
	//若不存在换行符，则执行一次后，size = 0， 循环正常退出
	//存在换行符时，需要循环读取，此时又不需要将size自增一字节，所以将循环条件改为size > 1
	while ( size > 1 )
	{
		memset(str, 0, sizeof(str));
		fgets(str, size, m_FilePoint);  //fgets较特殊，不判断是否成功，以res字符串判断读取成功
		res += str;
		size -= strlen(str);
	}

	return SUCCESS;
	
}


//从文件读取整行，包括 "\n" 字符。
//如果指定了一个非负数的参数，则返回指定大小的字节数，包括 "\n" 字符。
int TFile::ReadLine( string &res, long size )
{
	if ( size >= 0 )  //size不为负数，执行Read方法
	{
		return Read( res, size );
	}
	
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	if ( m_FileOpenMode.find("r") == string::npos && m_FileOpenMode.find("+") == string::npos)  //文件模式不可读
	{
		return PATH_NOT_READ;
	}

	res.clear();
	char str[MAXSIZE] = {'\0'};

	//使用库函数fgets读取一行字符串
	fgets(str, MAXSIZE, m_FilePoint); //fgets较特殊，不判断是否成功，以res字符串判断读取成功
	res = str;
	
	return SUCCESS;
}

//读取所有行(直到结束符 EOF)并将结果存储到字符串数组中
int TFile::ReadLines(vector<string> &res)
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}
	
	if ( m_FileOpenMode.find("r") == string::npos && m_FileOpenMode.find("+") == string::npos)  //文件模式不可读
	{
		return PATH_NOT_READ;
	}

	//循环读取文件的每一行并存取
	string temp;
	while (1)
	{
		ReadLine(temp);
		if ( temp.empty() == true )
		{
			break;
		}
		res.push_back(temp);
	}
	return SUCCESS;
}

//以二进制方式从文件读取指定的字节数，如果未给定或为负则读取所有。
//第一参数为读取后数据要放入的地址
//第二参数为操作后实际读取到的字节数
//第三参数为打算读取的字节数
//该方法并不安全，请慎用该方法！！！
int TFile::ReadB( void *res, long &readsize, long size )
{
	readsize = 0;
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	if ( m_FileOpenMode.find("r") == string::npos && m_FileOpenMode.find("+") == string::npos)  //文件模式不可读
	{
		return PATH_NOT_READ;
	}

	if ( size == 0 )  //size为0，直接返回
	{
		return SUCCESS;
	}

	int filesize = GetSize();    //获取文件大小
	long curpos = 0;
	Tell(curpos); //获取当前文件指针位置

	if ( curpos >= filesize )  //当前文件指针已到文件末尾，无法继续读取，直接放回
	{
		return SUCCESS;
	}

	if ( size >= filesize - curpos || size < 0 )  //读取字节数大于文件剩余字节或者size为负数，则转为读取剩下所有字节
	{
		size = filesize - curpos;
	}

	readsize = fread(res,1,size,m_FilePoint);
	if ( readsize <= 0 )
	{
		return FAIL;
	}
	
	return SUCCESS;
}


////将字符串写入文件
int TFile::Write( const char *str, bool buffer )
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	if ( m_FileOpenMode.find("w") == string::npos && m_FileOpenMode.find("a") == string::npos && m_FileOpenMode.find("+") == string::npos)  //文件模式不可写
	{
		return PATH_NOT_WRITE;
	}

	//调用库函数fputs写入字符串
	if ( fputs(str, m_FilePoint) == -1 )
	{
		return FAIL;
	}

	//是否开启缓冲
	if ( buffer == false )
	{
		Flush();
	}
	
	return SUCCESS;
}

//将字符串数组写入文件中
//写入过程不会自动换行，若想换行请在数组中字符串末尾加入换行符
int TFile::WriteLines( vector<string> &strs, bool buffer )
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	if ( m_FileOpenMode.find("w") == string::npos && m_FileOpenMode.find("a") == string::npos && m_FileOpenMode.find("+") == string::npos)  //文件模式不可写
	{
		return PATH_NOT_WRITE;
	}

	//循环执行写操作，将所有字符串写入文件中
	for ( size_t i = 0; i < strs.size(); i++ )
	{
		if ( Write(strs[i]) != SUCCESS )
		{
			return FAIL;
		}
	}
	
	//是否开启缓冲
	if ( buffer == false )
	{
		Flush();
	}

	return SUCCESS;
}

//将二进制数据写入文件,必须指定写入字节大小,若为负数返回失败
int TFile::WriteB( void *data, long size, bool buffer )
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	if ( m_FileOpenMode.find("w") == string::npos && m_FileOpenMode.find("a") == string::npos && m_FileOpenMode.find("+") == string::npos)  //文件模式不可写
	{
		return PATH_NOT_WRITE;
	}

	if( size < 0 )
	{
		return FAIL;
	}

	if ( size == 0 )
	{
		return SUCCESS;
	}

	if ( fwrite(data, 1, size, m_FilePoint) <= 0 )
	{
		return FAIL;
	}
	
	//是否开启缓冲
	if ( buffer == false )
	{
		Flush();
	}
	
	return SUCCESS;
}


//移动文件读取指针到指定位置
//offset为开始的偏移量，也就是代表需要移动偏移的字节数。
//whence为可选参数，默认值为 0。给 offset 定义一个参数，表示要从哪个位置开始偏移；0 代表从文件开头开始算起，1 代表从当前位置开始算起，2 代表从文件末尾算起。
int TFile::Seek( long offset, int whence )
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	//调用库函数fseek移动文件指针
	if ( fseek( m_FilePoint, offset, whence) != 0 )
	{
		return FAIL;
	}
	
	return SUCCESS;
}

//返回文件当前位置。
//文件位置从0开始，到EOF结束，共EOF-0个字节
int TFile::Tell(long &res)
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	//调用库函数ftell返回文件指针位置
	res = ftell(m_FilePoint);
	return SUCCESS;
}

//从文件的首行首字符开始截断，截断文件为 size 个字符。无 size 表示从当前位置截断；截断之后后面的所有字符被删除。
int TFile::Truncate( long size )
{
	if ( m_FilePoint == NULL )  //文件未打开
	{
		return PATH_NOT_OPEN;
	}

	if ( m_FileOpenMode.find("w") == string::npos && m_FileOpenMode.find("a") == string::npos && m_FileOpenMode.find("+") == string::npos)  //在写模式下才可截断
	{
		return PATH_NOT_WRITE;
	}

	//size为负数，从当前位置截断
	if ( size < 0 )
	{
		Tell(size);
	}

	//调用库函数ftruncate截断文件
	ftruncate(fileno(m_FilePoint), size);
	return SUCCESS;
}


//获取文件大小，以字节为单位，失败返回-1
int TFile::GetSize()
{
	//调用TFileTool方法，失败返回-1，具体错误码请直接调用TFileTool查看
	int size = -1;
	TFileTool::GetFileSize(size, m_FilePath);
	return size;
}

//获取文件创建时间，失败返回默认时间
TTime TFile::GetCTime()
{
	//调用TFileTool方法，失败返回默认时间，具体错误码请直接调用TFileTool查看
	TTime ctime;
	TFileTool::GetFileCTime(ctime, m_FilePath);
	return ctime;
}

//获取文件访问时间，失败返回默认时间
TTime TFile::GetATime()
{
	//调用TFileTool方法，失败返回默认时间，具体错误码请直接调用TFileTool查看
	TTime atime;
	TFileTool::GetFileATime(atime, m_FilePath);
	return atime;
}


//获取文件修改时间，失败返回默认时间
TTime TFile::GetMTime()
{
	//调用TFileTool方法，失败返回默认时间，具体错误码请直接调用TFileTool查看
	TTime mtime;
	TFileTool::GetFileMTime(mtime, m_FilePath);
	return mtime;
}


//获取文件权限,失败返回-1
int TFile::GetMode()
{
	//调用TFileTool方法，失败返回-1，具体错误码请直接调用TFileTool查看
	int mode = -1;
	TFileTool::GetFileMode(mode, m_FilePath);
	return mode;
}

//设置文件权限，失败返回false
bool TFile::SetMode( int mode )
{
	//调用TFileTool方法，失败返回false，具体错误码请直接调用Tfiletool查看
	if ( TFileTool::SetFileMode(m_FilePath, mode) != SUCCESS )
	{
		return false;
	}
	return true;
}

//获取文件名，包括扩展名
string TFile::GetFileAllName()
{
	//调用TFileTool方法，失败返回空对象，具体错误码请直接调用Tfiletool查看
	string fileallname;
	TFileTool::GetFileAllName(fileallname, m_FilePath);
	return fileallname;
}

//获取文件名，不包括扩展名
string TFile::GetFileName()
{
	//调用TFileTool方法，失败返回空对象，具体错误码请直接调用Tfiletool查看
	string filename;
	TFileTool::GetFileName(filename, m_FilePath);
	return filename;
	
}

//获取文件扩展名
string TFile::GetFileExt()
{
	//调用TFileTool方法，失败返回空对象，具体错误码请直接调用Tfiletool查看
	string fileext;
	TFileTool::GetFileExt(fileext, m_FilePath);
	return fileext;
}

//获取文件所在目录
string TFile::GetFileSuper()
{
	//调用TFileTool方法，失败返回空对象，具体错误码请直接调用Tfiletool查看
	string filesuper;
	TFileTool::GetFileSuper(filesuper, m_FilePath);
	return filesuper;
}

//文件是否可读
bool TFile::IsReadable()
{
	return TFileTool::IsReadable(m_FilePath);
}

//文件是否可写
bool TFile::IsWriteable()
{
	return TFileTool::IsWriteable(m_FilePath);
}

//文件是否可执行
bool TFile::IsExecuteable()
{
	return TFileTool::IsExecuteable(m_FilePath);
}


