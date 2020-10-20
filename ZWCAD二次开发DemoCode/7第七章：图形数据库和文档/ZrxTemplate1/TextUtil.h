#pragma once
class CTextUtil
{
public:
	CTextUtil();
	~CTextUtil();


	//创建文字（参数：插入点、文字内容、文字样式ID、文字高度、文字的旋转角度）
	static AcDbObjectId AddText(const AcGePoint3d &ptInsert, const TCHAR* text, AcDbObjectId style = AcDbObjectId::kNull, double height = 2.5, double rotation = 0);
	//创建多行文字（参数：插入点、多行文字内容、文字样式ID、文字高度、宽度）
	static AcDbObjectId AddMText(const AcGePoint3d &ptInsert, const TCHAR* text, AcDbObjectId style = AcDbObjectId::kNull, double height = 2.5, double width = 10);
};

