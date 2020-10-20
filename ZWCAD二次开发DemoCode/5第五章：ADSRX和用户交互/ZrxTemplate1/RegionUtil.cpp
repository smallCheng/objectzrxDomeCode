#include "stdafx.h"
#include "RegionUtil.h"


CRegionUtil::CRegionUtil()
{
}


CRegionUtil::~CRegionUtil()
{
}

AcDbObjectIdArray CRegionUtil::Add(const AcDbObjectIdArray & curveIds)
{
	AcDbObjectIdArray regionIds;	//生成的面域的ID数组
	AcDbVoidPtrArray curves;		//指向作为面域边界的曲线的指针数组
	AcDbVoidPtrArray regions;		//指向创建的面域对象指针的数组
	AcDbEntity *pEnt = NULL;		//临时指针，用来关闭边界曲线
	AcDbRegion *pRegion = NULL;		//临时对象，用来将面域添加到模型空间

									//用curveTds初始化curves
	for (int i = 0; i < curveIds.length(); i++)
	{
		acdbOpenAcDbEntity(pEnt, curveIds.at(i), AcDb::kForRead);
		if (pEnt->isKindOf(AcDbCurve::desc()))
		{
			curves.append(static_cast<void*>(pEnt));
		}
	}
	Acad::ErrorStatus es = AcDbRegion::createFromCurves(curves, regions);
	if (es == Acad::eOk)
	{
		//将生成的面域添加到模型空间
		for (int i = 0; i < regions.length(); i++)
		{
			pRegion = static_cast<AcDbRegion*>(regions[i]);
			pRegion->setDatabaseDefaults();
			AcDbObjectId regionId;
			regionId = CDwgDatabaseUtil::PostToModelSpace(pRegion);
			regionIds.append(regionId);
		}
	}
	else
	{
		//如果创建不成功，也要删除已经生成的面域
		for (int i = 0; i < regions.length(); i++)
		{
			delete (AcRxObject*)regions[i];
		}

	}

	//关闭作为边界对象
	for (int i = 0; i < curves.length(); i++)
	{
		pEnt = static_cast<AcDbCurve*>(curves[i]);
		pEnt->close();
	}
	return regionIds;
}
