
#include "stdafx.h"

#include "SampleDlg.h"
#include "AcExtensionModule.h"
#include "ZfgkTriangle.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND adsw_acadMainWnd();

AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);



void initApp()
{ 
	// register a command with the AutoCAD command mechanism
		//CAcModuleResourceOverride resOverride;

	//ZfgkTriangle::rxInit();
	//acrxBuildClassHierarchy();
}

void unloadApp()
{ 
		//acedRegCmds->removeGroup(_T("ZRX_DLG_SAMPLE"));  
	//deleteAcRxClass(ZfgkTriangle::desc());
}

//////////////////////////////////////////////////////////////
//
// Entry points
//
//////////////////////////////////////////////////////////////
extern "C" int APIENTRY
	DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		theArxDLL.AttachInstance(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		theArxDLL.DetachInstance();  
	}
	return 1;   // ok
}

extern "C" AcRx::AppRetCode zcrxEntryPoint( AcRx::AppMsgCode msg, void* appId)
{
	switch( msg ) 
	{
	case AcRx::kInitAppMsg: 
		acrxDynamicLinker->unlockApplication(appId);
		acrxDynamicLinker->registerAppMDIAware(appId);
		initApp(); 
		break;
	case AcRx::kUnloadAppMsg: 
		unloadApp(); 
		break;
	case AcRx::kInitDialogMsg:

		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}