#pragma once
#include "..\..\..\..\ZWCAD_2020_ZRXSDK\arxport\dbmain.h"

class CMyDbReactor : public AcDbDatabaseReactor
{
public:
	CMyDbReactor();
	virtual ~CMyDbReactor();

	virtual void objectAppended (const AcDbDatabase* dwg, const AcDbObject* dbObj);
};

