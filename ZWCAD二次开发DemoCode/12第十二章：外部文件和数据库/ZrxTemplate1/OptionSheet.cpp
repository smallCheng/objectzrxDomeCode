// OptionSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "OptionSheet.h"
#include "afxdialogex.h"
//#include "..\Zrx2020\inc\zaduiTabChildDialog.h"
#include "..\Zrx2020\arxport\aduiTabChildDialog.h"

// COptionSheet 对话框

IMPLEMENT_DYNAMIC(COptionSheet, CZcUiTabMainDialog)

COptionSheet::COptionSheet(CWnd* pParent /*=NULL*/,HINSTANCE HInstance /*=NULL*/)
	: CZcUiTabMainDialog(COptionSheet::IDD, pParent,HInstance)
{

}

COptionSheet::~COptionSheet()
{
}

void COptionSheet::DoDataExchange(CDataExchange* pDX)
{
	CZcUiTabMainDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
}


LRESULT COptionSheet::OnAcadKeepFocus(WPARAM,LPARAM)
{
	return (TRUE);
}

BEGIN_MESSAGE_MAP(COptionSheet, CZcUiTabMainDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()


// COptionSheet 消息处理程序


BOOL COptionSheet::OnInitDialog()
{
	CZcUiTabMainDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//添加选项卡
	//SetAcadTabChildFocus(&m_tabCtrl);
	m_tabCtrl.AddTab(0,_T("文字"),CTextPage::IDD,&m_textPage);
	m_tabCtrl.AddTab(1,_T("控件"),CControlPage::IDD,&m_controlPage);
	m_tabCtrl.SetActiveTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COptionSheet::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(!m_textPage.SaveProfiles()||!m_controlPage.SaveProfiles())
	{
		return;
	}


	CZcUiTabMainDialog::OnOK();
}
