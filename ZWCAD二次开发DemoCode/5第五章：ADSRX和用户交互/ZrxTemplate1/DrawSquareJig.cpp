#include "stdafx.h"
#include "DrawSquareJig.h"


CDrawSquareJig::CDrawSquareJig()
{
	m_pPoly = NULL;
}


CDrawSquareJig::~CDrawSquareJig()
{
}

bool CDrawSquareJig::doIt(const AcGePoint3d & centerPoint, AcDbObjectId & polyId)
{
	m_centerPoint = centerPoint;

	//拖动开始值钱：将多段线创建出来
	m_pPoly = new AcDbPolyline();
	for (int i = 0; i < 4; i++)
	{
		m_pPoly->addVertexAt(i, CConvertUtil::ToPoint2d(m_centerPoint));
	}
	m_pPoly->setClosed(Adesk::kTrue);

	//进入拖动流程
	CString prompt = _T("\n指定标注插入点:");
	setDispPrompt(prompt);
	AcEdJig::DragStatus stat = drag();

	//拖动结束：函数返部分
	if (stat == kNormal)
	{
		polyId = CDwgDatabaseUtil::PostToModelSpace(m_pPoly);
		return true;
	}
	else
	{
		delete m_pPoly;
		return false;
	}
}

AcEdJig::DragStatus CDrawSquareJig::sampler()
{
	setUserInputControls((UserInputControls)(AcEdJig::kAccept3dCoordinates | AcEdJig::kNoNegativeResponseAccepted | AcEdJig::kNullResponseAccepted));

	//一定要判断下一点是否发生了变化，否则update函数不停的被调用，实体反而不能被绘制出来
	static AcGePoint3d pointTemp;
	DragStatus stat = acquirePoint(m_curPoint);
	if (pointTemp != m_curPoint)
	{
		pointTemp = m_curPoint;
	}
	else if (stat == AcEdJig::kNormal)
	{
		return AcEdJig::kNoChange;
	}
	return stat;
}

Adesk::Boolean CDrawSquareJig::update()
{
	//实现你的更新操作，在这里更新的是m_pPoly
	double dist = CConvertUtil::ToPoint2d(m_centerPoint).distanceTo(CConvertUtil::ToPoint2d(m_curPoint));
	for (int i = 0; i < 4; i++)
	{
		double angle = i*CMathUtil::PI()*0.5 + CMathUtil::PI()*0.25;
		AcGePoint2d pt = CGePointUtil::PolarPoint(CConvertUtil::ToPoint2d(m_centerPoint), angle, dist);
		m_pPoly->setPointAt(i, pt);
	}
	return Adesk::kTrue;
}

AcDbEntity * CDrawSquareJig::entity() const
{
	return m_pPoly;
}
