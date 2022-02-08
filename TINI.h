/***************************************                                                           
     文件名：TINI.h
     作者：苏青羽
     功能描述：提供INI文件操作的常用方法
     创建日期：2022-02-08
     版本：v1.0
     最后修改日期：2022-02-08
***************************************/

#include <string>
#include <unordered_map>
#include "TFile.h"
using std::string;

typedef unordered_map<string, string> INI_NODE;  //ini结点类型

typedef unordered_map<string, INI_NODE> INI;  //ini类型

class TINI
{
	public:  //默认方法

		//TINI支持string构造初始化以及赋值操作
		TINI() = default;
		
		~TINI() = default;

		TINI( const TINI &other );

		TINI( const string &other );

		TINI( const char *other );

		TINI & operator=( const TINI &other );

		TINI & operator=( const string &other );

		TINI & operator=( const char *other );

	public: //实现方法

		//根据根结点和键获取对应值，以string返回
		int GetINIValue( string &res, const char *root, const char *key );

		//根据根结点和键获取对应值，以bool返回
		int GetINIValue( bool &res, const char *root, const char *key );

		//根据根结点和键获取对应值，以int返回
		int GetINIValue( int &res, const char *root, const char *key );

		//根据根结点和键获取对应值，以unsigned int返回
		int GetINIValue( unsigned int &res, const char *root, const char *key );

		//根据根结点和键获取对应值，以long返回
		int GetINIValue( long &res, const char *root, const char *key );

		//根据根结点和键获取对应值，以unsigned long返回
		int GetINIValue( unsigned long &res, const char *root, const char *key );

		//根据根结点和键获取对应值，以double返回
		int GetINIValue( double &res, const char *root, const char *key );

	public:

		//以Cstyle字符串类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const char *value );

		//以string类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const string &value )
		{
			return SetINIValue(root, key, value.c_str());
		}

		//以bool类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const bool value );

		//以int类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const int value );

		//以unsigned int类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const unsigned int value );

		//以long类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const long value );

		//以unsigned long字符串类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const unsigned long value );

		//以double类型设置ini键值，若不存在该结点则会被创建
		int SetINIValue( const char *root, const char *key, const double value );

	public:

		//从ini文件中导入内存，获取键值前必须先导入
		int Import();

		//将数据导出到ini文件
		int Export();

		//清除本地ini数据缓存
		void Clear();
		
	private:
		TFile m_INIFile;

		INI m_INI;
};


