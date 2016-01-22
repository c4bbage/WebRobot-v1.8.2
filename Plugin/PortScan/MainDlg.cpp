// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PortScan.h"
#include "MainDlg.h"
#include "ListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char	strHttpRequest[] = "HEAD / HTTP/1.0\r\n\r\n";

CMainDlg *g_pMainDlg = NULL;
/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_Url = _T("");
	m_nMaxThread = ((CPortScanApp *)AfxGetApp())->m_IniFile.GetInt("PortScan", "nMaxThread", 512);
	m_bGetBanner = ((CPortScanApp *)AfxGetApp())->m_IniFile.GetInt("PortScan", "bGetBanner", TRUE);
	m_nTimeOut = ((CPortScanApp *)AfxGetApp())->m_IniFile.GetInt("PortScan", "nTimeOut", 30);
	//}}AFX_DATA_INIT
	m_bScanning = FALSE;
	dwThreadsUsed = 0;
	((CPortScanApp *)AfxGetApp())->m_pMyMainDlg = this;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_btnScan);
	DDX_Control(pDX, IDC_BUTTON_RESOLVE, m_btnResolve);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDC_COMBO_IP, m_comboIP);
	DDX_Control(pDX, IDC_COMBO_IP_MODE, m_comboIPMode);
	DDX_Text(pDX, IDC_EDIT_URL, m_Url);
	DDX_Text(pDX, IDC_EDIT_MAX_THREAD, m_nMaxThread);
	DDV_MinMaxInt(pDX, m_nMaxThread, 1, 1024);
	DDX_Check(pDX, IDC_CHECK_BANNER, m_bGetBanner);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, 1, 300);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_RESOLVE, OnButtonResolve)
	ON_EN_SETFOCUS(IDC_EDIT_URL, OnSetfocusEditUrl)
	ON_CBN_SELCHANGE(IDC_COMBO_IP_MODE, OnSelchangeComboIpMode)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_CBN_SELCHANGE(IDC_COMBO_IP, OnSelchangeComboIp)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_WM_MEASUREITEM()
	ON_COMMAND(ID_MENUITEM_COPY, OnMenuitemCopy)
	ON_COMMAND(ID_MENUITEM_EXPAND, OnMenuitemExpand)
	ON_COMMAND(ID_MENUITEM_COLLAPSE, OnMenuitemCollapse)
	ON_COMMAND(ID_MENUITEM_EMPTY, OnMenuitemEmpty)
	ON_COMMAND(ID_MENUITEM_SAVE, OnMenuitemSave)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ON_INSERT_RESULT, OnMessageInsertResult)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CMainDlg)
EASYSIZE(IDC_TREE, ES_BORDER, IDC_COMBO_PORT, ES_BORDER, ES_BORDER, 0)//左上右下
END_EASYSIZE_MAP
/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

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
	INIT_EASYSIZE;
	// TODO: Add extra initialization here
	g_pMainDlg = (CMainDlg *)((CPortScanApp *)AfxGetApp())->m_pMyMainDlg;

	m_comboIPMode.SendMessage(CB_SETITEMHEIGHT, 0, 16);
	m_comboIP.SendMessage(CB_SETITEMHEIGHT, 0, 16);
	m_comboPort.SendMessage(CB_SETITEMHEIGHT, 0, 16);
	m_comboPort.AddString(((CPortScanApp *)AfxGetApp())->m_IniFile.GetString("PortScan", "Ports", "21,22,23,25,53,80,99,110,135,139,143,443,445,1433,3306,3389,8080,43958"));
	if (m_comboPort.FindString(0, "21,22,23,25,53,80,99,110,135,139,143,443,445,1433,3306,3389,8080,43958") == CB_ERR)
		m_comboPort.AddString("21,22,23,25,53,80,99,110,135,139,143,443,445,1433,3306,3389,8080,43958");

	m_comboPort.AddString("7,9,21,22,23,25,37,53,79,80,81,99,109,110,113,115,119,135,139,143,144,443,445,513,1024,1025,1033,1080,1114,1433,1434,2000,2001,3389,4000,4899,5000,5190,7001,7788,8000,8001,8080,12345,20034,31337");
	m_comboPort.AddString("1-1024");
	m_comboPort.AddString("1-8000");
	m_comboPort.AddString("1-65535");
	CString strPort;
	strPort = ((CPortScanApp *)AfxGetApp())->m_IniFile.GetString("PortScan", "Ports", "");
	if (m_comboPort.FindString(0, strPort) == -1 && strPort != "")
		m_comboPort.AddString(strPort);
	m_comboIPMode.SetCurSel(0);
	m_comboPort.SetCurSel(0);


	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1200001);
	int strPartDim[3]= {300, 500, -1};
	m_wndStatusBar.SetParts(3, strPartDim);

	m_infotip.Create(this);
	m_infotip.SetMaxTipWidth(450); 
	m_infotip.SetTipTextColor(RGB(0,170,64));
	m_infotip.AddTool(GetDlgItem(IDC_COMBO_IP), _T(""));
	m_infotip.AddTool(GetDlgItem(IDC_STATIC_IP), _T(""));
	m_infotip.AddTool(GetDlgItem(IDC_EDIT_TIMEOUT), _T("超时时间设置的越长，扫描结果就越不容易遗漏\r\n一般扫描时间30~60秒就不会遗漏了"));
	m_infotip.AddTool(GetDlgItem(IDC_COMBO_IP), _T("支持多段扫描"));

	m_ImageList.Create(17, 17, ILC_COLORDDB | ILC_MASK, 0, 1);
	LoadMyBitmap(&m_ImageList, IDB_BITMAP_IP);
	LoadMyBitmap(&m_ImageList, IDB_BITMAP_PORT);
	m_TreeCtrl.SetImageList(&m_ImageList,TVSIL_NORMAL);
	::SendMessage(m_TreeCtrl.m_hWnd, TVM_SETITEMHEIGHT, 23, 0);

	OnSelchangeComboPort();

	PostMessage(WM_SIZE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
	// TODO: Add your message handler code here
	if (GetParent()->m_hWnd != NULL && m_comboPort.m_hWnd != NULL)
	{
		RECT rc;
		GetParent()->GetClientRect(&rc);
		MoveWindow(&rc);
		m_wndStatusBar.MoveWindow(&rc);
	}
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

void CMainDlg::OnButtonResolve() 
{
	UpdateData(TRUE);
	StatusTextOut(1, "");
	StatusTextOut(2, "");
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
	m_hGetIpThread = ThreadTemplate::StartThread<CMainDlg, DWORD>(this, GetIpAddrThread);
	if (m_hGetIpThread == NULL)
	{
		StatusTextOut(0, "获取URL对应的IP失败！");
		return;
	}
}

DWORD CMainDlg::GetIpAddrThread()
{
	((CEdit *)GetDlgItem(IDC_EDIT_URL))->EnableWindow(FALSE);
	try
	{
		m_bScanning = TRUE;
		m_bIsCdnIp = FALSE;
		StatusTextOut(0, "正在获取URL对应的IP...");
		m_comboIP.ResetContent();
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
			m_bScanning = FALSE;
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
			m_bScanning = FALSE;
			return 0;
		}
		
		memset(hostname, 0, sizeof(hostname));
		wsprintf(hostname, "%s", m_Url);
		phostent = gethostbyname(hostname);
		if(phostent == NULL) 
		{
			m_bScanning = FALSE;
			return 0;
		}

		CString strTip;
		strTip = phostent->h_name;
		if (strTip != m_Url && m_Url.Find(strTip, 0) == -1)
		{
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
		
		for(n = 0; phostent->h_addr_list[n]; n++)
		{
			memcpy(&sa.sin_addr.s_addr,phostent->h_addr_list[n],phostent->h_length);
			m_ip.Format("%s", inet_ntoa(sa.sin_addr));
			m_comboIP.AddString(m_ip);
		}
		m_comboIP.SetCurSel(0);
		WSACleanup();
		
		m_comboIP.GetLBText(0, m_ip);
		CString strTmpIPAddr, strCSetionIP;
		strTmpIPAddr = m_ip.Mid(0, m_ip.ReverseFind('.'));
		CString strIPAddrStart;
		CString strIPAddrEnd;
		strIPAddrStart.Format("%s.1", strTmpIPAddr);
		strIPAddrEnd.Format("%s.254", strTmpIPAddr);
		StatusTextOut(0, "域名:[%s] -> IP:[%s]", m_Url, m_ip);
		strCSetionIP.Format("%s-%s", strIPAddrStart, strIPAddrEnd);
		m_comboIP.AddString(strCSetionIP);
	}
	catch (...)
	{
		StatusTextOut(0, "获取IP失败！");
	}
	((CEdit *)GetDlgItem(IDC_EDIT_URL))->EnableWindow(TRUE);
	m_bScanning = FALSE;

	return 0;
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE)//不加这个调试会崩溃
		m_infotip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CMainDlg::OnSelchangeComboIpMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString strIP;
	if (m_comboIPMode.GetCurSel() == 0)//单独IP
	{
		m_comboIPMode.SetCurSel(0);
		m_comboIP.GetWindowText(strIP);
		int nPos = strIP.Find("-", 0);
		if (nPos != -1)//找到“-”
		{
			strIP.SetAt(nPos, 0x00);
			m_comboIP.SetWindowText(strIP);
		}
		else if ((nPos = strIP.Find(",", 0)) != -1)//找到“,”
		{
			strIP.SetAt(nPos, 0x00);
			m_comboIP.SetWindowText(strIP);
		}
	}
	else if (m_comboIPMode.GetCurSel() == 1)//IP段
	{
		for (int i = 0; i < m_comboIP.GetCount(); i++)
		{
			m_comboIP.GetLBText(i, strIP);
			if (strIP.Find("-", 0) != -1)
			{
				m_comboIP.SetWindowText(strIP);
				break;
			}
		}
		m_comboIP.GetWindowText(strIP);
		if (strIP.Find("-", 0) == -1 && strIP.GetLength() >= 7)
		{
			int nPos = strIP.Find("-", 0);
			if (nPos != -1)
				strIP.SetAt(nPos, 0x00);
			nPos = strIP.Find(",", 0);
			if (nPos != -1)
				strIP.SetAt(nPos, 0x00);
			
			CString strTmpIPAddr, strCSetionIP;
			strTmpIPAddr = strIP.Mid(0, strIP.ReverseFind('.'));
			CString strIPAddrStart;
			CString strIPAddrEnd;
			strIPAddrStart.Format("%s.1", strTmpIPAddr);
			strIPAddrEnd.Format("%s.254", strTmpIPAddr);
			strCSetionIP.Format("%s-%s", strIPAddrStart, strIPAddrEnd);
			m_comboIP.SetWindowText(strCSetionIP);
		}
	}
	else if (m_comboIPMode.GetCurSel() == 2)//IP列表
	{
		CListDlg dlg;
		dlg.DoModal();
	}
}

bool initWinsock()
{
	WSAData wsaData;
	return (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
}

typedef struct tagThreadParameter
{
	u_long			uIpAddr;
	int				nPort;
	CMainDlg	*pThis;
}ThreadParameter,*LPThreadParameter;

struct IpPortPar
{
	char strIp[20];
	char strPortInfo[513];
};

unsigned long GetHostIpAddress(char *host)//由域名获取ip地址
{
	long i;
	struct hostent *he;
	
	if((i = inet_addr(host)) < 0)
	{
		if((he = (struct hostent*)gethostbyname(host)) == NULL)
			return(0);
		else
			return(*(unsigned long *)he->h_addr);
	}
	return(i);
}

BOOL myRecv(SOCKET s, char *buf, int len)
{
	int ret;
	BOOL result;
	struct timeval timeout;
	fd_set readfds;
	
	result = FALSE;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	
	if (1)//_isHttp
	{
		ret = send(s, strHttpRequest, sizeof(strHttpRequest), 0);
	}
	
	FD_ZERO(&readfds);
	FD_SET(s, &readfds);
	ret = select(0, &readfds, 0, 0, &timeout);
	if ( ret && ret != -1 )
	{
		if ( FD_ISSET(s, &readfds) )
		{
			result = recv(s, buf, len, 0) > 0;
		}
	}
	
	return result;
}

const char * getBanner(char *response)
{
	signed int result;

	if (response)
	{
		int	len = 0;
		if (1)//_isHttp
		{
			const char * tag = "Server: ";
			char * serverBanner = strstr(response, tag);
			if (!serverBanner)
			{
				serverBanner = response;
			}
			else
			{
				serverBanner += lstrlen(tag);
			}			
			len = lstrlen(serverBanner);
			for (int i = 0; i < len; i++)
			{
				if (serverBanner[i] == '\r' || serverBanner[i] == '\n')
				{
					serverBanner[i] = '\0';
					break;
				}
			}
			return serverBanner;
		}
		else
		{
			len = lstrlen(response);
			for (int i = 0; i < lstrlen(response); i++)
			{
				if (response[i] == '\r' || response[i] == '\n')
				{
					response[i] = '\0';
					break;
				}
			}
		}
		
		result = 1;
	}
	else
	{
		result = 0;
	}
	return response;
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

//socket TCP client connect非阻塞模式
void CMainDlg::SockSelect(u_long uIpAddr, int nPort)
{
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == SOCKET_ERROR) 
        return;
	
	struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = uIpAddr;
    serv_addr.sin_port = htons(nPort);
	
    int error = -1;
    int len = sizeof(int);
    timeval tm;
    fd_set writefds;
    unsigned long ul = 1;
    if (ioctlsocket(sockfd, FIONBIO, &ul) == SOCKET_ERROR)//设置为非阻塞模式
	{
		closesocket(sockfd);
		return;
	}

    bool ret = false;
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			tm.tv_sec  = m_nTimeOut;
			tm.tv_usec = 0;
			FD_ZERO(&writefds);
			FD_SET(sockfd, &writefds);
			if(select(sockfd + 1, NULL, &writefds, NULL, &tm) > 0)//超时为0，扫描iis put的话超时一定要长，不然会漏掉。这里最容易超时select为0
			{
				if (FD_ISSET(sockfd, &writefds))
					ret = true;
				else
				{
					getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, &len);
					if(error == 0) 
						ret = true;
					else 
						ret = false;
				}
			}
		}
    }
	else
        ret = true;
	
	if (ret)
	{
		int recvLen = 0;
		char targetHost[20];
		char response[512];
		const char *responseBanner = NULL;
		wsprintf(targetHost, "%d.%d.%d.%d", (unsigned __int8)uIpAddr, (unsigned __int16)((WORD)uIpAddr >> 8), (uIpAddr >> 16) & 0xFF, uIpAddr >> 24);
		IpPortPar *ipPar = new IpPortPar;
		lstrcpy(ipPar->strIp, targetHost);
		
		if (m_bGetBanner)//获取Banner
		{
			ul = 0;
			if(ioctlsocket(sockfd, FIONBIO, &ul) == SOCKET_ERROR) //设置为阻塞模式
			{
				closesocket(sockfd);
				return;
			}
			else
			{
				recvLen = myRecv(sockfd, response, sizeof(response));
				if (recvLen)
				{
					responseBanner = getBanner(response);
					sprintf(ipPar->strPortInfo, "%-5d -> %s", nPort, responseBanner);
					PostMessage(WM_ON_INSERT_RESULT, 0, (LPARAM)ipPar);//OnMessageInsertResult
				}
				else
				{
					sprintf(ipPar->strPortInfo, "%d", nPort);
					PostMessage(WM_ON_INSERT_RESULT, 0, (LPARAM)ipPar);//OnMessageInsertResult
				}
			}
		}
		else//不获取Banner
		{
			sprintf(ipPar->strPortInfo, "%d", nPort);
			PostMessage(WM_ON_INSERT_RESULT, 0, (LPARAM)ipPar);//OnMessageInsertResult
		}
	}

	closesocket( sockfd );
}

typedef struct 
{
	u_long uIpAddr;
	int nPort;
	CMainDlg *pThis;
} HttpClientParm;   

unsigned __stdcall CMainDlg::tcpScanThread(LPVOID lparam)
{
	ThreadParameter *tPar = (ThreadParameter *)lparam;
	CMainDlg *pThis = tPar->pThis;

	pThis->SockSelect(tPar->uIpAddr, tPar->nPort);

	EnterCriticalSection(&pThis->cs);
	++pThis->nIpScanned;
	if (pThis->dwThreadsUsed)
		--pThis->dwThreadsUsed;
	ReleaseSemaphore(pThis->hSemaphore, 1, 0);
	LeaveCriticalSection(&pThis->cs);

	delete tPar;
	return 0;
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

LRESULT CMainDlg::OnMessageInsertResult(WPARAM wParam, LPARAM lParam)
{
	IpPortPar *ipPar = (IpPortPar *)lParam;

	InsertInfoToTreeCtrl(ipPar->strIp, ipPar->strPortInfo);

	delete ipPar;
	return 0;
}

int CMainDlg::startScan(vector<CString> sIPList, vector<int> iPortList, int nMaxThreads)
{
	DWORD dwError;
	BOOL bSemaphore;
	int nCount;
	HANDLE hObject;
	LONG previousCount;
	int _maxThreads;
	DWORD dwTotalScannedNum = 0;
	DWORD dwTotalNum = sIPList.size() * iPortList.size();

	nIpScanned = 0;
	bSemaphore = FALSE;
	nCount = 0;

	_maxThreads = nMaxThreads;
	hSemaphore = CreateSemaphoreA(0, _maxThreads, _maxThreads, 0);
	if ( !hSemaphore )
	{
		StatusTextOut(0, "Fail To Create Semaphore");
		return 0;
	}

	for (int nIP = 0; nIP < sIPList.size(); nIP++)
	{
		if (bStopScan)
			break;

		u_long uCurIpAddr = GetHostIpAddress(sIPList[nIP].GetBuffer(0));
		StatusTextOut(0, "正在扫描: %s", sIPList[nIP]);
		for (int i = 0; i < iPortList.size(); i++)
		{
			if (bStopScan)
				break;

			StatusTextOut(1, "扫描端口: %d", iPortList[i]);
			StatusTextOut(2, "进度: %d/%d, 活动线程: %d", dwTotalScannedNum, dwTotalNum, dwThreadsUsed);

			ThreadParameter *lparam = new ThreadParameter;
			lparam->uIpAddr = uCurIpAddr;
			lparam->nPort = iPortList[i];
			lparam->pThis = this;
			hObject = (HANDLE)_beginthreadex(0, 0, (unsigned int (__stdcall *)(void *))tcpScanThread, (LPVOID)lparam, 0, NULL);
			if (hObject)
			{
				EnterCriticalSection(&cs);
				++nCount;
				++dwThreadsUsed;
				dwTotalScannedNum++;
				LeaveCriticalSection(&cs);
				CloseHandle(hObject);
			}
			if (iPortList.size() <= 10)
			{
				if (m_nMaxThread > 500)
					Sleep(500);
				else if (m_nMaxThread > 400)
					Sleep(400);
				else if (m_nMaxThread > 300)
					Sleep(300);
				else if (m_nMaxThread > 200)
					Sleep(200);
				else if (m_nMaxThread > 100)
					Sleep(100);
				else
					Sleep(100);
			}
			else if (iPortList.size() <= 100)//端口数量少就扫描的精细一点，不要漏了
			{
				if (m_nMaxThread > 500)
					Sleep(100);
				else if (m_nMaxThread > 400)
					Sleep(60);
				else if (m_nMaxThread > 300)
					Sleep(50);
				else if (m_nMaxThread > 200)
					Sleep(30);
				else if (m_nMaxThread > 100)
					Sleep(10);
				else
					Sleep(5);
			}

			WaitForSingleObject(hSemaphore, INFINITE);
		}
	}

	StatusTextOut(1, "");
	StatusTextOut(2, "");
	while ( dwThreadsUsed )
	{
		WaitForSingleObject(hSemaphore, INFINITE);

		//if (!bStopScan)//不是强制终止的才显示
		StatusTextOut(0, "仍有%d个线程正在扫描...", dwThreadsUsed);

		EnterCriticalSection(&cs);
		bSemaphore = ReleaseSemaphore(hSemaphore, 1, &previousCount);
		LeaveCriticalSection(&cs);
		if (!bSemaphore)
		{
			dwError = GetLastError();
			StatusTextOut(1, "Error Code: %d", dwError);
			Sleep(3000);
			break;
		}
		if (previousCount + 1 != _maxThreads)
		{
			Sleep(200);
			if (nIpScanned != nCount)//说明还没扫描完
				continue;
		}
		break;
	}
	if ( hSemaphore )
		CloseHandle(hSemaphore);

	return 0;
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

void CMainDlg::OnButtonScan() 
{
	UpdateData();
	if (!m_bScanning)
	{
		m_TreeCtrl.DeleteAllItems();
		if (m_nMaxThread <= 0 || m_nMaxThread > 1024)
		{
			StatusTextOut(0, "线程数设置错误!");
			return;
		}

		CString strBuffer;
		m_comboPort.GetLBText(m_comboPort.GetCurSel(), strBuffer);
		((CPortScanApp *)AfxGetApp())->m_IniFile.SetString("PortScan", "Ports", strBuffer);

		if (strBuffer.Find(",", 0) != -1)
			((CPortScanApp *)AfxGetApp())->m_IniFile.SetInt("PortScan", "nMaxThread1", m_nMaxThread);
		else
			((CPortScanApp *)AfxGetApp())->m_IniFile.SetInt("PortScan", "nMaxThread", m_nMaxThread);

		((CPortScanApp *)AfxGetApp())->m_IniFile.SetInt("PortScan", "bGetBanner", m_bGetBanner);
		((CPortScanApp *)AfxGetApp())->m_IniFile.SetInt("PortScan", "nTimeOut", m_nTimeOut);

		CString strIP, strPort;
		m_comboIP.GetWindowText(strIP);
		if (strIP.SpanIncluding("1234567890.,-") != strIP)
		{
			::MessageBox(NULL, _T("IP填写错误(含有“1234567890.,-”之外的其他字符)!"), _T("提示"), MB_ICONINFORMATION);
			return;
		}
		if (strIP.GetLength() < 7 || strIP.Find(".", 0) == -1)//长度大于7
		{
			::MessageBox(NULL, _T("IP填写错误!"), _T("提示"), MB_ICONINFORMATION);
			return;
		}
		m_comboPort.GetWindowText(strPort);
		if (strPort.SpanIncluding("1234567890,-") != strPort)
		{
			::MessageBox(NULL, _T("端口填写错误(含有“1234567890,-”之外的其他字符)!"), _T("提示"), MB_ICONINFORMATION);
			return;
		}
		m_bScanning = TRUE;//标记为正在扫描

		//获取要扫描的IP
		sIPList.clear();
		m_comboIP.GetWindowText(strBuffer);
		if (strBuffer.Find("-", 0) != -1)//IP段
		{
			CString strIPStart, strIPEnd;
			int nPos = strBuffer.Find("-", 0);
			strIPStart = strBuffer.Mid(0, nPos);
			strIPEnd = strBuffer.Mid(nPos + 1, strBuffer.GetLength() - nPos - 1);
			strIPStart.Replace(" ", "");
			strIPStart.Replace("\t", "");
			strIPEnd.Replace(" ", "");
			strIPEnd.Replace("\t", "");
			if (!isipv4str(strIPStart) || !isipv4str(strIPEnd))
			{
				StatusTextOut(0, "无效IP段");
				return;
			}

			DWORD dwStartIp;
			DWORD dwEndIp;
			dwStartIp = ntohl(inet_addr(strIPStart));
			dwEndIp = ntohl(inet_addr(strIPEnd));

			if (dwStartIp > dwEndIp)
			{
				StatusTextOut(0, "无效IP段");
				return;
			}
			DWORD dwCurrentIp = dwStartIp;
			while (dwCurrentIp <= dwEndIp)
			{
				unsigned char nFirst = (unsigned char)dwCurrentIp;
				if (nFirst == 0 || nFirst == 255)
				{
					dwCurrentIp++;
					continue;
				}
				unsigned char nSecond = (unsigned char)(dwCurrentIp >> 8);
				unsigned char nThird = (unsigned char)(dwCurrentIp >> 16);
				unsigned char nFourth = (unsigned char)(dwCurrentIp >> 24);
				strIPStart.Format("%d.%d.%d.%d", nFourth, nThird, nSecond, nFirst);
				sIPList.push_back(strIPStart);
				dwCurrentIp++;
			}
			
		}
		else if (strBuffer.Find(",", 0) != -1)//IP列表
		{
			if (strBuffer.GetAt(strBuffer.GetLength() - 1) != ',')
				strBuffer += ",";
			CString strKey;
			int nPos = 0, nPos1 = -1;
			while (1)
			{
				nPos1 = strBuffer.Find(",", nPos);
				if (nPos1 == -1)
					break;
				strKey = strBuffer.Mid(nPos, nPos1 - nPos);
				if (strKey.GetLength() > 0)
				{
					strKey.Replace(" ", "");
					strKey.Replace("\t", "");
					if (!isipv4str(strKey))
					{
						StatusTextOut(0, "存在无效IP");
						return;
					}
					sIPList.push_back(strKey);
				}
				nPos = nPos1 + 1;
			}
		}
		else//单个IP
		{
			if (!isipv4str(strBuffer))
			{
				StatusTextOut(0, "无效IP");
				return;
			}
			sIPList.push_back(strBuffer);
		}
		
		//获取要扫描的端口
		iPortList.clear();
		m_comboPort.GetWindowText(strBuffer);
		if (strBuffer.Find("-", 0) != -1)//端口段
		{
			CString strPortStart, strPortEnd;
			int nPos = strBuffer.Find("-", 0);
			strPortStart = strBuffer.Mid(0, nPos);
			strPortEnd = strBuffer.Mid(nPos + 1, strBuffer.GetLength() - nPos - 1);
			strPortStart.Replace(" ", "");
			strPortStart.Replace("\t", "");
			strPortEnd.Replace(" ", "");
			strPortEnd.Replace("\t", "");
			if (atoi(strPortEnd) > 65535)
			{
				StatusTextOut(0, "端口超出65535");
				return;
			}
			else if (atoi(strPortEnd) < 1)
			{
				StatusTextOut(0, "端口小于1");
				return;
			}

			for (int i = atoi(strPortStart); i <= atoi(strPortEnd); i++)
				iPortList.push_back(i);
		}
		else//类似 ->单个端口：80或者常用端口：21,22,23,25,53,80,99,110,135,139,143,443,445,1433,3306,3389,8080,43958
		{
			if (strBuffer.Find(",", 0) != -1)
				((CPortScanApp *)AfxGetApp())->m_IniFile.SetString("PortScan", "Ports", strBuffer);

			if (strBuffer.GetAt(strBuffer.GetLength() - 1) != ',')
				strBuffer += ",";
			CString strKey;
			int nPos = 0, nPos1 = -1;
			while (1)
			{
				nPos1 = strBuffer.Find(",", nPos);
				if (nPos1 == -1)
					break;
				strKey = strBuffer.Mid(nPos, nPos1 - nPos);
				if (strKey.GetLength() > 0)
				{
					strKey.Replace(" ", "");
					strKey.Replace("\t", "");
					if (atoi(strKey) > 65535 || atoi(strKey) < 1)
					{
						StatusTextOut(0, "端口错误");
						return;
					}
					iPortList.push_back(atoi(strKey));
				}
				nPos = nPos1 + 1;
			}
		}

		bStopScan = FALSE;
		SetDlgItemText(IDC_BUTTON_SCAN, "停止");
		
		HANDLE hScan = CreateThread(0, 0, ScanPortThread, this, 0, NULL);
		WaitForSingleObject(hScan, 100);
		CloseHandle(hScan);
	}
	else
	{
		bStopScan = TRUE;
		m_bScanning = FALSE;
		SetDlgItemText(IDC_BUTTON_SCAN, "扫描");
		m_btnScan.EnableWindow(FALSE);
		StatusTextOut(0, "正在停止扫描...");
		StatusTextOut(1, "");
		StatusTextOut(2, "");
	}
}

DWORD WINAPI CMainDlg::ScanPortThread(LPVOID lparam)
{
	CMainDlg *pThis = (CMainDlg *)lparam;

	//初始化socket，WSAStartup
	if (!initWinsock())
	{
		pThis->StatusTextOut(0, "Fail To Init Socket");
		return 0;
	}

	if (InitializeCriticalSectionAndSpinCount(&pThis->cs, 0x80000400))
	{
		pThis->dwThreadsUsed = 0;
		pThis->startScan(pThis->sIPList, pThis->iPortList, pThis->m_nMaxThread);

		DeleteCriticalSection(&pThis->cs);
	}
	WSACleanup();

	pThis->m_bScanning = FALSE;
	if (pThis->bStopScan)//不是强制终止的才显示
		pThis->StatusTextOut(0, "%s", "扫描已终止！");
	else
		pThis->StatusTextOut(0, "%s", "扫描完成！");

	pThis->m_btnScan.EnableWindow(TRUE);
	pThis->StatusTextOut(1, "");
	pThis->StatusTextOut(2, "");
	pThis->SetDlgItemText(IDC_BUTTON_SCAN, "扫描");

	return 0;
}

void CMainDlg::OnSelchangeComboIp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString strBuffer;
	m_comboIP.GetLBText(m_comboIP.GetCurSel(), strBuffer);
	if (strBuffer.Find("-", 0) != -1)
		m_comboIPMode.SetCurSel(1);
	else if (strBuffer.Find(",", 0) != -1)
		m_comboIPMode.SetCurSel(2);
	else
		m_comboIPMode.SetCurSel(0);
}

void CMainDlg::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hSelect = m_TreeCtrl.GetSelectedItem();
	HTREEITEM hLight = m_TreeCtrl.GetDropHilightItem();
	if(hLight != NULL && hSelect != hLight)
		hSelect = hLight;
	if(hSelect == NULL)
		return;
	m_TreeCtrl.SelectItem(hSelect);
	
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_TREE);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	
	*pResult = 0;
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 23;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CMainDlg::OnMenuitemCopy() 
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

void CMainDlg::OnMenuitemExpand() 
{
	MyExpandTree(TVI_ROOT, TVE_EXPAND);	
}

void CMainDlg::OnMenuitemCollapse() 
{
	MyExpandTree(TVI_ROOT, TVE_COLLAPSE);
}

void CMainDlg::OnMenuitemEmpty() 
{
	m_TreeCtrl.DeleteAllItems();
	StatusTextOut(0, "");
	StatusTextOut(1, "");
	StatusTextOut(2, "");
}

void CMainDlg::OnMenuitemSave() 
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("端口扫描：%Y-%m-%d %H-%M");
	
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
        ::MessageBox(0, "端口扫描结果导出成功", "提示", MB_ICONINFORMATION);
    }
}

CString intToString(int nIn)
{
	CString str;
	str.Format(_T("%d"), nIn);
	return str;
}

void CMainDlg::OnSelchangeComboPort() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString strBuffer;
	m_comboPort.GetLBText(m_comboPort.GetCurSel(), strBuffer);
	if (strBuffer.Find(",", 0) != -1)
		SetDlgItemText(IDC_EDIT_MAX_THREAD, intToString(((CPortScanApp *)AfxGetApp())->m_IniFile.GetInt("PortScan", "nMaxThread1", 100)));
	else
		SetDlgItemText(IDC_EDIT_MAX_THREAD, intToString(((CPortScanApp *)AfxGetApp())->m_IniFile.GetInt("PortScan", "nMaxThread", 512)));
}

void CMainDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;	//主程序结束的时候销毁
	CDialog::PostNcDestroy();
}
