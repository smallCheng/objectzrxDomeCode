#include "stdafx.h"
#include "GePointUtil.h"


CGePointUtil::CGePointUtil()
{
}


CGePointUtil::~CGePointUtil()
{
}

AcGePoint3d CGePointUtil::GetMiddlePoint(const AcGePoint3d & pt1, const AcGePoint3d & pt2)
{
	double x = (pt1.x + pt2.x)*0.5;
	double y = (pt1.y + pt2.y)*0.5;
	double z = (pt1.z + pt2.z)*0.5;
	return AcGePoint3d(x, y, z);
}

AcGePoint2d CGePointUtil::GetMiddlePoint(const AcGePoint2d & pt1, const AcGePoint2d & pt2)
{
	double x = (pt1.x + pt2.x)*0.5;
	double y = (pt1.y + pt2.y)*0.5;
	return AcGePoint2d(x, y);
}

AcGePoint3d CGePointUtil::PolarPoint(const AcGePoint3d & basePoint, double angle, double length)
{
	double x = basePoint.x + length*cos(angle);
	double y = basePoint.y + length*sin(angle);

	return AcGePoint3d(x, y, basePoint.z);
}

AcGePoint3d CGePointUtil::RelativePoint(const AcGePoint3d & pt, double x, double y)
{
	AcGePoint3d ptReturn(pt.x + x, pt.y + y, pt.z);
	return ptReturn;
}
