#pragma once
#include "d:\desktop\zwcad二次开发学习code\11第十一章：在objectarx中使用mfc\zrx2020\inc\zdbmain.h"
class CMyDbReactor : public AcDbDatabaseReactor
{
public:
	CMyDbReactor();
	virtual ~CMyDbReactor();

	virtual void objectAppended (const AcDbDatabase* dwg, const AcDbObject* dbObj);
};

