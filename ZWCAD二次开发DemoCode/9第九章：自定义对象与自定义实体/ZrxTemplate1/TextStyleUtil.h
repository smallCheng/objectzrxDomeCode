#pragma once
class CTextStyleUtil
{
public:
	CTextStyleUtil();
	~CTextStyleUtil();

	//添加文字样式记录
	static AcDbObjectId Add(const TCHAR *name, const TCHAR* fontFileName, const TCHAR *bigFontFileName);
	//获取指定名称的文字样式ID
	static AcDbObjectId GetAt(const TCHAR *name);
	//获取当前图形中所有的文字样式名称集合
	static void GetAll(std::vector<CString> &textStyles);


};

