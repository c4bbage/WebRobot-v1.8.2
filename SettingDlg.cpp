// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRobot.h"
#include "SettingDlg.h"
#include "WebRobotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWebRobotDlg* g_pWebRobotDlg;
/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog
IMPLEMENT_DYNCREATE(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_COMBO_USER_AGENT, m_comboUserAgent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comboUserAgent.SetWindowText(((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("WebRobot", "UserAgent", "User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (GetParent()->m_hWnd != NULL && m_comboUserAgent.m_hWnd != NULL)
	{
		RECT rc;
		g_pWebRobotDlg->m_TabCtrl.GetClientRect(&rc);
		rc.top += 2;
		MoveWindow(&rc);
	}
}

void CSettingDlg::OnButtonSave() 
{
	UpdateData();
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	CString strBuffer;
	m_comboUserAgent.GetWindowText(strBuffer);
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetString("WebRobot", "UserAgent", strBuffer);
	
	Sleep(300);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
}
