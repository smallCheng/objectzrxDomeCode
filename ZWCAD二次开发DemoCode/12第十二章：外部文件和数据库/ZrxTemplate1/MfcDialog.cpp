// MfcDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MfcDialog.h"
#include "afxdialogex.h"


// CMfcDialog 对话框

IMPLEMENT_DYNAMIC(CMfcDialog, CDialog)

CMfcDialog::CMfcDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMfcDialog::IDD, pParent)
{

}

CMfcDialog::~CMfcDialog()
{
}

void CMfcDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMfcDialog, CDialog)
END_MESSAGE_MAP()


// CMfcDialog 消息处理程序

