#pragma once
class CDoubleClickCircle :
	public AcDbDoubleClickEdit
{
public:
	CDoubleClickCircle();
	virtual ~CDoubleClickCircle();

	//开始编辑时调用的函数
	void startEdit(AcDbEntity *pEnt, AcGePoint3d pt);

	//完成编辑之后调用的函数
	void finishEdit(void);

};

