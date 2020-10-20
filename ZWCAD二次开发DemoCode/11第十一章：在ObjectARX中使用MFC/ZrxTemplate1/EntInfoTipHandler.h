#pragma once

class CEntInfoTipHandler : public AcEdInputPointMonitor
{
public:
	CEntInfoTipHandler(const bool storeInDocVars = true, ZcApDocument* pDoc = curDoc());
	virtual ~CEntInfoTipHandler();

	// 计算需要从自动捕捉中排除的实体
	virtual bool excludeFromOsnapCalculation(const AcArray<AcDbObjectId>& nestedEntity,
		int gsSelectionMark);

	// 监控输入点信息
	virtual Acad::ErrorStatus monitorInputPoint(
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
		const ZTCHAR*                         tooltipString);

private:
	// 指向这个对象所属的文档
	ZcApDocument* m_pDoc;
};

