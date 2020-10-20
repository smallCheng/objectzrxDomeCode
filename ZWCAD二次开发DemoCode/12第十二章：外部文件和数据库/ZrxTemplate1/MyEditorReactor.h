#pragma once
#include "d:\desktop\zwcad二次开发学习code\11第十一章：在objectarx中使用mfc\zrx2020\inc\zaced.h"
class CMyEditorReactor : public AcEditorReactor
{
public:
	CMyEditorReactor();
	virtual ~CMyEditorReactor();

	virtual void commandWillStart(const char* cmdStr);
	virtual void commandEnded(const TCHAR* cmdStr);
};

