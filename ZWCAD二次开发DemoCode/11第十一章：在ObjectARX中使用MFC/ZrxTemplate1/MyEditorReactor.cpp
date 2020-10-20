#include "StdAfx.h"
#include "MyEditorReactor.h"


CMyEditorReactor::CMyEditorReactor()
{

}

CMyEditorReactor::~CMyEditorReactor()
{

}

void CMyEditorReactor::commandEnded( const TCHAR* cmdStr )
{
	acutPrintf(TEXT("\n%s命令执行结束了."), cmdStr);
}

void CMyEditorReactor::commandWillStart( const char* cmdStr )
{
	acutPrintf(TEXT("\n%s命令将要开始执行."), cmdStr);
}
