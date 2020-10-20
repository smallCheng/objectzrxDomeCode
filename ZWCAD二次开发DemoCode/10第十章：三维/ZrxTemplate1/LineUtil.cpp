#include "stdafx.h"
#include "LineUtil.h"


CLineUtil::CLineUtil()
{
}


CLineUtil::~CLineUtil()
{
}

AcDbObjectId CLineUtil::Add(const AcGePoint3d & startPoint, const AcGePoint3d & endPoint,AcDbDatabase *pDb)
{
	//在堆区创建AcDbLine对象
	AcDbLine *pLine = new AcDbLine(startPoint, endPoint);
	return CDwgDatabaseUtil::PostToModelSpace(pLine,pDb);
}

bool CLineUtil::ThreePointIsCollinear(const AcGePoint2d & pt1, const AcGePoint2d & pt2, const AcGePoint2d & pt3)
{
	double xy = pt1.x*pt1.x + pt1.y + pt1.y;
	double xyse = xy - pt3.x*pt3.x - pt3.y*pt3.y;
	double syxm = xy - pt2.x*pt2.x - pt2.y*pt2.y;

	xy = (pt1.x - pt2.x)*(pt1.y - pt3.y) - (pt1.x - pt3.x)*(pt1.y - pt2.y);
	return (fabs(xy) < 1.0E-5);
}

int CLineUtil::PtInLeftOfLine(const AcGePoint3d & ptSratt, const AcGePoint3d & ptEnd, const AcGePoint3d & pt, double tol)
{
	return PtInLeftOfLine(ptSratt.x, ptSratt.y, ptEnd.x, ptEnd.y, pt.x, pt.y, tol);
}

int CLineUtil::PtInLeftOfLine(const AcGePoint2d & ptSratt, const AcGePoint2d & ptEnd, const AcGePoint2d & pt, double tol)
{
	return PtInLeftOfLine(ptSratt.x, ptSratt.y, ptEnd.x, ptEnd.y, pt.x, pt.y, tol);
}

int CLineUtil::PtInLeftOfLine(double x1, double y1, double x2, double y2, double x3, double y3, double tol)
{
	//两个矢量的叉乘结果是这两个矢量确定平行四边形的面积
	double a = (x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1);
	if (fabs(a) < tol)
	{
		return 0;
	}
	else if(a>0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

