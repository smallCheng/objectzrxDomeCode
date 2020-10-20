#include "StdAfx.h"
#include "MyObjectReactor.h"


void CMyObjectReactor::modified( const ZcDbObject* pObj )
{
	AcDbCircle *pCircle = AcDbCircle::cast(pObj);
	if (pCircle != NULL)
	{
		double radius = pCircle->radius();

		AcDbCircle *pLinkCircle = NULL;
		if (acdbOpenObject(pLinkCircle, m_entId, AcDb::kForWrite) == Acad::eOk)
		{
			pLinkCircle->setRadius(radius);

			pLinkCircle->close();
		}
	}
}

void CMyObjectReactor::erased( const ZcDbObject* dbObj, Adesk::Boolean pErasing /*= true*/ )
{
	AcDbCircle *pCircle = AcDbCircle::cast(dbObj);
	if (pCircle != NULL && pErasing)
	{
		AcDbCircle *pLinkCircle = NULL;
		if (acdbOpenObject(pLinkCircle, m_entId, AcDb::kForWrite) == Acad::eOk)
		{
			pLinkCircle->erase();

			pLinkCircle->close();
		}
	}
}

void CMyObjectReactor::SetLinkEntId( ZcDbObjectId entId )
{
	assertWriteEnabled();

	m_entId = entId;
}


Adesk::UInt32 CMyObjectReactor::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CMyObjectReactor, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, MYOBJECTREACTOR,
	"ZWCADAPP"
	"|Product Desc:     A description for your object"
	"|Company:          Your company name"
	"|WEB Address:      Your company WEB site address"
)

//-----------------------------------------------------------------------------
CMyObjectReactor::CMyObjectReactor () : AcDbObject () {
}

CMyObjectReactor::~CMyObjectReactor () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CMyObjectReactor::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CMyObjectReactor::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem((AcDbSoftPointerId&)m_entId);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CMyObjectReactor::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CMyObjectReactor::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CMyObjectReactor::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	pFiler->readItem((AcDbSoftPointerId*) &m_entId);

	return (pFiler->filerStatus ()) ;
}
