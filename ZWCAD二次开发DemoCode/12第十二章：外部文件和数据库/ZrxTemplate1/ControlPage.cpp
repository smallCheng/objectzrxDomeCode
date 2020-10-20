// ControlPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlPage.h"
#include "afxdialogex.h"


// CControlPage 对话框

IMPLEMENT_DYNAMIC(CControlPage, CZcUiTabChildDialog)

CControlPage::CControlPage(CWnd* pParent /*=NULL*/,HINSTANCE hInstance /*=NULL*/)
	: CZcUiTabChildDialog(pParent,hInstance)
	, m_nRadio1(0)
	, m_bCheck1(FALSE)
{

}

CControlPage::~CControlPage()
{
}

void CControlPage::OnTabActivation(BOOL bActivate)
{

}

BOOL CControlPage::OnTabChanging()
{
	return (TRUE);
}

void CControlPage::DoDataExchange(CDataExchange* pDX)
{
	CZcUiTabChildDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_RADIO1, m_nRadio1);
	DDX_Check(pDX, IDC_CHECK1, m_bCheck1);
}


BEGIN_MESSAGE_MAP(CControlPage, CZcUiTabChildDialog)
END_MESSAGE_MAP()


// CControlPage 消息处理程序


BOOL CControlPage::OnInitDialog()
{
	CZcUiTabChildDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//从INI文件中加载参数值
	CIniFile iniFile(CAppDirectoryUtil::GetCurrentDirctory()+_T("\\OptionSheet.ini"));
	CString field=_T("OptionSheet");
	iniFile.GetValueOfKey(field,_T("nRadiol"),m_nRadio1);
	iniFile.GetValueOfKey(field,_T("bCheckl"),m_bCheck1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool CControlPage::SaveProfiles()
{
	if(!UpdateData())
	{
		return false;
	}

	//保存参数值
	CIniFile iniFile(CAppDirectoryUtil::GetCurrentDirctory()+_T("\\OptionSheet.ini"));
	CString filed=_T("OptionSheet");
	iniFile.SetValueOfKey(filed,_T("nRadiol"),m_nRadio1);
	iniFile.SetValueOfKey(filed,_T("bCheckl"),m_bCheck1);

	return true;
}
