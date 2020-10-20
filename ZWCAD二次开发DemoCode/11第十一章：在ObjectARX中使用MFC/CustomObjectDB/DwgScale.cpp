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
//----- DwgScale.cpp : Implementation of DwgScale
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "DwgScale.h"
#include "rxboiler.h"
#include "zdbproxy.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 DwgScale::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ZCRX_DXF_DEFINE_MEMBERS (
	DwgScale, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	ZcDbProxyEntity::kNoOperation, DWGSCALE, DWGSCALE
	
)

//-----------------------------------------------------------------------------
DwgScale::DwgScale () : AcDbObject () {
	m_infoScale=0;
	m_lableScale=0;
}

DwgScale::~DwgScale () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus DwgScale::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (DwgScale::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem(m_infoScale);
	pFiler->writeItem(m_lableScale);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus DwgScale::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > DwgScale::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < DwgScale::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	switch(version)
	{
	case (1):
		// TODO:here you can file datamembers not
		//	created by the ObjectARX Add-In
		pFiler->readItem(&m_infoScale);
		pFiler->readItem(&m_lableScale);
		break;
	}

	return (pFiler->filerStatus ()) ;
}

void DwgScale::Set(int infoScale,int lableScale)
{
	assertWriteEnabled();

	m_infoScale=infoScale;
	m_lableScale=lableScale;
}

int DwgScale::GetInfoScale() const
{
	assertReadEnabled();

	return m_infoScale;
}

int DwgScale::GetLableScale() const
{
	assertReadEnabled();

	return m_lableScale;
}

