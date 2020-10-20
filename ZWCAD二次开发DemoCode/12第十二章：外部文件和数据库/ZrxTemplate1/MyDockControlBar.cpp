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
//----- BarTest.cpp : Implementation of CMyDockControlBar
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "MyDockControlBar.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CMyDockControlBar, CZcUiDockControlBar)

BEGIN_MESSAGE_MAP(CMyDockControlBar, CZcUiDockControlBar)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
//----- CMyDockControlBar *pInstance = new CMyDockControlBar;
//----- pInstance->Create (acedGetAcadFrame (), "My title bar") ;
//----- pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
//----- pInstance->RestoreControlBar () ;

//-----------------------------------------------------------------------------
static CLSID clsCMyDockControlBar = {0xeab78c04, 0x2194, 0x47ad, {0xa4, 0xf2, 0xad, 0xca, 0x3e, 0x3b, 0xb6, 0x3c}} ;


//-----------------------------------------------------------------------------
CMyDockControlBar::CMyDockControlBar () : CZcUiDockControlBar() {
}

//-----------------------------------------------------------------------------
CMyDockControlBar::~CMyDockControlBar () {
	
}

//-----------------------------------------------------------------------------
#ifdef _DEBUG
//- Please uncomment the 2 following lines to avoid linker error when compiling
//- in release mode. But make sure to uncomment these lines only once per project
//- if you derive multiple times from CAdUiDockControlBar/CZcUiDockControlBar
//- classes.

//void CAdUiDockControlBar::AssertValid () const {
//}
#endif

//-----------------------------------------------------------------------------
BOOL CMyDockControlBar::Create (CWnd *pParent, LPCTSTR lpszTitle) {
	CString strWndClass ;
	strWndClass =AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;
	CRect rect (0, 0, 250, 200) ;
	if (!CZcUiDockControlBar::Create (
			strWndClass, lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
			rect, pParent, 0
		)
	)
		return (FALSE) ;

	SetToolID (&clsCMyDockControlBar) ;

	// TODO: Add your code here
	
	return (TRUE) ;
}

//-----------------------------------------------------------------------------
//----- This member function is called when an application requests the window be 
//----- created by calling the Create or CreateEx member function
int CMyDockControlBar::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if ( CZcUiDockControlBar::OnCreate (lpCreateStruct) == -1 )
		return (-1) ;

	//----- Point to our resource
	CAcModuleResourceOverride resourceOverride; 	
	//----- Create it and set the parent as the dockctrl bar
	
	m_childDlg.Create (IDD_DIALOG, this) ;
	//----- Move the window over so we can see the control lines
	m_childDlg.MoveWindow (0, 0, 160, 160, TRUE) ;
	m_childDlg.ShowWindow(SW_SHOW);
	return (0) ;
}

//-----------------------------------------------------------------------------
void CMyDockControlBar::SizeChanged (CRect *lpRect, BOOL bFloating, int flags) {
	// If valid
	if (::IsWindow (m_childDlg.GetSafeHwnd ())) 
	{
	//	//----- Always point to our resource to be safe
		CAcModuleResourceOverride resourceOverride ;
	//	//----- Then update its window size relatively
		m_childDlg.SetWindowPos (this, lpRect->left + 4, lpRect->top + 4, lpRect->Width (), lpRect->Height (), SWP_NOZORDER) ;
	}
}

//-----------------------------------------------------------------------------
//-----  Function called when user selects a command from Control menu or when user 
//----- selects the Maximize or the Minimize button.
void CMyDockControlBar::OnSysCommand (UINT nID, LPARAM lParam) {
	CZcUiDockControlBar::OnSysCommand (nID, lParam) ;
}

//-----------------------------------------------------------------------------
//----- The framework calls this member function after the window's size has changed
void CMyDockControlBar::OnSize (UINT nType, int cx, int cy) {
	CZcUiDockControlBar::OnSize (nType, cx, cy) ;
	// If valid
	if (::IsWindow (m_childDlg.GetSafeHwnd ())) 
	{
	//	//----- Always point to our resource to be safe
		CAcModuleResourceOverride resourceOverride ;
	//	//----- then update its window position relatively
		m_childDlg.MoveWindow (0, 0, cx, cy) ;
	}
}
