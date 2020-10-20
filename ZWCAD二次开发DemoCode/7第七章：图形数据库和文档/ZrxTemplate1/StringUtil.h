#pragma once
class CStringUtil
{
public:
	CStringUtil();
	~CStringUtil();
	
	//×Ö·û´®·Ö¸ô
	static void Split(const CString &text, const TCHAR *separator, std::vector<CString> &result, bool bAddEmptyItem=true);
	//×Ö·û´®Æ´½Ó
	static CString Jion(const std::vector<CString> &item, const TCHAR *separator);

	
	
};

