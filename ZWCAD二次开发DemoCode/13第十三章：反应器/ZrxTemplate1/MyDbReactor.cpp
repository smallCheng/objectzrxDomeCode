#include "StdAfx.h"
#include "MyDbReactor.h"


CMyDbReactor::CMyDbReactor()
{

}

CMyDbReactor::~CMyDbReactor()
{

}

void CMyDbReactor::objectAppended( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
	if (dbObj->isKindOf(AcDbEntity::desc()) && const_cast<AcDbObject*>(dbObj)->upgradeOpen() == Acad::eOk)
	{
		AcDbEntity *pEnt = AcDbEntity::cast(dbObj);
		pEnt->setColorIndex(1);
	}
}
