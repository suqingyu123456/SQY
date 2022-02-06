
#include "TPreDef.h"

//存储库中预定义枚举值与相对应的描述信息
unordered_map<int, const char*> PREDEFMAP = {

	{ FAIL, "[ALL]#Fail#"},
	{ SUCCESS, "[ALL]#Success#" },
	{ STR_DEAD_LOOP, "[String]#Exist dead loop in string method#"},
	{ STR_OVER_RANGE, "[String]#Index out of the range in string method#"},
	{ STR_OVER_SIZE, "[String]#Size of string parameter so long#"},
	{ STR_EMPTY_STR, "[String]#Exist empty string in string parameter#"},
	{ PATH_NOT_EXIST, "[Path]#Not such file or dir#"},
	{ PATH_ALREADY_EXIST, "[Path]#This file or Dir has been already exist#"},
	{ PATH_NOT_READ, "[Path]#This file or dir can't be read，or open-mode isn't read#"},
	{ PATH_NOT_WRITE, "[Path]#This file or dir can't be write，or open-mode isn't write#"},
	{ PATH_NOT_READ_AND_WRITE, "[Path]#This file or dir can't be read and write, or open-mode isn't read and write#"},
	{ PATH_NOT_OPEN, "[Path]#This file or dir isn't open#"},	
	{ PATH_ALREADY_OPEN, "[Path]#This file or dir has been already open#"},
	{ PATH_INVALID_MODE, "[Path]#This file or dir has got invalid mode#"},
	{ PATH_NOT_FILE, "[Path]#This path isn't file#"},
	{ PATH_NOT_DIR, "[Path]#This path isn't dir#"},
	{ PATH_NOT_SAME_SUPER, "[Path]#Two super aren't same#"},
	{ PATH_NOT_SAME_FILENAME, "[Path]#Two filename aren't same#"},
	{ PATH_NOT_SAME_DIRNAME, "[Path]#Two dirname aren't same#"}

};


const char * GetDescription( int id ) //获取枚举值对应的描述信息
{
	return PREDEFMAP[id];
}

