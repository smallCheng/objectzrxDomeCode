#include "stdafx.h"
#include "EntInfoTipHandler.h"

CEntInfoTipHandler::CEntInfoTipHandler(const bool storeInDocVars, ZcApDocument* pDoc)
{
	if (storeInDocVars)
	{
		assert(DocVars.docData(pDoc).m_pIPM == NULL);

		// 添加输入点监视器
		m_pDoc = pDoc;
		m_pDoc->inputPointManager()->addPointMonitor(this);
		DocVars.docData().m_pIPM = this;
	}
	else
	{
		m_pDoc = NULL;
	}
}

CEntInfoTipHandler::~CEntInfoTipHandler()
{
	if (m_pDoc)
	{
		if (DocVars.docData(m_pDoc).m_pIPM)
		{
			// 删除输入点监视器
			m_pDoc->inputPointManager()->removePointMonitor(this);
			DocVars.docData(m_pDoc).m_pIPM = NULL;
		}
	}
}

bool CEntInfoTipHandler::excludeFromOsnapCalculation(const AcArray<AcDbObjectId>& nestedEntity, int gsSelectionMark)
{
	return false;
}

Acad::ErrorStatus CEntInfoTipHandler::monitorInputPoint(
	bool&                             appendToTooltipStr,
	ZTCHAR*&                            additionalTooltipString,

	ZcGiViewportDraw*                   drawContext,

	ZcApDocument*                       document,
	bool                                pointComputed,
	int                                 history,
	const ZcGePoint3d&                  lastPoint,
	const ZcGePoint3d&                  rawPoint,
	const ZcGePoint3d&                  grippedPoint,
	const ZcGePoint3d&                  cartesianSnappedPoint,
	const ZcGePoint3d&                  osnappedPoint,
	ZcDb::OsnapMask                     osnapMask,
	const ZcArray<ZcDbCustomOsnapMode*>& customOsnapModes,
	ZcDb::OsnapMask                     osnapOverrides,
	const ZcArray<ZcDbCustomOsnapMode*>& customOsnapOverrides,
	const ZcArray<ZcDbObjectId>&        apertureEntities,
	const ZcArray< ZcDbObjectIdArray, ZcArrayObjectCopyReallocator< ZcDbObjectIdArray > >&
	nestedApertureEntities,
	const ZcArray<ZSoft::GsMarker>&     gsSelectionMark,
	const ZcArray<ZcDbObjectId>&        keyPointEntities,
	const ZcArray< ZcDbObjectIdArray, ZcArrayObjectCopyReallocator< ZcDbObjectIdArray > >&
	nestedKeyPointEntities,
	const ZcArray<ZSoft::GsMarker>&     keyPointGsSelectionMark,
	const ZcArray<ZcGeCurve3d*>&        alignmentPaths,
	const ZcGePoint3d&                  computedPoint,
	const ZTCHAR*                         tooltipString)
{
	// 一定要注意检查缓冲区的大小，避免越界导致的Acad直接跳出
	TCHAR mtooltipStr[1024], tempStr[1024];
	mtooltipStr[0] = '\0';

	Acad::ErrorStatus es;
	AcDbEntity* pEnt;
	AcDbObjectId highlightId = AcDbObjectId::kNull;

	if (pointComputed)
	{
		// 分析光标所覆盖的实体
		if (apertureEntities.length() > 0)
		{
			for (int i = 0; i < apertureEntities.length(); ++i)
			{
				// 避免显示更多的实体（根据需要确定是否需要）
				if (i > 0)
				{
					break;
				}

				if (Acad::eOk != (acdbOpenAcDbEntity(pEnt, apertureEntities[i], AcDb::kForRead)))
				{
					continue;
				}

				if (pEnt->isKindOf(AcDbLine::desc()))
				{
					// 实体类型信息
					if (_tcslen(mtooltipStr) > 0)
					{
						_tcscpy(mtooltipStr, TEXT("直线:"));
					}
					else
					{
						_tcscpy(mtooltipStr, TEXT("\n直线:"));
					}

					// 实体详细信息
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					double length = pLine->startPoint().distanceTo(pLine->endPoint());
					AcGeVector3d vec = pLine->endPoint() - pLine->startPoint();
					double angle = vec.convert2d(AcGePlane::kXYPlane).angle();
					_stprintf(tempStr, TEXT("\n 长度: %.2f \n 倾角: %.2f"), length, CConvertUtil::RadianToAngle(angle));
					_tcscat(mtooltipStr, tempStr);
				}
				else if (pEnt->isKindOf(AcDbCircle::desc()))
				{
					// 实体类型信息
					if (_tcslen(mtooltipStr) > 0)
					{
						_tcscpy(mtooltipStr, TEXT("圆:"));
					}
					else
					{
						_tcscpy(mtooltipStr, TEXT("\n圆:"));
					}

					// 实体详细信息
					AcDbCircle* pCircle = AcDbCircle::cast(pEnt);
					double radius = pCircle->radius();
					double area = CMathUtil::PI() * radius * radius;
					_stprintf(tempStr, TEXT("\n 半径: %.2f \n 面积: %.2f"), radius, area);
					_tcscat(mtooltipStr, tempStr);
				}

				pEnt->close();
			}
			highlightId = apertureEntities[0];
		}
	}

	// 执行高亮显示，只有在显示最顶层的实体会被高亮显示
	static AcDbObjectId oldHighlightId = AcDbObjectId::kNull;
	if (highlightId != oldHighlightId)
	{
		if (AcDbObjectId::kNull != oldHighlightId)
		{
			es = acdbOpenAcDbEntity(pEnt, oldHighlightId, AcDb::kForRead);
			if (es == Acad::eOk)
			{
				es = pEnt->unhighlight();
				pEnt->close();
				oldHighlightId = AcDbObjectId::kNull;
			}
		}
		es = acdbOpenAcDbEntity(pEnt, highlightId, AcDb::kForRead);
		if (es == Acad::eOk)
		{
			es = pEnt->highlight();
			pEnt->close();
			oldHighlightId = highlightId;
		}
	}

	// 显示其他的提示内容
	appendToTooltipStr = true;
	additionalTooltipString = mtooltipStr;

	return Acad::eOk;
}

