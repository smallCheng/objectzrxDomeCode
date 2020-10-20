#include "stdafx.h"
#include "TextStyleUtil.h"


CTextStyleUtil::CTextStyleUtil()
{
}


CTextStyleUtil::~CTextStyleUtil()
{
}

AcDbObjectId CTextStyleUtil::Add(const TCHAR *name, const TCHAR* fontFileName, const TCHAR *bigFontFileName)
{
	Acad::ErrorStatus es;
	AcDbTextStyleTable *pTextStyleTable = NULL;
	es = acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTable, AcDb::kForWrite);
	AcDbTextStyleTableRecord *pTextStyleRecord = new AcDbTextStyleTableRecord();
	es = pTextStyleRecord->setName(name);
	es = pTextStyleRecord->setBigFontFileName(bigFontFileName);	//大字体文件
	es = pTextStyleRecord->setFileName(fontFileName);	//字体文件
	es = pTextStyleRecord->setXScale(1.0);	//文字高宽比（一般这里都设置为1，在文字属性中决定高宽比）
	es = pTextStyleTable->add(pTextStyleRecord);
	AcDbObjectId styleId = pTextStyleRecord->objectId();
	pTextStyleTable->close();
	pTextStyleRecord->close();

	return styleId;
}

AcDbObjectId CTextStyleUtil::GetAt(const TCHAR * name)
{
	AcDbObjectId textStyleId;
	if (_tcslen(name) > 0)
	{
		AcDbTextStyleTable *pTextStyleTable = NULL;
		acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTable, AcDb::kForRead);
		pTextStyleTable->getAt(name, textStyleId);	//如果不存在，textStyleId不会被赋值
		pTextStyleTable->close();
	}
	return textStyleId;
}

void CTextStyleUtil::GetAll(std::vector<CString>& textStyles)
{
	textStyles.clear();

	AcDbTextStyleTable *ptextStyleTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(ptextStyleTbl, AcDb::kForRead);
	AcDbTextStyleTableIterator *pIt = NULL;
	ptextStyleTbl->newIterator(pIt);

	for (pIt->start(); !pIt->done(); pIt->step())
	{
		AcDbTextStyleTableRecord *pRcd = NULL;
		if (pIt->getRecord(pRcd, AcDb::kForWrite) == Acad::eOk)
		{
			TCHAR *szNmae = NULL;
			pRcd->getName(szNmae);
			if (_tcslen(szNmae) > 0)
			{
				textStyles.push_back(szNmae);
			}
			acutDelString(szNmae);

			pRcd->close();
		}
	}
	delete pIt;
	ptextStyleTbl->close();
}
