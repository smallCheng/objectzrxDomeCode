#pragma once
#include "..\..\..\..\ZWCAD_2020_ZRXSDK\arxport\AcApDMgr.h"

class CMyDocReactor : public AcApDocManagerReactor
{
public:
	CMyDocReactor();
	virtual ~CMyDocReactor();

	//”√”⁄Ω˚”√Line√¸¡Ó;
	virtual void documentLockModeChanged(AcApDocument* pDoc,
		AcAp::DocLockMode myPreviousMode,
		AcAp::DocLockMode myCurrentMode,
		AcAp::DocLockMode currentMode,
		const char* pGlobalCmdName);

	virtual void documentToBeDestroyed(AcApDocument* pDocToDestroy);
	virtual void documentCreated(AcApDocument* pDocCreating);
};

