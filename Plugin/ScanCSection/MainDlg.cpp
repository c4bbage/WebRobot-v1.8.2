// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScanCSection.h"
#include "MainDlg.h"
#include "HtmlCode.h"
#include "BingKeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef struct tagThreadParameter
{
	int nIpPos;
	CString strCurrentIp;
	CMainDlg	*pThis;
}CScanThreadParameter,*LPCScanThreadParameter;

#define SHOW_1 "剩余线程数:%d    \\"
#define SHOW_2 "剩余线程数:%d    /"
#define SHOW_3 "剩余线程数:%d    —"
#define TEXT_NOINFO				"@NOINFO@"
#define TEXT_KEY_INVALID		"@KEYINVALID@"
#define TEXT_CONNECT_STRING		" -> "

#define WM_ON_SHOWDLG (WM_USER+100)

CMainDlg *g_pMainDlg = NULL;
/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_Url = _T("");
	m_bGetIpAddress = ((CScanCSectionApp *)AfxGetApp())->m_IniFile.GetInt("ScanCSection", "bGetIpAddress", TRUE);
	m_bGetDetailServerInfo = ((CScanCSectionApp *)AfxGetApp())->m_IniFile.GetInt("ScanCSection", "bGetDetailServerInfo", FALSE);
	m_nCScanBegin = 1;
	m_nCScanEnd = 254;
	//}}AFX_DATA_INIT
	m_bScanning = FALSE;
	m_bCurrentScanModeIsSingleServerScan = TRUE;
	m_bIsCdnIp = FALSE;
	((CScanCSectionApp *)AfxGetApp())->m_pMyMainDlg = this;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_EDIT_SHOW, m_editShow);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_btnScan);
	DDX_Control(pDX, IDC_BUTTON_PARSE, m_btnParseServer);
	DDX_Control(pDX, IDC_COMBO_IP, m_comboIP);
	DDX_Control(pDX, IDC_COMBO_INTERFACE, m_comboInterface);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT_URL, m_Url);
	DDX_Check(pDX, IDC_CHECK_GET_DOMAIN_IP, m_bGetIpAddress);
	DDX_Check(pDX, IDC_CHECK_GET_SERVER_TYPE, m_bGetDetailServerInfo);
	DDX_Text(pDX, IDC_EDIT_START_IP, m_nCScanBegin);
	DDV_MinMaxInt(pDX, m_nCScanBegin, 1, 254);
	DDX_Text(pDX, IDC_EDIT_END_IP, m_nCScanEnd);
	DDV_MinMaxInt(pDX, m_nCScanEnd, 1, 254);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PARSE, OnButtonParseUrl)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_COMMAND(ID_MENUITEM_EXPAND, OnMenuitemExpand)
	ON_COMMAND(ID_MENUITEM_COLLAPSE, OnMenuitemCollapse)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_COMMAND(ID_MENUITEM_SAVE_HTML, OnMenuitemSaveHtml)
	ON_COMMAND(ID_MENUITEM_SAVE_TXT, OnMenuitemSaveTxt)
	ON_COMMAND(ID_MENUITEM_SAVE_URL, OnMenuitemSaveUrl)
	ON_COMMAND(ID_MENUITEM_COPY_SELECTED, OnMenuitemCopySelected)
	ON_COMMAND(ID_MENUITEM_CLEAR, OnMenuitemClear)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_CHECK_GET_SERVER_TYPE, OnCheckGetServerType)
	ON_BN_CLICKED(IDC_CHECK_GET_DOMAIN_IP, OnCheckGetDomainIp)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, OnSelchangeComboInterface)
	ON_BN_CLICKED(IDC_STATIC_INTERFACE, OnStaticInterface)
	ON_BN_CLICKED(IDC_STATIC_IP, OnStaticIp)
	ON_EN_SETFOCUS(IDC_EDIT_URL, OnSetfocusEditUrl)
	ON_COMMAND(ID_MENUITEM_DELETE_SELECTED, OnMenuitemDeleteSelected)
	ON_CBN_SELCHANGE(IDC_COMBO_IP, OnSelchangeComboIp)
	ON_COMMAND(ID_MENUITEM_DELETE_ERROR_URL, OnMenuitemDeleteErrorUrl)
	ON_CBN_SETFOCUS(IDC_COMBO_IP, OnSetfocusComboIp)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ON_SHOWDLG, OnMessageShowDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers
//进入Internet选项，选择高级选项卡，把SSL 2.0、3.0和TLS 1.2的选项全部勾选上。刷新页面，可以正常地获取了！问题解决！
void WriteReg()
{
	HKEY hKey;
	DWORD dwLastError;
	dwLastError = 168;
	char strSubKey[1024] = "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings";
	if (RegOpenKey(HKEY_CURRENT_USER, strSubKey, &hKey) == ERROR_SUCCESS)
		RegSetValueEx(hKey, "SecureProtocols", 0, REG_DWORD, (LPBYTE)&dwLastError, sizeof(DWORD));
	RegCloseKey(hKey);
}

void TransParentDC(CRect rect,CDC * pDC)
{
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(pDC);
	CBitmap m_Bitmap;
	m_Bitmap.CreateCompatibleBitmap
		(pDC,rect.Width(),rect.Height());
	CBitmap *pOldBitmap = 
		m_MemDC.SelectObject(&m_Bitmap);
	m_MemDC.FillSolidRect(0,0,rect.Width(),
		rect.Height(),GetSysColor(COLOR_MENU));
	
	COLORREF cor =  pDC->GetPixel(0,0);
	for(int y = 0; y < rect.Height();y++)
	{
		for(int x = 0; x < rect.Width(); x++)
		{
			COLORREF ch = pDC->GetPixel(x,y);
			if(ch != cor)
				m_MemDC.SetPixelV(x,y,ch);
		}
	}
	pDC->BitBlt(0,0,rect.Width(),
		rect.Height(),&m_MemDC,0,0,SRCCOPY);
	m_MemDC.SelectObject(pOldBitmap);
	m_Bitmap.DeleteObject();
}

CSize CMainDlg::LoadMyBitmap(CImageList *imgList, UINT nID)//使bmp图片透明
{
	CDC * pDC = GetDC();
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(pDC);
	CSize m_Size = pDC->GetTextExtent("刷新");
	ReleaseDC(pDC);
	CRect rect(0,0,60,32);
	CBitmap *pBitmap, *pOldBitmap;
	pBitmap = new CBitmap;
	pBitmap->LoadBitmap(nID);
	pOldBitmap = m_MemDC.SelectObject(pBitmap);
	TransParentDC(rect,&m_MemDC);
	m_MemDC.SelectObject(pOldBitmap);
	imgList->Add(pBitmap,GetSysColor(COLOR_MENU));
	pBitmap->DeleteObject();
	delete pBitmap;
	return m_Size;
}

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_pMainDlg = (CMainDlg *)((CScanCSectionApp *)AfxGetApp())->m_pMyMainDlg;
	// TODO: Add extra initialization here
	m_editShow.ShowWindow(SW_HIDE);

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);

	m_comboMode.SetCurSel(0);
	m_comboInterface.SetCurSel(0);
	m_comboMode.SendMessage(CB_SETITEMHEIGHT, 0, 18);
	m_comboInterface.SendMessage(CB_SETITEMHEIGHT, 0, 18);
	ShowCScanCtrl(SW_HIDE);

	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1200001);
	int strPartDim[2]= {600, -1};
	m_wndStatusBar.SetParts(2, strPartDim);

	m_ImageList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	LoadMyBitmap(&m_ImageList, IDB_BITMAP_IP);
	LoadMyBitmap(&m_ImageList, IDB_BITMAP_SITE);
	m_TreeCtrl.SetImageList(&m_ImageList,TVSIL_NORMAL);
	::SendMessage(m_TreeCtrl.m_hWnd, TVM_SETITEMHEIGHT, 23, 0);

	SetDlgItemText(IDC_EDIT_URL, "http://");

	WriteReg();
	m_strCurrentKey = ((CScanCSectionApp *)AfxGetApp())->m_IniFile.GetString("ScanCSection", "CurrentKey", "");

	m_infotip.Create(this);
	m_infotip.SetMaxTipWidth(450); 
	m_infotip.SetTipTextColor(RGB(0,170,64));
	m_infotip.AddTool(GetDlgItem(IDC_COMBO_IP), _T(""));
	m_infotip.AddTool(GetDlgItem(IDC_STATIC_IP), _T(""));
	m_infotip.AddTool(GetDlgItem(IDC_STATIC_INTERFACE), _T("添加删除Bing Key"));

	PostMessage(WM_SIZE);

	return FALSE;  // return TRUE unless you set the focus to a control
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

void CMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if (GetParent()->m_hWnd != NULL && m_wndStatusBar.m_hWnd != NULL)
	{
		RECT rc;
		GetParent()->GetClientRect(&rc);
		MoveWindow(&rc);
		Resize();
		m_wndStatusBar.MoveWindow(&rc);
	}
}

void CMainDlg::MoveDlgItem(int nID, const CRect& rcPos, BOOL bRepaint)
{
    CWnd* pWnd = this->GetDlgItem(nID);
    if (NULL == pWnd)
        return;
    pWnd->MoveWindow(rcPos, bRepaint);
}

void CMainDlg::Resize()
{
    CRect rcDlg;
	GetClientRect(rcDlg);
	
	if (m_TreeCtrl.m_hWnd)
	{
		CRect rcTree;
		rcTree.left    = rcDlg.left + 3;
		rcTree.right   = rcDlg.right - 3;
		rcTree.top     = rcDlg.top + 95;
		rcTree.bottom  = rcDlg.bottom - 25;
		this->MoveDlgItem(IDC_TREE, rcTree, TRUE);
	}
}

void CMainDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;	//主程序结束的时候销毁
	CDialog::PostNcDestroy();
}

void CMainDlg::ShowCScanCtrl(BOOL bShow)
{	
	GetDlgItem(IDC_STATIC_1)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_2)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_START_IP)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_END_IP)->ShowWindow(bShow);
}

void CMainDlg::StatusTextOut(int iPane,LPCTSTR ptzFormat, ...)
{
	TCHAR tzText[1024];
	va_list vlArgs;
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	va_end(vlArgs);
	
	m_wndStatusBar.SetText(tzText, iPane, 0);
}

CString intToString(int nIn)
{
	CString str;
	str.Format(_T("%d"), nIn);
	return str;
}

//获取网页代码
CString getURLContext(LPCTSTR pstrURL, CString strHeader)
{
	CString szData, szAllData;
	CInternetSession cs(_T("session"), 0, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD);//设置不缓冲
	CHttpFile *pF = NULL;
	try
	{
		pF = (CHttpFile*)cs.OpenURL(pstrURL, 1, INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD, strHeader, strHeader.GetLength());
		
		while(pF->ReadString(szData))
			szAllData += szData;
		
		if (pF != NULL)
		{
			pF->Close();
			delete pF;
			pF = NULL;
		}
		if (cs != NULL)
		{
			cs.Close();
			delete cs;
		}
	}
	catch(...)
	{
		szAllData = "";
		if (pF != NULL)
		{
			pF->Close();
			delete pF;
			pF = NULL;
		}
		if (cs != NULL)
		{
			cs.Close();
			delete cs;
		}
	}
	return szAllData;
}

CString GetIPAddr()
{
	CString strTemp;
	int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
	n1 = 90;
	n2 = 82;
	n3 = 3 + rand()%251;
	n4 = 2 + rand()%252;
	strTemp.Format("%d.%d.%d.%d", n1, n2, n3, n4);
	
	return strTemp;
}

CString CMainDlg::GetBingApiData(CString strURL)
{
	int nRetry = 1;
	DWORD dwFlags;
	DWORD dwStatus = 0;
	DWORD dwStatusLen = sizeof(dwStatus);
	CString strLine, m_strHtml;
	
	DWORD dwRet = 0, dwLength = 0;
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	CString strServer = _T(""), strObject = _T("");
	
	DWORD dwType = 0;
	INTERNET_PORT wPort = 80;
	
	AfxParseURL(strURL, dwType, strServer, strObject, wPort);

	try {
START:
		CInternetSession m_InetSession(_T(""),
			0,
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL,
			NULL,
			INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD);    //设置不缓冲
		
		m_InetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5 * 1000);
		m_InetSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 5 * 1000);
		m_InetSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 5 * 1000, 0);
		m_InetSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 5 * 1000); //发送超时
		m_InetSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 5 * 1000); //接收超时

		pServer = m_InetSession.GetHttpConnection(strServer, INTERNET_FLAG_SECURE, wPort, NULL, NULL);

		if(pServer) 
		{
			pFile = (CHttpFile*)pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1,
				NULL, NULL,
				INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_SECURE |
				INTERNET_FLAG_EXISTING_CONNECT |
				INTERNET_FLAG_RELOAD |
				INTERNET_FLAG_NO_CACHE_WRITE |
				INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
				INTERNET_FLAG_IGNORE_CERT_CN_INVALID
				);
			//get web server option
			pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			//set web server option
			pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
			pFile->AddRequestHeaders("X-Forwarded-For: " + GetIPAddr());
			pFile->AddRequestHeaders("User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5");
			
			CString   strHeaders;  
			strHeaders = _T("Authorization:Basic ");
			strHeaders += Base64Encode("%00:" + m_strCurrentKey);
			pFile->AddRequestHeaders(strHeaders);
			if(pFile->SendRequest()) 
			{
				pFile->QueryInfoStatusCode(dwStatus);
				while(pFile->ReadString(strLine) > 0) 
				{
					m_strHtml += strLine;
				}
			}
		}
		if (m_InetSession != NULL)
			m_InetSession.Close();
	} 
	catch (CInternetException* e)
	{
		e->Delete();
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
	
	if (pFile != NULL)
	{
		pFile->Close();
		delete pFile;
		pFile = NULL;
	}
	if (pServer != NULL)
	{
		pServer->Close();
		delete pServer;
		pServer = NULL;
	}

	if (dwStatus == 401 || dwStatus == 503)
		return TEXT_KEY_INVALID;
	else if(m_strHtml.Find("{\"results\":[{\"", 0) != -1 && m_strHtml.Find("__metadata", 0) != -1)
		return m_strHtml;
	else if (dwStatus != 200)
	{
		if(nRetry--)//重试
		{
			Sleep(1000);
			goto START;
		}
		CString strShow, strError;
		m_editShow.GetWindowText(strShow);
		strError.Format("【获取失败】\r\n响应码: %d\r\n返回信息: %s\r\n请测试一下页面是否能打开: %s", dwStatus, m_strHtml, strURL);
		if (strShow.GetLength() != 0)
			strShow += "\r\n\r\n";
		strShow += strError;
		m_editShow.ShowWindow(SW_SHOW);
		m_editShow.SetWindowText(strShow);
		return "";
	}
	else
		return "";
}

CString GetIpAddress(CString strURL)
{
	try
	{
		strURL.MakeLower();
		strURL.Replace("http://", "");
		strURL.Replace("https://", "");
		
		int nPos1 = strURL.Find("/", 0);
		if (nPos1 != -1)
		{
			strURL = strURL.Mid(0, nPos1);
		}
		else
		{
			//strURL = strURL;
		}
		strURL.TrimLeft(" ");
		strURL.TrimLeft("\t");
		strURL.TrimRight(" ");
		strURL.TrimRight("\t");
		
		CString m_ip;
		PHOSTENT hostinfo;
		hostinfo = gethostbyname(strURL);
		m_ip = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
		strURL = m_ip;
	}
	catch (...)
	{
		strURL = "获取IP失败！";
	}
	return strURL;
}

CString GetServerDetailInfo(CString strServer)
{
	int nPos = strServer.Find(" (", 0);
	if (nPos != -1)
	{
		strServer.SetAt(nPos, 0x00);
		strServer.Insert(0, "http://");
	}
	nPos = strServer.Find(" -> ", 0);
	if (nPos != -1)
		strServer.SetAt(nPos, 0x00);

	CString strReturn = "Unknown", szAllData;
	CInternetSession ss(_T("session"), 0, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD);//设置不缓冲
	CHttpFile *pF = NULL;
    try
	{
		ss.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 10 * 1000);
		pF = (CHttpFile *)ss.OpenURL(strServer, 1, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_NO_AUTO_REDIRECT);
		pF->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, szAllData, 0);
		int nPos1 = szAllData.Find("Server: ", 0);
		if (nPos1 == -1)
		{
			strReturn = "Unknown";
		}
		else
		{
			nPos1 += 8;
			int nPos2 = szAllData.Find("\r\n", nPos1);
			strReturn = szAllData.Mid(nPos1, nPos2 - nPos1);
			if (strReturn.Find("Microsoft", 0) != -1)
			{
				strReturn.Insert(0, "Windows, ");
			}
		}
	}
	catch(...)
	{
		strReturn = "获取失败!";
	}
	if (pF != NULL)
	{
		pF->Close();
		delete pF;
		pF = NULL;
	}
	if (ss != NULL)
	{
		ss.Close();
		delete ss;
	}
	return strReturn;
}

CString TrimFileName(CString strURL)
{
	int nPos = 0;
	int nCurPos = 0;
	CString strTmpURL;
	int nMarkNum = 0;
	while (1)
	{
		nPos = strURL.Find("/", nPos);
		if (nPos == -1)
			break;
		nPos++;
		nMarkNum++;
		nCurPos = nPos;
		if (nMarkNum > 2)
			break;
	}
	strTmpURL = strURL.Mid(0, nCurPos);
	return strTmpURL;
}

//UTF8转ANSI
void Utf8ToAnsi(CString &strUTF8)
{
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,NULL,NULL);
	WCHAR *wszBuffer = new WCHAR[nLen+1];
	nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,wszBuffer,nLen);
	wszBuffer[nLen] = 0;
	
	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
	CHAR *szBuffer = new CHAR[nLen+1];
	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
	szBuffer[nLen] = 0;
	
	strUTF8 = szBuffer;
	//清理内存
	delete []szBuffer;
	delete []wszBuffer;
}

void CMainDlg::OnButtonParseUrl()
{
	UpdateData(TRUE);
	if (m_bScanning || m_Url.GetLength() == 0)
	{
		return;
	}
	m_Url.TrimLeft(" ");
	m_Url.TrimLeft("\t");
	m_Url.TrimRight(" ");
	m_Url.TrimRight("\t");
	if (m_Url.GetLength() < 1)
	{
		StatusTextOut(0, "域名格式错误！");
		return;
	}
	if (m_Url.Find(".", 0) == -1)
	{
		StatusTextOut(0, "域名格式错误！");
		return;
	}
	
	if (m_hGetIpThread != NULL)
	{
		DWORD dwExitCode;
		WaitForSingleObject(m_hGetIpThread, 100);
		if(GetExitCodeThread(m_hGetIpThread, &dwExitCode))
		{
			if(dwExitCode == STILL_ACTIVE)
				TerminateThread(m_hGetIpThread, dwExitCode);
		}
		m_hGetIpThread = NULL;
	}

	m_comboIP.ResetContent();
	m_bCurrentScanModeIsSingleServerScan = (m_comboMode.GetCurSel() == 0)?TRUE:FALSE;
	m_hGetIpThread = ThreadTemplate::StartThread<CMainDlg, DWORD>(this, GetIpAddrThread);
	if (m_hGetIpThread == NULL)
	{
		StatusTextOut(0, "获取URL对应的IP失败！");
		return;
	}
}

int isipv4str(const char *str)
{
    int i, a[4];
	char end;
    if( sscanf(str, "%d.%d.%d.%d%c", &a[0], &a[1], &a[2], &a[3], &end) != 4 )
        return 0;
    for(i=0; i<4; i++) if (a[i] < 0 || a[i] > 255) return 0;
    return 1;
}

DWORD CMainDlg::GetIpAddrThread()
{
	((CEdit *)GetDlgItem(IDC_EDIT_URL))->EnableWindow(FALSE);
	try
	{
		m_bIsCdnIp = FALSE;
		StatusTextOut(0, "正在获取URL对应的IP...");
		SetDlgItemText(IDC_COMBO_IP, "");
		m_Url.MakeLower();
		m_Url.Replace("http://", "");
		m_Url.Replace("https://", "");
		m_Url.TrimLeft("/");
		m_Url.TrimLeft("\\");
		m_Url.TrimLeft(" ");
		m_Url.TrimLeft("\t");
		int nPos = m_Url.Find(":", 0);
		if (nPos != -1)
		{
			m_Url.SetAt(nPos, 0x00);
			m_Url.Format("%s", m_Url);
		}
		
		int nPos1 = m_Url.Find("/", 0);
		if (nPos1 != -1)
		{
			m_Url = m_Url.Mid(0, nPos1);
		}
		else if (nPos1 == -1)
		{
			//m_Url = m_Url;
		}
		else
		{
			StatusTextOut(0, "域名格式错误！");
			return 0;
		}
		m_Url.TrimLeft(" ");
		m_Url.TrimLeft("\t");
		m_Url.TrimRight(" ");
		m_Url.TrimRight("\t");
		
		CString m_ip;
		WSADATA wsadata; 
		int n; 
		char hostname[256];
		hostent *phostent;
		struct sockaddr_in sa;
		if(WSAStartup(MAKEWORD(2, 2),&wsadata)!=0)
		{
			StatusTextOut(0, "获取域名失败[WSAStartup() failed]！");
			return 0;
		}
		
		memset(hostname, 0, sizeof(hostname));
		wsprintf(hostname, "%s", m_Url);
		phostent = gethostbyname(hostname);
		if(phostent == NULL) 
			return 0;

		for(n = 0; phostent->h_addr_list[n]; n++)
		{
			memcpy(&sa.sin_addr.s_addr,phostent->h_addr_list[n],phostent->h_length);
			m_ip.Format("%s", inet_ntoa(sa.sin_addr));
			m_comboIP.AddString(m_ip);
		}

		CString strTip;
		strTip = phostent->h_name;
		if (strTip != m_Url && m_Url.Find(strTip, 0) == -1)
		{
			m_comboIP.AddString(phostent->h_name);
			m_bIsCdnIp = TRUE;
			strTip.Format("加速或代理: %s", phostent->h_name);
			m_infotip.UpdateTipText(strTip, GetDlgItem(IDC_COMBO_IP));
			m_infotip.UpdateTipText(strTip, GetDlgItem(IDC_STATIC_IP));
			SetDlgItemText(IDC_STATIC_IP, "CDN:");
		}
		else
		{
			m_infotip.UpdateTipText("真实主机", GetDlgItem(IDC_COMBO_IP));
			m_infotip.UpdateTipText("真实主机", GetDlgItem(IDC_STATIC_IP));
			SetDlgItemText(IDC_STATIC_IP, "真实主机:");
		}

		GetDlgItem(IDC_COMBO_IP)->RedrawWindow();
		GetDlgItem(IDC_STATIC_IP)->RedrawWindow();

		m_comboIP.SetCurSel(0);
		WSACleanup();
		
		m_comboIP.GetLBText(0, m_ip);
		CString strTmpIPAddr;
		strTmpIPAddr = m_ip.Mid(0, m_ip.ReverseFind('.'));
		CString strIPAddrStart;
		CString strIPAddrEnd;
		strIPAddrStart.Format("%s.1", strTmpIPAddr);
		strIPAddrEnd.Format("%s.254", strTmpIPAddr);
		StatusTextOut(0, "域名:[%s] -> IP:[%s] -> C段:[%s ～ %s]", m_Url, m_ip, strIPAddrStart, strIPAddrEnd);
		SetDlgItemText(IDC_EDIT_START_IP, "1");
		SetDlgItemText(IDC_EDIT_END_IP, "254");


		//获取地理位置
		CString szData, szAllData;
		szAllData = "http://int.dpool.sina.com.cn/iplookup/iplookup.php?ip=";
		szAllData += m_ip;
		
		try
		{
			CInternetSession cs(_T("session"), 0, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD);//设置不缓冲
			CHttpFile *pF = NULL;
			try
			{
				pF = (CHttpFile*)cs.OpenURL(szAllData, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD);
				szAllData.Empty();
				
				while(pF->ReadString(szData))
					szAllData += szData;
				
				if (pF != NULL)
				{
					pF->Close();
					delete pF;
					pF = NULL;
				}
				if (cs != NULL)
				{
					cs.Close();
					delete cs;
				}
			}
			catch(...)
			{
				szAllData = "";
				if (pF != NULL)
				{
					pF->Close();
					delete pF;
					pF = NULL;
				}
				if (cs != NULL)
				{
					cs.Close();
					delete cs;
				}
			}
			szAllData.Replace(_T("-"), _T(""));
			szAllData.Replace(_T("4"), _T(""));
			szAllData.Replace(_T("3"), _T(""));
			szAllData.Replace(_T("2"), _T(""));
			szAllData.Replace(_T("1"), _T(""));
			szAllData.Replace(_T("0"), _T(""));
			szAllData.TrimLeft(_T("\t"));
			szAllData.TrimRight(_T("\t"));
			szAllData.Replace(_T("\t"), _T(" "));
			StatusTextOut(1, szAllData);
		}
		catch (...){StatusTextOut(1, "获取地理位置失败");}
	}
	catch (...)
	{
		StatusTextOut(0, "获取IP失败！");
	}
	((CEdit *)GetDlgItem(IDC_EDIT_URL))->EnableWindow(TRUE);
	return 0;
}

CString GetHttpDomain(CString strURL)
{
	CString strReturn;
	CString strServerName;
	CString strBuffer;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	if (AfxParseURL((LPCTSTR)strURL, dwServiceType, strServerName, strBuffer, nPort))
	{
		if (nPort == 80)
			strReturn = "http://" + strServerName + "/";
		else
			strReturn = "https://" + strServerName + "/";
	}
	
	return strReturn;
}

CString SplitString(CString str, CString strLeft, CString strRight)
{
	CString strReturn = "";
	int nPos0 = 0, nPos1 = 0;
	if ((nPos0 = str.Find(strLeft)) != -1)
	{
		if ((nPos1 = str.Find(strRight, nPos0)) != -1)
		{
			strReturn = str.Left(nPos1);
			strReturn.Delete(0, nPos0);
			strReturn.Replace(strLeft, "");
			strReturn.TrimLeft(" ");
			strReturn.TrimRight(" ");
		}
	}
	return strReturn;
}

CString SplitString(CString str, CString strEnd, int nMode = 0)//nMode	0:获取左边，1:获取右边
{
	CString strReturn = "";
	int nPos0 = 0;
	if ((nPos0 = str.Find(strEnd, 0)) != -1)
	{
		if (nMode == 0)
			strReturn = str.Left(nPos0);
		else
			strReturn = str.Right(str.GetLength() - nPos0 - 4);

		strReturn.TrimLeft(" ");
		strReturn.TrimRight(" ");
	}
	return strReturn;
}

void GetBingApiResult(CString str, CStringArray &sArray)
{
	int nPos1 = 0;
	CString strBuffer, strTitle, strUrl;
	int nPos = str.Find("{\"results\":[{\"", 0);
	if (nPos == -1)
		return;
	
	while (1)
	{
		nPos1 = str.Find("\",\"Title\":\"", nPos);
		
		if (nPos1 == -1)
			break;
		
		int nPos2 = str.Find("\"}", nPos1);
		
		strBuffer = str.Mid(nPos1, nPos2 - nPos1 + 2);
		
		strTitle = SplitString(strBuffer, "\"Title\":\"", "\",\"");
		strUrl = SplitString(strBuffer, "\"Url\":\"", "\"}");
		strUrl = GetHttpDomain(strUrl);
		strUrl.MakeLower();
		sArray.Add(strUrl + " -> " + strTitle);

		nPos = nPos2;//"__metadata":
	}
}

void CMainDlg::OnWorkEnd()
{
	StatusTextOut(0, "正在终止扫描...");
	DWORD dwExitCode;
	if(m_hWorkThread != NULL)
	{
		WaitForSingleObject(m_hWorkThread, 200);
		if(GetExitCodeThread(m_hWorkThread, &dwExitCode))
		{
			if(dwExitCode == STILL_ACTIVE)
				TerminateThread(m_hWorkThread, dwExitCode);
		}
		m_hWorkThread = NULL;
		
		//节点下面没数据的删掉
		int n = 0;
		HTREEITEM nItem[256] = {0};
		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
		nItem[n] = hNextItem;
		while (hNextItem != NULL)
		{
			hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
			if (m_TreeCtrl.GetChildItem(hNextItem) == NULL)
			{
				n++;
				nItem[n] = hNextItem;
			}
		}
		for (int i = 0; i < 256; i++)
		{
			if (nItem[i] == 0)
				break;
			m_TreeCtrl.DeleteItem(nItem[i]);
		}
		
		m_bScanning = FALSE;
		StatusTextOut(0, "扫描已停止！");
		SetDlgItemText(IDC_BUTTON_SCAN, "开始扫描");
	}
}

void CMainDlg::OnButtonScan() 
{
	UpdateData();

	m_bCurrentScanModeIsSingleServerScan = (m_comboMode.GetCurSel() == 0)?TRUE:FALSE;
	if (m_bScanning == FALSE)	//开始扫描
	{
		m_comboIP.GetWindowText(m_IpAddr);
		if (m_IpAddr.GetLength() < 1)
		{
			StatusTextOut(0, "ip地址不能为空！");
			return;
		}
		m_TreeCtrl.DeleteAllItems();
		m_editShow.ShowWindow(SW_HIDE);
		m_editShow.SetWindowText("");

		if (m_comboInterface.GetCurSel() == 0)
			m_hWorkThread = ThreadTemplate::StartThread<CMainDlg, DWORD>(this, BingAPIScanThread);
		else if (m_comboInterface.GetCurSel() == 1)
			m_hWorkThread = ThreadTemplate::StartThread<CMainDlg, DWORD>(this, BingWebScanThread);

		if (m_hWorkThread == NULL)
		{
			StatusTextOut(0, "扫描失败！");
			return;
		}
		StatusTextOut(0, "扫描已开始！");
		SetDlgItemText(IDC_BUTTON_SCAN, "停止扫描");
	}
	else						//手动终止扫描
		OnWorkEnd();
}

void CMainDlg::InsertIP(HTREEITEM hItem)
{
	CString strData;
	strData.Format(_T("%s"), m_TreeCtrl.GetItemText(hItem));
	if (strData.GetLength() == 0)
		return;
	int nPos = strData.Find(TEXT_CONNECT_STRING, 0);
	if (nPos == -1)
		return;
	strData.Insert(nPos + 4, "[" + GetIpAddress(strData) + "] ");
	m_TreeCtrl.SetItemText(hItem, strData);
}

CString CMainDlg::GetFirstChildItemUrl(HTREEITEM hItem)
{
	CString strUrl;
	HTREEITEM hChildItem = m_TreeCtrl.GetChildItem(hItem);
	if (hChildItem != NULL)
		strUrl.Format(_T("%s"), m_TreeCtrl.GetItemText(hChildItem));
	else
		strUrl.Format(_T("%s"), m_TreeCtrl.GetItemText(hItem));

	return strUrl;
}

DWORD CMainDlg::BingAPIScanThread()
{
	m_bScanning = TRUE;

	try
	{
		CString strCurrentUrl, strCurrentIpAddr, strIpPos, strDetailInfo;
		int iMainItem = 0;
		int nShowPoint = 0;
		if (m_bCurrentScanModeIsSingleServerScan)//扫描单服务器
		{
			nThreadNum = 0;
			nThreadNum++;
			strCurrentIpAddr = m_IpAddr;
			strCurrentIpAddr.TrimLeft(" ");
			strCurrentIpAddr.TrimLeft("\t");
			strCurrentIpAddr.TrimRight(" ");
			strCurrentIpAddr.TrimRight("\t");

			//初始化线程所需的参数
			int nFindTime = 0;
			for (int m = 0; m < 15; m++)
			{
				if (strCurrentIpAddr.GetAt(m) == '.')
				{
					nFindTime++;
					if (nFindTime == 3)
					{
						strCurrentUrl = strCurrentIpAddr.Mid(0, m);
						strIpPos = strCurrentIpAddr.Mid(m + 1, strCurrentIpAddr.GetLength() - m);
						break;
					}
				}
			}

			CScanThreadParameter lparam;
			lparam.pThis = this;
			lparam.nIpPos = atoi(strIpPos);
			lparam.strCurrentIp = strCurrentUrl;

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BingApiGetData, (LPVOID)&lparam, 0, NULL);
			Sleep(200);

			while (nThreadNum > 0)
			{
				Sleep(300);
				nShowPoint++;
				if (nShowPoint == 1)
				{
					StatusTextOut(0, SHOW_1, nThreadNum);
				}
				else if (nShowPoint == 2)
				{
					StatusTextOut(0, SHOW_2, nThreadNum);
				}
				else
				{
					StatusTextOut(0, SHOW_3, nThreadNum);
					nShowPoint = 0;
				}
			}
			MyExpandTree(TVI_ROOT, TVE_EXPAND);
		}
		else
		{
			CString strTmpIPAddr, strBuffer;
			strTmpIPAddr = m_IpAddr.Mid(0, m_IpAddr.ReverseFind('.'));
			CString strCSection;

			if (m_nCScanBegin < 1 || m_nCScanEnd > 254 || m_nCScanBegin > m_nCScanEnd)
			{
				m_nCScanBegin = 1;
				m_nCScanEnd = 254;
			}
			
			strCSection.Format("%s.%d ～ %s.%d", strTmpIPAddr, m_nCScanBegin, strTmpIPAddr, m_nCScanEnd);
			
			CScanThreadParameter lparam;
			lparam.pThis = this;
			
			nThreadNum = 0;
			for (int n = m_nCScanBegin; n <= m_nCScanEnd; n++)
			{
				strBuffer = strTmpIPAddr;
				strBuffer += "." + intToString(n);
				m_TreeCtrl.InsertItem(strBuffer, 0, 0, TVI_ROOT, TVI_LAST);
			}

			for (n = m_nCScanBegin; n <= m_nCScanEnd; n++)
			{
				if (!m_bScanning)//已停止扫描
					break;

				nThreadNum++;
				lparam.nIpPos = n;
				lparam.strCurrentIp = strTmpIPAddr;
				
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BingApiGetData, (LPVOID)&lparam, 0, NULL);
				StatusTextOut(0, "线程数: %d, 扫描进度: %d/%d, 正在扫描:%s.%d", nThreadNum, n - m_nCScanBegin, m_nCScanEnd - m_nCScanBegin + 1, strTmpIPAddr, n);
				Sleep(200);
			}
			
			while (nThreadNum > 0)
			{
				Sleep(300);
				nShowPoint++;
				if (nShowPoint == 1)
				{
					StatusTextOut(0, SHOW_1, nThreadNum);
				}
				else if (nShowPoint == 2)
				{
					StatusTextOut(0, SHOW_2, nThreadNum);
				}
				else
				{
					StatusTextOut(0, SHOW_3, nThreadNum);
					nShowPoint = 0;
				}
			}
		}
		if (m_bGetDetailServerInfo || m_bGetIpAddress)
		{
			//获取IP和服务器信息
			CString strIP, strTempUrl;
			HTREEITEM hChildItem = NULL;
			HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
			while (hNextItem != NULL)
			{
				//插入服务器信息
				if (m_bGetDetailServerInfo)
				{
					StatusTextOut(0, "正在获取服务器信息...");
					strIP = m_TreeCtrl.GetItemText(hNextItem);
					strDetailInfo = GetServerDetailInfo(GetFirstChildItemUrl(hNextItem));
					strDetailInfo.Insert(0, "   ⊕ ");
					strDetailInfo.Insert(0, strIP);
					m_TreeCtrl.SetItemText(hNextItem, strDetailInfo);
				}

				//插入IP信息
				if (m_bGetIpAddress)
				{
					StatusTextOut(0, "正在获取网站对应的IP...");
					hChildItem = m_TreeCtrl.GetChildItem(hNextItem);
					while (hChildItem != NULL)
					{
						InsertIP(hChildItem);
						hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
					}
				}

				hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
			}
		}
	}
	catch (...)
	{
		::MessageBox(0, "获取数据失败！", "提示", MB_ICONINFORMATION);
	}

	m_bScanning = FALSE;
	StatusTextOut(0, "扫描完成！");
	SetDlgItemText(IDC_BUTTON_SCAN, "开始扫描");
	MessageBeep(MB_ICONEXCLAMATION);

	return 0;
}

void CMainDlg::MyExpandTree(HTREEITEM hTreeItem, UINT nCode)
{
	if(!hTreeItem) 
		return; 
	HTREEITEM hTempTreeItem = hTreeItem; 
	m_TreeCtrl.Expand(hTempTreeItem, nCode); 
	
    HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hTreeItem);
    while (hNextItem != NULL)
    {
		m_TreeCtrl.Expand(hNextItem, nCode);
        MyExpandTree(hNextItem, nCode);
        hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
    }
	m_TreeCtrl.Expand(hTreeItem, nCode);
}

HTREEITEM CMainDlg::IsItemExist(HTREEITEM hItemParent, CString const& strItem)
{
	BOOL bDoesItemExist=FALSE;
	HTREEITEM hExistItem = NULL;
	ASSERT(strItem.GetLength());
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hItemParent?hItemParent:TVI_ROOT);
	while (NULL != hChild && !bDoesItemExist)	
	{
		CString str = m_TreeCtrl.GetItemText(hChild);//获取结点字符串信息
		if (0 == str.CompareNoCase(strItem))
		{
			bDoesItemExist = TRUE;
			hExistItem = hChild;
			break;
		}
		else
		{
			hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
		}
	}
	return hExistItem;
}

CString GetDomain(CString strURL)
{
	CString strServerName;
	CString strBuffer;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	if (!AfxParseURL((LPCTSTR)strURL, dwServiceType, strServerName, strBuffer, nPort))
		return "";

	return strServerName;
}

void CMainDlg::QuChongFu(CStringArray &sArray)
{
	int nPos = -1;
	BOOL bAdd = TRUE;
	CString strTemp;
	CStringArray sArrayTemp;
	for (int i = sArray.GetSize() - 1; i >= 0; i--)
	{
		strTemp = GetDomain(sArray.GetAt(i));
		bAdd = TRUE;
		for (int j = 0; j < sArrayTemp.GetSize(); j++)
		{
			if (sArrayTemp.GetAt(j) == strTemp)
			{
				bAdd = FALSE;
				sArray.RemoveAt(i);
				break;
			}
		}
		if (bAdd)
			sArrayTemp.Add(strTemp);
	}
}

HTREEITEM CMainDlg::InsertInfoToTreeCtrl(CString strIP, CString strInfo, BOOL bInsertNum)
{
	HTREEITEM hRoot = NULL;
	HTREEITEM hTempItem = NULL;
	
	hRoot = IsItemExist(TVI_ROOT, strIP);//IP
	if (hRoot == NULL)
	{
		hRoot = m_TreeCtrl.InsertItem(strIP, 0, 0, TVI_ROOT, TVI_LAST);
		m_TreeCtrl.SetItemData(hRoot, -1);
	}
	if (hRoot != NULL)
	{
		if (strInfo != "")
		{
			if (bInsertNum)//插入网站数目
			{
				m_TreeCtrl.SetItemText(hRoot, strIP + " (" + strInfo + ")");
				return NULL;
			}
			else if (strInfo == TEXT_NOINFO)//什么都没查到，删除
			{
				m_TreeCtrl.DeleteItem(hRoot);
				return NULL;
			}
			hTempItem = IsItemExist(hRoot, strInfo);
			if (hTempItem == NULL)
			{
				hTempItem = m_TreeCtrl.InsertItem(strInfo, 1, 1, hRoot, TVI_LAST);
				if (hTempItem != NULL)
					m_TreeCtrl.SetItemData(hTempItem, -2);
			}
		}
	}
	return hRoot;
}

DWORD WINAPI CMainDlg::BingApiGetData(LPVOID lparam)
{
	CScanThreadParameter *par = (CScanThreadParameter *)lparam;
	int nIpAddr = par->nIpPos;
	CString strTempAddr = par->strCurrentIp;
	CMainDlg *pThis = par->pThis;

	CStringArray sArray;
	CString strCurrentIpAddr;
	strCurrentIpAddr.Format("%s.%d", strTempAddr, nIpAddr);
	pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, "");

	int nCurPos = 0;
begin:
	try
	{
		CString strCurrentUrl, strCurrentHtmlData, strShowData, strTempURL, strTempTitle, strURLListTempData, strTempItemText;
		strCurrentUrl = "https://api.datamarket.azure.com/Bing/Search/Web?Query=%27ip:";
		strCurrentUrl += strCurrentIpAddr;
		strCurrentUrl += "%27&$format=json&$top=50&$skip=";
		strCurrentUrl += intToString(nCurPos);
		
		strCurrentHtmlData.Empty();

		strCurrentHtmlData = pThis->GetBingApiData(strCurrentUrl);
		if (strCurrentHtmlData == TEXT_KEY_INVALID)
		{
			if (pThis->m_bScanning)
			{
				pThis->m_bScanning = FALSE;
				::MessageBox(NULL, _T("key暂时无法使用或已失效，请更换一个有效的key！"), _T("提示"), MB_ICONINFORMATION);
				pThis->PostMessage(WM_ON_SHOWDLG, 0, 0);//OnMessageShowDlg
			}

			return -1;
		}
		Utf8ToAnsi(strCurrentHtmlData);
		GetBingApiResult(strCurrentHtmlData, sArray);

		if (strCurrentHtmlData.Find(",\"__next\":\"", 0) != -1)//还有数据没读完
		{
			nCurPos += 50;
			goto begin;
		}
	}
	catch (...){}

	if (sArray.GetSize() == 0)
		pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, TEXT_NOINFO);//没有信息，删除该节点
	else
	{
		pThis->QuChongFu(sArray);
		for (int i = 0; i < sArray.GetSize(); i++)
			pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, sArray.GetAt(i));
		
		pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, intToString(sArray.GetSize()), TRUE);//插入数量
	}

	pThis->nThreadNum--;
	return -1;
}

void CMainDlg::OnSelchangeComboMode() 
{
	UpdateData();
	if (m_comboMode.GetCurSel() == 0)
		ShowCScanCtrl(SW_HIDE);
	else
		ShowCScanCtrl(SW_SHOW);
}

void CMainDlg::OnMenuitemExpand() 
{
	MyExpandTree(TVI_ROOT, TVE_EXPAND);
}

void CMainDlg::OnMenuitemCollapse() 
{
	MyExpandTree(TVI_ROOT, TVE_COLLAPSE);
}

void CMainDlg::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hSelect = m_TreeCtrl.GetSelectedItem();
	HTREEITEM hLight = m_TreeCtrl.GetDropHilightItem();
	if(hLight != NULL && hSelect != hLight)
		hSelect = hLight;
	if(hSelect == NULL)
		return;
	m_TreeCtrl.SelectItem(hSelect);

	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	
	*pResult = 0;
}

void CMainDlg::OnMenuitemSaveHtml() 
{
	SYSTEMTIME	SysTime;
	GetLocalTime(&SysTime);
	char strFileName[MAX_PATH] = {0};
	wsprintf
		(
		strFileName,
		"C段扫描：%d-%02d-%02d %02d^%02d^%02d.htm",
		SysTime.wYear, SysTime.wMonth, SysTime.wDay,
		SysTime.wHour, SysTime.wMinute, SysTime.wSecond
		);
	
    CFileDialog dlg(FALSE, _T("htm"), strFileName, OFN_OVERWRITEPROMPT, _T("htm文件(*.htm)|*.htm||"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
	SaveTreeCtrlData(dlg.GetPathName());
	::MessageBox(0, "数据导出成功！", "提示", MB_ICONINFORMATION);
}

void CMainDlg::OnMenuitemSaveTxt() 
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("C段：%Y-%m-%d %H-%M-%S");
	
    CFileDialog dlg(FALSE, _T("txt"), strTime, OFN_OVERWRITEPROMPT, _T("*.txt|*.txt||"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
	
    CFile file;
	CString strData;
    if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
    {
		HTREEITEM hChildItem = NULL;
		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
		while (hNextItem != NULL)
		{
			strData.Empty();
			strData.Format(_T("【%s】"), m_TreeCtrl.GetItemText(hNextItem));
			if (strData.GetLength() == 0)
				continue;
			strData += "\r\n";
			file.Write(strData, strData.GetLength());

			hChildItem = m_TreeCtrl.GetChildItem(hNextItem);
			while (hChildItem != NULL)
			{
				strData.Format(_T("%s"), m_TreeCtrl.GetItemText(hChildItem));
				if (strData.GetLength() == 0)
					break;
				strData += "\r\n";
				file.Write(strData, strData.GetLength());
				hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
			}
			strData = "\r\n";
			file.Write(strData, strData.GetLength());

			hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
		}

        file.Close();
        ::MessageBox(0, "列表内容导出成功", "提示", MB_ICONINFORMATION);
    }
}

void CMainDlg::OnMenuitemSaveUrl() 
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("C段纯URL：%Y-%m-%d %H-%M-%S");
	
    CFileDialog dlg(FALSE, _T("txt"), strTime, OFN_OVERWRITEPROMPT, _T("*.txt|*.txt||"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
	
    CFile file;
	CString strData;
    if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
    {
		HTREEITEM hChildItem = NULL;
		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
		while (hNextItem != NULL)
		{
			hChildItem = m_TreeCtrl.GetChildItem(hNextItem);
			while (hChildItem != NULL)
			{
				strData.Format(_T("%s"), m_TreeCtrl.GetItemText(hChildItem));
				strData = SplitString(strData, " -> ");
				if (strData.GetLength() == 0)
					break;
				strData += "\r\n";
				file.Write(strData, strData.GetLength());
				hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
			}
			hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
		}

        file.Close();
        ::MessageBox(0, "列表内容导出成功", "提示", MB_ICONINFORMATION);
    }
}

void CMainDlg::SaveTreeCtrlData(CString strSavePath)
{
	UpdateData();
	SYSTEMTIME	SysTime;
	GetLocalTime(&SysTime);
	char strLogFileName[MAX_PATH] = {0};
	char strTime1[MAX_PATH] = {0};
	char strTime2[MAX_PATH] = {0};

	CString strMyDomain = m_Url;
	strMyDomain.MakeLower();
	strMyDomain.Replace("http://", "");
	strMyDomain.Replace("https://", "");
	
	int nPos1 = strMyDomain.Find("/", 0);
	if (nPos1 != -1)
	{
		strMyDomain = strMyDomain.Mid(0, nPos1);
	}
	else if (nPos1 == -1)
	{
		//strMyDomain = strMyDomain;
	}
	else
	{
		return;
	}
	strMyDomain.TrimLeft(" ");
	strMyDomain.TrimLeft("\t");
	strMyDomain.TrimRight(" ");
	strMyDomain.TrimRight("\t");

	wsprintf
		(
		strTime1,
		"\\%s[%d-%02d-%02d %02d^%02d^%02d ScanReprot].htm",
		strMyDomain, SysTime.wYear, SysTime.wMonth, SysTime.wDay,
		SysTime.wHour, SysTime.wMinute, SysTime.wSecond
		);
	wsprintf
		(
		strTime2,
		"%d-%02d-%02d %02d:%02d:%02d",
		SysTime.wYear, SysTime.wMonth, SysTime.wDay,
		SysTime.wHour, SysTime.wMinute, SysTime.wSecond
		);
	CFile file;
	if (!file.Open(strSavePath, CFile::modeCreate | CFile::modeWrite))
		return;

	file.Write(HTMLHeader, sizeof(HTMLHeader));
	file.Write(strTime2, strlen(strTime2));
	file.Write(HTMLData, sizeof(HTMLData));
	
	//获取IP和服务器信息
	CString strIP, strTempUrl, strHostIP, strHostURL, strTmp1, strTmp2;
	HTREEITEM hChildItem = NULL;
	HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
	while (hNextItem != NULL)
	{
		strIP = m_TreeCtrl.GetItemText(hNextItem);
		//写入IP
		strIP.Replace("   ", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
		strHostIP = "\r\n      <tr>\r\n        <td class=\"sub\" width=\"40%\">主机 - ";
		strHostIP += strIP;
		strHostIP += "</td>\r\n	<td class=sub width=\"60%\">网站名称</td></tr>\r\n      </tr>\r\n      ";
		file.Write(strHostIP, strHostIP.GetLength());
		
		hChildItem = m_TreeCtrl.GetChildItem(hNextItem);
		while (hChildItem != NULL)
		{
			//写入网址及title
			strTmp1 = m_TreeCtrl.GetItemText(hChildItem);
			strTmp2 = SplitString(strTmp1, TEXT_CONNECT_STRING);
			strHostURL = "      <tr>\r\n        <td class=\"default\" width=\"40%\"><a href=\"";
			strHostURL += strTmp2;
			strHostURL += "\"target=\"_blank\">";
			strHostURL += strTmp2;
			strHostURL += "</a></td>\r\n	<td class=default width=\"60%\">";
			strHostURL += SplitString(strTmp1, " -> ", 1);
			strHostURL += "</td></tr>\r\n";
			file.Write(strHostURL, strHostURL.GetLength());
			hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
		}
		
		hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
	}
	file.Write(HTMLEnd, sizeof(HTMLEnd));
	
	file.Close();
}

void CMainDlg::OnMenuitemCopySelected() 
{
	HTREEITEM hSelect = m_TreeCtrl.GetSelectedItem();
	CString strText = m_TreeCtrl.GetItemText(hSelect);

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

void CMainDlg::OnMenuitemClear() 
{
	m_TreeCtrl.DeleteAllItems();
}

void CMainDlg::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hSelect = m_TreeCtrl.GetSelectedItem();
	CString strText = m_TreeCtrl.GetItemText(hSelect);
	
	int nPos = strText.Find(TEXT_CONNECT_STRING, 0);
	if (nPos == -1)
		return;
	strText = strText.Mid(0, nPos);
	
	ShellExecute(0, "open", strText, NULL, NULL, SW_SHOW);
	
	*pResult = 0;
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 23;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE)//不加这个调试会崩溃
		m_infotip.RelayEvent(pMsg);

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
		else if (nVirtKey == 83 && GetKeyState(VK_CONTROL) < 0)//Ctrl+S
		{
			HTREEITEM hRoot = m_TreeCtrl.GetChildItem(TVI_ROOT);
			if (hRoot != NULL)
				OnMenuitemSaveHtml();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMainDlg::OnCheckGetServerType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CScanCSectionApp *)AfxGetApp())->m_IniFile.SetInt("ScanCSection", "bGetDetailServerInfo", m_bGetDetailServerInfo);
}

void CMainDlg::OnCheckGetDomainIp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CScanCSectionApp *)AfxGetApp())->m_IniFile.SetInt("ScanCSection", "bGetIpAddress", m_bGetIpAddress);
}

HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_INTERFACE && m_comboInterface.GetCurSel() == 0)
		pDC->SetTextColor(RGB(0, 100, 255));

	if (m_bIsCdnIp && pWnd->GetDlgCtrlID() == IDC_STATIC_IP)
		pDC->SetTextColor(RGB(255, 0, 0));
	else if (!m_bIsCdnIp && pWnd->GetDlgCtrlID() == IDC_STATIC_IP)
		pDC->SetTextColor(RGB(0, 0, 0));

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CMainDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pos;   
    GetCursorPos(&pos);  //获取当前鼠标位置
    CRect rc;   
    GetDlgItem(IDC_STATIC_INTERFACE)->GetWindowRect(&rc); //获取ID为IDC_STATIC_INTERFACE的Static Text的范围
    if (rc.PtInRect(pos)  && m_comboInterface.GetCurSel() == 0) //如果鼠标在这个范围之内   
    {   
        SetCursor(LoadCursor(NULL, IDC_HAND)); //设置成手状   
        return TRUE; //记得返回TRUE   
    }
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CMainDlg::OnSelchangeComboInterface() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_INTERFACE)->RedrawWindow();
}

void CMainDlg::OnStaticInterface() 
{
	// TODO: Add your control notification handler code here
	CBingKeyDlg dlg;
	dlg.DoModal();
}

LRESULT CMainDlg::OnMessageShowDlg(WPARAM wParam, LPARAM lParam)
{
	OnWorkEnd();//结束扫描线程
	CBingKeyDlg dlg;
	dlg.DoModal();

	return 0;
}

void CMainDlg::OnStaticIp() 
{
	// TODO: Add your control notification handler code here
	if (m_bIsCdnIp)
		ShellExecute(0, "open", "http://www.baidu.com/s?wd=%E5%A6%82%E4%BD%95%E6%9F%A5%E8%AF%A2%E4%BD%BF%E7%94%A8%E4%BA%86CDN%E7%9A%84%E7%BD%91%E7%AB%99%E7%9A%84%E7%9C%9F%E5%AE%9EIP", NULL, NULL, SW_SHOW);
}

CString CMainDlg::GetKeyboardText()
{
	HGLOBAL   hglb; 
	LPTSTR    lptstr;
	if (!IsClipboardFormatAvailable(CF_TEXT)) 
		return ""; 
	if (!OpenClipboard()) 
		return ""; 
	
	hglb = GetClipboardData(CF_TEXT); 
	if (hglb != NULL) 
	{ 
		lptstr = (char *)GlobalLock(hglb); 
		if (lptstr != NULL)
		{
			GlobalUnlock(hglb);
			CloseClipboard();
			if (strstr(lptstr, "://") != NULL)
				return lptstr;
		}
	}
	return "";
}

void CMainDlg::OnSetfocusEditUrl() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_Url.GetLength() > 7)
		return;
	CString strText = GetKeyboardText();
	if (strText.GetLength() > 8)
	{
		SetDlgItemText(IDC_EDIT_URL, strText);
		((CEdit *)GetDlgItem(IDC_EDIT_URL))->PostMessage(EM_SETSEL, 0, -1);
	}
}

void CMainDlg::OnMenuitemDeleteSelected() 
{
	// TODO: Add your command handler code here
	UpdateData();
	HTREEITEM hSelect = m_TreeCtrl.GetSelectedItem();
	if (hSelect != NULL)
		m_TreeCtrl.DeleteItem(hSelect);

	ReSetAllItemNum();
}

void CMainDlg::OnSelchangeComboIp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString strText;
	m_comboIP.GetLBText(m_comboIP.GetCurSel(), strText);
	if (!isipv4str(strText))
	{
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
			StatusTextOut(0, "%s 已复制到剪切板!", strText);
			m_comboIP.SetCurSel(0);
		}
	}
}

CString GetIP(CString str, int Mode)
{
	if (Mode == 0)//115.239.210.27 (10)   ⊕ BWS/1.1
	{
		int nPos = str.Find(" (", 0);
		if (nPos != -1)
		{
			return str.Mid(0, nPos);
		}
	}
	else if (Mode == 1)//http://www.baidu.com/ -> [115.239.211.112] 海德曼2904的百度个人主页
	{
		int nPos = str.Find("-> [", 0);
		if (nPos != -1)
		{
			int nPosEnd = str.Find("]", 0);
			return str.Mid(nPos + 4, nPosEnd - nPos - 4);
		}
	}
	return str;
}

void CMainDlg::ReSetChildItemNum(HTREEITEM hParent, int nNum)
{
	CString strText, strNum, strTmp;
	strText = m_TreeCtrl.GetItemText(hParent);
	int nPos = strText.Find("(", 0);
	if (nPos != -1)
	{
		int nPosEnd = strText.Find(")", 0);
		strNum = strText.Mid(nPos, nPosEnd - nPos + 1);
		strTmp.Format("(%d)", nNum);
		strText.Replace(strNum, strTmp);
		m_TreeCtrl.SetItemText(hParent, strText);
	}
}

void CMainDlg::ReSetAllItemNum()
{
	int nNum = 0;
	HTREEITEM hDelete = NULL;
	HTREEITEM hChildItem = NULL;
	HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
	while (hNextItem != NULL)
	{
		nNum = 0;
		hChildItem = m_TreeCtrl.GetChildItem(hNextItem);
		while (hChildItem != NULL)
		{
			nNum++;
			hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
		}
		ReSetChildItemNum(hNextItem, nNum);
		if (nNum == 0)
		{
			hDelete = hNextItem;
			hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
			m_TreeCtrl.DeleteItem(hDelete);
		}
		else
			hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
	}
}

void CMainDlg::OnMenuitemDeleteErrorUrl() 
{
	CString strIP, strUrlIP;
	HTREEITEM hChildItem = NULL;
	HTREEITEM hDelete = NULL;
	HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
	while (hNextItem != NULL)
	{
		strIP.Empty();
		strIP.Format(_T("%s"), m_TreeCtrl.GetItemText(hNextItem));
		strIP = GetIP(strIP, 0);
		if (strIP.GetLength() == 0)
			continue;

		hChildItem = m_TreeCtrl.GetChildItem(hNextItem);
		while (hChildItem != NULL)
		{
			strUrlIP.Format(_T("%s"), m_TreeCtrl.GetItemText(hChildItem));
			strUrlIP = GetIP(strUrlIP, 1);
			if (strUrlIP.GetLength() == 0)
				break;

			if (strIP != strUrlIP || !isipv4str(strUrlIP))
			{
				hDelete = hChildItem;
				hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
				m_TreeCtrl.DeleteItem(hDelete);
			}
			else
			{
				hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
			}
		}
		hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
	}
	ReSetAllItemNum();
}

//***************************************************************************

DWORD WINAPI CMainDlg::BingWebApiGetData(LPVOID lparam)
{
	CScanThreadParameter *par = (CScanThreadParameter *)lparam;
	int nIpAddr = par->nIpPos;
	CString strTempAddr = par->strCurrentIp;
	CMainDlg *pThis = par->pThis;

	CString strCurrentUrl, strCurrentIpAddr, strCurrentHtmlData, strShowData, strHeader;
	CStringArray sArray;
	sArray.RemoveAll();

	strCurrentIpAddr.Format("%s.%d", strTempAddr, nIpAddr);
	try
	{
		pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, "");
		for (int nSearchNum = 1; nSearchNum <= 151; nSearchNum += 50)
		{
			strCurrentUrl.Format("http://www.bing.com/search?first=%d&count=50&q=ip:%s", nSearchNum, strCurrentIpAddr);
			
			strHeader = "X-Forwarded-For: ";
			strHeader += GetIPAddr();
			strHeader += "\r\nUser-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5";

			strCurrentHtmlData = getURLContext(strCurrentUrl, strHeader);
			
			int nPos1 = 0;
			int nPos2 = 0;
			if (nSearchNum == 1)
			{
				nPos1 = strCurrentHtmlData.Find("<h2><a href=\"", nPos1);
				if (nPos1 == -1)
				{
					break;
				}
			}

			Utf8ToAnsi(strCurrentHtmlData);

			//获取页面里的所有url和title
			nPos1 = 0;
			while (1)
			{
				CString strInfo, strTempTitle, strTempURL;
				
				nPos1 = strCurrentHtmlData.Find("<h2><a href=\"", nPos1);
				if (nPos1 == -1)
					break;

				nPos2 = strCurrentHtmlData.Find("><div class", nPos1);
				if (nPos2 == -1)//例如：118.67.120.233,找不到“</p> <div class”
				{
					nPos1 += 13;
					int nPos4 = strCurrentHtmlData.Find("\" target=", 0);
					strTempURL = strCurrentHtmlData.Mid(nPos1, nPos4);
					strTempURL = TrimFileName(strTempURL);
				}
				else
				{
					nPos1 += 13;//"<h3><a href=\""的长度
					int nLenth = nPos2 - nPos1;
					nPos2 += 11;//"</p><div class"的长度
					
					strShowData.Empty();
					strShowData = strCurrentHtmlData.Mid(nPos1, nLenth);
					//::MessageBox(NULL, strShowData, _T("提示"), MB_ICONINFORMATION);
					
					int nPos4 = strShowData.Find("\" ", 0);
					strTempURL = strShowData.Mid(0, nPos4);
					strTempURL = TrimFileName(strTempURL);
					//::MessageBox(NULL, strTempURL, _T("提示"), MB_ICONINFORMATION);
					
					//int nPos5 = strShowData.Find("')\">", 0);
					int nPos5 = strShowData.Find("\">", 0);
					int nPos6 = strShowData.Find("</a>", 0);
					strTempTitle = strShowData.Mid(nPos5 + 2, nPos6 - nPos5 - 2);
					//::MessageBox(NULL, strTempTitle, _T("提示"), MB_ICONINFORMATION);
				}

				strInfo.Format("%s -> %s", strTempURL, strTempTitle);

				BOOL bAddToStringList = TRUE;
				for (int nListCount = 0; nListCount < sArray.GetSize(); nListCount++)
				{
					CString strURLListTempData = sArray.GetAt(nListCount);
					if (strTempURL.CompareNoCase(strURLListTempData) == 0)
					{
						bAddToStringList = FALSE;
					}
				}
				if (strTempURL.Find("://ip.chinaz.com/", 0) != -1)
					bAddToStringList = FALSE;
				
				if (bAddToStringList)
				{
					sArray.Add(strInfo);
				}
			}
		}
	}
	catch (...){}

	if (sArray.GetSize() == 0)
	{
		pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, TEXT_NOINFO);//没有信息，删除该节点
	}
	else
	{
		pThis->QuChongFu(sArray);
		for (int i = 0; i < sArray.GetSize(); i++)
			pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, sArray.GetAt(i));
		
		pThis->InsertInfoToTreeCtrl(strCurrentIpAddr, intToString(sArray.GetSize()), TRUE);
	}

	sArray.RemoveAll();
	pThis->nThreadNum--;
	return -1;
}

DWORD CMainDlg::BingWebScanThread()
{
	m_bScanning = TRUE;

	try
	{
		CString strCurrentUrl, strCurrentIpAddr, strCurrentHtmlData, strShowData, strIpPos;
		int iItem = 0;
		HTREEITEM hRoot = NULL;
		int nShowPoint = 0;
		CStringList strURLList;
		CString strDetailInfo;
		int nDetailInfoRow = 0;
		if (m_bCurrentScanModeIsSingleServerScan)//扫描单服务器
		{
			nThreadNum = 0;
			nThreadNum++;
			strCurrentIpAddr = m_IpAddr;
			strCurrentIpAddr.TrimLeft(" ");
			strCurrentIpAddr.TrimLeft("\t");
			strCurrentIpAddr.TrimRight(" ");
			strCurrentIpAddr.TrimRight("\t");
			
			//初始化线程所需的参数
			int nFindTime = 0;
			for (int m = 0; m < 15; m++)
			{
				if (strCurrentIpAddr.GetAt(m) == '.')
				{
					nFindTime++;
					if (nFindTime == 3)
					{
						strCurrentUrl = strCurrentIpAddr.Mid(0, m);
						strIpPos = strCurrentIpAddr.Mid(m + 1, strCurrentIpAddr.GetLength() - m);
						break;
					}
				}
			}
			
			CScanThreadParameter lparam;
			lparam.pThis = this;
			lparam.nIpPos = atoi(strIpPos);
			lparam.strCurrentIp = strCurrentUrl;
			
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BingWebApiGetData, (LPVOID)&lparam, 0, NULL);
			Sleep(200);
			
			while (nThreadNum > 0)
			{
				Sleep(300);
				nShowPoint++;
				if (nShowPoint == 1)
				{
					StatusTextOut(0, SHOW_1, nThreadNum);
				}
				else if (nShowPoint == 2)
				{
					StatusTextOut(0, SHOW_2, nThreadNum);
				}
				else
				{
					StatusTextOut(0, SHOW_3, nThreadNum);
					nShowPoint = 0;
				}
			}
			MyExpandTree(TVI_ROOT, TVE_EXPAND);
		}
		else
		{
			CString strTmpIPAddr, strBuffer;
			
			int nFindTime = 0;
			for (int m = 0; m < 15; m++)
			{
				if (m_IpAddr.GetAt(m) == '.')
				{
					nFindTime++;
					if (nFindTime == 3)
					{
						strTmpIPAddr = m_IpAddr.Mid(0, m);
						break;
					}
				}
			}
			
			if (m_nCScanBegin < 1 || m_nCScanEnd > 255 || m_nCScanBegin > m_nCScanEnd)
			{
				m_nCScanBegin = 1;
				m_nCScanEnd = 255;
			}
			
			CScanThreadParameter lparam;
			lparam.pThis = this;
			
			nThreadNum = 0;
			for (int n = m_nCScanBegin; n <= m_nCScanEnd; n++)
			{
				strBuffer = strTmpIPAddr;
				strBuffer += "." + intToString(n);
				m_TreeCtrl.InsertItem(strBuffer, 0, 0, TVI_ROOT, TVI_LAST);
			}
			for (n = m_nCScanBegin; n <= m_nCScanEnd; n++)
			{
				strCurrentIpAddr.Format("%s.%d", strTmpIPAddr, n);
				nThreadNum++;
				lparam.nIpPos = n;
				lparam.strCurrentIp = strTmpIPAddr;
				
				HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BingWebApiGetData, (LPVOID)&lparam, 0, NULL);
				StatusTextOut(0, "线程数: %d, 扫描进度: %d/%d, 正在扫描:%s", nThreadNum, n - m_nCScanBegin, m_nCScanEnd - m_nCScanBegin + 1, strCurrentIpAddr);
				Sleep(200);
				CloseHandle(hThread);
			}
			
			while (nThreadNum > 0)
			{
				Sleep(300);
				nShowPoint++;
				if (nShowPoint == 1)
				{
					StatusTextOut(0, SHOW_1, nThreadNum);
				}
				else if (nShowPoint == 2)
				{
					StatusTextOut(0, SHOW_2, nThreadNum);
				}
				else
				{
					StatusTextOut(0, SHOW_3, nThreadNum);
					nShowPoint = 0;
				}
			}
		}
		StatusTextOut(0, "正在显示本次查询的数据...");
	}
	catch (...)
	{
		::MessageBox(0, "获取数据失败！", "提示", MB_ICONINFORMATION);
	}

	try
	{
		//去除没有结果的IP项
		HTREEITEM hChildItem = NULL;
		HTREEITEM hDelete = NULL;
		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
		while (hNextItem != NULL)
		{
			if (!m_TreeCtrl.ItemHasChildren(hNextItem))
			{
				hDelete = hNextItem;
				hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
				m_TreeCtrl.DeleteItem(hDelete);
			}
			else
				hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
		}
	}
	catch (...){}

	try
	{
		if (m_bGetDetailServerInfo || m_bGetIpAddress)
		{
			//获取IP和服务器信息
			CString strIP, strTempUrl, strDetailInfo;
			HTREEITEM hChildItem = NULL;
			HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(TVI_ROOT);
			while (hNextItem != NULL)
			{
				//插入服务器信息
				if (m_bGetDetailServerInfo)
				{
					StatusTextOut(0, "正在获取服务器信息...");
					strIP = m_TreeCtrl.GetItemText(hNextItem);
					strDetailInfo = GetServerDetailInfo(GetFirstChildItemUrl(hNextItem));
					strDetailInfo.Insert(0, "   ⊕ ");
					strDetailInfo.Insert(0, strIP);
					m_TreeCtrl.SetItemText(hNextItem, strDetailInfo);
				}
				
				//插入IP信息
				if (m_bGetIpAddress)
				{
					StatusTextOut(0, "正在获取网站对应的IP...");
					hChildItem = m_TreeCtrl.GetChildItem(hNextItem);
					while (hChildItem != NULL)
					{
						InsertIP(hChildItem);
						hChildItem = m_TreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
					}
				}
				
				hNextItem = m_TreeCtrl.GetNextItem(hNextItem, TVGN_NEXT);
			}
		}
	}
	catch (...){}

	m_bScanning = FALSE;
	StatusTextOut(0, "扫描完成！");
	SetDlgItemText(IDC_BUTTON_SCAN, "开始扫描");
	MessageBeep(MB_ICONEXCLAMATION);

	return 0;
}
//***************************************************************************

void CMainDlg::OnSetfocusComboIp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString strIP;
	m_comboIP.GetWindowText(strIP);
	if (strIP == "")
	{
		OnButtonParseUrl();
	}
}
