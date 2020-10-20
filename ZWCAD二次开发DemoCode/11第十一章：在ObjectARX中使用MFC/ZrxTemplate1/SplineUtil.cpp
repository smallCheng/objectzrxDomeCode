#include "stdafx.h"
#include "SplineUtil.h"


CSplineUtil::CSplineUtil()
{
}


CSplineUtil::~CSplineUtil()
{
}

AcDbObjectId CSplineUtil::Add(const AcGePoint3dArray & points, int order, double fitTolerance)
{
	assert(order >= 2 && order <= 26);
	AcDbSpline *pSpline = new AcDbSpline(points, order, fitTolerance);
	return CDwgDatabaseUtil::PostToModelSpace(pSpline);
}

AcDbObjectId CSplineUtil::Add(const AcGePoint3dArray & points, const AcGeVector3d & startTangent, const AcGeVector3d & endTangent, int order, double fitTolerance)
{
	assert(order >= 2 && order <= 26);
	AcDbSpline *pSpline = new AcDbSpline(points, startTangent, endTangent, order, fitTolerance);
	return CDwgDatabaseUtil::PostToModelSpace(pSpline);
}
