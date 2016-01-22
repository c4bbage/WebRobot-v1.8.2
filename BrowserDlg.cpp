// BrowserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRobot.h"
#include "BrowserDlg.h"
#include "BrowserSettingDlg.h"
#include "WebRobotDlg.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainDlg* g_pMainDlg;
extern CWebRobotDlg* g_pWebRobotDlg;
#define WM_ON_GETURLS (WM_USER+101)
/////////////////////////////////////////////////////////////////////////////
// CBrowserDlg dialog
IMPLEMENT_DYNCREATE(CBrowserDlg, CDialog)

CBrowserDlg::CBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowserDlg)
	m_bGetUrls = FALSE;
	//}}AFX_DATA_INIT
	m_lpDisp = NULL;
	m_bNeedUpdateWindow = FALSE;
	m_nListHeight = 200;
}


void CBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowserDlg)
	DDX_Control(pDX, IDC_LIST_SEARCH_KEYWORD, m_ListCtrlSearchKeyword);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_URL, m_ComboUrl);
	DDX_Control(pDX, IDC_CHECK_URLS, m_checkUrls);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BUTTON_BACKWARD, m_btnBackward);
	DDX_Control(pDX, IDC_BUTTON_FORWARD, m_btnForward);
	DDX_Control(pDX, IDC_BUTTON_GO, m_btnGo);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnSetting);
	DDX_Control(pDX, IDC_EXPLORER, m_WebBrowser);
	DDX_Check(pDX, IDC_CHECK_URLS, m_bGetUrls);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrowserDlg, CDialog)
	//{{AFX_MSG_MAP(CBrowserDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnButtonSetting)
	ON_BN_CLICKED(IDC_BUTTON_GO, OnButtonGo)
	ON_BN_CLICKED(IDC_BUTTON_BACKWARD, OnButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON_FORWARD, OnButtonForward)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_CHECK_URLS, OnCheckUrls)
	ON_COMMAND(ID_MENUITEM_OPENURL, OnMenuitemOpenurl)
	ON_COMMAND(ID_MENUITEM_COPYURL, OnMenuitemCopyurl)
	ON_COMMAND(ID_MENUITEM_SAVEURLS, OnMenuitemSaveurls)
	ON_COMMAND(ID_MENUITEM_DELETEURL, OnMenuitemDeleteurl)
	ON_COMMAND(ID_MENUITEM_CLEARLIST, OnMenuitemClearlist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SEARCH_KEYWORD, OnDblclkListSearchKeyword)
	ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTER2, OnMaxMinInfo)
	ON_NOTIFY(SPN_DELTA,     IDC_SPLITTER2, OnSplitter1Delta)
	ON_COMMAND(ID_MENUITEM_SENDTO_INJECTION, OnMenuitemSendtoInjection)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ON_GETURLS, OnMessageGetUrls)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowserDlg message handlers

BOOL CBrowserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CBitmap bm1;
	bm1.LoadBitmap(IDB_BITMAP_URL);
	m_ImageList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	m_ImageList.Add(&bm1, RGB(255, 255, 255));
	m_ListCtrl.InsertColumn(0, _T("URL"), LVCFMT_LEFT, 350);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_ListCtrlSearchKeyword.InsertColumn(0, _T("搜索技巧"), LVCFMT_LEFT, 170);
	m_ListCtrlSearchKeyword.SetExtendedStyle(m_ListCtrlSearchKeyword.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	m_ListCtrlSearchKeyword.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_ListCtrlSearchKeyword.InsertItem(0, "inurl:");
	m_ListCtrlSearchKeyword.InsertItem(1, "site:");
	m_ListCtrlSearchKeyword.InsertItem(2, "intext:");
	m_ListCtrlSearchKeyword.InsertItem(3, "allintext:");
	m_ListCtrlSearchKeyword.InsertItem(4, "intitle:");
	m_ListCtrlSearchKeyword.InsertItem(5, "filetype:");
	m_ListCtrlSearchKeyword.InsertItem(6, "link:");
	m_ListCtrlSearchKeyword.InsertItem(7, "ip:");
	m_ListCtrlSearchKeyword.InsertItem(8, "related:");
	m_ListCtrlSearchKeyword.InsertItem(9, "cache:");
	m_ListCtrlSearchKeyword.InsertItem(10, "phonebook:");
	m_ListCtrlSearchKeyword.InsertItem(11, "site:xxx.com mail");
	m_ListCtrlSearchKeyword.InsertItem(12, "site:xxx.com email");
	m_ListCtrlSearchKeyword.InsertItem(13, "site:xxx.com 邮箱");
	m_ListCtrlSearchKeyword.InsertItem(14, "site:xxx.com gmail.com");
	m_ListCtrlSearchKeyword.InsertItem(15, "\"Index of\"");
	m_ListCtrlSearchKeyword.InsertItem(16, "\"<b>Warning</b>\"");

	//设置字体
	m_ComboUrl.SendMessage(CB_SETITEMHEIGHT, 0, 18);
	m_btnSetting.SetBitmaps(IDB_BITMAP_SETTING, RGB(255, 255, 255));
	m_btnGo.SetBitmaps(IDB_BITMAP_GO, RGB(255, 255, 255));
	m_btnBackward.SetBitmaps(IDB_BITMAP_BACK, RGB(255, 255, 255));
	m_btnForward.SetBitmaps(IDB_BITMAP_FORWARD, RGB(255, 255, 255));
	m_btnRefresh.SetBitmaps(IDB_BITMAP_REFRESH, RGB(255, 255, 255));
	m_checkUrls.SetBitmaps(IDB_BITMAP_UNCHECK, RGB(255,255,255), IDB_BITMAP_CHECK, RGB(255,255,255));
	
	m_infotip.Create(this);
	m_infotip.SetMaxTipWidth(450); 
	m_infotip.SetTipTextColor(RGB(0,170,64));
	m_infotip.AddTool(GetDlgItem(IDC_BUTTON_SETTING), _T("抓取链接设置"));
	m_infotip.AddTool(GetDlgItem(IDC_BUTTON_GO), _T("转到"));
	m_infotip.AddTool(GetDlgItem(IDC_BUTTON_BACKWARD), _T("后退"));
	m_infotip.AddTool(GetDlgItem(IDC_BUTTON_FORWARD), _T("前进"));
	m_infotip.AddTool(GetDlgItem(IDC_BUTTON_REFRESH), _T("刷新"));
	m_infotip.AddTool(GetDlgItem(IDC_CHECK_URLS), _T("开启读取网页链接功能"));
	m_infotip.AddTool(GetDlgItem(IDC_LIST_SEARCH_KEYWORD), _T("双击复制"));
	
	//StatusBar
	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1200001);
	int strPartDim[2]= {300, -1};
	m_wndStatusBar.SetParts(2, strPartDim);
	StatusTextOut(0, "就绪");
	
	//Splitter
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_SPLITTER2);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER2, SPS_HORIZONTAL|SPS_DELTA_NOTIFY);
    //  register windows for splitter
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_UP,    GetDlgItem(IDC_EXPLORER));
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_DOWN,    GetDlgItem(IDC_LIST));
    m_wndSplitter.Relayout();
	m_ListCtrl.ShowWindow(SW_HIDE);
	m_ListCtrlSearchKeyword.ShowWindow(SW_HIDE);
	m_wndSplitter.ShowWindow(SW_HIDE);
	
	GetParent()->GetClientRect(&rc);
	MoveWindow(&rc);
	
	PostMessage(WM_SIZE);
	
	m_strGoogleMirror = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("GetUrls", "GoogleMirror", "s.bt.gg");
	m_strUserAgent = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("WebRobot", "UserAgent", "User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5");
	SetDlgItemText(IDC_COMBO_URL, "about:blank");
	m_WebBrowser.Navigate("about:blank", NULL, NULL, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrowserDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (GetParent()->m_hWnd != NULL && m_WebBrowser.m_hWnd != NULL)
	{
		RECT rc;
		g_pWebRobotDlg->m_TabCtrl.GetClientRect(&rc);
		rc.top += 2;
		MoveWindow(&rc);
		
		long nClientWidth = (rc.right - rc.left) - 230;
		m_ListCtrl.SetColumnWidth(0, nClientWidth);
		
		Resize();
		m_wndStatusBar.MoveWindow(&rc);
	}
}

void CBrowserDlg::MoveDlgItem(int nID, const CRect& rcPos, BOOL bRepaint)
{
    CWnd* pWnd = GetDlgItem(nID);
    if (NULL == pWnd)
        return;
    pWnd->MoveWindow(rcPos, bRepaint);
}

#define STD_GAP             3
#define STD_BUTTON_HEIGHT   21
void CBrowserDlg::Resize()
{
    CRect rcDlg;
    GetClientRect(rcDlg);
	
    CRect rcCombo;
    rcCombo.left    = rcDlg.left + 35;
    rcCombo.right   = rcDlg.right   - 200;
    rcCombo.top     = rcDlg.top     + 1;
    rcCombo.bottom  = rcCombo.top   + STD_BUTTON_HEIGHT;
    MoveDlgItem(IDC_COMBO_URL, rcCombo, TRUE);
	
	CRect rcBtnGo;
    rcBtnGo.left       = rcCombo.right + STD_GAP;
    rcBtnGo.right      = rcBtnGo.left + 35;
    rcBtnGo.bottom     = rcCombo.bottom;
    rcBtnGo.top        = rcCombo.top;
    MoveDlgItem(IDC_BUTTON_GO, rcBtnGo, TRUE);
	
	CRect rcBtnBack;
    rcBtnBack.left       = rcBtnGo.right + STD_GAP;
    rcBtnBack.right      = rcBtnBack.left + 35;
    rcBtnBack.bottom     = rcCombo.bottom;
    rcBtnBack.top        = rcCombo.top;
    MoveDlgItem(IDC_BUTTON_BACKWARD, rcBtnBack, TRUE);
	
	CRect rcBtnForward;
    rcBtnForward.left       = rcBtnBack.right + STD_GAP;
    rcBtnForward.right      = rcBtnForward.left + 35;
    rcBtnForward.bottom     = rcCombo.bottom;
    rcBtnForward.top        = rcCombo.top;
    MoveDlgItem(IDC_BUTTON_FORWARD, rcBtnForward, TRUE);
	
	CRect rcBtnRefresh;
    rcBtnRefresh.left       = rcBtnForward.right + STD_GAP;
    rcBtnRefresh.right      = rcBtnRefresh.left + 35;
    rcBtnRefresh.bottom     = rcCombo.bottom;
    rcBtnRefresh.top        = rcCombo.top;
    MoveDlgItem(IDC_BUTTON_REFRESH, rcBtnRefresh, TRUE);
	
	CRect rcBtnGetUrls;
    rcBtnGetUrls.left       = rcBtnRefresh.right + STD_GAP;
    rcBtnGetUrls.right      = rcBtnGetUrls.left + 35;
    rcBtnGetUrls.bottom     = rcCombo.bottom;
    rcBtnGetUrls.top        = rcCombo.top;
    MoveDlgItem(IDC_CHECK_URLS, rcBtnGetUrls, TRUE);
	if (m_wndSplitter.GetStyle() & WS_VISIBLE)
	{
		CRect rcSplit1;
		m_wndSplitter.GetWindowRect(rcSplit1);
		ScreenToClient(rcSplit1);
		rcSplit1.left = rcDlg.left;
		rcSplit1.right = rcDlg.right;
		rcSplit1.top = rcDlg.bottom - m_nListHeight - 3;
		rcSplit1.bottom = rcDlg.bottom - m_nListHeight;
		m_wndSplitter.MoveWindow(rcSplit1, TRUE);
		
		CRect rcList;
		rcList.left     = rcDlg.left;
		rcList.right    = rcDlg.right - 200;
		rcList.top      = rcSplit1.bottom + 2;
		rcList.bottom   = rcDlg.bottom - 23;
		MoveDlgItem(IDC_LIST, rcList, TRUE);

		CRect rcListSearchKeyword;
		rcListSearchKeyword.left     = rcList.right + 3;
		rcListSearchKeyword.right    = rcDlg.right;
		rcListSearchKeyword.top      = rcSplit1.bottom + 2;
		rcListSearchKeyword.bottom   = rcDlg.bottom - 23;
		MoveDlgItem(IDC_LIST_SEARCH_KEYWORD, rcListSearchKeyword, TRUE);
		
		CRect rcBrowser;
		rcBrowser.left     = rcDlg.left;
		rcBrowser.right    = rcDlg.right - 2;
		rcBrowser.top      = rcDlg.top + 26;
		rcBrowser.bottom   = rcSplit1.top;
		MoveDlgItem(IDC_EXPLORER, rcBrowser, TRUE);
	}
	else
	{
		CRect rcBrowser;
		rcBrowser.left     = rcDlg.left;
		rcBrowser.right    = rcDlg.right - 3;
		rcBrowser.top      = rcDlg.top + 26;
		rcBrowser.bottom   = rcDlg.bottom - 24;
		MoveDlgItem(IDC_EXPLORER, rcBrowser, TRUE);
	}
	
	CRect rcStatus;
	rcStatus.left     = rcDlg.left;
	rcStatus.right    = rcDlg.right;
	rcStatus.top      = rcDlg.bottom - STD_BUTTON_HEIGHT;
	rcStatus.bottom   = rcDlg.bottom;
	m_wndStatusBar.MoveWindow(rcStatus, TRUE);
}

void CBrowserDlg::OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  Get current pos of the child controls
    CRect rcBrowser;
    CRect rcList;
    m_WebBrowser.GetWindowRect(rcBrowser);
    m_ListCtrl.GetWindowRect(rcList);
    ScreenToClient(rcBrowser);
    ScreenToClient(rcList);
    //  return the pos limit
    SPC_NM_MAXMINPOS* pNewMaxMinPos = (SPC_NM_MAXMINPOS*)pNMHDR;
    if (IDC_SPLITTER2 == pNMHDR->idFrom)
    {
        pNewMaxMinPos->lMin = rcBrowser.top + 50;
        pNewMaxMinPos->lMax = rcList.bottom - 70;
    }
}

void CBrowserDlg::OnSplitter1Delta(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  this function just want to show you how to use the delta event
    *pResult = 0;
	
    SPC_NM_DELTA* pDelta = (SPC_NM_DELTA*)pNMHDR;
    if (NULL == pDelta)
        return;
	
	CRect rc;
	m_ListCtrl.GetClientRect(rc);
	m_nListHeight = rc.bottom - rc.top;
	if (m_nListHeight > 500)
		m_nListHeight = 500;
	
	PostMessage(WM_SIZE);
}

void CBrowserDlg::StatusTextOut(int iPane,LPCTSTR ptzFormat, ...)
{
	try
	{
		TCHAR tzText[1024];
		va_list vlArgs;
		va_start(vlArgs, ptzFormat);
		wvsprintf(tzText, ptzFormat, vlArgs);
		va_end(vlArgs);
		
		m_wndStatusBar.SetText(tzText, iPane, 0);
	}
	catch (...){}
}

void CBrowserDlg::OnButtonSetting() 
{
	CBrowserSettingDlg dlg;
	dlg.DoModal();
}

void CBrowserDlg::OnButtonGo() 
{
	try
	{
		CString strURL;
		GetDlgItemText(IDC_COMBO_URL, strURL);
		if (strURL.GetLength() < 7)
			return;
		m_WebBrowser.Navigate(strURL, 0, 0, 0, 0);
		
		//添加URL到ComboBox列表里
		CComboBox *m_box = (CComboBox *)GetDlgItem(IDC_COMBO_URL);
		CString strComboBoxText;
		BOOL bAddToComboBox = TRUE;
		for (int nComboBoxCount = 0; nComboBoxCount < m_box->GetCount(); nComboBoxCount++)
		{
			m_box->GetLBText(nComboBoxCount, strComboBoxText);
			if (strURL.CompareNoCase(strComboBoxText) == 0)
			{
				bAddToComboBox = FALSE;
				break;
			}
		}
		if (bAddToComboBox)
			m_box->AddString(strURL);
	}
	catch (...){}
}

void CBrowserDlg::OnButtonBackward() 
{
	try
	{
		m_WebBrowser.GoBack();
	}
	catch (...){}
}

void CBrowserDlg::OnButtonForward() 
{
	try
	{
		m_WebBrowser.GoForward();
	}
	catch (...){}
}

void CBrowserDlg::OnButtonRefresh() 
{
	try
	{
		m_WebBrowser.Refresh();
	}
	catch (...){}
}

void CBrowserDlg::OnCheckUrls() 
{
	try
	{
		if (m_checkUrls.GetCheck())
		{
			m_bGetUrls = TRUE;
			m_ListCtrl.ShowWindow(SW_SHOW);
			m_ListCtrlSearchKeyword.ShowWindow(SW_SHOW);
			m_wndSplitter.ShowWindow(SW_SHOW);
			StatusTextOut(0, "读取网页链接功能已开启");
			m_infotip.UpdateTipText(_T("关闭读取网页链接功能"), GetDlgItem(IDC_CHECK_URLS));
		}
		else
		{
			m_bGetUrls = FALSE;
			m_ListCtrl.ShowWindow(SW_HIDE);
			m_ListCtrlSearchKeyword.ShowWindow(SW_HIDE);
			m_wndSplitter.ShowWindow(SW_HIDE);
			StatusTextOut(0, "已关闭读取所打开网页链接的功能！");
			m_infotip.UpdateTipText(_T("开启读取网页链接功能"), GetDlgItem(IDC_CHECK_URLS));
		}
		PostMessage(WM_SIZE);
	}
	catch (...){}
}

BEGIN_EVENTSINK_MAP(CBrowserDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBrowserDlg)
	ON_EVENT(CBrowserDlg, IDC_EXPLORER, 252 /* NavigateComplete2 */, OnNavigateComplete2Explorer, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CBrowserDlg, IDC_EXPLORER, 259 /* DocumentComplete */, OnDocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CBrowserDlg, IDC_EXPLORER, 102 /* StatusTextChange */, OnStatusTextChangeExplorer, VTS_BSTR)
	ON_EVENT(CBrowserDlg, IDC_EXPLORER, 251 /* NewWindow2 */, OnNewWindow2Explorer, VTS_PDISPATCH VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CBrowserDlg::OnNavigateComplete2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	if(!m_lpDisp)
		m_lpDisp = pDisp;
}

void CBrowserDlg::OnDocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	try
	{
		if(m_lpDisp && (m_lpDisp == pDisp))//判断网页是否真正加载完毕,用于多Frame网页
		{
			//if the LPDISPATCH are same,that means it is the final DocumentComplete
			m_lpDisp = NULL;//Reset glpDisp
			
			//刷新窗口
			if (m_bNeedUpdateWindow)
				m_bNeedUpdateWindow = FALSE;
			
			PostMessage(WM_ON_GETURLS, 0, 0);//OnMessageGetUrls
		}
	}
	catch (...){}
}

BOOL CBrowserDlg::GetBaiduSearchResultRedirectUrl(CString &strReturn)
{
	BOOL bReturn = FALSE;
	CString strTemp;
	try
	{
		DWORD dwStatusCode = 0;
		CString strServer;
		DWORD dwType = 0;
		CString strTempObject;
		INTERNET_PORT wPort = 80;
		if (!AfxParseURL(strReturn, dwType, strServer, strTempObject, wPort))
			return FALSE;

		CInternetSession Session ;
		CHttpConnection *pHttpConnect = Session.GetHttpConnection(strServer);
		if( pHttpConnect )
		{
			CHttpFile* pFile = pHttpConnect->OpenRequest( CHttpConnection::HTTP_VERB_GET,
				strTempObject,
				NULL,
				1,
				NULL,
				NULL,
				INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_NO_AUTO_REDIRECT); //
			if ( pFile )
			{   
				pFile->AddRequestHeaders("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
				pFile->AddRequestHeaders(m_strUserAgent);
				pFile->AddRequestHeaders("Content-Type: application/x-www-form-urlencoded");
				pFile->AddRequestHeaders("Accept-Language: zh-cn,zh;q=0.5");
				pFile->AddRequestHeaders("Connection: Keep-Alive");
				pFile->SendRequest();
				
				pFile->QueryInfoStatusCode(dwStatusCode);
				if (dwStatusCode == 200)//<script>window.location.replace("http://web2.qq.com/")</script><noscript><META http-equiv="refresh" content="0;URL='http://web2.qq.com/'"></noscript>
				{
					CString szAllData, szData;
					szAllData.Empty();
					szData.Empty();
					while(pFile->ReadString(szData) > 0)
						szAllData += szData;
					int nPos = szAllData.Find("(\"", 0);
					if (nPos != -1)
					{
						int nPosEnd = szAllData.Find("\")</script>", 0);
						if (nPosEnd != -1)
						{
							strTemp = szAllData.Mid(nPos, nPosEnd - nPos);
							strTemp.Replace("(\"", "");
							bReturn  = TRUE;
							strReturn = strTemp;
						}
					}
				}
				else
				{
					CString strHeaderInfo;
					pFile->QueryInfo(HTTP_QUERY_LOCATION, strHeaderInfo, 0);
					
					if (strHeaderInfo.Find("http", 0) != -1)
					{
						strReturn = strHeaderInfo;
						bReturn  = TRUE;
					}
				}

				pFile->Close();
				delete pFile ;
			}
			else
				bReturn = FALSE;
			
			pHttpConnect->Close() ;
			delete pHttpConnect ;
		}
		else
			bReturn = FALSE;
	}
	catch( CInternetException *e )
	{
		bReturn = FALSE;
		e->Delete();
	}
	return bReturn;
}

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

inline BYTE toByte(const BYTE &x)
{
	return x > 57? x - 55: x - 48;
}

CString URLDecode(CString sIn)
{
	CString sOut;
	try
	{
		const int nLen = sIn.GetLength() + 1;
		register LPBYTE pOutTmp = NULL;
		LPBYTE pOutBuf = NULL;
		register LPBYTE pInTmp = NULL;
		LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
		//alloc out buffer
		pOutBuf = (LPBYTE)sOut.GetBuffer(nLen);
		
		if(pOutBuf)
		{
			pInTmp   = pInBuf;
			pOutTmp = pOutBuf;
			// do encoding
			while (*pInTmp)
			{
				if('%'==*pInTmp)
				{
					pInTmp++;
					*pOutTmp++ = (toByte(*pInTmp)%16<<4) + toByte(*(pInTmp+1))%16;//高4位+低4位
					pInTmp++;
				}
				else if('+'==*pInTmp)
					*pOutTmp++ = ' ';
				else
					*pOutTmp++ = *pInTmp;
				pInTmp++;
			}
			*pOutTmp = '\0';
			sOut.ReleaseBuffer();
		}
		sIn.ReleaseBuffer();
	}
	catch (...){}
	return sOut;
}

CString URLEncode(CString sIn)
{
	CString sOut;
	try
	{
		const int nLen = sIn.GetLength() + 1;
		register LPBYTE pOutTmp = NULL;
		LPBYTE pOutBuf = NULL;
		register LPBYTE pInTmp = NULL;
		LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
		//alloc out buffer
		pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3);
		
		if(pOutBuf)
		{
			pInTmp   = pInBuf;
			pOutTmp = pOutBuf;
			// do encoding
			while (*pInTmp)
			{
				if(isalnum(*pInTmp) || '-'==*pInTmp || '_'==*pInTmp || '.'==*pInTmp)
					*pOutTmp++ = *pInTmp;
				else if(isspace(*pInTmp))
					*pOutTmp++ = '+';
				else
				{
					*pOutTmp++ = '%';
					*pOutTmp++ = toHex(*pInTmp>>4);//高4位
					*pOutTmp++ = toHex(*pInTmp%16);//低4位
				}
				pInTmp++;
			}
			*pOutTmp = '\0';
			sOut.ReleaseBuffer();
		}
		sIn.ReleaseBuffer();
	}
	catch (...){}

	return sOut;
}

//编码URL中的汉字
CString UrlEncodeChinese(CString str, bool fRaw)
{
	CString urlstr;
	try
	{
		for(int i = 0; i < str.GetLength(); i++)
		{
			CHAR c = str[i];
			if(fRaw && c == '+') urlstr += "%2B";
			else if(c > 0x20 && c < 0x7f && c != '&') urlstr += c;
			else if(c == 0x20) urlstr += fRaw ? ' ' : '+';
			else {CString tmp; tmp.Format("%%%02x", (BYTE)c); urlstr += tmp;}
		}
	}
	catch (...){return str;}
	
	return urlstr;
}

CString Utf8ToAnsi(CString szUtf8)
{
	TCHAR * pSrc = (TCHAR*)szUtf8.GetBuffer(0);
	
	int i = MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, NULL, 0);  
	
	WCHAR * pszW = new WCHAR[i+1]; pszW[i] = 0;
	
	MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, pszW, i);
	i = WideCharToMultiByte(CP_ACP, 0, pszW, -1, NULL, 0, NULL, NULL);   
	
	TCHAR * pszT = new TCHAR[i+1]; pszT[i] = 0;
	WideCharToMultiByte(CP_ACP, 0, pszW, -1, pszT, i, NULL, NULL);
	
	szUtf8 = pszT;
	
	delete [] pszT;
	delete [] pszW;
	
	return szUtf8;
}

BOOL IsWin8System()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if(GetVersionEx((OSVERSIONINFO *)&os))
    {
		if (os.dwMajorVersion == 6 && os.dwMinorVersion > 1)
			return TRUE;
		else if (os.dwMajorVersion > 6)
			return TRUE;
    }
    else
        return FALSE;
	
	return FALSE;
}

BOOL CBrowserDlg::GetUrlFile(CString url, CString& refString)
{
	BOOL bRetVal = FALSE;
	if (IsWin8System())//win8系统特殊处理
	{
		if(m_GetWeb.OnGetTheWeb(url, refString))//这个类里我设置了获取的文件大于5Mb则停止获取数据，直接取前部分数据
		{
			bRetVal = TRUE;
		}
		else
		{
			StatusTextOut(0, "%s", m_GetWeb.OnQueryErrer());
			bRetVal = FALSE;
		}
	}
	else
	{
		IHTMLDocument2 *spDisp=NULL;
		spDisp = (IHTMLDocument2*)m_WebBrowser.GetDocument();
		
		if (spDisp != NULL)
		{
			HGLOBAL hMemory;
			hMemory = GlobalAlloc(GMEM_MOVEABLE, 0);
			if (hMemory != NULL)
			{
				CComQIPtr<IPersistStreamInit> spPersistStream = spDisp;
				if (spPersistStream != NULL)
				{
					CComPtr<IStream> spStream;
					if (SUCCEEDED(CreateStreamOnHGlobal(hMemory, TRUE, &spStream)))
					{
						spPersistStream->Save(spStream, FALSE);
						LPCTSTR pstr = (LPCTSTR) GlobalLock(hMemory);
						if (pstr != NULL)
						{
							// Stream is always ANSI, but CString assignment operator will convert implicitly.
							bRetVal = TRUE;
							TRY
							{                        
								refString = pstr;
							}
							CATCH_ALL(e)
							{
								bRetVal = FALSE;
								e->Delete();
							}
							END_CATCH_ALL
								
								if(bRetVal == FALSE)
									GlobalFree(hMemory);
								else
									GlobalUnlock(hMemory);
						}
					}
				}
			}
		}
	}
    return bRetVal;
}

bool CBrowserDlg::ScrollToEnd()
{
	try
	{
		CComPtr <IDispatch > spDisp; 
		spDisp = m_WebBrowser.GetDocument();

		HRESULT hr;
		IHTMLDocument2 *spDoc = NULL;
		IHTMLElementCollection* pCollection;
		hr = spDisp->QueryInterface( IID_IHTMLDocument2, (void**)&spDoc );
		if ( FAILED( hr ))
		{
			return false;
		}
		hr = spDoc->get_all(&pCollection);
		if (FAILED(hr))
		{
			return false;
		}
		long len;
		hr = pCollection->get_length(&len);
		if (FAILED(hr))	
		{
			return false;
		}
		// 遍历所有的元素
		for (long l=0; l <len; l++)	
		{
			VARIANT varIndex, var2;
			VariantInit(&varIndex);
			VariantInit(&var2);
			varIndex.vt = VT_I4;
			varIndex.lVal = l;
			IDispatch* spDisp = NULL;
			pCollection->item( varIndex, var2, &spDisp );
			IHTMLElement* pElem;
			spDisp->QueryInterface( IID_IHTMLElement, (LPVOID*) &pElem );
			CComBSTR tagName;
			pElem->get_tagName(&tagName);
			CString str = tagName;
			str.MakeUpper();
			//if ( str.Compare( _T( "IFRAME" ) ) == 0 || str.Compare( _T( "FRAME" ) ) == 0 )
			{
				IHTMLElement2* pElement2 = NULL;
				hr = pElem->QueryInterface(IID_IHTMLElement2, (void**)&pElement2);
				if (FAILED( hr ))
				{
					return false;
				}
				long lScrollHeight;
				hr = pElement2->get_scrollHeight(&lScrollHeight);
				if ( FAILED( hr ))
				{
					return false;	
				}
				hr = pElement2->put_scrollTop( lScrollHeight );
				if ( FAILED( hr ))
				{
					return false;
				}
				pElement2->Release();
			}
			pElem->Release();
			spDisp->Release();
		}
		pCollection->Release();
		spDoc->Release();
	}
	catch (...){}
	return true;
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

// 判断URL里是否包含字符“?”
BOOL CBrowserDlg::IsQMarkNotExist(CString strUrl)
{
	if (strUrl.Find("?", 0) == -1)//没找到问号
		return TRUE;
	return FALSE;
}

DWORD WINAPI CBrowserDlg::GetURLThread(LPVOID lparam)
{
	CBrowserDlg *pThis = (CBrowserDlg *)lparam;

	CoInitialize(NULL);//以单线程的方式创建com对象
	try
	{
		CString str_htmlCode;
		if(pThis->GetUrlFile(pThis->strCurrentUrl, str_htmlCode))
		{
			pThis->StatusTextOut(0, "%s", "获取页面源码成功，可以继续浏览别的网页。");
			HyperLink m_thisURL;
			// URL Vector
			vector<HyperLink>m_pageURL;
			m_thisURL.str_Hyperlink = pThis->strCurrentUrl;
			CWebHost m_host(str_htmlCode, m_pageURL, m_thisURL);
			// 过滤包含特定字符的URL
			CString strFilter;
			BOOL bFindFilterString = FALSE;

			// 对获取来的URL逐个判断，并加入到列表
			for(int i = 0 ; i < m_pageURL.size(); i++)
			{
				bFindFilterString = FALSE;
				CString strTemp;
				strTemp.Format("%s", m_pageURL[i].str_Hyperlink);

				// 判断URL里是否包含字符“http”
				CString strTemp1 = strTemp.Mid(0, 4);
				if (strTemp1.CompareNoCase("http") != 0)
					continue;

				//处理百度搜索的数据
				if (strTemp.Find(".baidu.com", 0) != -1)
				{
					if (strTemp.Find(".baidu.com/link?url=", 0) == -1)
						continue;
					if (!pThis->GetBaiduSearchResultRedirectUrl(strTemp))
						continue;
					if (pThis->m_bCheckQMark && pThis->IsQMarkNotExist(strTemp))
						continue;
					strTemp.Format("%s", URLDecode(strTemp));
				}
				else if (strTemp.Find(".google.", 0) != -1 || strTemp.Find("s.bt.gg", 0) != -1 || strTemp.Find(pThis->m_strGoogleMirror, 0) != -1)//处理谷歌搜索的数据
				{
					int nPosGoogleUrlStart = strTemp.Find("/url?q=", 0);
					if (nPosGoogleUrlStart == -1)
					{
						nPosGoogleUrlStart = strTemp.Find("rl?url=", 0);
						if (nPosGoogleUrlStart == -1)
							continue;
					}

					int nPosGoogleUrlEnd = strTemp.Find("&", nPosGoogleUrlStart);
					if (nPosGoogleUrlEnd != -1)
					{
						int nEnd = strTemp.Find("&amp;", nPosGoogleUrlStart);
						if (nEnd != -1)
						{
							strTemp.SetAt(nEnd, 0x00);
							strTemp.SetAt(nEnd + 1, 0x00);
						}

						strTemp = strTemp.Mid(nPosGoogleUrlStart + 7, nPosGoogleUrlEnd - nPosGoogleUrlStart - 7);
						if (pThis->m_bCheckQMark && pThis->IsQMarkNotExist(strTemp))
							continue;
						
						strTemp.Format("%s", URLDecode(strTemp));
					}
					else
						continue;
				}
				else if (pThis->m_bCheckQMark && pThis->IsQMarkNotExist(strTemp))
					continue;
				// 判断url里是否包含被过滤的字串
				for (int nStringListCount = 0; nStringListCount < pThis->strUrlFilterArray.GetSize(); nStringListCount++)
				{
					strFilter = pThis->strUrlFilterArray.GetAt(nStringListCount);
					if (strTemp.Find(strFilter, 0) != -1)
					{
						bFindFilterString = TRUE;
						break;
					}
				}
				if (bFindFilterString)
					continue;

				pThis->m_ListCtrl.InsertItem(pThis->m_ListCtrl.GetItemCount(), strTemp);
			}
			
			CString strColumnHeaderText;
			strColumnHeaderText.Format("URL - (%d)", pThis->m_ListCtrl.GetItemCount());
			LVCOLUMN lvCom = {0};
			lvCom.mask = LVCF_TEXT;
			lvCom.cchTextMax = 256;
			lvCom.pszText = strColumnHeaderText.GetBuffer(0);
			pThis->m_ListCtrl.SetColumn(0, &lvCom);
			
			pThis->StatusTextOut(0, "※ 读取网页所有链接完成！");
			if (pThis->m_bBeepSound)
				MessageBeep(MB_OK);
		}
	}
	catch (...)
	{
		pThis->StatusTextOut(0, "获取网页链接失败！");
	}
	CoUninitialize();//关闭当前线程的COM库

	return -1;	
}

LRESULT CBrowserDlg::OnMessageGetUrls(WPARAM wParam, LPARAM lParam)//@@@
{
	try
	{
		//把打开的页面链接放到地址栏
		strCurrentUrl.Format("%s", m_WebBrowser.GetLocationURL());
		if (strCurrentUrl == "http://www.baidu.com/" || strCurrentUrl == "http://www.google.com/" || strCurrentUrl == "http://www.google.com.hk/" || strCurrentUrl == "http://www.bing.com/" || strCurrentUrl == "http://cn.bing.com/")
			return 0;

		SetDlgItemText(IDC_COMBO_URL, strCurrentUrl);
		//抓取链接完毕时把浏览器滚动条滚动到末尾
		if (((CWebRobotApp *)AfxGetApp())->m_IniFile.GetInt("GetUrls", "bVSCrollToEnd", TRUE))
			ScrollToEnd();

		m_bBeepSound = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetInt("GetUrls", "bBeepSound", TRUE);
		m_bCheckQMark = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetInt("GetUrls", "CheckQMark", FALSE);
		
		strUrlFilterArray.RemoveAll();
		CString strAllFilters = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("GetUrls", "FilterKeyWord", TEXT_FILTER);
		CString strKey;
		int nPos = 0, nPos1 = -1;
		while (1)
		{
			nPos1 = strAllFilters.Find("|", nPos);
			if (nPos1 == -1)
				break;
			strKey = strAllFilters.Mid(nPos, nPos1 - nPos);
			if (strKey.GetLength() > 0)
				strUrlFilterArray.Add(strKey);
			nPos = nPos1 + 1;
		}
		
		//读取网页里的全部url
		if (m_bGetUrls)
		{
			StatusTextOut(0, "读取网页链接中...");
			
			HANDLE hIpThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GetURLThread, this, 0, NULL);
			WaitForSingleObject(hIpThread, 100);
			CloseHandle(hIpThread);
		}
		else
			StatusTextOut(0, "当前未开启读取网页链接功能！");
	}
	catch (...){}
	return 0;
}

void CBrowserDlg::OnStatusTextChangeExplorer(LPCTSTR Text) 
{
	try
	{
		CString strStatusText;
		strStatusText.Format("%s", Text);
		StatusTextOut(1, "%s", strStatusText);
		CString strTemp = strStatusText.Mid(0, 4);
		if (strTemp.CompareNoCase("http") == 0)
			strNewWindowUrl.Format("%s", Text);
	}
	catch (...){}
}

void CBrowserDlg::OnNewWindow2Explorer(LPDISPATCH FAR* ppDisp, BOOL FAR* Cancel) 
{
	try
	{
		m_bNeedUpdateWindow = TRUE;
		CRect rc;
		int nResult = MessageBox("检测到有动作欲使用用外部浏览器打开网页，用本程序打开网页请点“是”，用外部浏览器打开请点“否”，否则请点“取消”。", "提示", MB_YESNOCANCEL | MB_ICONINFORMATION);
		switch(nResult)
		{
		case IDYES:
			*ppDisp = m_WebBrowser.GetApplication();
			m_WebBrowser.Navigate(strNewWindowUrl, 0, 0, 0, 0);
			break;
		case IDNO:
			break;
		case IDCANCEL:
			*ppDisp = m_WebBrowser.GetApplication();
			break;
		}
	}
	catch (...){}
}

void CBrowserDlg::OnMenuitemOpenurl() 
{
	UpdateData();
	try
	{
		POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
		while(pos)
		{
			int	nItem = m_ListCtrl.GetNextSelectedItem(pos);
			CString strURL = m_ListCtrl.GetItemText(nItem, 0);
			ShellExecute(0, "open", strURL, NULL, NULL, SW_SHOW);
		}
	}
	catch (...){}
}

void CBrowserDlg::OnMenuitemCopyurl() 
{
    int nItem;
    CString strText(_T(""));
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_ListCtrl.GetNextSelectedItem(pos);
        strText += m_ListCtrl.GetItemText(nItem, 0) + _T("\r\n");
    }
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

void CBrowserDlg::OnMenuitemSaveurls() 
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("URL：%Y-%m-%d %H-%M-%S");
	
    CFileDialog dlg(FALSE, _T("txt"), strTime, OFN_OVERWRITEPROMPT, _T("*.txt|*.txt||"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
	
    CFile file;
	CString strData;
    if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
    {
		for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
		{
            strData.Empty();
            strData.Format(_T("%s\r\n"), m_ListCtrl.GetItemText(i, 0));
            file.Write(strData, strData.GetLength());
		}
        file.Close();
		StatusTextOut(0, "列表内容导出成功");
    }
}

void CBrowserDlg::OnMenuitemDeleteurl() 
{
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos  ==  NULL)
		return;
	else
	{
		while (pos)
		{
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);
			m_ListCtrl.DeleteItem(nItem);
			pos = m_ListCtrl.GetFirstSelectedItemPosition();
		}
	}
	CString strColumnHeaderText;
	strColumnHeaderText.Format("URL - (%d)", m_ListCtrl.GetItemCount());
	LVCOLUMN lvCom = {0};
	lvCom.mask = LVCF_TEXT;
	lvCom.cchTextMax = 256;
	lvCom.pszText = strColumnHeaderText.GetBuffer(0);
	m_ListCtrl.SetColumn(0, &lvCom);
}

void CBrowserDlg::OnMenuitemClearlist() 
{
	m_ListCtrl.DeleteAllItems();
	
	CString strColumnHeaderText;
	strColumnHeaderText.Format("URL - (%d)", 0);
	LVCOLUMN lvCom = {0};
	lvCom.mask = LVCF_TEXT;
	lvCom.cchTextMax = 256;
	lvCom.pszText = strColumnHeaderText.GetBuffer(0);
	m_ListCtrl.SetColumn(0, &lvCom);
}

BOOL CBrowserDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE)//不加这个调试会崩溃
		m_infotip.RelayEvent(pMsg);
	
	if (pMsg->wParam == 83 && GetKeyState(VK_CONTROL) < 0)//Ctrl+S
	{
		if ((m_ListCtrl.GetStyle() & WS_VISIBLE) && m_ListCtrl.GetItemCount() > 0)
		{
			OnMenuitemSaveurls();
			return TRUE;
		}
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

void CBrowserDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_BROWSER_LIST);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	int	count = pSubMenu->GetMenuItemCount();
	if (m_ListCtrl.GetSelectedCount() == 0)
	{
		for (int i = 0; i < count; i++)
		{
			pSubMenu->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	
	*pResult = 0;
}

void CBrowserDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 23;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CBrowserDlg::OnDblclkListSearchKeyword(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nItem;
    CString strText(_T(""));
    POSITION pos = m_ListCtrlSearchKeyword.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_ListCtrlSearchKeyword.GetNextSelectedItem(pos);
        strText += m_ListCtrlSearchKeyword.GetItemText(nItem, 0);
		break;
    }
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
	*pResult = 0;
}

void CBrowserDlg::OnMenuitemSendtoInjection() 
{
	// TODO: Add your command handler code here
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("TEMPURLFILE_%Y-%m-%d %H-%M-%S.txt");
	
	char strTempTxtPath[MAX_PATH];
	GetTempPath(MAX_PATH, strTempTxtPath);
	lstrcat(strTempTxtPath, strTime);
	
    CFile file;
	CString strData;
    if (file.Open(strTempTxtPath, CFile::modeCreate | CFile::modeWrite))
    {
		for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
		{
            strData.Empty();
            strData.Format(_T("%s\r\n"), m_ListCtrl.GetItemText(i, 0));
            file.Write(strData, strData.GetLength());
		}
        file.Close();

		g_pMainDlg->LoadCustomPlugin("注入检测", strTempTxtPath);
    }
}
