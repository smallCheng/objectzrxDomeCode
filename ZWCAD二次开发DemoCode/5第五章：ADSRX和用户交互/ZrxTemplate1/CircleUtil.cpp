#include "stdafx.h"
#include "CircleUtil.h"


CCircleUtil::CCircleUtil()
{
}


CCircleUtil::~CCircleUtil()
{
}

AcDbObjectId CCircleUtil::Add(const AcGePoint3d & centerPoint, double radius)
{
	AcGeVector3d normal(0, 0, 1);
	AcDbCircle *pCircle = new AcDbCircle(centerPoint, normal, radius);
	return CDwgDatabaseUtil::PostToModelSpace(pCircle);
}

AcDbObjectId CCircleUtil::Add(const AcGePoint3d & pt1, const AcGePoint3d & pt2)
{
	AcGeVector3d normal(0, 0, 1);
	//计算圆心和半径
	AcGePoint3d center = CGePointUtil::GetMiddlePoint(pt1, pt2);
	double radius = center.distanceTo(pt1);
	//创建圆
	AcDbCircle *pCircle = new AcDbCircle(center, normal, radius);
	return CDwgDatabaseUtil::PostToModelSpace(pCircle);
}

AcDbObjectId CCircleUtil::Add(const AcGePoint3d & pt1, const AcGePoint3d & pt2, const AcGePoint3d & pt3)
{
	//使用数学方法
	double xysm = 0, xyse = 0, xy = 0;
	AcGePoint3d ptCenter;
	double radius = 0;

	xy = pow(pt1.x, 2) + pow(pt1.y, 2);
	xyse = xy - pow(pt3.x, 2) - pow(pt3.y, 2);
	xysm = xy - pow(pt2.x, 2) - pow(pt2.y, 2);
	xy = (pt1.x - pt2.x)*(pt1.y - pt3.y) - (pt1.x - pt3.x)*(pt1.y - pt2.y);

	//判断参数有效性
	if (fabs(xy) < 0.000001)
	{
		AfxMessageBox(_T("\n所输入的参数无法创建圆形！"));
		return AcDbObjectId::kNull;
	}

	//获取圆心和半径
	ptCenter.x = (xysm*(pt1.y - pt3.y) - xyse*(pt1.y - pt2.y)) / (2 * xy);
	ptCenter.y = (xyse*(pt1.x - pt2.x) - xysm*(pt1.x - pt3.x)) / (2 * xy);
	ptCenter.z = 0;
	radius = sqrt((pt1.x - ptCenter.x)*(pt1.x - ptCenter.x) + (pt1.y - ptCenter.y)*(pt1.y - ptCenter.y));
	if (radius < 0.000001)
	{
		AfxMessageBox(_T("\n半径过小！"));
		return AcDbObjectId::kNull;
	}
	//创建圆
	AcGeVector3d normal(0, 0, 1);
	AcDbCircle *pCricle = new AcDbCircle(ptCenter, normal, radius);
	return CDwgDatabaseUtil::PostToModelSpace(pCricle);
}

AcDbObjectId CCircleUtil::Add(const AcGePoint2d & pt1, const AcGePoint2d & pt2, const AcGePoint2d & pt3)
{
	AcGeCircArc2d geArc(pt1, pt2, pt3);
	AcGePoint3d ptCenter(geArc.center().x, geArc.center().y, 0);
	AcGeVector3d normal(0, 0, 1);
	AcDbCircle *pCircle = new AcDbCircle(ptCenter, normal, geArc.radius());

	return CDwgDatabaseUtil::PostToModelSpace(pCircle);
}
