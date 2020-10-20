#pragma once
class CGeRectangleUtil
{
public:
	CGeRectangleUtil();
	~CGeRectangleUtil();

	//判断两个矩形是否相交
	static bool IntersectWithRectangle(const AcGePoint2d &rect1Point1, const AcGePoint2d &rect1Point2, const AcGePoint2d &rect2Point1, const AcGePoint2d &rect2Point2);
};

