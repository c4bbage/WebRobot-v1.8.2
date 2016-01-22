// PluginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRobot.h"
#include "PluginDlg.h"
#include "WebRobotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWebRobotDlg* g_pWebRobotDlg;
/////////////////////////////////////////////////////////////////////////////
// CPluginDlg dialog
IMPLEMENT_DYNCREATE(CPluginDlg, CDialog)

CPluginDlg::CPluginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPluginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPluginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginDlg, CDialog)
	//{{AFX_MSG_MAP(CPluginDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginDlg message handlers

BOOL CPluginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

__inline VOID MyOutputDebugString(LPCTSTR ptzFormat, ...)
{
	va_list vlArgs;
	TCHAR tzText[1024];
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	OutputDebugString(tzText);
	va_end(vlArgs);
}

void CPluginDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	HWND hWnd = ::FindWindowEx(this->m_hWnd, NULL, "#32770", "");
	if (hWnd != NULL)
	{
		if (::IsWindowVisible(hWnd))
		{
			if (g_pWebRobotDlg->m_TabCtrl.m_hWnd != NULL)
			{
				RECT rc;
				g_pWebRobotDlg->m_TabCtrl.GetClientRect(&rc);
				rc.top += 2;
				MoveWindow(&rc);
				::PostMessage(hWnd, WM_SIZE, 0, 0);
			}
		}
	}
}

PreTM m_PreTM = NULL;//导出函数指针
BOOL CPluginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_PreTM != NULL)
	{
		if(m_PreTM(pMsg))
			return true;
	}
	if (pMsg->message == WM_KEYDOWN)   
	{   
		int nVirtKey = (int)pMsg->wParam;           
		if (nVirtKey == VK_RETURN)   
		{
			//如果是回车在这里做你要做的事情,或者什么也不作
			return TRUE;
		}   
		else if (nVirtKey == VK_ESCAPE)   
		{   
			//如果是ESC在这里做你要做的事情,或者什么也不作
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
