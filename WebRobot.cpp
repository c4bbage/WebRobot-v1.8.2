// WebRobot.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WebRobot.h"
#include "WebRobotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebRobotApp

BEGIN_MESSAGE_MAP(CWebRobotApp, CWinApp)
	//{{AFX_MSG_MAP(CWebRobotApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebRobotApp construction

CWebRobotApp::CWebRobotApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWebRobotApp object

CWebRobotApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWebRobotApp initialization

BOOL CWebRobotApp::AlreadyRunning()
{
	BOOL bFound = FALSE;
	
	TCHAR strCurrentPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, strCurrentPath, sizeof(strCurrentPath));
	CString str = strCurrentPath;
	str.Replace(_T("\\"), _T("|"));
	// Try to create a mutex with the app's name
	HANDLE hMutexOneInstance = ::CreateMutex(NULL,TRUE, str);//Mutex的名字，除了Global\或者Local\开头外，其他部分不能使用斜杠
	// Already there...means that we are already running an instance
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
		bFound = TRUE;
	// Release the mutex
	if(hMutexOneInstance)
		::ReleaseMutex(hMutexOneInstance);
	return(bFound);
}

BOOL CWebRobotApp::InitInstance()
{
	if(AlreadyRunning())
	{
		if (::MessageBox(NULL, _T("已有同样的程序在运行，是否再开启一个?"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
			return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CWebRobotDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
