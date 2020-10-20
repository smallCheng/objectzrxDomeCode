#include "stdafx.h"
#include "CADUtil.h"


CCADUtil::CCADUtil()
{
}


CCADUtil::~CCADUtil()
{
}

AcDbObjectIdArray CCADUtil::GetAllEntityIds()
{
	AcDbObjectIdArray entIds;	//满足条件的实体集合

	//获得块表
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

	//获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	//创建遍历器,依次访问模型空间的每一个实体
	AcDbBlockTableRecordIterator *it = NULL;
	pBlkTblRcd->newIterator(it);
	AcDbObjectId objId;
	for (it->start(); !it->done(); it->step())
	{
		Acad::ErrorStatus es = it->getEntityId(objId);
		if (es == Acad::eOk)
		{
			entIds.append(objId);
		}
	}
	delete it;
	pBlkTblRcd->close();
	return entIds;
}

AcDbObjectIdArray CCADUtil::GetAllEntityIdsByLayerName(const TCHAR * layerName)
{
	AcDbObjectIdArray entIds;	//满足条件的实体集合
	bool bFilterLayer = false;	//是否需要过滤图层
	AcDbObjectId layerId;

	if (layerName != NULL)
	{
		AcDbLayerTable *pLayerTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
		if (!pLayerTbl->has(layerName))
		{
			pLayerTbl->close();
			return entIds;
			
		}
		pLayerTbl->getAt(layerName, layerId);
		pLayerTbl->close();

		bFilterLayer = true;
	}

	//获得块表
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

	//获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	//创建遍历器,依次访问模型空间的每一个实体
	AcDbBlockTableRecordIterator *it = NULL;
	pBlkTblRcd->newIterator(it);
	AcDbEntity *pEnt = NULL;
	for (it->start(); !it->done(); it->step())
	{
		Acad::ErrorStatus es = it->getEntity(pEnt,AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (bFilterLayer)	//过滤图层
			{
				if (pEnt->layerId() == layerId)
				{
					entIds.append(pEnt->objectId());
				}
			}
			else
			{
				entIds.append(pEnt->objectId());
			}
			pEnt->close();
		}
		else
		{
			acutPrintf(L"\nCADUtil::GetAllEntityIdsByLayer中打开实体失败（错误代码：%d ）", (int)es);
		}
	}
	delete it;
	pBlkTblRcd->close();
	return entIds;
}
