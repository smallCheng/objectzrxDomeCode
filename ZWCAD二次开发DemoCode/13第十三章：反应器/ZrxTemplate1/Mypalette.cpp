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
//----- Mypalette.cpp : Implementation of CMyPalette
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "Mypalette.h"

//-----------------------------------------------------------------------------
//- Import the MSXML functionality
#import "msxml6.dll" named_guids
#define MSXML MSXML2
//using namespace MSXML ;

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CMyPalette, CZdUiPalette)

BEGIN_MESSAGE_MAP(CMyPalette, CZdUiPalette)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CMyPalette::CMyPalette (HINSTANCE hInstance) : CZdUiPalette (), mChildDlg(NULL,hInstance) {
}

//-----------------------------------------------------------------------------
CMyPalette::~CMyPalette () {
}

//-----------------------------------------------------------------------------
//- Load the input data from xml.
BOOL CMyPalette::Load (IUnknown *pUnk) {	
	//- Call base class first 
	CZdUiPalette::Load (pUnk) ;
	//- Initialise for XML
	CComQIPtr<MSXML::IXMLDOMNode> pNode(pUnk) ;
	MSXML::IXMLDOMNodePtr pChild, pChild1 ;

	// TODO: read in data from palette XML

	return (TRUE) ;
}

//-----------------------------------------------------------------------------
//- Save the input data to xml.
BOOL CMyPalette::Save (IUnknown *pUnk) {
	//- Call base class first 
	CZdUiPalette::Save (pUnk) ;
	//- initialise for XML
	CComQIPtr<MSXML::IXMLDOMNode> pNode(pUnk) ;
	MSXML::IXMLDOMNodePtr pChild, pChild1 ;

	// TODO: save data to palette XML

	return (TRUE) ;
}

//-----------------------------------------------------------------------------
//- Used to add a dialog resource
int CMyPalette::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if ( CZdUiPalette::OnCreate (lpCreateStruct) == -1 )
		return (-1) ;
	//- Create it and set the parent as the dockctrl bar
	mChildDlg.Create (IDD_DIALOG, this) ;
	CRect rect ;
	GetWindowRect (&rect) ;

	mChildDlg.ShowWindow(SW_SHOW);
	//- Move the window over so we can see the control lines
	mChildDlg.MoveWindow (0, 0, rect.Width (), rect.Height (), TRUE) ;
	return (0) ;
}

//-----------------------------------------------------------------------------
//- Called by the palette set when the palette is made active
void CMyPalette::OnSetActive () {
	return (CZdUiPalette::OnSetActive ()) ;
}

//-----------------------------------------------------------------------------
//- Called by AutoCAD to steal focus from the palette
bool CMyPalette::CanFrameworkTakeFocus () {
	//- Not simply calling IsFloating() (a BOOL) avoids warning C4800
	return (GetPaletteSet ()->IsFloating () == TRUE ? true : false) ;
}

//-----------------------------------------------------------------------------
void CMyPalette::OnSize (UINT nType, int cx, int cy) {
	CZdUiPalette::OnSize (nType, cx, cy) ;

	if(::IsWindow(mChildDlg.GetSafeHwnd()))
	{
		mChildDlg.MoveWindow(0,0,cx,cy,TRUE);
	}
}

