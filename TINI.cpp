#include "TINI.h"
#include "TStrTool.h"


TINI::TINI( const TINI &other )
: m_INIFile(other.m_INIFile), m_INI(other.m_INI)
{
	;
}

TINI::TINI( const string &other )
: m_INIFile(other)
{
	;
}

TINI::TINI( const char *other )
: m_INIFile(other)
{
	;
}
	TINI & TINI::operator=( const TINI &other )
{
	m_INIFile = other.m_INIFile;
	m_INI = m_INI;
	return *this;
}

TINI & TINI::operator=( const string &other )
{
	m_INIFile = other;
	return *this;
}

TINI & TINI::operator=( const char *other )
{
	m_INIFile = other;
	return *this;
}

//从ini文件中导入数据，获取键值前必须先导入
int TINI::Import()
{
	int id;
	id = m_INIFile.Open("r");  //只读打开ini文件

	if ( id != SUCCESS )
	{
		return id;
	}

	Clear();  //清空原缓存

	string line;
	string root;

	while ( m_INIFile.ReadLine(line) == SUCCESS )  //遍历读取ini文件
	{
		size_t lpos = line.find("[");
		size_t rpos = line.find("]");
		if ( lpos != string::npos && rpos != string::npos )  //匹配根
		{
			root = line.substr( lpos + 1, rpos - lpos - 1 ); //去除"[]"
			TStrTool::RemoveLRBlank(root, root);  //去除空白
			continue;
		}
		

		size_t pos = line.find("=");  //匹配键

		if ( pos != string::npos )
		{
			string key = line.substr(0, pos);  //获取键
			string value = line.substr(pos + 1, line.size() - pos - 1 );  //获取键值
			
			TStrTool::RemoveLRBlank(key, key);  //去除空白
			TStrTool::RemoveLRBlank(value, value);

			
			//根，键，值均不为空，加入缓存
			if ( root.empty() != true && key.empty() != true && value.empty() != true )
			{
				m_INI[root][key] = value;
			}
			
		}
	}

	m_INIFile.Close();  //关闭文件

	return SUCCESS;
}

//将数据导出到ini文件
int TINI::Export()
{
	int id;
	id = m_INIFile.Clear();  //清除文件内容
	if ( id != SUCCESS )
	{
		return id;
	}
	
	id = m_INIFile.Open("w");  //只写打开ini文件
	if ( id != SUCCESS )
	{
		return id;
	}

	//遍历根
	for ( auto root_iter = m_INI.begin(); root_iter != m_INI.end(); root_iter++ )
	{
		//将根按格式写入文件
		id = m_INIFile.Write( string("[") + (root_iter -> first) + string("]") + "\n" );
		if ( id != SUCCESS )
		{
			return id;
		}

		//遍历键值
		for ( auto key_iter = (root_iter -> second).begin(); key_iter != (root_iter -> second).end(); key_iter++ )
		{	
			//将键值对按格式写入文件
			id = m_INIFile.Write( (key_iter -> first) + " = "  + (key_iter -> second) + "\n" );
			if ( id != SUCCESS )
			{
				return id;
			}
		}
	}

	m_INIFile.Close();  //关闭文件

	return SUCCESS;
}

//清除本地ini数据缓存
void TINI::Clear()
{
	m_INI.clear();  //调用clear
}

//根据根结点和键获取对应值，以string返回
int TINI::GetINIValue( string &res, const char *root, const char *key )
{
	res.clear();

	//根存在
	auto root_iter = m_INI.find(root);
	if ( root_iter != m_INI.end() )
	{
		auto key_iter = ( root_iter -> second ).find(key);

		//键值对存在
		if ( key_iter != ( root_iter -> second ).end() )
		{
			res = key_iter -> second;  //返回对应值
			return SUCCESS;
		}
	}

	return FAIL;  //搜索不到对应键值，返回错误
}

//根据根结点和键获取对应值，以bool返回
int TINI::GetINIValue( bool &res, const char *root, const char *key )
{
	string value;
	int id = GetINIValue(value, root, key ); //以string类型获取键值
	if ( id != SUCCESS )
	{
		return id;
	}

	if ( value == "true" )  //对应赋值
	{
		res = true;
	}
	else if ( value == "false" )
	{
		res = false;
	}
	else
	{
		return FAIL;
	}

	return SUCCESS;
}

//根据根结点和键获取对应值，以int返回
int TINI::GetINIValue( int &res, const char *root, const char *key )
{
	string value;
	int id = GetINIValue(value, root, key ); //以string类型获取键值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = atoi(value.c_str());  //调用atoi函数

	return SUCCESS;
}

//根据根结点和键获取对应值，以unsigned int返回
int TINI::GetINIValue( unsigned int &res, const char *root, const char *key )
{
	string value;
	int id = GetINIValue(value, root, key ); //以string类型获取键值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = (unsigned int)atoi(value.c_str());  //调用atoi函数

	return SUCCESS;
}

//根据根结点和键获取对应值，以long返回
int TINI::GetINIValue( long &res, const char *root, const char *key )
{
	string value;
	int id = GetINIValue(value, root, key ); //以string类型获取键值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = atol(value.c_str());  //调用atol函数

	return SUCCESS;
}

//根据根结点和键获取对应值，以unsigned long返回
int TINI::GetINIValue( unsigned long &res, const char *root, const char *key )
{
	string value;
	int id = GetINIValue(value, root, key ); //以string类型获取键值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = (unsigned long)atol(value.c_str());  //调用atol函数

	return SUCCESS;
}

//根据根结点和键获取对应值，以double返回
int TINI::GetINIValue( double &res, const char *root, const char *key )
{
	string value;
	int id = GetINIValue(value, root, key ); //以string类型获取键值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = atof(value.c_str());  //调用atof函数

	return SUCCESS;
}

//以Cstyle字符串类型设置ini键值，若不存在该结点则会被创建
int TINI::SetINIValue( const char *root, const char *key, const char *value )
{
	m_INI[root][key] = value;
	return SUCCESS;
}

//以bool类型设置ini键值，若不存在该结点则会被创建
int TINI::SetINIValue( const char *root, const char *key, const bool value )
{
	if ( value == true )
	{
		return SetINIValue(root, key, "true");
	}

	return SetINIValue(root, key, "false");
}

//以int类型设置ini键值，若不存在该结点则会被创建
int TINI::SetINIValue( const char *root, const char *key, const int value )
{
	return SetINIValue(root, key, std::to_string(value));
}

//以unsigned int类型设置ini键值，若不存在该结点则会被创建
int TINI::SetINIValue( const char *root, const char *key, const unsigned int value )
{
	return SetINIValue(root, key, std::to_string(value));
}

//以long类型设置ini键值，若不存在该结点则会被创建
int TINI::SetINIValue( const char *root, const char *key, const long value )
{
	return SetINIValue(root, key, std::to_string(value));
}

//以unsigned long字符串类型设置ini键值，若不存在该结点则会被创建
int TINI::SetINIValue( const char *root, const char *key, const unsigned long value )
{
	return SetINIValue(root, key, std::to_string(value));
}

//以double类型设置ini键值，若不存在该结点则会被创建
int TINI::SetINIValue( const char *root, const char *key, const double value )
{
	return SetINIValue(root, key, std::to_string(value));
}