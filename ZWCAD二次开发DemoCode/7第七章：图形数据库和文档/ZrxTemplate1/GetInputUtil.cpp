#include "stdafx.h"
#include "GetInputUtil.h"


CGetInputUtil::CGetInputUtil()
{
}


CGetInputUtil::~CGetInputUtil()
{
}

int CGetInputUtil::GetPointReturnCode(const AcGePoint3d & basePoint, const TCHAR * prompt, AcGePoint3d & point)
{
	//将基地转换为UCS坐标
	AcGePoint3d ucsBasePoint = CConvertUtil::WcsToUcsPoint(basePoint);

	int nReturn = acedGetPoint(asDblArray(ucsBasePoint), prompt, asDblArray(point));
	if (nReturn == RTNORM)
	{
		//acedGetPoint 得到UCS坐标，转换为WCS
		point = CConvertUtil::UcsToWcsPoint(point);
	}
	return nReturn;
}

bool CGetInputUtil::GetPoint(const AcGePoint3d & basePoint, const TCHAR * prompt, AcGePoint3d & point)
{
	return (GetPointReturnCode(basePoint, prompt, point) == RTNORM);
}

int CGetInputUtil::GetPointReturnCode(const TCHAR * prompt, AcGePoint3d & point)
{
	int nReturn = acedGetPoint(NULL, prompt, asDblArray(point));
	if (nReturn == RTNORM)
	{
		point = CConvertUtil::UcsToWcsPoint(point);
	}
	return nReturn;
}

bool CGetInputUtil::GetPoint(const TCHAR *prompt, AcGePoint3d &point)
{
	return (GetPointReturnCode(prompt, point) == RTNORM);
}

bool CGetInputUtil::GetKeyword(const TCHAR * prompt, const TCHAR * keywords, const TCHAR * firstDefault, int defaultKeyIndex, int &nRet)
{
	int rc;		//返回值
	TCHAR kword[20];	//关键字
	//firstDefault参数输入NULL表示不允许直接按Enter键
	acedInitGet(firstDefault == NULL ? 1 : 0, keywords);
	CString strPrompt = prompt;
	if (firstDefault != NULL)
	{
		strPrompt += _T("<");
		strPrompt += firstDefault;
		strPrompt += _T(">");
	}
	else
	{
		strPrompt += _T(":");
	}

	rc = acedGetKword(strPrompt, kword);

	switch (rc)
	{
	case RTCAN:		//按Esc键
		return false;
	case RTNONE:	//按Enter键
		nRet = defaultKeyIndex;
		return true;
	case RTNORM:
	{
		std::vector<CString> items;
		CStringUtil::Split(keywords, _T(" "), items);
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i].CompareNoCase(kword) == 0)
			{
				nRet = i;
				break;
			}
		}
		return true;
	}
	default:
		return false;
	}

}

bool CGetInputUtil::GetReal(const TCHAR * prompt, double defaultVal, int precision, double& ret)
{
	CString strPrompt = prompt;
	strPrompt.TrimRight();
	strPrompt.TrimRight(_T(":"));
	CString strDefaultValue;
	strDefaultValue.Format(_T("<%%.%df>:"), precision);	//得到类似"%.2f"d格式字符串
	strPrompt.Format(strPrompt + strDefaultValue, defaultVal);
	ret = defaultVal;
	int rc = acedGetReal(strPrompt, &ret);
	if (rc == RTNORM || rc == RTNONE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
