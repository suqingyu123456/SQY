/***************************************
  文件名：TPreDef.h
  作者：苏青羽
  功能：为库提供预定义
  创建日期：2022-01-01
  版本：v1.0
  最后修改日期：2022-02-06
***************************************/

#ifndef _TPREDEF_H_
#define _TPREDEF_H_

#include <unordered_map>
using std::unordered_map;

const char * GetDescription( int id ); //获取枚举值对应的描述

enum{
	FAIL,  //失败，原因无法定位的失败，需调试解决，如C语言库函数错误
	SUCCESS  //成功
};

//字符串
enum {
	STR_START = SUCCESS,  //边界定义，请勿使用
		
    STR_DEAD_LOOP,  //在字符串方法里存在死循环
    STR_OVER_RANGE,  //在字符串方法中存在访问越界，如索引不正确
    STR_OVER_SIZE,  //字符串参数的长度过长
	STR_EMPTY_STR, //存在空字符串参数
    
    STR_END //边界定义，请勿使用
};

enum {
	PATH_START = STR_END,

	PATH_NOT_EXIST,  //该路径不存在或者为无效路径
	PATH_ALREADY_EXIST, //该路径已存在
	PATH_NOT_READ, //该路径指向的文件或目录没有读权限，或者文件没有以读模式打开
	PATH_NOT_WRITE,  //该路径指向的文件或目录没有写权限，或者文件没有以写模式打开
	PATH_NOT_READ_AND_WRITE,  //该路径指向的文件或目录没有读写权限， 或者文件没有以读写模式打开
	PATH_NOT_OPEN, //该路径指向的文件或目录没有打开
	PATH_ALREADY_OPEN,  //该路径指向的文件或目录已经打开
	PATH_INVALID_MODE, //该路径指向的文件或目录的权限为无效模式
	PATH_NOT_FILE,  //该路径不是文件
	PATH_NOT_DIR,  //该路径不是目录
	PATH_NOT_SAME_SUPER,  //两个路径的上级目录不相同
	PATH_NOT_SAME_FILENAME,  //两个文件的文件名不相同
	PATH_NOT_SAME_DIRNAME,  //两个目录的目录名不相同
	
	PATH_END
};

#endif
