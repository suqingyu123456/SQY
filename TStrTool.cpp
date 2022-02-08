
#include "TStrTool.h"
#include <cstring>
#include <cstdarg>


//转化字符串，将小写字母转为大写字母，忽略非小写字母
int TStrTool::Upper( string &res, const char *str)
{
	if ( IsEmptyStr(str) == true )
	{
	  	//str为空
		return STR_EMPTY_STR;
	}

	res = str;
	for ( size_t i = 0; i < res.size(); i++ )
	{
		if ( str[i] >= 'a' && str[i] < 'z' )
		{
			res[i] -= 32;  //遍历字符串，遇到小写字母则转化
		}
	}

	return SUCCESS;
}

//转化字符串，将大写字母转为小写字母，忽略非大写字母
int TStrTool::Lower( string &res, const char *str )
{
	if ( IsEmptyStr(str) == true )
	{
	  	//str为空
		return STR_EMPTY_STR;
	}
	
	res = str;
	for ( size_t i = 0; i < res.size(); i++ )
	{
		if ( str[i] >= 'A' && str[i] < 'Z' )
		{
			res[i] += 32;  //遍历字符串，遇到大写字母则转化
		}
	}
	
	return SUCCESS;
}


//删除字符串左边子字符串，直到执行成功或者字符串被清空
int TStrTool::RemoveLStr( string &res, const char *str, const char *leftstr )
{
	if ( IsEmptyStr(str) == true || IsEmptyStr(leftstr) == true )
	{
		//str或者leftstr为空
		return STR_EMPTY_STR;
	}

	if ( strlen(str) < strlen(leftstr) )
	{
		//要删除的字符串比原字符串长
		return STR_OVER_SIZE;
	}

	//循环寻找子字符串，若子字符串不位于字符串最左边，则返回截取结果
	const char *pos = strstr( str, leftstr );
	while ( pos != NULL && pos == str )
	{
		str += strlen(leftstr); 
		pos = strstr( str, leftstr );
	}

	res = str;
	return SUCCESS;
}


//删除字符串右边子字符串，直到执行成功或者字符串被清空
int TStrTool::RemoveRStr( string &res, const char *str, const char *rightstr )
{
	if ( IsEmptyStr(str) == true || IsEmptyStr(rightstr) == true )
	{
		//str或者rightstr为空
		return STR_EMPTY_STR;
	}

	if ( strlen(str) < strlen(rightstr) )
	{
		//要删除的字符串比原字符串长
		return STR_OVER_SIZE;
	}

	//循环寻找最右边子字符串，若子字符串不位于字符串最右边，则返回截取结果
	res = str;
	for ( size_t pos = res.rfind(rightstr); pos != string::npos; pos = res.rfind(rightstr) )
	{
		if ( pos + strlen(rightstr) != res.size() )
		{
			break;
		}
		res.erase( pos, strlen(rightstr) );
	}
	
	return SUCCESS;
}


//删除字符串左右两边子字符串，直到执行成功或者字符串被清空
int TStrTool::RemoveLRStr( string &res, const char *str, const char *lrstr )
{
	int left = RemoveLStr( res, str, lrstr); //删除左边
	if ( left != SUCCESS )
	{
		return left;
	}
	return RemoveRStr( res, res, lrstr);  //删除右边并返回
}

//删除str字符串中的oldstr子字符串，loop为是否循环删除
int TStrTool::RemoveStr( string &res, const char *str, const char *oldstr, bool loop)
{
	//str或oldstr为空字符串，不用删除，直接返回原字符串
	if ( IsEmptyStr(str) == true || IsEmptyStr(oldstr) == true) 
	{
		//str或者oldstr为空
		return STR_EMPTY_STR;
	}

	if ( strlen(oldstr) > strlen(str) )
	{
		//要删除的字符串比原字符串长
		return STR_OVER_SIZE;
	}
	
	res = str;
	if ( loop == true )  //循环删除
	{
		//搜索oldstr并删除，删除后在结果字符串中从头搜索，直到搜索失败
		for ( size_t pos = res.find(oldstr); pos != string::npos; pos = res.find(oldstr) )
		{
		 	res.erase( pos, strlen(oldstr) );
		}
	}
	else  //不循环删除
	{
		//搜索oldstr并删除，删除后在结果字符串中从当前位置搜索，直到搜索失败
		for ( size_t pos = res.find(oldstr); pos != string::npos; pos = res.find(oldstr, pos) )
		{
			res.erase( pos, strlen(oldstr) );
		}
	}

	return SUCCESS;
}


//删除字符串左边空白，包括空格与不可打印字符，直到执行成功或者字符串为空
int TStrTool::RemoveLBlank( string &res, const char *str )
{
	if ( IsEmptyStr(str) == true )
	{  
		//str为空
		return STR_EMPTY_STR;
	}

	int i = 0, size = strlen(str);
	for ( ; i < size; i++ )
	{
		if ( str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r' )
		{
			break;
		}
	}

	res = str + i;
	return SUCCESS;
}

//删除字符串右边空白，包括空格与不可打印字符，直到执行成功或者字符串为空
int TStrTool::RemoveRBlank( string &res, const char *str )
{
	if ( IsEmptyStr(str) == true )
	{  
		//str为空
		return STR_EMPTY_STR;
	}

	int i = strlen(str) - 1;
	for ( ; i >= 0; i-- )
	{
		if ( str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r' )
		{
			break;
		}
	}

	res = string( str, str + i + 1 );
	return SUCCESS;
}


//删除字符串左右两边空白，包括空格与不可打印字符，直到执行成功或者字符串为空
int TStrTool::RemoveLRBlank( string &res, const char *str )
{
	int left = RemoveLBlank( res, str); //删除左空白
	if ( left != SUCCESS )
	{
		return left;
	}
	return RemoveRBlank( res, res);  //删除右空白并返回
}



//替换str字符串中的子字符串，将oldstr字符串替换为newstr字符串，loop为是否循环替换，默认为true
int TStrTool::ReplaceStr( string &res, const char *str, const char *oldstr, const char *newstr, bool loop )
{
	//str或oldstr为空字符串，不用执行替换，直接返回原字符串
	if ( IsEmptyStr(str) == true || IsEmptyStr(oldstr) == true )
	{
	 	//str或者oldstr为空
		return STR_EMPTY_STR;
	}

	if ( strlen(oldstr) > strlen(str) )
	{
		//要替换的字符串比原字符串长
		return STR_OVER_SIZE;
	}
	
	if ( IsEmptyStr(newstr) == true )  //newstr为空，相当于删除子字符串
	{
		return RemoveStr(res, str, oldstr, loop);
	}

	//若执行循环替换，且newstr中包含oldstr，存在逻辑错误, 无法替换，直接返回原字符串
	if ( loop == true && strstr(newstr,oldstr) != NULL )  
	{
		return STR_DEAD_LOOP;
	}
	
	res = str;
	if ( loop == true )  //循环替换
	{
		//搜索oldstr并替换，替换后在结果字符串中从头搜索，直到搜索失败
		for ( size_t pos = res.find(oldstr); pos != string::npos; pos = res.find(oldstr) )
		{
		 	res.erase( pos, strlen(oldstr) );
			res.insert( pos, newstr );
		}
	}
	else
	{
		//搜索oldstr并替换，替换后在结果字符串中从当前位置+newstr长度的位置继续搜索，直到搜索失败
		for ( size_t pos = res.find(oldstr); pos != string::npos; pos = res.find(oldstr, pos + strlen(newstr) ) )
		{
		 	res.erase( pos, strlen(oldstr) );
			res.insert( pos, newstr );
		}
	}
	
	return SUCCESS;
}

//将str字符串以spstr字符串为分隔符号进行分隔
int TStrTool::SplitStr( vector<string> &res, const char *str, const char *spstr )
{
	
	if ( IsEmptyStr(str) == true || IsEmptyStr(spstr) == true )
	{
		//str或者spstr为空
		return STR_EMPTY_STR;
	}

	if ( strlen(spstr) > strlen(str) )
	{
		//分隔符长度比源字符串长
		return STR_OVER_SIZE;
	}

	//遍历源字符串寻找分隔符，并将分隔出来的子字符串放进结果数组内
	const char *pos = strstr( str, spstr );
	while ( pos != NULL )
	{
		res.push_back( string( str, pos ) );
		str = pos + strlen(spstr);
		pos = strstr(str, spstr);
	}
	res.push_back( string(str) );

	return SUCCESS;
}

//将str字符串以cutstr字符串为分隔符进行切割两分化，返回自定义类型StrPair为结果
//cutindex切割点，默认为0, 0为左边第一个切割字符串索引，自左向右递增，可为负数，自右向左递减
//请看以下用例
//CutStr(res, "root/program/test", "/" ) = root 与 program/test
//CutStr(res, "root/program/test", "/", 1 ) = root/program 与 test
//CutStr(res, "root/program/test", "/", -1 ) = root/program 与 test
//CutStr(res, "root/program/test", "/", -2) = root 与 program/test
int TStrTool::CutStr( StrPair &res, const char *str, const char *cutstr, int cutindex )
{
	
	if ( IsEmptyStr(str) == true || IsEmptyStr(cutstr) == true )
	{
		//str或者cutstr为空
		return STR_EMPTY_STR;
	}
	
	if ( strlen(cutstr) > strlen(str) )
	{
		//分隔符长度比源字符串长
		return STR_OVER_SIZE;
	}
	
	string tempstr(str);
	size_t pos;
	
	if ( cutindex >= 0 )  //索引大于或等于0
	{
		//从左边遍历源字符串，寻找分隔符
		pos = -1;
		while ( cutindex >= 0 )
		{
			pos = tempstr.find(cutstr, pos + 1);
			if ( pos == string::npos )  //索引指定分隔符不存在，切割操作越界，直接返回
			{
				return STR_OVER_RANGE;
			}
			cutindex--;
		}
	}
	else  //索引小于0
	{
		//从右边遍历源字符串，寻找分隔符
		pos = strlen(str);
		while ( cutindex < 0 )
		{
			pos = tempstr.rfind(cutstr, pos - 1);
			if ( pos == string::npos ) //索引指定分隔符不存在，切割操作越界，直接返回
			{
				return STR_OVER_RANGE;
			}
			cutindex++;
		}
	}

	//分隔点已找到，将字符串切割两分化，并对结果赋值
	res.first = string( tempstr, 0, pos );
	res.second = string( tempstr, pos + strlen(cutstr), tempstr.size() );
	
	return SUCCESS;
}

//按正则表达式规则匹配字符串，完全匹配返回true，否则返回false
bool TStrTool::Match( const char *str, const char *pattern )
{
	if ( IsEmptyStr(pattern) == true )  //正则表达为空，返回true
	{
		return true;
	}
	
	std::regex reg(pattern);  //使用regex
	return std::regex_match( str, reg );  //调用regex_match函数
}

//按正则表达式规则搜索字符串，搜索成功返回第一个匹配的字符串，否则返回空对象
vector<string> TStrTool::SearchFull( const char *str, const char *pattern )
{
	vector<string> res;
	if ( IsEmptyStr(pattern) == true )  //正则表达为空，返回空数组
	{
		return res;
	}
			
	std::regex reg(pattern);  //使用regex
			
	std::cmatch cma;  //使用cmatch，存放结果
				
	while ( regex_search(str, cma, reg) )  //调用regex_search函数
	{
		res.push_back(cma[0]);  //循环查找匹配结果，并存入数组
		str = cma[0].second;
	}

	return res;
}

string TStrTool::Search( const char *str, const char *pattern )
{
	if ( IsEmptyStr(pattern) == true )  //正则表达为空，返回空字符串
	{
		return "";
	}
			
	std::regex reg(pattern);  //使用regex
			
	std::cmatch cma;  //使用cmatch，存放结果
	
	bool res = regex_search(str, cma, reg);  //调用regex_search函数

	if ( res == true )  //查找成功，返回匹配字符串
	{
		return cma[0];
	}

	return "";
}

//按正则表达式规则替换字符串，返回替换后的字符串
string TStrTool::Replace( const char *str, const char *pattern, const char *newstr )
{
	if ( IsEmptyStr(pattern) == true )  //正则表达为空，返回空字符串
	{
		return "";
	}
			
	std::regex reg(pattern);  //使用regex

	return regex_replace(str, reg, newstr );  //调用regex_replace函数
			
}

