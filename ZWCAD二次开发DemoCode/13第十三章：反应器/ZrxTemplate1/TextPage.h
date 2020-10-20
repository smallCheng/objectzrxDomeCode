#pragma once
#include "resource.h"
#include "afxwin.h"

// CTextPage 对话框

class CTextPage : public CZcUiTabChildDialog
{
	DECLARE_DYNAMIC(CTextPage)

public:
	CTextPage(CWnd* pParent = NULL,HINSTANCE hInstance=NULL);   // 标准构造函数
	virtual ~CTextPage();

// 对话框数据
	enum { IDD = IDD_TEXT_PAGE };
public:
	virtual void OnTabActivation(BOOL bActivate);
	virtual BOOL OnTabChanging();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_textHeight;
	CComboBox m_cboTextStyle;
	virtual BOOL OnInitDialog();

	//保存用户输入的参数保存到INI文件
	bool SaveProfiles();

	 
};
