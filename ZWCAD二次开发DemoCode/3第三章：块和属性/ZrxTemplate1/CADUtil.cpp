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

AcDbObjectId CCADUtil::InsertBlockRef(AcDbObjectId blkDefId, const AcGePoint3d & insertPoint, double scale, double rotation)
{
	//设置图块的插入点、旋转角度、比例等
	AcDbBlockReference *pBlkRef = new AcDbBlockReference(insertPoint, blkDefId);
	pBlkRef->setRotation(rotation);
	SetScaleFactor(pBlkRef, scale);

	//把新建的图块参照实体加入到新建的块表记录中，并取得插入图块的实体ID
	//用于返回值
	return ZrxCreateEntitys::PostToModelSpace(pBlkRef);
}

void CCADUtil::SetScaleFactor(AcDbBlockReference * pBlkRef, double scale)
{
	assert(pBlkRef);
	AcGeScale3d geScale(scale, scale, scale);
	pBlkRef->setScaleFactors(geScale);
}

AcDbObjectId CCADUtil::GetBlkDefId(const TCHAR * blkDefName)
{
	AcDbBlockTable *pBlockTable = NULL;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
	AcDbObjectId blkDefId;
	es = pBlockTable->getAt(blkDefName, blkDefId);
	if (es == Acad::eOk)
	{
		return blkDefId;
	}
	else
	{
		return AcDbObjectId::kNull;
	}
}

AcDbObjectId CCADUtil::InsertBlockRefWithAttribute(const AcDbObjectId &blkDefId, const AcGePoint3d &insertPoint, double scale, double rotation)
{
	//创建块参照对象
	AcDbBlockReference *pBlkRef = new AcDbBlockReference(insertPoint, blkDefId);
	SetScaleFactor(pBlkRef, scale);
	pBlkRef->setRotation(rotation);

	//判断指定的块表记录是否包含属性定义
	AcDbBlockTableRecord *pBlkDefRcd = NULL;
	if (acdbOpenObject(pBlkDefRcd, pBlkRef->blockTableRecord(), AcDb::kForRead) == Acad::eOk)
	{
		if (pBlkDefRcd->hasAttributeDefinitions())
		{
			//创建块表记录遍历器，用于访问块定义中所有的实体
			AcDbBlockTableRecordIterator *pItr = NULL;
			pBlkDefRcd->newIterator(pItr);
			for (pItr->start(); !pItr->done(); pItr->step())
			{
				AcDbEntity *pEnt = NULL;
				if (pItr->getEntity(pEnt, AcDb::kForRead) == Acad::eOk)
				{
					//如果是属性定义，就向块参照添加属性
					AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt);
					if (pAttDef != NULL)
					{
						AppendAttributeToBlkRef(pBlkRef, pAttDef);
					}
					pEnt->close();
				}
			}
			delete pItr;
		}
		pBlkDefRcd->close();
	}
	//将块参照添加到模型空间
	return ZrxCreateEntitys::PostToModelSpace(pBlkRef);
}

void CCADUtil::AppendAttributeToBlkRef(AcDbBlockReference * pBlkRef, AcDbAttributeDefinition * pAttDef)
{
	//创建一个新的属性对象
	AcDbAttribute *pAtt = new AcDbAttribute();
	//从属性定义获得属性对象的对象特征
	pAtt->setPropertiesFrom(pBlkRef);
	pAtt->setLayer(pBlkRef->layerId());
	//设置属性对象的其他特征
	pAtt->setInvisible(pAttDef->isInvisible());
	pAtt->setPosition(pAttDef->position());
	pAtt->setHeight(pAttDef->height());
	pAtt->setWidthFactor(pAttDef->widthFactor());
	pAtt->setRotation(pAttDef->rotation());
	pAtt->setHorizontalMode(pAttDef->horizontalMode());
	pAtt->setVerticalMode(pAttDef->verticalMode());
	pAtt->setAlignmentPoint(pAttDef->alignmentPoint());
	pAtt->setTextStyle(pAttDef->textStyle());
	pAtt->setAttributeFromBlock(pBlkRef->blockTransform());

	//获得属性对象的Tag、Promat和TextString
	TCHAR  *pStr = pAttDef->tag();
	pAtt->setTag(pStr);
	acutDelString(pStr);
	pAtt->setFieldLength(pAttDef->fieldLength());
	pAtt->setTextString(pAttDef->textString());

	//向块参照追加属性对象
	pBlkRef->appendAttribute(pAtt);
	pAtt->close();
}

bool CCADUtil::SetBlockRefAttribute(AcDbBlockReference * pBlikRef, const TCHAR * tag, const TCHAR * val)
{
	AcDbBlockTableRecord *pBlkDefRcd = NULL;
	bool bRet = true;
	if (acdbOpenObject(pBlkDefRcd, pBlikRef->blockTableRecord(), AcDb::kForRead) == Acad::eOk)
	{
		//块定义中是否包含了对应的属性
		if (pBlkDefRcd->hasAttributeDefinitions())
		{
			AcDbBlockTableRecordIterator *pItr = NULL;
			pBlkDefRcd->newIterator(pItr);
			AcDbEntity *pEnt = NULL;
			for (pItr->start(); !pItr->done(); pItr->step())
			{
				pItr->getEntity(pEnt, AcDb::kForRead);
				AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt);
				if (pAttDef != NULL)
				{
					TCHAR *pszTag = pAttDef->tag();
					CString strTag = pszTag;
					acutDelString(pszTag);
					if (strTag.CompareNoCase(tag) == 0)
					{
						//遍历块参照所有属性，判断是否包含了对应的属性
						bool bFound = false;
						AcDbObjectIterator *attIt = pBlikRef->attributeIterator();
						for (attIt->start(); !attIt->done(); attIt->step())
						{
							AcDbAttribute *pAtt = NULL;
							Acad::ErrorStatus es = acdbOpenObject(pAtt, attIt->objectId(), AcDb::kForWrite);
							if (es == Acad::eOk)
							{
								pszTag = pAtt->tag();
								strTag = pszTag;
								acutDelString(pszTag);
								if (strTag.CompareNoCase(tag) == 0)
								{
									pAtt->setTextString(val);
									bFound = true;
								}
								pAtt->close();
							}
						}
						delete attIt;
						//如果没有找到给定名称的属性，就新建这个属性
						if (!bFound)
						{
							AppendAttributeToBlkRef(pBlikRef, pAttDef);
						}
					}
				}
				pEnt->close();
				
			}
			delete pItr;
		}
		else
		{
			bRet = false;
		}
		pBlkDefRcd->close();
	}
	return bRet;
}

bool CCADUtil::GetAttributeValue(AcDbBlockReference *pBlkRef, const TCHAR *tag, CString &value)
{
	//创建块表记录的属性遍历器
	bool bFound = false;
	AcDbObjectIterator *pItr = pBlkRef->attributeIterator();
	if (pItr)
	{
		while (!pItr->done())
		{
			AcDbObjectId attId = pItr->objectId();
			AcDbAttribute *pAtt = NULL;
			if (pBlkRef->openAttribute(pAtt, attId, AcDb::kForRead) == Acad::eOk)
			{
				//块参照的属性Tag永远会是大写字母，因此传入的参数需要进行转换
				TCHAR *szTag = pAtt->tag();
				CString strTag = tag;
				strTag.MakeUpper();
				if (_tcscmp(strTag, szTag) == 0)
				{
					TCHAR *szValue = pAtt->textString();
					value = szValue;
					acutDelString(szValue);
					bFound = true;
				}
				acutDelString(szTag);
				pAtt->close();
			}
			pItr->step();
		}
		delete pItr;
		return bFound;
	}
	else
	{
		return false;
	}
}

void CCADUtil::IntersectWith(AcDbBlockReference * pBlkRef, AcDbEntity * pEnt, AcGePoint3dArray & intPonts, bool bIntersectWithText, bool bProjectToXOY, bool bExtendEntArg)
{
	assert(pBlkRef);
	assert(pEnt);

	//获取块参照对应的变换矩阵
	AcGeMatrix3d xForm = pBlkRef->blockTransform();

	AcDbObjectId blkTblId = pBlkRef->blockTableRecord();
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	if (acdbOpenObject(pBlkTblRcd, blkTblId, AcDb::kForRead) == Acad::eOk)
	{
		//块表记录遍历器访问块定义中的每个实体
		AcDbBlockTableRecordIterator *pIt = NULL;
		pBlkTblRcd->newIterator(pIt);
		for (pIt->start(); !pIt->done(); pIt->step())
		{
			AcDbEntity *pSubEnt = NULL;
			if (pIt->getEntity(pSubEnt, AcDb::kForRead) == Acad::eOk)
			{
				//如果不与文字求交点，判断当前实体类型，发现是文字就跳过去
				if ((!bIntersectWithText) && (pSubEnt->isKindOf(AcDbText::desc()) || pSubEnt->isKindOf(AcDbMText::desc())))
				{
					pSubEnt->close();
					continue;
				}

				//复制一个实体进行变换，避免修改块定义中的子实体
				AcDbEntity *pEntCopy = NULL;
				if (pSubEnt->getTransformedCopy(xForm, pEntCopy) == Acad::eOk)
				{
					//是否延长交点
					AcDb::Intersect inMode = bExtendEntArg ? AcDb::kExtendArg : AcDb::kOnBothOperands;
					if (bProjectToXOY)
					{
						pEntCopy->intersectWith(pEnt, inMode, AcGePlane::kXYPlane, intPonts);
					}
					else
					{
						pEntCopy->intersectWith(pEnt, inMode, intPonts);
					}
					delete pEntCopy;
				}
				pSubEnt->close();
			}
		}
		delete pIt;
		pBlkTblRcd->close();
	}
}
