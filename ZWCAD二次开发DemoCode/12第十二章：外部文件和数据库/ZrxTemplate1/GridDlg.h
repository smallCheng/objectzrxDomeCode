#pragma once
#include "GridCtrl.h"
#include "resource.h"

//-----------------------------------------------------------------------------
class CGridDlg : public CZcUiDialog 
{
	DECLARE_DYNAMIC (CGridDlg)

public:
	CGridDlg (CWnd* pParent =NULL, HINSTANCE hInstance =NULL) ;

	//{{AFX_DATA(CGridDlg)
	enum { IDD = IDD_GRID } ;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CGridDlg)
	
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
private:
	// 获得所有列的标题
	static std::vector<CString> GetAllColumnName();

	// 填充输出列表格
	void UpdateOutColGrid();

private:
	CGridCtrl m_grid;
} ;

