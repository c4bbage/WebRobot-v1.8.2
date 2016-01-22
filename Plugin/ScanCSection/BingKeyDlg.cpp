// BingKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScanCSection.h"
#include "BingKeyDlg.h"
#include "MainDlg.h"
#include "../common/InputDlg/InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainDlg *g_pMainDlg;
/////////////////////////////////////////////////////////////////////////////
// CBingKeyDlg dialog


CBingKeyDlg::CBingKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBingKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBingKeyDlg)
	//}}AFX_DATA_INIT
}


void CBingKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBingKeyDlg)
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBingKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CBingKeyDlg)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_COMMAND(ID_MENUITEM_ADD_KEY, OnMenuitemAddKey)
	ON_COMMAND(ID_MENUITEM_DELETE_KEY, OnMenuitemDeleteKey)
	ON_COMMAND(ID_MENUITEM_USE_KEY, OnMenuitemUseKey)
	ON_COMMAND(ID_MENUITEM_COPY_KEY, OnMenuitemCopyKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBingKeyDlg message handlers

BOOL CBingKeyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CBitmap bm1;
	bm1.LoadBitmap(IDB_BITMAP_KEY);
	m_ImageList.Create(20, 20, ILC_COLORDDB | ILC_MASK, 0, 1);
	m_ImageList.Add(&bm1, RGB(255, 255, 255));
	m_ListCtrl.InsertColumn(0, _T("Key"), LVCFMT_LEFT, 430);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.SetImageList(&m_ImageList,LVSIL_SMALL);

	//m_ListCtrl.InsertItem(0, "JTAwOkpGYlRiVmMwdVdmcjViaEs3NjRKMWR5M1QyeVhDVElnRGVSM0NwUVg5cTA=");
	CString strBuffer = ((CScanCSectionApp *)AfxGetApp())->m_IniFile.GetString("ScanCSection", "Keys", "");

	CString strKey;
	int nPos = 0, nPos1 = -1;
	while (1)
	{
		nPos1 = strBuffer.Find("|", nPos);
		if (nPos1 == -1)
			break;
		strKey = strBuffer.Mid(nPos, nPos1 - nPos);
		if (strKey.GetLength() > 10)
			m_ListCtrl.InsertItem(0, strKey);
		nPos = nPos1 + 1;
	}

	CString strCurKey = ((CScanCSectionApp *)AfxGetApp())->m_IniFile.GetString("ScanCSection", "CurrentKey", "");
	RefreshList(strCurKey);
		
		return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBingKeyDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_KEY);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	int	count = pSubMenu->GetMenuItemCount();
	if (m_ListCtrl.GetSelectedCount() == 0)
	{
		pSubMenu->DeleteMenu(4, MF_BYPOSITION);
		pSubMenu->DeleteMenu(2, MF_BYPOSITION);
		pSubMenu->DeleteMenu(1, MF_BYPOSITION);
		pSubMenu->DeleteMenu(0, MF_BYPOSITION);
	}
	CPoint oPoint;
	GetCursorPos(&oPoint);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	*pResult = 0;
}

void CBingKeyDlg::OnMenuitemAddKey() 
{
	// TODO: Add your command handler code here
	CString strFakeIP;
	CInputDialog dlg;
	dlg.m_bRequired = FALSE;
	dlg.Init(_T("添加Key"), _T("请输入一个有效的Bing Key:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	m_ListCtrl.InsertItem(0, dlg.m_str);

	CString strBuffer;
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
		strBuffer += m_ListCtrl.GetItemText(i, 0) + "|";

	((CScanCSectionApp *)AfxGetApp())->m_IniFile.SetString("ScanCSection", "Keys", strBuffer);
	if (m_ListCtrl.GetItemCount() == 1)
	{
		strBuffer.TrimRight("|");
		((CScanCSectionApp *)AfxGetApp())->m_IniFile.SetString("ScanCSection", "CurrentKey", strBuffer);

		RefreshList(strBuffer);
	}
}

void CBingKeyDlg::OnMenuitemDeleteKey() 
{
	if (MessageBox("确认要删除这个key吗?", "提示", MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int iCurrSel= m_ListCtrl.GetNextSelectedItem(pos);
	if (iCurrSel < 0)
		return;//未选中含内容项
	m_ListCtrl.DeleteItem(iCurrSel);

	CString strBuffer = "";
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
		strBuffer += m_ListCtrl.GetItemText(i, 0) + "|";

	((CScanCSectionApp *)AfxGetApp())->m_IniFile.SetString("ScanCSection", "Keys", strBuffer);
	if (m_ListCtrl.GetItemCount() == 0)
		((CScanCSectionApp *)AfxGetApp())->m_IniFile.SetString("ScanCSection", "CurrentKey", "");
}

void CBingKeyDlg::OnMenuitemUseKey() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int iCurrSel= m_ListCtrl.GetNextSelectedItem(pos);
	if (iCurrSel < 0)
		return;//未选中含内容项
	CString strCurrentKey = m_ListCtrl.GetItemText(iCurrSel, 0);
	g_pMainDlg->m_strCurrentKey = strCurrentKey;
	((CScanCSectionApp *)AfxGetApp())->m_IniFile.SetString("ScanCSection", "CurrentKey", strCurrentKey);

	RefreshList(strCurrentKey);
}

void CBingKeyDlg::OnMenuitemCopyKey() 
{
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int iCurrSel= m_ListCtrl.GetNextSelectedItem(pos);
	if (iCurrSel < 0)
		return;//未选中含内容项
	CString strText = m_ListCtrl.GetItemText(iCurrSel, 0);
    //将内容保存到剪贴板。
    if (!strText.IsEmpty())
    {
        if (OpenClipboard())
        {
            EmptyClipboard();
            HGLOBAL hClipboardData = GlobalAlloc(GHND | GMEM_SHARE, (strText.GetLength() + 1) * sizeof(TCHAR));
            if (hClipboardData)
            {
                TCHAR* pszData = (TCHAR *)GlobalLock(hClipboardData);
                _tcscpy(pszData, strText);
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_TEXT, hClipboardData);
            }
            CloseClipboard();
        }
    }
}

void CBingKeyDlg::RefreshList(CString strCurrentKey)
{
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		if (strCurrentKey == m_ListCtrl.GetItemText(i, 0))
			m_ListCtrl.SetItemTextColor(i, RGB(0,0,255));
		else
			m_ListCtrl.SetItemTextColor(i, RGB(0,0,0));
	}
}