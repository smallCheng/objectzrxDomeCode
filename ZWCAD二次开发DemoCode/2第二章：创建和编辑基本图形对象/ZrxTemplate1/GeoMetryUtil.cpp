#include "stdafx.h"
#include "GeoMetryUtil.h"


GeoMetryUtil::GeoMetryUtil()
{
}


GeoMetryUtil::~GeoMetryUtil()
{
}

AcGePoint3d GeoMetryUtil::GetMiddlePoint(const AcGePoint3d &pt1, const AcGePoint3d &pt2)
{
	double x = (pt1.x + pt2.x)*0.5;
	double y = (pt1.y + pt2.y)*0.5;
	double z = (pt1.z + pt2.z)*0.5;
	return AcGePoint3d(x, y, z);
}

AcGePoint3d GeoMetryUtil::ToPoint3d(const AcGePoint2d &point2d, double z /*= 0*/)
{
	return AcGePoint3d(point2d.x, point2d.y, z);
}

double GeoMetryUtil::PI()
{
	return atan(1.0) * 4;
}

double GeoMetryUtil::AngleToRadian(double angle)
{
	return angle*PI() / 180;
}

double GeoMetryUtil::RadianToAngle(double radian)
{
	return radian * 180 / PI();
}

AcGePoint3d GeoMetryUtil::PolarPoint(const AcGePoint3d & basePoint, double angle, double length)
{
	double x = basePoint.x + length*cos(angle);
	double y = basePoint.y + length*sin(angle);

	return AcGePoint3d(x, y, basePoint.z);
}

AcGePoint3d GeoMetryUtil::RelativePoint(const AcGePoint3d & pt, double x, double y)
{
	AcGePoint3d ptReturn(pt.x + x, pt.y + y, pt.z);
	return ptReturn;
}
