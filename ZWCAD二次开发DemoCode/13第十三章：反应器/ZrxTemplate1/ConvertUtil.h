#pragma once
class CConvertUtil
{
public:
	CConvertUtil();
	~CConvertUtil();

	static CString ToString(int val);

	//三维点转二维点
	static AcGePoint2d ToPoint2d(const AcGePoint3d &point);
	//二维点转换为三维点
	static AcGePoint3d ToPoint3d(const AcGePoint2d &point2d, double z = 0);
	//角度转弧度
	static double AngleToRadian(double angle);
	//弧度转角度
	static double RadianToAngle(double radian);

	// 字符串转换为特定的类型
	static double ToDouble(const TCHAR* str);

	//世界坐标转换到显示坐标   wcs->dcs
	static AcGePoint3d WcsToDcsPoint(const AcGePoint3d &point);
	//用户坐标转换到世界坐标	ucs->wcs
	static AcGePoint3d UcsToWcsPoint(const AcGePoint3d &point);
	//世界坐标转换到用户坐标	wcs->ucs
	static AcGePoint3d WcsToUcsPoint(const AcGePoint3d &point);
};

