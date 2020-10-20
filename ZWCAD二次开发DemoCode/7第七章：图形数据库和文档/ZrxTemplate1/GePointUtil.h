#pragma once
class CGePointUtil
{
public:
	CGePointUtil();
	~CGePointUtil();

	//求两点的中点
	static AcGePoint3d GetMiddlePoint(const AcGePoint3d &pt1, const AcGePoint3d &pt2);
	//求两点的中点
	static AcGePoint2d GetMiddlePoint(const AcGePoint2d &pt1, const AcGePoint2d &pt2);
	//根据相对极坐标来确定一个点的位置
	static AcGePoint3d PolarPoint(const AcGePoint3d &basePoint, double angle, double length);
	//根据相对极坐标来确定一个点的位置
	static AcGePoint2d PolarPoint(const AcGePoint2d &basePoint, double angle, double length);
	//根据相对直角坐标来计算一个点的位置
	static AcGePoint3d RelativePoint(const AcGePoint3d &pt, double x, double y);
};

