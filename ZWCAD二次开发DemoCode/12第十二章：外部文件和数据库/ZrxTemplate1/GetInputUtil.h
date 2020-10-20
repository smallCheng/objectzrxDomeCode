#pragma once
class CGetInputUtil
{
public:
	CGetInputUtil();
	~CGetInputUtil();

	//指定基点的情况下提示用户拾取一个点
	static int GetPointReturnCode(const AcGePoint3d &basePoint, const TCHAR *prompt, AcGePoint3d &point);
	static bool GetPoint(const AcGePoint3d &basePoint, const TCHAR *prompt, AcGePoint3d &point);
	//提示用户拾取一个点
	static int GetPointReturnCode(const TCHAR *prompt, AcGePoint3d &point);
	static bool GetPoint(const TCHAR *prompt, AcGePoint3d &point);

	static bool GetKeyword(const TCHAR* prompt, const TCHAR *keywords, const TCHAR *firstDefault, int defaultKeyIndex, int &nRet);
	static bool GetReal(const TCHAR* prompt, double defaultVal, int precision, double &ret);
};

