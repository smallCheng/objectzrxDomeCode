#pragma once
class CTextFileUtil
{
public:
	CTextFileUtil();
	~CTextFileUtil();

	//保存(按行)
	static void Save(const TCHAR* fileName, const std::vector<CString> &lines);
	//加载到集合中（按行）
	static void Load(const TCHAR *fileName, std::vector<CString> &lines);



};

