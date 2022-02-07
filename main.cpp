#include <iostream>
#include "TStrTool.h"
#include <vector>
#include "TTime.h"
#include "TTimeTool.h"
#include "TPathTool.h"
#include "TFile.h"
#include "TFileTool.h"
#include "TDir.h"
#include "TDirTool.h"
//#include "TLog.h"
using namespace std;

//TFile类构造支持=char*


int main()
{
	TFile s = "./temp";
	cout << s.GetFilePath() << endl;
	//TLog a;
	
	//cout << a.GetFilePath() << endl;
	//if ( a.GetFilePoint() != NULL )
	//{
	//	cout << 1 << endl;
	//}
    return 0;
}
