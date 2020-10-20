#pragma once
class GeoMetryUtil
{
public:
	GeoMetryUtil();
	~GeoMetryUtil();
	//求两点的中点
	static AcGePoint3d GetMiddlePoint(const AcGePoint3d &pt1, const AcGePoint3d &pt2);
	//二维点转换为三维点
	static AcGePoint3d ToPoint3d(const AcGePoint2d &point2d, double z = 0);
	//PI
	static double PI();
	//角度转弧度
	static double AngleToRadian(double angle);
	//弧度转角度
	static double RadianToAngle(double radian);
	//根据相对极坐标来确定一个点的位置
	static AcGePoint3d PolarPoint(const AcGePoint3d &basePoint ,double angle, double length);
	//根据相对直角坐标来计算一个点的位置
	static AcGePoint3d RelativePoint(const AcGePoint3d &pt, double x, double y);



};

