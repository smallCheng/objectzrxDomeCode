#pragma once
#include "..\..\..\..\ZWCAD_2020_ZRXSDK\arxport\aced.h"

class CMyEditorReactor : public AcEditorReactor
{
public:
	CMyEditorReactor();
	virtual ~CMyEditorReactor();

	virtual void commandWillStart(const char* cmdStr);
	virtual void commandEnded(const TCHAR* cmdStr);
};

