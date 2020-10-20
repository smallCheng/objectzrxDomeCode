#pragma once
class CDwgDatabaseUtil
{
public:
	CDwgDatabaseUtil();
	~CDwgDatabaseUtil();

	//添加实体到模型空间
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase());
	//获取指定模型空间指定图层上所有实体对象ID,参数为NULL时获取所有对象ID
	static AcDbObjectIdArray GetAllEntityIds(const TCHAR* layerName = NULL, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase());
	//获取图形数据库模型空间的最下范围
	static AcDbExtents GetModelSpaceExtent(AcDbDatabase *pDb);
};

