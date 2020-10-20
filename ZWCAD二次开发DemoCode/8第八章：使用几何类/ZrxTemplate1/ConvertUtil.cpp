#include "stdafx.h"
#include "ConvertUtil.h"


CConvertUtil::CConvertUtil()
{
}


CConvertUtil::~CConvertUtil()
{
}

CString CConvertUtil::ToString(int val)
{
	CString str;
	str.Format(_T("%d"), val);
	return str;
}

AcGePoint2d CConvertUtil::ToPoint2d(const AcGePoint3d & point)
{
	return AcGePoint2d(point.x, point.y);
}

AcGePoint3d CConvertUtil::ToPoint3d(const AcGePoint2d & point2d, double z)
{
	return AcGePoint3d(point2d.x, point2d.y, z);
}

double CConvertUtil::AngleToRadian(double angle)
{
	return angle*CMathUtil::PI() / 180;
}

double CConvertUtil::RadianToAngle(double radian)
{
	return radian * 180 / CMathUtil::PI();
}

AcGePoint3d CConvertUtil::WcsToDcsPoint(const AcGePoint3d & point)
{
	//转换成显示坐标系
	AcGePoint3d pt;
	struct resbuf rbFrom, rbTo;
	rbFrom.restype = RTSHORT;
	rbFrom.resval.rint = 0;	//from WCS
	rbTo.restype = RTSHORT;
	rbTo.resval.rint = 2;	//to DCS

	acedTrans(asDblArray(point), &rbFrom, &rbTo, Adesk::kFalse, asDblArray(pt));

	return pt;
}

AcGePoint3d CConvertUtil::UcsToWcsPoint(const AcGePoint3d & point)
{
	//转换成世界坐标
	AcGePoint3d pt;
	struct resbuf rbFrom, rbTo;
	rbFrom.restype = RTSHORT;
	rbFrom.resval.rint = 1;	//用户坐标
	rbTo.restype = RTSHORT;
	rbTo.resval.rint = 0;	//世界坐标

	acedTrans(asDblArray(point), &rbFrom, &rbTo, Adesk::kFalse, asDblArray(pt));

	return pt;
}

AcGePoint3d CConvertUtil::WcsToUcsPoint(const AcGePoint3d & point)
{
	//转换成用户坐标
	AcGePoint3d pt;
	struct resbuf rbFrom, rbTo;
	rbFrom.restype = RTSHORT;
	rbFrom.resval.rint = 0;
	rbTo.restype = RTSHORT;
	rbTo.resval.rint = 1;

	acedTrans(asDblArray(point), &rbFrom, &rbTo, Adesk::kFalse, asDblArray(pt));
	
	return pt;
}

