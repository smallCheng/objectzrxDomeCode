// SampleDlg.cpp : 
//

#include "stdafx.h"
#include "SampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// SampleDlg


SampleDlg::SampleDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(SampleDlg::IDD, pParent)
{

}



void SampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SampleDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &SampleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// SampleDlg

void SampleDlg::OnBnClickedButton1()
{
	MessageBox(_T("HelloWorld!"), _T("DlgSample"), MB_OK);
}
