#pragma once
class CLayerUtil
{
public:
	CLayerUtil();
	~CLayerUtil();
	//添加图层记录
	static void Add(const TCHAR* layerName, int colorIndex=7);
	//根据指定图层名称获取指定图层ID
	static AcDbObjectId GetLayerId(const TCHAR* layerName);
	//修改图层颜色
	static bool SetColor(const TCHAR *layerName, int colorIndex);
	//获取当前图形中所有图层的ID集合
	static void GetLayerList(AcDbObjectIdArray &layers);

};

