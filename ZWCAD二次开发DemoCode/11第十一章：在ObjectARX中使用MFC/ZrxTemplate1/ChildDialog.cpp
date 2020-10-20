// ChildDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ChildDialog.h"
#include "afxdialogex.h"


// CChildDialog 对话框

IMPLEMENT_DYNAMIC(CChildDialog, CAcUiDialog)

CChildDialog::CChildDialog(CWnd* pParent /*=NULL*/,HINSTANCE hInstance /*=NULL*/)
	: CZcUiDialog(CChildDialog::IDD, pParent,hInstance)
{

}

CChildDialog::~CChildDialog()
{
}

void CChildDialog::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDialog, CAcUiDialog)
	ON_BN_CLICKED(IDC_LINE, &CChildDialog::OnBnClickedLine)
END_MESSAGE_MAP()


// CChildDialog 消息处理程序


void CChildDialog::OnBnClickedLine()
{
	// TODO: 在此添加控件通知处理程序代码

	//类似与非模态对话框，需要锁定文档和解锁文档
	acDocManager->lockDocument(acDocManager->curDocument());

	//绘制一条直线
	CLineUtil::Add(AcGePoint3d(4.0,2.0,2.0),AcGePoint3d(10.0,7.0,0.0));

	acDocManager->unlockDocument(acDocManager->curDocument());
}

