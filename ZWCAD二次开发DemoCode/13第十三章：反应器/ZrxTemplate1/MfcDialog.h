#pragma once

#include "Resource.h"
#include "afxwin.h"

// CMfcDialog 对话框

class CMfcDialog : public CDialog
{
	DECLARE_DYNAMIC(CMfcDialog)

public:
	CMfcDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMfcDialog();

// 对话框数据
	enum { IDD = IDD_MFC_MODAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
};
