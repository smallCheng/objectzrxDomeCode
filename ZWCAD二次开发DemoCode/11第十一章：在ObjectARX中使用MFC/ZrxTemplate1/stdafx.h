// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#define ZRXTEMPLATE1_MODULE

//////////////////////////////////////////////////////////////////////////
// undef _DEBUG
#ifdef _DEBUG
#define _DEBUG_DEFINED
#undef _DEBUG
#endif

#pragma pack (push, 8)
#pragma warning(disable: 4786 4996)
//#pragma warning(disable: 4098)

//-----------------------------------------------------------------------------
#define STRICT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			//- Exclude rarely-used stuff from Windows headers
#endif

//- Modify the following defines if you have to target a platform prior to the ones specified below.
//- Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER							//- Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0600			//- Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif


//-----------------------------------------------------------------------------
#include <afxwin.h>				//- MFC core and standard components
#include <afxext.h>				//- MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//- MFC OLE classes
#include <afxodlgs.h>			//- MFC OLE dialog classes
#include <afxdisp.h>			//- MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//- MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//- MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>			//- MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//- MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// dllex header
#include <afxdllx.h>

//- ObjectARX and OMF headers needs this
#include <map>


//////////////////////////////////////////////////////////////////////////
//- Include ObjectDBX/ObjectARX headers
//- Uncomment one of the following lines to bring a given library in your project.
//#define _BREP_SUPPORT_					//- Support for the BRep API
//#define _HLR_SUPPORT_						//- Support for the Hidden Line Removal API
// #define _AMODELER_SUPPORT_				//- Support for the AModeler API
//#define _ASE_SUPPORT_							//- Support for the ASI/ASE API
//#define _RENDER_SUPPORT_					//- Support for the AutoCAD Render API
//#define _ARX_CUSTOM_DRAG_N_DROP_	//- Support for the ObjectARX Drag'n Drop API
//#define _INC_LEAGACY_HEADERS_			//- Include legacy headers in this project
#include "arxHeaders.h"


//////////////////////////////////////////////////////////////////////////
// TODO: 其他系统库include

#pragma region common include 通用功能代码包含头文件
//Entity
#include "EntityUtil.h"
#include "LineUtil.h"
#include "CircleUtil.h"
#include "ArcUtil.h"
#include "PolylineUtil.h"
#include "EllipseUtil.h"
#include "SplineUtil.h"
#include "RegionUtil.h"
#include "TextUtil.h"
#include "HatchUtil.h"
#include "DimensionUtil.h"
#include "BlockUtil.h"
//Document
#include "DwgDatabaseUtil.h"
//SymbolTable
#include "LayerUtil.h"
#include "TextStyleUtil.h"
#include "ViewUtil.h"
//Geometry
#include "GePointUtil.h"
#include "MathUtil.h"
#include "GeRectangleUtil.h"
//Interaction
#include "GetInputUtil.h"
#include "SelectUtil.h"
//IO
#include "TextFileUtil.h"
#include "AppDirectoryUtil.h"
#include "IniFile.h"
//others
#include "StringUtil.h"
#include "ConvertUtil.h"
#pragma endregion 

//JIG
#include "DrawSquareJig.h"
#include "ArcBlockJigEntity.h"
#include "ArcBlockJig.h"
//DoubleClickEdit
#include "DoubleClickCircle.h"

#include "../CustomObjectDB/DwgScale.h"
#include "..\TriangleEntity\ZfgkTriangle.h"

//模态对话框
#include "MfcDialog.h"
#include "ArxDialog.h"
//非模态对话框
#include "ModelessDialog.h"
//标签对话框
#include "OptionSheet.h"
//停靠栏
#include "MyDockControlBar.h"


//编辑反应器
#include "MyEditorReactor.h"
//文档反应器
#include "MyDocReactor.h"
//数据库反应器
#include "MyDbReactor.h"
//对象反应器
#include "MyObjectReactor.h"


//外部声明变量
extern CModelessDialog *pDialog;
extern CMyDockControlBar *g_pDlgBar;
extern CMyDbReactor *g_dbReactor;	//数据库反应器

#pragma pack (pop)

#ifdef _DEBUG_DEFINED
#undef _DEBUG_DEFINED
#define _DEBUG
#endif
// redefine _DEBUG
//////////////////////////////////////////////////////////////////////////


#include "DocData.h" //- Your document specific data class holder

//- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;

