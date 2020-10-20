#include "stdafx.h"
#include "LineUtil.h"


CLineUtil::CLineUtil()
{
}


CLineUtil::~CLineUtil()
{
}

AcDbObjectId CLineUtil::Add(const AcGePoint3d & startPoint, const AcGePoint3d & endPoint)
{
	//在堆区创建AcDbLine对象
	AcDbLine *pLine = new AcDbLine(startPoint, endPoint);
	return CDwgDatabaseUtil::PostToModelSpace(pLine);
}
