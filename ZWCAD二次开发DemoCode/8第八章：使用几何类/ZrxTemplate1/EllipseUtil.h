#pragma once
class CEllipseUtil
{
public:
	CEllipseUtil();
	~CEllipseUtil();

	//创建椭圆(参数:椭圆中心、平面法向量、1/2长轴矢量、短轴/长轴比例)
	static AcDbObjectId Add(const AcGePoint3d &ptCenter, const AcGeVector3d &vecNormal, const AcGeVector3d &majorAxis, double ratio);
	//创建椭圆（参数：外接矩形的两个角点）
	static AcDbObjectId Add(const AcGePoint2d &pt1, const AcGePoint2d &pt2);
};

