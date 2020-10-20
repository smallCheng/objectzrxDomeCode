#include "stdafx.h"
#include "StringUtil.h"


CStringUtil::CStringUtil()
{
}


CStringUtil::~CStringUtil()
{
}

void CStringUtil::Split(const CString & text, const TCHAR * separator, std::vector<CString>& result, bool bAddEmptyItem)
{
	int prev = 0, current = 0;	//找到当前匹配和上一次匹配
	CString strCopyText = text;
	strCopyText.TrimRight(_T(" "));

	//第一次先查找目标字符串，有没有包含分隔符
	if (strCopyText.Find(separator, 0) < 0)
	{
		if (strCopyText.GetLength() > 0)
		{
			result.push_back(strCopyText);
		}
		return;
	}

	//循环提取中间字符串
	int length = strCopyText.GetLength();
	bool bExit = false;
	while (1)
	{
		current = strCopyText.Find(separator, prev);
		if (current == -1)
		{
			current = length;	//到最后一组了
			bExit = true;
		}

		//上一个分隔符之后和这个分隔符之前的字符，就是有效的字符串
		CString item = strCopyText.Mid(prev, current - prev);
		if (bAddEmptyItem || item.GetLength() > 0)
		{
			result.push_back(item);
		}

		if (bExit)
		{
			break;
		}
		prev = current + _tcslen(separator);
	}
	
}

CString CStringUtil::Jion(const std::vector<CString>& item, const TCHAR * separator)
{
	CString strResult;
	for (int i = 0; i < item.size(); i++)
	{
		strResult += item[i] + separator;
	}
	strResult.TrimRight(separator);
	return strResult;
}


