// AutoGetUrls.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AutoGetUrls.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//
CMainDlg *m_pMainDlg;
/////////////////////////////////////////////////////////////////////////////
// CAutoGetUrlsApp

BEGIN_MESSAGE_MAP(CAutoGetUrlsApp, CWinApp)
	//{{AFX_MSG_MAP(CAutoGetUrlsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoGetUrlsApp construction

CAutoGetUrlsApp::CAutoGetUrlsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pMainDlg = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAutoGetUrlsApp object

CAutoGetUrlsApp theApp;

TCHAR *pName			= _T("�Զ�ץȡ");
TCHAR *pDescription		= _T("�Զ�ץȡ���������������");
TCHAR *pAbout		= _T("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"green\">�Զ�ץȡ</font><br><br><br>&nbsp;&nbsp;&nbsp;\
						 ֧��Baidu��Google��Bing������������������Զ�ץȡ��<br><br>\
						 &nbsp;&nbsp;&nbsp;��������Ҫ��֤�������������޷��л�IP����ʹ���ֶ�ץȡ���Ӳ����<br><br>\
						 ");
TCHAR *pVersion			= _T("v1.0");

typedef struct
{
	TCHAR	*pName;
	TCHAR	*pDescription;
	TCHAR	*pAbout;
	TCHAR	*pVersion;
}DLLINFO;

//��ò����Ϣ��
extern "C" __declspec(dllexport) char *GetInfo()
{
	DLLINFO *pInfo = (DLLINFO*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DLLINFO));//ʹ�õ�һ�Ķѣ������ڴ�ʹ��[�ڽ���Ĭ�϶��ڴ���������һ�������ƶ�����СΪsizeof(DLLINFO)���ڴ�ռ�]
	pInfo->pName = pName;
	pInfo->pDescription = pDescription;
	pInfo->pAbout = pAbout;
	pInfo->pVersion = pVersion;
	return (char*)pInfo;
}

extern "C" __declspec(dllexport) CDialog *ShowDlg(CWnd* pParentCWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//�������������������ǰ��ʼ����/��ָ���ǰ�������ģ��״̬,���ӻ����
	m_pMainDlg = new CMainDlg(pParentCWnd);
	if (m_pMainDlg)
	{
		m_pMainDlg->Create(IDD_MAIN_DLG);
		m_pMainDlg->SetParent(pParentCWnd);
		m_pMainDlg->ShowWindow(SW_SHOW);
	}
	
	return m_pMainDlg;
}

extern "C" _declspec(dllexport) BOOL PreTranslateMessage(MSG *pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_pMainDlg != NULL)
	{
		if (pMsg->message == WM_KEYDOWN)   
		{   
			int nVirtKey = (int)pMsg->wParam;           
			if (nVirtKey == VK_RETURN)   
			{   
				//����ǻس�����������Ҫ��������,����ʲôҲ����
				return TRUE;
			}   
			if (nVirtKey == VK_ESCAPE)   
			{   
				//�����ESC����������Ҫ��������,����ʲôҲ����
				return TRUE;
			}
		}
		return m_pMainDlg->PreTranslateMessage(pMsg);
	}
	return false;
}