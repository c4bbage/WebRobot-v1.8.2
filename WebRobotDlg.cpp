// WebRobotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRobot.h"
#include "WebRobotDlg.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWebRobotDlg* g_pWebRobotDlg = NULL;
BOOL m_bShutdown;	// OK to terminate TRAYTEST
#define WM_ICON_NOTIFY WM_USER+10
#define WM_TIMER_HIDEBALLOON (WM_USER+11)
/////////////////////////////////////////////////////////////////////////////
// CWebRobotDlg dialog

CWebRobotDlg::CWebRobotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebRobotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebRobotDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_bShutdown = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	((CWebRobotApp *)AfxGetApp())->m_pWebRobotDlg = this;
}

void CWebRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebRobotDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWebRobotDlg, CDialog)
	//{{AFX_MSG_MAP(CWebRobotDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENUITEM_SHOW, OnMenuitemShow)
	ON_COMMAND(ID_APP_SUSPEND, OnAppSuspend)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebRobotDlg message handlers

BOOL CWebRobotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	g_pWebRobotDlg = (CWebRobotDlg *)((CWebRobotApp *)AfxGetApp())->m_pWebRobotDlg;
	//创建tab控件
	m_TabCtrl.Create(TCS_UP | WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 300), this, 125);
	m_TabCtrl.AddPage(RUNTIME_CLASS(CMainDlg), IDD_MAIN_DLG, _T(" 主页 "), IDI_ICON_MAIN);
	m_TabCtrl.UpdateWindow();

	if (!m_TrayIcon.Enabled())
		m_TrayIcon.Create(this, WM_ICON_NOTIFY, _T(""), AfxGetApp()->LoadIcon(IDR_MAINFRAME), IDR_TRAYICON, TRUE); //最后一个参数设置是弹出气泡还是鼠标指向的时候显示消息
	
	PostMessage(WM_SIZE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWebRobotDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWebRobotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWebRobotDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_TabCtrl.m_hWnd != NULL)
	{
		//设置tab大小
		RECT rect;
		GetClientRect(&rect);
		rect.left -= 1;
		rect.right += 1;
		rect.top += 2;
		rect.bottom += 3;
		m_TabCtrl.MoveWindow(&rect);
	}
}

BOOL CWebRobotDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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

LRESULT CWebRobotDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) == WM_LBUTTONDOWN || LOWORD(lParam) == WM_LBUTTONDBLCLK)//左键单机托盘显示界面
	{
		ShowWindow(SW_NORMAL);
		BringWindowToTop();
		return TRUE;
	}
	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CWebRobotDlg::OnMenuitemShow() 
{
	ShowWindow(SW_NORMAL);	
}

void CWebRobotDlg::OnAppSuspend() 
{
	m_bShutdown = TRUE;		// really exit
	SendMessage(WM_CLOSE);
}

void CWebRobotDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bShutdown)
	{
		if (MessageBox(_T("确认退出程序吗?"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			m_bShutdown = FALSE;
			return;
		}
		CDialog::OnClose();
	}
	else
	{
		ShowWindow(SW_HIDE);
		MySetTooltipText(_T("程序已最小化到托盘..."));
	}
}

void CWebRobotDlg::MySetTooltipText(CString strText, UINT nMilliseconds)
{
	m_TrayIcon.SetTooltipText(strText, TRUE);
	SetTimer(WM_TIMER_HIDEBALLOON, nMilliseconds, NULL);
}

void CWebRobotDlg::OnTimer(UINT nIDEvent) 
{
	CString strTitle;
	switch (nIDEvent)
	{ 
	case WM_TIMER_HIDEBALLOON:
		m_TrayIcon.SetTooltipText(_T(""), TRUE);
		KillTimer(WM_TIMER_HIDEBALLOON);
		
		GetWindowText(strTitle);
		m_TrayIcon.SetTooltipText(strTitle, FALSE);
		break;
	default :
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
