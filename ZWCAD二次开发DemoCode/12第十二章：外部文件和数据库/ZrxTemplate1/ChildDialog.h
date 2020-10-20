#pragma once
#include "resource.h"

// CChildDialog 对话框

class CChildDialog : public CZcUiDialog
{
	DECLARE_DYNAMIC(CChildDialog)

public:
	CChildDialog(CWnd* pParent = NULL,HINSTANCE hInstance=NULL);  // 标准构造函数
	virtual ~CChildDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLine();
};
