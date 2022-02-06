all: main

main: main.cpp TStrTool.cpp TPreDef.cpp TTime.cpp TTimeTool.cpp TPathTool.cpp TFile.cpp TFileTool.cpp TDir.cpp TDirTool.cpp
	g++ -Wall -g -o main main.cpp TStrTool.cpp TPreDef.cpp TTime.cpp TTimeTool.cpp TPathTool.cpp TFile.cpp TFileTool.cpp TDir.cpp TDirTool.cpp

clean:
	rm -rf main
