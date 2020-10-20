#pragma once

class CMyObjectReactor : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(CMyObjectReactor) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CMyObjectReactor () ;
	virtual ~CMyObjectReactor () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	void modified(const ZcDbObject* pObj);
	virtual void erased(const ZcDbObject* dbObj, Adesk::Boolean pErasing = true);

	// 设置与其关联的实体
	void SetLinkEntId(ZcDbObjectId entId);

private:
	ZcDbObjectId m_entId;		// 与通知对象关联的实体ID

} ;
