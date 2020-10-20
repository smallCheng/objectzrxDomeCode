#pragma once
#include "resource.h"

// CControlPage 对话框

class CControlPage : public CZcUiTabChildDialog
{
	DECLARE_DYNAMIC(CControlPage)

public:
	CControlPage(CWnd* pParent = NULL,HINSTANCE hInstance=NULL);   // 标准构造函数
	virtual ~CControlPage();

// 对话框数据
	enum { IDD = IDD_CONTROL_PAGE };
	public:
	virtual void OnTabActivation(BOOL bActivate);
	virtual BOOL OnTabChanging();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//保存参数
	bool SaveProfiles();

	int m_nRadio1;
	BOOL m_bCheck1;
};
