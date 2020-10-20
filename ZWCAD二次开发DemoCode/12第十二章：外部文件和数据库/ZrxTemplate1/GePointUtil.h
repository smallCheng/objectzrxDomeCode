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
	// 两个点是否相同
	static bool IsEqual(const AcGePoint3d &firstPoint, const AcGePoint3d &secondPoint, double tol = 1.0E-7);
	static bool IsEqual(const AcGePoint2d &firstPoint, const AcGePoint2d &secondPoint, double tol = 1.0E-7);

	// 数组中是否包含某个点
	// 返回: 点在数组中的索引，如果返回值小于0表示数组中不包含这个点
	static int FindPoint(const AcGePoint3dArray &points, const AcGePoint3d &point, double tol = 1.0E-7);
	static int FindPoint(const AcGePoint2dArray &points, const AcGePoint2d &point, double tol = 1.0E-7);

	// 从数组中过滤掉重复点
	// points: 点数组
	// tol: 判断点重合时的精度（两点之间的距离小于tol认为这两个点重合）
	static void FilterEqualPoints(AcGePoint3dArray &points, double tol = 1.0E-7);
	static void FilterEqualPoints(AcGePoint3dArray &points, const AcGePoint2d &pt, double tol = 1.0E-7);
};

