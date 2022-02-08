#include "TXMLTool.h"

#define MAXSIZE 128 //字段名最大值

//获取xml中的string类型字段值
int TXMLTool::GetXMLValue( string &res, const char *xml, const char *field )
{
	//xml或者字段名为空，返回错误
	if ( TStrTool::IsEmptyStr(xml) == true || TStrTool::IsEmptyStr(xml) == true )
	{
		return STR_EMPTY_STR;
	}

	char pattern[MAXSIZE] = {'\0'};
	sprintf( pattern, "<%s>.*</%s>", field, field ); //设置正则表达式格式

	res = TStrTool::Search(xml, pattern);  //搜索字段

	if ( res.empty() == true )  //字段为空，搜索失败，返回错误
	{
		return FAIL;
	}

	res = TStrTool::Search(res, ">.*<");  //再次搜索

	res = res.substr(1, res.size() - 2 );  //裁剪字符串，返回字段值

	TStrTool::RemoveLRBlank(res, res);  //去除字段值左右空白

	return SUCCESS;
	
}

//获取xml中的bool类型字段值
int TXMLTool::GetXMLValue( bool &res, const char *xml, const char *field )
{
	string value;
	int id = GetXMLValue(value, xml, field ); //以string类型获取字段值
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

//获取xml中的int类型字段值
int TXMLTool::GetXMLValue( int &res, const char *xml, const char *field )
{
	string value;
	int id = GetXMLValue(value, xml, field ); //以string类型获取字段值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = atoi(value.c_str());  //调用atoi函数

	return SUCCESS;
}

//获取xml中的unsigned int类型字段值
int TXMLTool::GetXMLValue( unsigned int &res, const char *xml, const char *field )
{
	string value;
	int id = GetXMLValue(value, xml, field ); //以string类型获取字段值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = (unsigned int)atoi(value.c_str()); //调用atoi函数

	return SUCCESS;
}

//获取xml中的long类型字段值
int TXMLTool::GetXMLValue( long &res, const char *xml, const char *field )
{
	string value;
	int id = GetXMLValue(value, xml, field ); //以string类型获取字段值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = atol(value.c_str());  //调用atol函数

	return SUCCESS;
}

//获取xml中的unsigned long类型字段值
int TXMLTool::GetXMLValue( unsigned long &res, const char *xml, const char *field )
{
	string value;
	int id = GetXMLValue(value, xml, field ); //以string类型获取字段值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = (unsigned long)atol(value.c_str());  //调用atol函数

	return SUCCESS;
}


//获取xml中的double类型字段值
int TXMLTool::GetXMLValue( double &res, const char *xml, const char *field )
{
	string value;
	int id = GetXMLValue(value, xml, field ); //以string类型获取字段值
	if ( id != SUCCESS )
	{
		return id;
	}

	res = atof(value.c_str());  //调用atof函数

	return SUCCESS;
}

//以string类型设置xml中的字段值, 原xml没有该字段则会被添加
int TXMLTool::SetXMLValue( string &xml, const char *field, const char *value )
{
	//xml或者字段名为空，返回错误
	if ( TStrTool::IsEmptyStr(field) == true || TStrTool::IsEmptyStr(value) == true )
	{
		return STR_EMPTY_STR;
	}

	char pattern[MAXSIZE] = {'\0'};
	sprintf( pattern, "<%s>.*</%s>", field, field ); //设置正则表达式格式

	//搜索xml字段
	string oldvalue = TStrTool::Search(xml, pattern);

	//组合新字段
	string newvalue = string("<") + string(field) + string(">") + string(value) + string("</") + string(field) + string(">");

	if ( oldvalue.empty() )  //原xml没有该字段，添加该字段
	{
		xml += newvalue;
	}
	else  //xml有该字段，修改字段值
	{
		int id = TStrTool::ReplaceStr(xml, xml, oldvalue, newvalue );
		if ( id != SUCCESS )
		{
			return id;
		}
	}

	return SUCCESS;
}

//以bool类型设置xml中的字段值, 原xml没有该字段则会被添加
int TXMLTool::SetXMLValue( string &xml, const char *field, const bool value )
{
	if ( value == true )
	{
		return SetXMLValue(xml, field, "true");
	}

	return SetXMLValue(xml, field, "false");
}

//以int类型设置xml中的字段值, 原xml没有该字段则会被添加
int TXMLTool::SetXMLValue( string &xml, const char *field, const int value )
{
	return SetXMLValue(xml, field, std::to_string(value));
}

//以unsigned int类型设置xml中的字段值, 原xml没有该字段则会被添加
int TXMLTool::SetXMLValue( string &xml, const char *field, const unsigned int value )
{
	return SetXMLValue(xml, field, std::to_string(value));
}

//以long类型设置xml中的字段值, 原xml没有该字段则会被添加
int TXMLTool::SetXMLValue( string &xml, const char *field, const long value )
{
	return SetXMLValue(xml, field, std::to_string(value));
}

//以unsigned long类型设置xml中的字段值, 原xml没有该字段则会被添加
int TXMLTool::SetXMLValue( string &xml, const char *field, const unsigned long value )
{
	return SetXMLValue(xml, field, std::to_string(value));
}

//以double类型设置xml中的字段值, 原xml没有该字段则会被添加
int TXMLTool::SetXMLValue( string &xml, const char *field, const double value )
{
	return SetXMLValue(xml, field, std::to_string(value));
}




