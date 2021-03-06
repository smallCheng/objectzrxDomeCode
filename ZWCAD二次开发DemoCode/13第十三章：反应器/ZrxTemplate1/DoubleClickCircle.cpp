#include "stdafx.h"
#include "DoubleClickCircle.h"
#include "GetColorDlg.h"

CDoubleClickCircle::CDoubleClickCircle()
{
}


CDoubleClickCircle::~CDoubleClickCircle()
{
}

void CDoubleClickCircle::startEdit(AcDbEntity * pEnt, AcGePoint3d pt)
{
	//获得当前文档
	AcApDocument *pDoc = acDocManager->curDocument();

	//将AcDbEntity指针转换为AcDbCircle指针
	if (pEnt->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *pCircle = AcDbCircle::cast(pEnt);

		//锁定文档
		acDocManager->lockDocument(pDoc);

		//将实体的打开状态升级为可写状态
		pCircle->upgradeOpen();

		//指定新的颜色
		//pCircle->setColorIndex(1);
		//pCircle->close();

		//提示用户输入指定更新的颜色
		int oldColorIndex = pCircle->colorIndex();
		CAcModuleResourceOverride myResource;	//防止资源冲突
		CGetColorDlg dlg;
		dlg.SetParams(oldColorIndex);
		if (dlg.DoModal() == IDOK)
		{
				
			int newColorIndex = 0;
			dlg.GetParams(newColorIndex);
			pCircle->setColorIndex(newColorIndex);
		}
		pCircle->close();

		//解锁文档
		acDocManager->unlockDocument(pDoc);

		//清理PickFirst选择集中的内容
		acedSSSetFirst(NULL, NULL);

		//更新图形显示
		pCircle->draw();	//在关闭之后还能使用
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
	}
	else
	{
		acutPrintf(_T("\n无效的AcDbCircle对象..."));
		pEnt->close();
		return;
	}
}

void CDoubleClickCircle::finishEdit(void)
{
	//此处不用
}
