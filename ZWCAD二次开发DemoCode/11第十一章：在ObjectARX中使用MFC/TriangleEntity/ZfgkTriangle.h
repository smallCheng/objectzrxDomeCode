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
//----- ZfgkTriangle.h : Declaration of the ZfgkTriangle
//-----------------------------------------------------------------------------
#pragma once

#ifdef TRIANGLEENTITY_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
//#ifndef VCUSETRIANGLEENTITY_MODULE
//#include "MgTriangle.h"
//#endif
#include <dbhatch.h>

//-----------------------------------------------------------------------------
class DLLIMPEXP ZfgkTriangle : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ZfgkTriangle) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ZfgkTriangle () ;
	ZfgkTriangle (const AcGePoint3d &pt1,const AcGePoint3d &pt2,const AcGePoint3d &pt3) ;
	virtual ~ZfgkTriangle () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
	virtual Adesk::Boolean worldDraw (AcGiWorldDraw *mode) ;

	//- Osnap points protocol
	virtual Acad::ErrorStatus getOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus getOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeFastTransform &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus getOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds,
		const AcGeMatrix3d &insertionMat) const ;
	virtual Acad::ErrorStatus getOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeFastTransform &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds,
		const AcGeMatrix3d &insertionMat) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus getGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus moveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus getGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags) const ;
	virtual Acad::ErrorStatus moveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) ;

public:
	AcGePoint3d m_verts[3];
//#ifndef VCUSETRIANGLEENTITY_MODULE
//	gcroot<zfgk::TrangleEntityDB::MgTriangle *> m_pManaged;
//#endif
public:
	/*自定义函数*/
	//创建AcDbFace对象
	AcDbFace *GetFace() const;
	AcDbPolyline* GetPolyline() const;
	AcGePoint2d ToPoint2d(const AcGePoint3d &pt) const;
	void GetVerts(AcGePoint3dArray &verts) const;
	double GetArea() const;
	void SetVertAt(int index,const AcGePoint3d &point);
	/*#ifndef VCUSETRIANGLEENTITY_MODULE
	void set_ManagedWrapper(gcroot<zfgk::TrangleEntityDB::MgTriangle *> p);
	#endif*/
public:
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus getGeomExtents(AcDbExtents & extents) const;
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus transformBy(const AcGeMatrix3d & xform);
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus explode(AcDbVoidPtrArray & entitySet) const;
	// -----------------------------------------------------------------------------
	virtual void list(void) const;
} ;

//#ifdef TRIANGLEENTITY_MODULE
//ACDB_REGISTER_OBJECT_ENTRY_AUTO(ZfgkTriangle)
//#endif
