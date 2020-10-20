#pragma once
class CHatchUtil
{
public:
	CHatchUtil();
	~CHatchUtil();

	//创建填充（参数：填充边界ID数组、填充物名、填充比例）
	static AcDbObjectId Add(const AcDbObjectIdArray &loopIds, const TCHAR* patName, double patternScale);
};

