// BrowserSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRobot.h"
#include "BrowserSettingDlg.h"
#include "ExClass/InputDlg/InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowserSettingDlg dialog


CBrowserSettingDlg::CBrowserSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowserSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowserSettingDlg)
	m_bCheckQMark = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetInt("GetUrls", "CheckQMark", FALSE);
	m_bBeepSound = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetInt("GetUrls", "bBeepSound", TRUE);
	m_bCheckVSCrollToEnd = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetInt("GetUrls", "bVSCrollToEnd", TRUE);
	m_strGoogleMirror = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("GetUrls", "GoogleMirror", "s.bt.gg");
	//}}AFX_DATA_INIT
}


void CBrowserSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowserSettingDlg)
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnGoogleMirror);
	DDX_Control(pDX, IDC_LIST_URL_FILTER, m_ListCtrl);
	DDX_Check(pDX, IDC_CHECK_QMARK, m_bCheckQMark);
	DDX_Check(pDX, IDC_CHECK_BEEP, m_bBeepSound);
	DDX_Check(pDX, IDC_CHECK_VSCROLLTOEND, m_bCheckVSCrollToEnd);
	DDX_Text(pDX, IDC_EDIT_GOOGLE_MIRROR, m_strGoogleMirror);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrowserSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CBrowserSettingDlg)
	ON_BN_CLICKED(IDC_CHECK_QMARK, OnCheckQmark)
	ON_BN_CLICKED(IDC_CHECK_BEEP, OnCheckBeep)
	ON_BN_CLICKED(IDC_CHECK_VSCROLLTOEND, OnCheckVscrolltoend)
	ON_COMMAND(ID_MENUITEM_INSERT, OnMenuitemInsert)
	ON_COMMAND(ID_MENUITEM_DELETE, OnMenuitemDelete)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_URL_FILTER, OnRclickListUrlFilter)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowserSettingDlg message handlers

BOOL CBrowserSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ListView_SetExtendedListViewStyle(m_ListCtrl.m_hWnd, LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0, "浏览器抓取URL时过滤以下字串", LVCFMT_LEFT, 379);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
	
	CString strBuffer = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("GetUrls", "FilterKeyWord", TEXT_FILTER);
	
	CString strKey;
	int nPos = 0, nPos1 = -1;
	while (1)
	{
		nPos1 = strBuffer.Find("|", nPos);
		if (nPos1 == -1)
			break;
		strKey = strBuffer.Mid(nPos, nPos1 - nPos);
		if (strKey.GetLength() > 0)
			m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), strKey);
		nPos = nPos1 + 1;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrowserSettingDlg::OnCheckQmark() 
{
	UpdateData();
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetInt("GetUrls", "CheckQMark", m_bCheckQMark);
}

void CBrowserSettingDlg::OnCheckBeep() 
{
	UpdateData();
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetInt("GetUrls", "bBeepSound", m_bBeepSound);
}

void CBrowserSettingDlg::OnCheckVscrolltoend() 
{
	UpdateData();
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetInt("GetUrls", "bVSCrollToEnd", m_bCheckVSCrollToEnd);
}

void CBrowserSettingDlg::OnMenuitemInsert() 
{
	CInputDialog	dlg;
	dlg.Init(_T("插入关键字"), _T("请输入要过滤的关键字:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	if (dlg.m_str.Find("|", 0) != -1)
	{
		::MessageBox(NULL, _T("关键字里不能包含“|”"), _T("提示"), MB_ICONINFORMATION);
		return;
	}
	
	int nCount = m_ListCtrl.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CString str1 = m_ListCtrl.GetItemText(i, 0);
		if (dlg.m_str.CompareNoCase(str1) == 0)
		{
			::MessageBox(NULL, "列表里已存在相同的字串！", "提示", MB_ICONINFORMATION);
			return;
		}
	}
	m_ListCtrl.InsertItem(nCount, dlg.m_str);
	
	CString strText;
	nCount = m_ListCtrl.GetItemCount();
	for (int j = 0; j < nCount; j++)
	{
		strText += m_ListCtrl.GetItemText(j, 0);
		strText += "|";
	}
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetString("GetUrls", "FilterKeyWord", strText);
}

void CBrowserSettingDlg::OnMenuitemDelete() 
{
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos  ==  NULL)
		return;
	int nItem = m_ListCtrl.GetNextSelectedItem(pos);
	m_ListCtrl.DeleteItem(nItem);
	
	CString strText;
	for (int j = 0; j < m_ListCtrl.GetItemCount(); j++)
	{
		strText += m_ListCtrl.GetItemText(j, 0);
		strText += "|";
	}
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetString("GetUrls", "FilterKeyWord", strText);
}

void CBrowserSettingDlg::OnRclickListUrlFilter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_BROWSER_SETTING);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	int	count = pSubMenu->GetMenuItemCount();
	if (m_ListCtrl.GetSelectedCount() == 0)
	{
		for (int i = count - 1; i >= 0; i--)
		{
			if (i == 0)
				continue;
			pSubMenu->DeleteMenu(i, MF_BYPOSITION);
		}
	}
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CBrowserSettingDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetString("GetUrls", "GoogleMirror", m_strGoogleMirror);
}
