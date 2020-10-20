#pragma once
#include "d:\desktop\zwcad二次开发学习code\11第十一章：在objectarx中使用mfc\zrx2020\inc\zacdocman.h"
class CMyDocReactor : public AcApDocManagerReactor
{
public:
	CMyDocReactor();
	virtual ~CMyDocReactor();

	//用于禁用Line命令;
	virtual void documentLockModeChanged(AcApDocument* pDoc,
		AcAp::DocLockMode myPreviousMode,
		AcAp::DocLockMode myCurrentMode,
		AcAp::DocLockMode currentMode,
		const char* pGlobalCmdName);

	virtual void documentToBeDestroyed(AcApDocument* pDocToDestroy);
	virtual void documentCreated(AcApDocument* pDocCreating);
};

