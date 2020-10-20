#pragma once
class CDwgDatabaseUtil
{
public:
	CDwgDatabaseUtil();
	~CDwgDatabaseUtil();

	//添加实体到模型空间
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt);
	//获取当前模型空间指定图层上所有实体对象ID,参数为NULL时获取所有对象ID
	static AcDbObjectIdArray GetAllEntityIds(const TCHAR* layerName = NULL);
};

