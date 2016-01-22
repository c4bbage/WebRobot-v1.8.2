// ScanCSection.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ScanCSection.h"

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
// CScanCSectionApp

BEGIN_MESSAGE_MAP(CScanCSectionApp, CWinApp)
	//{{AFX_MSG_MAP(CScanCSectionApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanCSectionApp construction

CScanCSectionApp::CScanCSectionApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pMainDlg = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CScanCSectionApp object

CScanCSectionApp theApp;

TCHAR *pName			= _T("C段查询");
TCHAR *pDescription		= _T("查询同服域名与C段域名");
TCHAR *pAbout		= _T("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"green\">C段查询</font><br><br><br>&nbsp;&nbsp;&nbsp;\
						 可选择使用Bing API或Bing网页搜索的方式获取同IP域名，并自动去除重复。<br><br>\
						 &nbsp;&nbsp;&nbsp;可获取域名对应的IP及服务器类型。<br><br>\
						 &nbsp;&nbsp;&nbsp;因免费的BING API每月只有5000次的查询限制，因此需自行申请KEY。<br><br>\
						 &nbsp;&nbsp;&nbsp;Bing API申请地址:<a href=\"http://datamarket.azure.com/dataset/bing/search\">\
						 <font color=\"blue\">http://datamarket.azure.com/dataset/bing/search</font></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"http://www.baidu.com/s?wd=申请Bing%20Search%20API\"><font color=\"blue\">申请方法</font></a><br><br>\
						 ");
TCHAR *pVersion			= _T("v1.0");

typedef struct
{
	TCHAR	*pName;
	TCHAR	*pDescription;
	TCHAR	*pAbout;
	TCHAR	*pVersion;
}DLLINFO;

//获得插件信息的
extern "C" __declspec(dllexport) char *GetInfo()
{
	DLLINFO *pInfo = (DLLINFO*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DLLINFO));//使用单一的堆，分配内存使用[在进程默认堆内存上申请了一个不可移动、大小为sizeof(DLLINFO)的内存空间]
	pInfo->pName = pName;
	pInfo->pDescription = pDescription;
	pInfo->pAbout = pAbout;
	pInfo->pVersion = pVersion;
	return (char*)pInfo;
}

extern "C" __declspec(dllexport) CDialog *ShowDlg(CWnd* pParentCWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//调用这个函数来设置先前初始化和/或恢复先前清理后的模块状态,不加会出错
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
				//如果是回车在这里做你要做的事情,或者什么也不作
				return TRUE;
			}   
			if (nVirtKey == VK_ESCAPE)   
			{   
				//如果是ESC在这里做你要做的事情,或者什么也不作
				return TRUE;
			}
		}
		return m_pMainDlg->PreTranslateMessage(pMsg);
	}
	return false;
}
