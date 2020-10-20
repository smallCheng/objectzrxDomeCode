// CModelessDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ModelessDialog.h"
#include "afxdialogex.h"


// CModelessDialog 对话框

IMPLEMENT_DYNAMIC(CModelessDialog, CAcUiDialog)
BEGIN_MESSAGE_MAP(CModelessDialog, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,OnACADKeepFocus)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_ST, &CModelessDialog::OnBnClickedButtonSt)
	ON_BN_CLICKED(IDC_BUTTON_EN, &CModelessDialog::OnBnClickedButtonEn)
	ON_BN_CLICKED(IDC_BUTTON_LINE, &CModelessDialog::OnBnClickedButtonLine)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CModelessDialog::OnBnClickedButtonClose)
END_MESSAGE_MAP()
CModelessDialog::CModelessDialog(CWnd* pParent /*=NULL*/,HINSTANCE hInstance /*=NULL*/)
	: CAcUiDialog(CModelessDialog::IDD, pParent,hInstance)
	, m_strStX(_T(""))
	, m_strStY(_T(""))
	, m_strStZ(_T(""))
	, m_strEnX(_T(""))
	, m_strEnY(_T(""))
	, m_strEnZ(_T(""))
{

}

CModelessDialog::~CModelessDialog()
{
}

void CModelessDialog::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STX, m_strStX);
	DDX_Text(pDX, IDC_EDIT_STY, m_strStY);
	DDX_Text(pDX, IDC_EDIT_STZ, m_strStZ);
	DDX_Text(pDX, IDC_EDIT_ENX, m_strEnX);
	DDX_Text(pDX, IDC_EDIT_ENY, m_strEnY);
	DDX_Text(pDX, IDC_EDIT_ENZ, m_strEnZ);
	DDX_Control(pDX, IDC_BUTTON_ST, m_btnPickStart);
	DDX_Control(pDX, IDC_BUTTON_EN, m_btnPickEnd);
}


LRESULT CModelessDialog::OnACADKeepFocus(WPARAM,LPARAM)
{
	return (TRUE);
}




// CModelessDialog 消息处理程序


void CModelessDialog::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	//释放非模态对话框的内存
	delete this;
	
	if(pDialog!=NULL)
	{
		pDialog=NULL;
	}

	CZcUiDialog::PostNcDestroy();
}


BOOL CModelessDialog::OnInitDialog()
{
	CZcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	//“拾取点”按钮位图
	m_btnPickStart.AutoLoad();
	m_btnPickEnd.AutoLoad();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CModelessDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CZcUiDialog::OnClose();

	//销毁对话框
	DestroyWindow();
}


void CModelessDialog::OnBnClickedButtonSt()
{
	// TODO: 在此添加控件通知处理程序代码


	//隐藏对话框把控制权交给AutoCAD
	BeginEditorCommand();

	//提示用户输入一个点
	AcGePoint3d pt;
	if(CGetInputUtil::GetPoint(_T("\n输入一个点:"),pt))
	{
		//如果点有效，继续执行
		CompleteEditorCommand();
		m_strStX.Format(_T("%.2f"),pt.x);
		m_strStY.Format(_T("%.2f"),pt.y);
		m_strStZ.Format(_T("%.2f"),pt.z);
	}
	else
	{
		//否则取消命令（包括对话框）
		CancelEditorCommand();
	}

	//用成员变量的值更新文本显示的内容
	UpdateData(FALSE);
}


void CModelessDialog::OnBnClickedButtonEn()
{
	// TODO: 在此添加控件通知处理程序代码

	//隐藏对话框把控制权交给AutoCAD
	BeginEditorCommand();

	//提示用户输入一个点
	AcGePoint3d pt;
	if(CGetInputUtil::GetPoint(_T("\n输入一个点:"),pt))
	{
		//如果点有效，继续执行
		CompleteEditorCommand();
		m_strEnX.Format(_T("%.2f"),pt.x);
		m_strEnY.Format(_T("%.2f"),pt.y);
		m_strEnZ.Format(_T("%.2f"),pt.z);
	}
	else
	{
		//否则取消命令（包括对话框）
		CancelEditorCommand();
	}

	//用成员变量的值更新文本显示的内容
	UpdateData(FALSE);
}


void CModelessDialog::OnBnClickedButtonLine()
{
	// TODO: 在此添加控件通知处理程序代码

	//获得起点和终点的坐标
	AcGePoint3d ptStart(_wtof(m_strStX),_wtof(m_strStY),_wtof(m_strStZ));
	AcGePoint3d ptEnd(_wtof(m_strEnX),_wtof(m_strEnY),_wtof(m_strEnZ));

	//锁定文档
	acDocManager->lockDocument(curDoc());

	//创建直线
	CLineUtil::Add(ptStart,ptEnd);

	//刷新图形
	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();

	//解锁文档
	acDocManager->unlockDocument(curDoc());
}


void CModelessDialog::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码

	//销毁对话框
	DestroyWindow();
}
