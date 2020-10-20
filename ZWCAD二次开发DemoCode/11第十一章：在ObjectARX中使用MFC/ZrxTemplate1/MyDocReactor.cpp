#include "StdAfx.h"
#include "MyDocReactor.h"


CMyDocReactor::CMyDocReactor()
{

}

CMyDocReactor::~CMyDocReactor()
{

}

void CMyDocReactor::documentLockModeChanged( AcApDocument* pDoc, AcAp::DocLockMode myPreviousMode, 
	AcAp::DocLockMode myCurrentMode, AcAp::DocLockMode currentMode, 
	const char* pGlobalCmdName )
{
	if (strcmp(pGlobalCmdName, "LINE") == 0)
	{
		acutPrintf(TEXT("\nLine命令被当前软件禁止."));
		veto();
	}
}

void CMyDocReactor::documentToBeDestroyed( AcApDocument* pDocToDestroy )
{
	pDocToDestroy->database()->removeReactor(g_dbReactor);
}

void CMyDocReactor::documentCreated( AcApDocument* pDocCreating )
{
	pDocCreating->database()->addReactor(g_dbReactor);
}
