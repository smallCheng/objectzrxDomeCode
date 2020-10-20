



#if !defined(SAMPLEDLG_H)
#define SAMPLEDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SampleDlg

#include "resource.h"
#include <dbsymtb.h>
#include <dbapserv.h>
#include <adslib.h>
#include <adui.h>
#include <acui.h>

class SampleDlg : public CAcUiDialog
{

public:
	SampleDlg(CWnd* pParent = NULL);  


	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

#endif	//SAMPLEDLG_H