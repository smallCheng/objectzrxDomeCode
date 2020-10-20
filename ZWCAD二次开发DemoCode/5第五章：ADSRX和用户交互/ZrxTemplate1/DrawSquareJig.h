#pragma once
class CDrawSquareJig :AcEdJig
{
public:
	CDrawSquareJig();
	~CDrawSquareJig();

	//外部调用的函数，一般用于jig的初始化
	bool doIt(const AcGePoint3d &centerPoint,AcDbObjectId &polyId);

	//此函数将被drag函数调用以获得用户输入
	virtual AcEdJig::DragStatus sampler();

	//对需要在拖动过程中发生的实体进行修改
	virtual Adesk::Boolean update();

	//指定jig所操作的对象
	virtual AcDbEntity *entity() const;

private:
	AcDbPolyline *m_pPoly;	//拖动过程中动态变化的实体
	AcGePoint3d m_curPoint;	//储存用户光标移动时点的临时位子
	AcGePoint3d m_centerPoint;	//正方形的中心点

};

