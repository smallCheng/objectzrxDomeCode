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
//----- DwgScale.h : Declaration of the DwgScale
//-----------------------------------------------------------------------------
#pragma once

#ifdef CUSTOMOBJECTDB_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif


//-----------------------------------------------------------------------------
#include "dbmain.h"
#include "zdbxEntryPoint.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP DwgScale : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(DwgScale) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DwgScale () ;
	virtual ~DwgScale () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

public:
	Adesk::Int32 m_infoScale;	//条件比例
	Adesk::Int32 m_lableScale;	//出图比例

public:
	void Set(int infoScale,int lableScale);
	int GetInfoScale() const;
	int GetLableScale() const;
	
} ;

//#ifdef CUSTOMOBJECTDB_MODULE
//ZCDB_REGISTER_OBJECT_ENTRY_AUTO(DwgScale)
//#endif
