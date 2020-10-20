#pragma once
class CCircleUtil
{
public:
	CCircleUtil();
	~CCircleUtil();

	//创建圆(参数：圆心、半径)
	static AcDbObjectId Add(const AcGePoint3d &centerPoint, double radius);
	//创建圆(参数：直径两端点)
	static AcDbObjectId Add(const AcGePoint3d &pt1, const AcGePoint3d &pt2);
	//创建圆（参数：圆上三点）
	static AcDbObjectId Add(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d &pt3);
	//创建圆（参数：圆上三点）
	static AcDbObjectId Add(const AcGePoint2d &pt1, const AcGePoint2d &pt2, const AcGePoint2d &pt3);
};

