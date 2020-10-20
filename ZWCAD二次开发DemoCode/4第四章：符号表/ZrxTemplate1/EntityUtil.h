#pragma once
class CEntityUtil
{
public:
	CEntityUtil();
	~CEntityUtil();

	//修改实体颜色
	static Acad::ErrorStatus SetColor(AcDbObjectId entId, Adesk::UInt16 colorIndex);
	//修改实体图层
	static void SetLayer(AcDbObjectId entId, CString strLayer);
	//修改实体线型
	static void SetLineType(AcDbObjectId entId, CString strLineType);
	//按照指定的角度（用弧度值表示）旋转指定的实体
	static Acad::ErrorStatus Rotate(AcDbObjectId entId, const AcGePoint2d &ptBase, double rotation);
	//移动实体
	static Acad::ErrorStatus Move(AcDbObjectId entId, const AcGePoint3d &ptBase, const AcGePoint3d &ptDest);
	//缩放实体
	static Acad::ErrorStatus Scale(AcDbObjectId entId, const AcGePoint3d &ptBase, double scaleFactor);
};

