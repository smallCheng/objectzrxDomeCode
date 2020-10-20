#pragma once
class CSplineUtil
{
public:
	CSplineUtil();
	~CSplineUtil();

	//创建样条曲线（参数：拟合点、阶数、允许拟合误差）
	static AcDbObjectId Add(const AcGePoint3dArray &points, int order = 4, double fitTolerance = 0.0);
	//创建样条曲线（参数：拟合点、起点切线方向、终点切线方向、阶数、允许拟合误差）
	static AcDbObjectId Add(const AcGePoint3dArray &points, const AcGeVector3d &startTangent, const AcGeVector3d &endTangent, int order = 4, double fitTolerance = 0.0);
};

