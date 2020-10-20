#pragma once
class CRegionUtil
{
public:
	CRegionUtil();
	~CRegionUtil();

	//创建面域（参数：生成面域所需对象的ID数组）
	static AcDbObjectIdArray Add(const AcDbObjectIdArray &curveIds);
};

