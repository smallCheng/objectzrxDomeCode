#pragma once
class CArcUtil
{
public:
	CArcUtil();
	~CArcUtil();

	//创建圆弧（参数：圆心、平面法向量、半径、起始角度、终止角度）
	static AcDbObjectId Add(const AcGePoint3d &ptCenter, const AcGeVector3d &vec, double radius, double startAngle, double endAngle);
	//在XOY平面创建圆弧（参数：圆心、半径、起始角度、终止角度）
	static AcDbObjectId Add(const AcGePoint2d &ptCenter, double radius, double startAngle, double endAngle);
	//创建圆弧（参数：起点、终点、弧上一点）
	static AcDbObjectId Add(const AcGePoint2d &ptStart, const AcGePoint2d &ptOnArc, const AcGePoint2d &ptEnd);
	//创建圆弧（参数：起点、终点、圆心）
	static AcDbObjectId AddSCE(const AcGePoint2d &ptStart, const AcGePoint2d &ptCenter, const AcGePoint2d &ptEnd);
	//创建圆弧（参数：起点、圆心、圆弧角度）
	static AcDbObjectId Add(const AcGePoint2d &ptStart, const AcGePoint2d &ptCenter, double angle);
};

