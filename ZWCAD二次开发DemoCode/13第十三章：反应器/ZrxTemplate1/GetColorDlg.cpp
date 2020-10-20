// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- GetColorDlg.cpp : Implementation of CGetColorDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "GetColorDlg.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CGetColorDlg, CZcUiDialog)

BEGIN_MESSAGE_MAP(CGetColorDlg, CZcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CGetColorDlg::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CGetColorDlg::CGetColorDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CZcUiDialog (CGetColorDlg::IDD, pParent, hInstance), m_colorIndex(0)
{
}

//-----------------------------------------------------------------------------
void CGetColorDlg::DoDataExchange (CDataExchange *pDX) {
	CZcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COLOR_INDEX, m_colorIndex);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CGetColorDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}


BOOL CGetColorDlg::OnInitDialog()
{
	CZcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//根据成员变量更新资源控件
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CGetColorDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!UpdateData(TRUE))
	{
		return;
	}
	CZcUiDialog::OnOK();
}

void CGetColorDlg::SetParams(int colorIndex)
{
	m_colorIndex = colorIndex;

}

void CGetColorDlg::GetParams(int &colorIndex)
{
	colorIndex = m_colorIndex;
}
