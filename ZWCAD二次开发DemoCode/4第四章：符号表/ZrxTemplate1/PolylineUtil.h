#pragma once
class CPolylineUtil
{
public:
	CPolylineUtil();
	~CPolylineUtil();

	//创建多段线（轻量多段线）（参数：点集、宽度）
	static AcDbObjectId Add(const AcGePoint2dArray &points, double width = 0);
	//创建多段线（一条直线）(参数：开始点、起始点、线宽)
	static AcDbObjectId Add(const AcGePoint2d &ptStart, const AcGePoint2d &ptEnd, double width);
	//创建三维多段线（参数：点集）
	static AcDbObjectId Add3dPolyline(const AcGePoint3dArray &points);
	//创建正多边形（参数：中心、边数、外接圆半径、旋转角度（弧度值）、线宽）（由多段线构建）
	static AcDbObjectId AddPolygon(const AcGePoint2d &ptCenter, int number, double radius, double rotation, double width);
	//创建矩形（参数：两个角点、线宽）（由多段线构建）
	static AcDbObjectId AddPolyRectangle(const AcGePoint2d &pt1, const AcGePoint2d &pt2, double width);
	//创建圆（参数：圆心、半径和线宽）（由多段线构建）
	static AcDbObjectId AddPolyCircle(const AcGePoint2d &ptCenter, double radius, double width);
	//创建圆弧（参数：圆心、半径、起始角度、终止角度、线宽）（由多段线构建）
	static AcDbObjectId AddPolyArc(const AcGePoint2d &ptCenter, double radius, double angleStart, double angleEnd, double width);
};

