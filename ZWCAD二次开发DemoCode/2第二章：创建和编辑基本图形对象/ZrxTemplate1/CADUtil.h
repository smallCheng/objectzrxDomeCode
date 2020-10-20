#pragma once
class CCADUtil
{
public:
	CCADUtil();
	~CCADUtil();

	//获取当前模型空间所有实体对象ID
	static AcDbObjectIdArray GetAllEntityIds();
	//获取当前模型空间指定图层上所有实体对象ID
	static AcDbObjectIdArray GetAllEntityIdsByLayerName(const TCHAR* layerName = NULL);
	

};

