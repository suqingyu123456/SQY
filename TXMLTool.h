/***************************************                                                           
     文件名：TXMLTool.h
     作者：苏青羽
     功能描述：提供XML格式字符串操作的常用方法
     创建日期：2022-02-07
     版本：v1.0
     最后修改日期：2022-02-07
***************************************/

#include "TStrTool.h"
#include <string>
using std::string;

class TXMLTool
{
	public:  //默认方法
		
			TXMLTool() = delete;
	
			~TXMLTool() = delete;
	
			TXMLTool( const TXMLTool& ) = delete;
	
			TXMLTool & operator=( const TXMLTool& ) = delete;

	public:

		//获取xml中的string类型字段值
		static int GetXMLValue( string &res, const char *xml, const char *field );

		//获取xml中的bool类型字段值
		static int GetXMLValue( bool &res, const char *xml, const char *field );

		//获取xml中的int类型字段值
		static int GetXMLValue( int &res, const char *xml, const char *field );

		//获取xml中的unsigned int类型字段值
		static int GetXMLValue( unsigned int &res, const char *xml, const char *field );

		//获取xml中的long类型字段值
		static int GetXMLValue( long &res, const char *xml, const char *field );

		//获取xml中的unsigned long类型字段值
		static int GetXMLValue( unsigned long &res, const char *xml, const char *field );

		//获取xml中的double类型字段值
		static int GetXMLValue( double &res, const char *xml, const char *field );

	public:

		//以Cstyle字符串类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const char *value );
	
		//以string类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const string &value )
		{
			return SetXMLValue(xml, field, value.c_str());
		}

		//以bool类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const bool value );

		//以int类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const int value );

		//以unsigned int类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const unsigned int value );

		//以long类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const long value );

		//以unsigned long类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const unsigned long value );

		//以double类型设置xml中的字段值, 原xml没有该字段则会被添加
		static int SetXMLValue( string &xml, const char *field, const double value );

};

