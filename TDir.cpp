#include "TDir.h"
#include "TDirTool.h"


TDir::TDir()
: m_DirPath(""), m_DirPoint(NULL)
{
	;
}

TDir::~TDir()
{
	Close();
}


TDir::TDir( const TDir &other )
: m_DirPath(other.m_DirPath), m_DirPoint(NULL)
{
	;
}


TDir::TDir( const string other )
:m_DirPath(other), m_DirPoint(NULL)
{
	;
}

TDir & TDir::operator=( const TDir &other )
{
	Close();
	m_DirPath = other.m_DirPath;
	return *this;
}

TDir & TDir::operator=( const string other )
{
	Close();
	m_DirPath = other;
	return *this;
}

//打开目录
int TDir::Open()
{
	if ( TPathTool::IsExist(m_DirPath) == false )  //判断路径存在
	{
		return PATH_NOT_EXIST;
	}

	if ( TPathTool::IsDir(m_DirPath) == false )  //判断路径是否为目录
	{
		return PATH_NOT_DIR;
	}
	
	if ( TPathTool::IsReadable(m_DirPath) == false )  //判断是否可读
	{
		return PATH_NOT_READ;
	}
		
	m_DirPoint = opendir(m_DirPath.c_str());  //调用库函数opendir

	if ( m_DirPoint == NULL )  //失败返回Fail
	{
		return FAIL;
	}

	return SUCCESS;
}

//关闭目录
void TDir::Close()
{
	if ( m_DirPoint == NULL )  //目录未打开
	{
		return;
	}

	closedir(m_DirPoint);  //调用库函数closedir，释放资源
	m_DirPoint = NULL;
}

//重新打开目录
int TDir::Reopen()
{
	Close();
	
	return Open();
}


//列出目录下文件，包括目录和文件，但不包括子目录下的次级文件
//第二参数为是否显示隐藏文件，默认不显示
//第三参数为正则表达式的匹配模式，默认为空
int TDir::List( vector<string> &res, bool display, const string pattern )
{
	int id = Reopen();  //重新打开目录
	if ( id != SUCCESS )  //失败返回id
	{
		return id;
	}

	res.clear();  //清空
	
	struct dirent *st_dir;
	string path;
	
	while ( ( st_dir = readdir(m_DirPoint) ) != NULL )  //通过循环调用库函数readdir，读取目录
	{
		if (  display == false && *(st_dir -> d_name) == '.' )  //是否读取隐藏文件
		{
			continue;
		}
		
		if ( TStrTool::Match(st_dir -> d_name, pattern) == true )  //是否匹配
		{
			path.clear();
			TPathTool::JointPath(path, m_DirPath, st_dir -> d_name );  //连接目录与文件名
			res.push_back(path);
		}
	}

	return SUCCESS;
}

//只列出目录下文件，不包括目录，也不包括子目录下的次级文件
//第二参数为是否显示隐藏文件，默认不显示
//第三参数为正则表达式的匹配模式，默认为空
int TDir::ListFile( vector<string> &res, bool display, const string pattern )
{
	int id = Reopen();  //重新打开目录
	if ( id != SUCCESS )  //失败返回id
	{
		return id;
	}

	res.clear();  //清空
	
	struct dirent *st_dir;
	string path;
	
	while ( ( st_dir = readdir(m_DirPoint) ) != NULL )  //通过循环调用库函数readdir，读取目录
	{	
		if ( display == false && *(st_dir -> d_name) == '.' )  //是否读取隐藏文件
		{
			continue;
		}
		
		path.clear();
		TPathTool::JointPath(path, m_DirPath, st_dir -> d_name );  //连接目录与文件名

		//是否为文件且匹配
		if ( TPathTool::IsFile(path) == true && TStrTool::Match(st_dir -> d_name, pattern) == true )
		{
			res.push_back(path);
		}
	}

	return SUCCESS;
}

//只列出目录下目录，不包括文件，也不包括子目录下的次级文件
//第二参数为是否显示隐藏文件，默认不显示
//第三参数为正则表达式的匹配模式，默认为空
int TDir::ListDir( vector<string> &res, bool display, const string pattern )
{
	int id = Reopen();  //重新打开目录
	if ( id != SUCCESS )  //失败返回id
	{
		return id;
	}

	res.clear();  //清空
	
	struct dirent *st_dir;
	string path;
	
	while ( ( st_dir = readdir(m_DirPoint) ) != NULL )  //通过循环调用库函数readdir，读取目录
	{	
		if ( display == false && *(st_dir -> d_name) == '.' )   //是否读取隐藏文件
		{
			continue;
		}
		
		path.clear();
		TPathTool::JointPath(path, m_DirPath, st_dir -> d_name );  //连接目录与文件名

		//是否为目录且匹配
		if ( TPathTool::IsDir(path) == true && TStrTool::Match(st_dir -> d_name, pattern) == true )
		{
			res.push_back(path);
		}
	}

	return SUCCESS;
}

//列出目录下所有文件，包括目录和文件，包括子目录下的次级文件
//第二参数为是否显示隐藏文件，默认不显示
//该方法若设置匹配模式会有歧义，因此不能设置匹配模式
int TDir::ListFull( vector<string> &res, bool display )
{
	res.clear();
	return _ListFull(res, display);
}

//ListFull实现方法，内含递归调用
int TDir::_ListFull( vector<string> &res, bool display )
{  
	int id = Reopen();  //重新打开目录
	if ( id != SUCCESS )  //失败返回id
	{
		return id;
	}

	struct dirent *st_dir;
	string path;
	
	while ( ( st_dir = readdir(m_DirPoint) ) != NULL )  //通过循环调用库函数readdir，读取目录
	{
		if ( display == false && *(st_dir -> d_name) == '.' )  //是否读取隐藏文件
		{
			continue;
		}
		
		path.clear();
		TPathTool::JointPath(path, m_DirPath, st_dir -> d_name );  //连接目录与文件名
		res.push_back(path);

		//这两个目录为当前目录和上级目录，进入会导致无限递归，跳过
		if ( strcmp( st_dir -> d_name, "." ) == 0 || strcmp( st_dir -> d_name, ".." ) == 0 ) 
		{
			continue;
		}

		//是目录进入递归调用
		if ( TPathTool::IsDir(path) == true )
		{
			TDir sub_dir(path);
			id = sub_dir._ListFull( res, display );
			if ( id != SUCCESS )
			{
				return id;
			}
		}

	}

	return SUCCESS;
}

//列出目录下所有文件，不包括目录，包括子目录下的次级文件
//第二参数为是否显示隐藏文件，默认不显示
//第三参数为正则表达式的匹配模式，默认为空
int TDir::ListFullFile( vector<string> &res, bool display, const string pattern )
{
	res.clear();
	return _ListFullFile(res, display, pattern);
}

//ListFullFile实现方法，内含递归调用
int TDir::_ListFullFile( vector<string> &res, bool display, const string pattern )
{
	int id = Reopen();  //重新打开目录
	if ( id != SUCCESS )  //失败返回id
	{
		return id;
	}

	struct dirent *st_dir;
	string path;
	
	while ( ( st_dir = readdir(m_DirPoint) ) != NULL )  //通过循环调用库函数readdir，读取目录
	{
		if ( display == false && *(st_dir -> d_name) == '.' )  //是否读取隐藏文件
		{
			continue;
		}

		//这两个目录为当前目录和上级目录，进入会导致无限递归，跳过
		if ( strcmp( st_dir -> d_name, "." ) == 0 || strcmp( st_dir -> d_name, ".." ) == 0 )
		{
			continue;
		}

		path.clear();
		TPathTool::JointPath(path, m_DirPath, st_dir -> d_name ); //连接目录与文件名

		//是目录进入递归调用
		if ( TPathTool::IsDir(path) == true )
		{
			TDir sub_dir(path);
			id = sub_dir._ListFullFile( res, display, pattern );
			if ( id != SUCCESS )
			{
				return id;
			}
		}
		else if ( TStrTool::Match(st_dir -> d_name, pattern) == true )
		{	
			//是文件且匹配成功，加入结果数组
			res.push_back(path);
		}

	}

	return SUCCESS;
}

//列出目录下所有子目录，不包括文件，包括子目录下的次级目录
//第二参数为是否显示隐藏文件，默认不显示
//第三参数为正则表达式的匹配模式，默认为空
int TDir::ListFullDir( vector<string> &res, bool display, const string pattern )
{
	res.clear();
	return _ListFullDir(res, display, pattern);
}


//ListFullDir实现方法，内含递归调用
int TDir::_ListFullDir( vector<string> &res, bool display, const string pattern )
{
	int id = Reopen();  //重新打开目录
	if ( id != SUCCESS )  //失败返回id
	{
		return id;
	}

	struct dirent *st_dir;
	string path;
	
	while ( ( st_dir = readdir(m_DirPoint) ) != NULL )  //通过循环调用库函数readdir，读取目录
	{
		if ( display == false && *(st_dir -> d_name) == '.' )  //是否读取隐藏文件
		{
			continue;
		}

		path.clear();
		TPathTool::JointPath(path, m_DirPath, st_dir -> d_name );  //连接目录与文件名

		//判断是否为目录
		if ( TPathTool::IsDir(path) == true )
		{	
			//匹配成功
			if ( TStrTool::Match(st_dir -> d_name, pattern) == true )
			{
				res.push_back(path);
			}

			//这两个目录为当前目录和上级目录，进入会导致无限递归，跳过
			if ( strcmp( st_dir -> d_name, "." ) == 0 || strcmp( st_dir -> d_name, ".." ) == 0 )
			{
				continue;
			}

			//递归调用
			TDir sub_dir(path);
			id = sub_dir._ListFullDir( res, display, pattern );
			if ( id != SUCCESS )
			{
				return id;
			}
		}

	}

	return SUCCESS;
}

//获取目录路径
string TDir::GetDirPath()
{
	return m_DirPath;
}

//获取目录指针
DIR * TDir::GetDirPoint()
{
	return m_DirPoint;
}

//获取目录大小，以字节为单位，失败返回-1
int TDir::GetSize()
{
	//调用TDirTool方法，失败返回-1，具体错误码请直接调用TDirTool查看
	int size = -1;
	TDirTool::GetDirSize(size, m_DirPath);
	return size;
}

//获取目录创建时间，失败返回默认时间
TTime TDir::GetCTime()
{
	//调用TDirTool方法，失败返回默认时间，具体错误码请直接调用TDirTool查看
	TTime ctime;
	TDirTool::GetDirCTime(ctime, m_DirPath);
	return ctime;
}

//获取目录访问时间，失败返回默认时间
TTime TDir::GetATime()
{
	//调用TDirTool方法，失败返回默认时间，具体错误码请直接调用TDirTool查看
	TTime atime;
	TDirTool::GetDirATime(atime, m_DirPath);
	return atime;
}

//获取目录修改时间，失败返回默认时间
TTime TDir::GetMTime()
{
	//调用TDirTool方法，失败返回默认时间，具体错误码请直接调用TDirTool查看
	TTime mtime;
	TDirTool::GetDirATime(mtime, m_DirPath);
	return mtime;
}


//获取目录权限,失败返回-1
int TDir::GetMode()
{
	//调用TDirTool方法，失败返回-1，具体错误码请直接调用TDirTool查看
	int mode = -1;
	TDirTool::GetDirMode(mode, m_DirPath);
	return mode;
}

//设置目录权限，失败返回false
bool TDir::SetMode( int mode )
{
	//调用TDirTool方法，失败返回false，具体错误码请直接调用TDirTool查看
	if ( TDirTool::SetDirMode(m_DirPath, mode) != SUCCESS )
	{
		return false;
	}
	return true;
}

//获取目录名
string TDir::GetDirName()
{
	//调用TDirTool方法，失败返回空对象，具体错误码请直接调用TDirTool查看
	string dirname;
	TDirTool::GetDirName(dirname, m_DirPath);
	return dirname;
}

//获取目录所在上级目录
string TDir::GetDirSuper()
{
	//调用TDirTool方法，失败返回空对象，具体错误码请直接调用TDirTool查看
	string dirsuper;
	TDirTool::GetDirSuper(dirsuper, m_DirPath);
	return dirsuper;
}

//目录是否可读
bool TDir::IsReadable()
{
	return TDirTool::IsReadable(m_DirPath);
}

//文件是否可写
bool TDir::IsWriteable()
{
	return TDirTool::IsWriteable(m_DirPath);
}

//文件是否可执行
bool TDir::IsExecuteable()
{
	return TDirTool::IsExecuteable(m_DirPath);
}

