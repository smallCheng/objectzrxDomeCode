#pragma once
#include "resource.h"
#include "afxwin.h"

// CModelessDialog 对话框

class CModelessDialog : public CZcUiDialog
{
	DECLARE_DYNAMIC(CModelessDialog)

public:
	CModelessDialog(CWnd* pParent = NULL,HINSTANCE hInstance=NULL);   // 标准构造函数
	virtual ~CModelessDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_MODELESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnACADKeepFocus(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CString m_strStX;
	CString m_strStY;
	CString m_strStZ;
	CString m_strEnX;
	CString m_strEnY;
	CString m_strEnZ;
	afx_msg void OnBnClickedButtonSt();
	afx_msg void OnBnClickedButtonEn();
	afx_msg void OnBnClickedButtonLine();
	afx_msg void OnBnClickedButtonClose();
	//CButton m_btnPickStart;
	//CButton m_btnPickEnd;
	CAcUiPickButton m_btnPickStart;
	CAcUiPickButton m_btnPickEnd;


};
