#pragma once
class CViewUtil
{
public:
	CViewUtil();
	~CViewUtil();
	
	//获取当前视图信息
	static void GetCurrentView(AcDbViewTableRecord &view);
	//给定显示范围的最大、最小角坐标和缩放比例，修改当前视图
	static void Set(const AcGePoint3d &pMin, const AcGePoint3d &pMax, double scale);
	//将视图移动到给定的中心点
	static void SetCenter(const AcGePoint3d &center);
	//缩放视图，显示模型空间的所有实体
	static void ZoomExtent();
	//ZOOM E
	static void DwgZoomExtent(AcDbDatabase *pDb);


};

