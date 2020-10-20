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

AcGePoint2d CGePointUtil::PolarPoint(const AcGePoint2d & basePoint, double angle, double length)
{
	double x = basePoint.x + length*cos(angle);
	double y = basePoint.y + length*sin(angle);

	return AcGePoint2d(x, y);
}

AcGePoint3d CGePointUtil::RelativePoint(const AcGePoint3d & pt, double x, double y)
{
	AcGePoint3d ptReturn(pt.x + x, pt.y + y, pt.z);
	return ptReturn;
}

// 两个点是否相同
bool CGePointUtil::IsEqual( const AcGePoint3d &firstPoint, const AcGePoint3d &secondPoint, double tol /*= 1.0E-7*/ )
{
	return (fabs(firstPoint.x - secondPoint.x) < tol && 
		fabs(firstPoint.y - secondPoint.y) < tol && 
		fabs(firstPoint.z - secondPoint.z) < tol);
}

// 两个点是否相同
bool CGePointUtil::IsEqual( const AcGePoint2d &firstPoint, const AcGePoint2d &secondPoint, double tol /*= 1.0E-7*/ )
{
	return (fabs(firstPoint.x - secondPoint.x) < tol && 
		fabs(firstPoint.y - secondPoint.y) < tol);
}

// 在数组中查找某个点，返回点在数组中的索引，未找到则返回-1
int CGePointUtil::FindPoint( const AcGePoint3dArray &points, const AcGePoint3d &point, double tol /*= 1.0E-7*/ )
{
	for (int i = 0; i < points.length(); i++)
	{
		if (IsEqual(points[i], point, tol))
		{
			return i;
		}
	}

	return -1;
}

// 在数组中查找某个点，返回点在数组中的索引，未找到则返回-1
int CGePointUtil::FindPoint( const AcGePoint2dArray &points, const AcGePoint2d &point, double tol /*= 1.0E-7*/ )
{
	for (int i = 0; i < points.length(); i++)
	{
		if (IsEqual(points[i], point, tol))
		{
			return i;
		}
	}

	return -1;
}

// 从数组中过滤掉重复点
void CGePointUtil::FilterEqualPoints( AcGePoint3dArray &points, double tol /*= 1.0E-7*/ )
{
	for (int i = points.length() - 1; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (CMathUtil::IsEqual(points[i].x, points[j].x, tol) && CMathUtil::IsEqual(points[i].y, points[j].y, tol))
			{
				points.removeAt(i);
				break;
			}
		}
	}
}

// 从数组中过滤掉重复点
void CGePointUtil::FilterEqualPoints( AcGePoint3dArray &points, const AcGePoint2d &pt, double tol /*= 1.0E-7*/ )
{
	AcGePoint3dArray tempPoints;
	for (int i = 0; i < points.length(); i++)
	{
		if (CConvertUtil::ToPoint2d(points[i]).distanceTo(pt) > tol)
		{
			tempPoints.append(points[i]);
		}
	}

	points = tempPoints;
}

