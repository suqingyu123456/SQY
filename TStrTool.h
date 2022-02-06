/***************************************                                                           
     文件名：TStrTool.h
     作者：苏青羽
     功能：提供各种常用的字符串操作方法，支持Cstyle字符串和C++字符串
     创建日期：2022-01-01
     版本：v1.0
     最后修改日期：2022-02-06
***************************************/

#ifndef _TSTRTOOL_H_
#define _TSTRTOOL_H_

#include <string>
#include <utility>
#include <vector>
#include <cstring>
#include <regex>
#include "TPreDef.h"
using std::string;
using std::pair;
using std::vector;

typedef pair<string, string> StrPair;

class TStrTool
{
	public:  //默认方法
		
			TStrTool() = delete;
	
			~TStrTool() = delete;
	
			TStrTool( const TStrTool& ) = delete;
	
			TStrTool & operator=( const TStrTool& ) = delete;

    public:  //实现方法
	
		//判断空字符串
		static bool IsEmptyStr( const string &str )
		{
			return str.empty();
		}
		static bool IsEmptyStr( const char *str )
		{
			return str == NULL || strlen(str) < 1;
		}
		
		//转化字符串，将小写字母转为大写字母，忽略非小写字母
		static int Upper( string &res, const char *str);
		static int Upper( string &res, const string &str )
		{
			return Upper( res, str.c_str() );
		}

		//转化字符串，将大写字母转为小写字母，忽略非大写字母
		static int Lower( string &res, const char *str );
		static int Lower( string &res, const string &str )
		{
			return Lower( res, str.c_str() );
		}
		
		//删除字符串左边子字符串，直到执行成功或者字符串被清空
		static int RemoveLStr( string &res, const char *str, const char *leftstr );
		static int RemoveLStr( string &res, const string &str, const char *leftstr )
        {
        	return RemoveLStr( res, str.c_str(), leftstr);
        }
		static int RemoveLStr( string &res, const char *str, const string &leftstr )
        {
        	return RemoveLStr( res, str, leftstr.c_str() );
        }
		static int RemoveLStr( string &res, const string &str, const string &leftstr )
        {
        	return RemoveLStr( res, str.c_str(), leftstr.c_str() );
        }

		//删除字符串右边子字符串，直到执行成功或者字符串被清空
		static int RemoveRStr( string &res, const char *str, const char *rightst );
		static int RemoveRStr( string &res, const string &str, const char *rightstr )
        {
        	return RemoveRStr( res, str.c_str(), rightstr );
        }
		static int RemoveRStr( string &res, const char *str, const string &rightstr )
        {
        	return RemoveRStr( res, str, rightstr.c_str() );
        }
		static int RemoveRStr( string &res, const string &str, const string &rightstr )
        {
        	return RemoveRStr( res, str.c_str(), rightstr.c_str() );
        }

		//删除字符串左右两边子字符串，直到执行成功或者字符串被清空
		static int RemoveLRStr( string &res, const char *str, const char *lrstr );
		static int RemoveLRStr( string &res, const string &str, const char *lrstr )
		{
			return RemoveLRStr( res, str.c_str(), lrstr );
		}
		static int RemoveLRStr( string &res, const char *str, const string &lrstr )
		{
			return RemoveLRStr( res, str, lrstr.c_str() );
		}
		static int RemoveLRStr( string &res, const string &str, const string &lrstr )
        {
        	return RemoveLRStr( res, str.c_str(), lrstr.c_str() );
        }

		//删除str字符串中的oldstr子字符串，loop为是否循环删除
		static int RemoveStr( string &res, const char *str, const char *oldstr, bool loop = true);
		static int RemoveStr( string &res, const string &str, const char *oldstr, bool loop = true)
		{
			return RemoveStr( res, str.c_str(), oldstr, loop );
		}
		static int RemoveStr( string &res, const char *str, const string &oldstr, bool loop = true)
		{
			return RemoveStr( res, str, oldstr.c_str(), loop );
		}
		static int RemoveStr( string &res, const string &str, const string &oldstr, bool loop = true)
		{
			return RemoveStr( res, str.c_str(), oldstr.c_str(), loop );
		}

		//删除字符串左边空白，包括空格与不可打印字符，直到执行成功或者字符串为空
		static int RemoveLBlank( string &res, const char *str );
		static int RemoveLBlank( string &res, const string &str )
		{
			return RemoveLBlank( res, str.c_str() );
		}

		//删除字符串右边空白，包括空格与不可打印字符，直到执行成功或者字符串为空
		static int RemoveRBlank( string &res, const char *str );
		static int RemoveRBlank( string &res, const string &str )
		{
			return RemoveRBlank( res, str.c_str() );
		}

		//删除字符串左右两边空白，包括空格与不可打印字符，直到执行成功或者字符串为空
		static int RemoveLRBlank( string &res, const char *str );
		static int RemoveLRBlank( string &res, const string &str )
		{
			return RemoveLRBlank( res, str.c_str() );
		}

		//替换str字符串中的子字符串，将oldstr字符串替换为newstr字符串，loop为是否循环替换，默认为true
		static int ReplaceStr( string &res, const char *str, const char *oldstr, const char *newstr, bool loop = true);
		static int ReplaceStr( string &res, const string &str, const char *oldstr, const char *newstr, bool loop = true)
		{
			return ReplaceStr( res, str.c_str(), oldstr, newstr, loop );
		}
		static int ReplaceStr( string &res, const char *str, const string &oldstr, const char *newstr, bool loop = true)
		{
			return ReplaceStr( res, str, oldstr.c_str(), newstr, loop );
		}
		static int ReplaceStr( string &res, const char *str, const char *oldstr, const string &newstr, bool loop = true)
		{
			return ReplaceStr( res, str, oldstr, newstr.c_str(), loop );
		}
		static int ReplaceStr( string &res, const string &str, const string &oldstr, const char *newstr, bool loop = true)
		{
			return ReplaceStr( res, str.c_str(), oldstr.c_str(), newstr, loop );
		}
		static int ReplaceStr( string &res, const string &str, const char *oldstr, const string &newstr, bool loop = true)
		{
			return ReplaceStr( res, str.c_str(), oldstr, newstr.c_str(), loop );
		}
		static int ReplaceStr( string &res, const char *str, const string &oldstr, const string &newstr, bool loop = true)
		{
			return ReplaceStr( res, str, oldstr.c_str(), newstr.c_str(), loop );
		}
		static int ReplaceStr( string &res, const string &str, const string &oldstr, const string &newstr, bool loop = true)
        {
        	return ReplaceStr( res, str.c_str(), oldstr.c_str(), newstr.c_str(), loop );
        }

		//将str字符串以spstr字符串为分隔符号进行分隔
		static int SplitStr( vector<string> &res, const char *str, const char *spstr );
		static int SplitStr( vector<string> &res, const char *str, const string &spstr )
        {
        	return SplitStr( res, str, spstr.c_str() );
        }
		static int SplitStr( vector<string> &res, const string &str, const char *spstr )
        {
        	return SplitStr( res, str.c_str(), spstr );
        }
		static int SplitStr( vector<string> &res, const string &str, const string &spstr )
        {
        	return SplitStr( res, str.c_str(), spstr.c_str() );
        }

		//将str字符串以cutstr字符串为分隔符进行切割两分化，返回自定义类型StrPair为结果
		//cutindex切割点，默认为0, 0为左边第一个切割字符串索引，自左向右递增，可为负数，自右向左递减
		//请看以下用例
		//CutStr(res, "root/program/test", "/" ) = root 与 program/test
		//CutStr(res, "root/program/test", "/", 1 ) = root/program 与 test
		//CutStr(res, "root/program/test", "/", -1 ) = root/program 与 test
		//CutStr(res, "root/program/test", "/", -2) = root 与 program/test
		static int CutStr( StrPair &res, const char *str, const char *cutstr, int cutindex = 0 );
        static int CutStr( StrPair &res, const string &str, const string &cutstr, int cutindex = 0 )
        {
        	return CutStr( res, str.c_str(), cutstr.c_str(), cutindex );
        }
		static int CutStr( StrPair &res, const string &str, const char *cutstr, int cutindex = 0 )
        {
        	return CutStr( res, str.c_str(), cutstr, cutindex );
        }
		static int CutStr( StrPair &res, const char *str, const string &cutstr, int cutindex = 0 )
        {
        	return CutStr( res, str, cutstr.c_str(), cutindex );
        }

		//C++11已实现正则表达式，这里只是对其regex_match函数简单封装
		//表达式使用规则请百度regex库
		//C++11表达式功能强大，更多功能请百度
		static bool Match( const char *str, const char *pattern )
		{
			if ( IsEmptyStr(pattern) == true )
			{
				return true;
			}
			std::regex reg(pattern);
			return std::regex_match( str, reg );
		}
		static bool Match( const string &str, const char *pattern )
		{
			if ( IsEmptyStr(pattern) == true )
			{
				return true;
			}
			std::regex reg(pattern);
			return std::regex_match( str, reg );
		}
		static bool Match( const char *str, const string &pattern )
		{
			if ( pattern.empty() == true )
			{
				return true;
			}
			std::regex reg(pattern);
			return std::regex_match( str, reg );
		}
		static bool Match( const string &str, const string &pattern )
		{
			if ( pattern.empty() == true )
			{
				return true;
			}
			std::regex reg(pattern);
			return std::regex_match( str, reg );
		}

};

#endif
