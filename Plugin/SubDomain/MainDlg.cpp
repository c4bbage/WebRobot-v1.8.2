// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SubDomain.h"
#include "MainDlg.h"
#include "HtmlCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_nMaxThread = ((CSubDomainApp *)AfxGetApp())->m_IniFile.GetInt("SubDomain", "nMaxThreadNum", 200);
	m_strDomain = _T("");
	m_checkGetDetailInfo = ((CSubDomainApp *)AfxGetApp())->m_IniFile.GetInt("SubDomain", "GetDetailInfo", FALSE);
	//}}AFX_DATA_INIT
	m_bScanning = FALSE;
	dwThreadsUsed = 0;
	hSemaphore = NULL;
	bStopScan = TRUE;
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_btnScan);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_THREAD, m_nMaxThread);
	DDX_Text(pDX, IDC_EDIT_DOMAIN, m_strDomain);
	DDX_Check(pDX, IDC_CHECK_GET_INFO, m_checkGetDetailInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_COMMAND(ID_MENUITEM_COPY_DOMAIN, OnMenuitemCopyDomain)
	ON_COMMAND(ID_MENUITEM_COPY_IP, OnMenuitemCopyIp)
	ON_COMMAND(ID_MENUITEM_SAVE_DOMAIN, OnMenuitemSaveDomain)
	ON_COMMAND(ID_MENUITEM_SAVE_DOMAIN_IP, OnMenuitemSaveDomainIp)
	ON_COMMAND(ID_MENUITEM_DELETE_SELETED, OnMenuitemDeleteSeleted)
	ON_COMMAND(ID_MENUITEM_CLEAR_LIST, OnMenuitemClearList)
	ON_WM_MEASUREITEM()
	ON_COMMAND(ID_MENUITEM_OPEN_URL, OnMenuitemOpenUrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_COMMAND(ID_MENUITEM_COPY_HTTP_DOMAIN, OnMenuitemCopyHttpDomain)
	ON_COMMAND(ID_MENUITEM_SAVE_HTTP_DOMAIN, OnMenuitemSaveHttpDomain)
	ON_COMMAND(ID_MENUITEM_SAVE_HTML, OnMenuitemSaveHtml)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ON_INSERT_RESULT, OnMessageInsertResult)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CMainDlg)
EASYSIZE(IDC_LIST, ES_BORDER, IDC_EDIT_DOMAIN, ES_BORDER, ES_BORDER, 0)//左上右下
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
	char strFilePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, strFilePath, sizeof(strFilePath));
	PathRemoveFileSpec(strFilePath);
	lstrcat(strFilePath, "\\plugin\\");
	lstrcat(strFilePath, "SubDomain.txt");
	m_strDictionaryPath.Format("%s", strFilePath);

	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1200001);
	int strPartDim[2]= {300, -1};
	m_wndStatusBar.SetParts(2, strPartDim);
	
	PostMessage(WM_SIZE);

	m_ImageList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	LoadMyBitmap(&m_ImageList, IDB_BITMAP_URL);
	m_ListCtrl.InsertColumn(0, _T("域名"), LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 300);
	m_ListCtrl.InsertColumn(2, _T("主机名称"), LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn(3, _T("服务器类型"), LVCFMT_LEFT, 120);
	m_ListCtrl.InsertColumn(4, _T("标题"), LVCFMT_LEFT, 200);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
	// TODO: Add your message handler code here
	if (GetParent()->m_hWnd != NULL && m_wndStatusBar.m_hWnd != NULL)
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

__inline VOID MyOutputDebugString(LPCTSTR ptzFormat, ...)
{
	va_list vlArgs;
	TCHAR tzText[1024];
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	OutputDebugString(tzText);
	va_end(vlArgs);
}

void CMainDlg::OnButtonScan() 
{
	UpdateData();
	if (!m_bScanning)
	{
		m_ListCtrl.DeleteAllItems();
		if (m_nMaxThread <= 0 || m_nMaxThread > 1024)
		{
			StatusTextOut(0, "线程数设置错误!");
			return;
		}

		bStopScan = FALSE;
		m_bScanning = TRUE;//标记为正在扫描
		SetDlgItemText(IDC_BUTTON_SCAN, "停止");
		((CSubDomainApp *)AfxGetApp())->m_IniFile.SetInt("SubDomain", "nMaxThreadNum", m_nMaxThread);
		((CSubDomainApp *)AfxGetApp())->m_IniFile.SetInt("SubDomain", "GetDetailInfo", m_checkGetDetailInfo);
		
		HANDLE hScan = CreateThread(0, 0, ScanThread, this, 0, NULL);
		WaitForSingleObject(hScan, 100);
		CloseHandle(hScan);
	}
	else
	{
		bStopScan = TRUE;
		m_bScanning = FALSE;
		m_btnScan.EnableWindow(FALSE);
		StatusTextOut(0, "%s", "正在等待所有扫描线程结束...");
		SetDlgItemText(IDC_BUTTON_SCAN, "扫描");
	}
}

DWORD WINAPI CMainDlg::ScanThread(LPVOID lparam)
{
	CMainDlg *pThis = (CMainDlg *)lparam;

	// 初始化Winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	pThis->sDomainArray.RemoveAll();
	//读取字典
	try
	{
		CStdioFile file;
		CString strBuffer, strTmp;
		file.Open(pThis->m_strDictionaryPath, CFile::modeRead);
		while(file.ReadString(strBuffer) > 0)
		{
			try
			{
				if(strBuffer.GetLength() > 0)
				{
					strTmp = strBuffer;
					strTmp += ".";
					strTmp += pThis->m_strDomain;
					pThis->sDomainArray.Add(strTmp);
				}
			}
			catch (...){}
		}
		file.Close();
	}
	catch (...){}

	
	if (InitializeCriticalSectionAndSpinCount(&pThis->cs, 0x80000400))
	{
		pThis->dwThreadsUsed = 0;
		pThis->startScanSubDomain(&pThis->sDomainArray);
		
		DeleteCriticalSection(&pThis->cs);
	}
	pThis->m_bScanning = FALSE;
	pThis->StatusTextOut(0, "%s", "扫描完成！");
	
	pThis->StatusTextOut(1, "");
	pThis->m_btnScan.EnableWindow(TRUE);
	pThis->SetDlgItemText(IDC_BUTTON_SCAN, "开始");
	WSACleanup();
	
	return 0;
}

CString GetIP(CString strDomain, CString &strHostName)
{
	CString strReturn;
    struct hostent *remoteHost;
    struct in_addr addr;    

	remoteHost = gethostbyname(strDomain);
	if(remoteHost == NULL) 
		return "";
	strHostName = remoteHost->h_name;
    int i = 0;
	while (remoteHost->h_addr_list[i] != 0) {
		addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
		strReturn += inet_ntoa(addr);
		strReturn += ",";
	}
	return strReturn;
}

typedef struct tagThreadParameter
{
	CString strDomain;
	CString strIP;
	CString strHostName;
	CString strServerInfo;
	CString strTitle;
	CMainDlg	*pThis;
}ThreadParameter,*LPThreadParameter;

int CMainDlg::startScanSubDomain(CStringArray *sArray)
{
	DWORD dwError;
	BOOL bSemaphore = FALSE;
	DWORD dwThreadId;
	HANDLE hObject = NULL;
	LONG previousCount;
	int nMaxThread;
	int nScannedCount = 0;
	CString strDomain, strIP, strHostName;

	int nTotalNum = sArray->GetSize();
	nMaxThread = m_nMaxThread;
	m_nIpScanned = 0;
	hSemaphore = CreateSemaphore(0, nMaxThread, nMaxThread, 0);
	if (!hSemaphore)
	{
		StatusTextOut(0, "Fail To Create Semaphore");
		return 0;
	}
	
	for (int i = 0; i < nTotalNum; i++)
	{
		if (bStopScan)
			break;

		strDomain = sArray->GetAt(i);
		if (GetTickCount()%10 == 0)
			StatusTextOut(0, "%s", strDomain);
		if (GetTickCount()%5 == 0)
			StatusTextOut(1, "进度: %d/%d", i, nTotalNum);
		
		ThreadParameter *lparam = new ThreadParameter;
		lparam->strDomain = strDomain;
		lparam->pThis = this;
		hObject = CreateThread(0, 0, DomainScanThread, lparam, 0, &dwThreadId);
		if (hObject)
		{
			EnterCriticalSection(&cs);
			++nScannedCount;
			++dwThreadsUsed;
			LeaveCriticalSection(&cs);
			CloseHandle(hObject);
		}
		WaitForSingleObject(hSemaphore, INFINITE);
	}
	
	while (dwThreadsUsed)
	{
		WaitForSingleObject(hSemaphore, INFINITE);
		
		if (!bStopScan)//不是强制终止的才显示
		{
			StatusTextOut(0, "仍有%d线程正在扫描...", dwThreadsUsed);
			StatusTextOut(1, "");
		}
		
		EnterCriticalSection(&cs);
		bSemaphore = ReleaseSemaphore(hSemaphore, 1, &previousCount);
		LeaveCriticalSection(&cs);
		if (!bSemaphore)
		{
			dwError = GetLastError();
			StatusTextOut(1, "Error Code: %d", dwError);
			Sleep(1000);
			break;
		}
		if (previousCount + 1 != nMaxThread)
		{
			Sleep(200);
			if (m_nIpScanned != nScannedCount)//说明还没扫描完
				continue;
		}
		break;
	}
	if ( hSemaphore )
		CloseHandle(hSemaphore);

	return 0;
}

LRESULT CMainDlg::OnMessageInsertResult(WPARAM wParam, LPARAM lParam)
{
	ThreadParameter *par = (ThreadParameter *)lParam;

	int nCount = m_ListCtrl.GetItemCount();
	m_ListCtrl.InsertItem(nCount, par->strDomain, 0);
	m_ListCtrl.SetItemText(nCount, 1, par->strIP);
	m_ListCtrl.SetItemText(nCount, 2, par->strHostName);
	m_ListCtrl.SetItemText(nCount, 3, par->strServerInfo);
	m_ListCtrl.SetItemText(nCount, 4, par->strTitle);
	ShowRowNum();

	delete par;
	return 0;
}

void CMainDlg::ShowRowNum()
{
	CString strColumnHeaderText;
	strColumnHeaderText.Format("域名 - (%d)", m_ListCtrl.GetItemCount());
    LVCOLUMN lvCom = {0};
    char szBuffer[256] = {0};
    strcpy(szBuffer, strColumnHeaderText.GetBuffer(0));
    lvCom.mask = LVCF_TEXT;
    lvCom.cchTextMax = 256;
    lvCom.pszText = szBuffer;
    m_ListCtrl.SetColumn(0, &lvCom);
	strColumnHeaderText.ReleaseBuffer();
}

INT IsTextUTF8(LPSTR lpstrInputStream, INT iLen)
{
    INT   i;
    DWORD cOctets;  // octets to go in this UTF-8 encoded character
    UCHAR chr;
    BOOL  bAllAscii= TRUE;
    cOctets= 0;
    for( i=0; i < iLen; i++ ) {
        chr= *(lpstrInputStream+i);
        if( (chr&0x80) != 0 ) bAllAscii= FALSE;
        if( cOctets == 0 )  {
            //
            // 7 bit ascii after 7 bit ascii is just fine.  Handle start of encoding case.
            //
            if( chr >= 0x80 ) {  
				//
				// count of the leading 1 bits is the number of characters encoded
				//
				do {
					chr <<= 1;
					cOctets++;
				}
				while( (chr&0x80) != 0 );
				
				cOctets--;                        // count includes this character
				if( cOctets == 0 ) return FALSE;  // must start with 11xxxxxx
            }
        }
        else {
            // non-leading bytes must start as 10xxxxxx
            if( (chr&0xC0) != 0x80 ) {
                return FALSE;
            }
            cOctets--;                           // processed another octet in encoding
        }
    }
    // End of text.  Check for consistency.
    if( cOctets > 0 ) {   // anything left over at the end is an error
        return FALSE;
    }
    if( bAllAscii ) {     // Not utf-8 if all ascii.  Forces caller to use code pages for conversion
        return FALSE;
    }
    return TRUE;
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

CString GetServerDetailInfo(CString strServer, CString &strTitle)
{
	strServer.Insert(0, "http://");
	CString strReturn = "Unknown", szAllData, szData;
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

		szAllData.Empty();
		while(pF->ReadString(szData))
			szAllData += szData;

		szAllData.MakeLower();
		nPos1 = szAllData.Find("<title>", 0);
		if (nPos1 != -1)
		{
			int nPos2 = szAllData.Find("</title>", nPos1);
			if (nPos2 != -1)
			{
				strTitle = szAllData.Mid(nPos1 + 7, nPos2 - nPos1 - 7);
				if (IsTextUTF8(strTitle.GetBuffer(0), strTitle.GetLength()))
					Utf8ToAnsi(strTitle);
			}
		}
	}
	catch(...)
	{
		strReturn = "";
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

DWORD WINAPI CMainDlg::DomainScanThread(LPVOID lparam)
{
	ThreadParameter *par = (ThreadParameter *)lparam;
	CMainDlg *pThis = par->pThis;
	CString strDomain = par->strDomain;

	CString strIP, strHostName;
	strIP = GetIP(par->strDomain, strHostName);
	if (strIP != "")
	{
		par->strIP = strIP;
		par->strHostName = strHostName;
		if (pThis->m_checkGetDetailInfo)
			par->strServerInfo = GetServerDetailInfo(strDomain, par->strTitle);

		EnterCriticalSection(&pThis->cs);
		pThis->SendMessage(WM_ON_INSERT_RESULT, 0, (LPARAM)par);//OnMessageInsertResult
		LeaveCriticalSection(&pThis->cs);
	}
	
	EnterCriticalSection(&pThis->cs);
	++pThis->m_nIpScanned;
	if (pThis->dwThreadsUsed)
		--pThis->dwThreadsUsed;
	ReleaseSemaphore(pThis->hSemaphore, 1, 0);
	LeaveCriticalSection(&pThis->cs);
	
	//delete par;
	return 0;
}

void CMainDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_LIST);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	int	count = pSubMenu->GetMenuItemCount();
	if (m_ListCtrl.GetSelectedCount() == 0)
	{
		for (int i = count - 1; i >= 0; i--)
		{
			pSubMenu->DeleteMenu(i, MF_BYPOSITION);
		}
	}
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	
	*pResult = 0;
}

void CMainDlg::OnMenuitemCopyDomain() 
{
	// TODO: Add your command handler code here
	int nItem;
    CString strText(_T(""));
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_ListCtrl.GetNextSelectedItem(pos);
        strText += m_ListCtrl.GetItemText(nItem, 0) + _T("\r\n");
    }
	strText.MakeLower();
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

void CMainDlg::OnMenuitemCopyIp() 
{
	// TODO: Add your command handler code here
	int nItem;
    CString strText(_T(""));
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_ListCtrl.GetNextSelectedItem(pos);
        strText += m_ListCtrl.GetItemText(nItem, 1) + _T("\r\n");
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

void CMainDlg::OnMenuitemSaveDomain() 
{
	// TODO: Add your command handler code here
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("Domain：%Y-%m-%d %H-%M-%S");
	
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
			strData.MakeLower();
            file.Write(strData, strData.GetLength());
		}
        file.Close();
        ::MessageBox(0, "列表内容导出成功", "提示", MB_ICONINFORMATION);
    }
}

void CMainDlg::OnMenuitemSaveDomainIp() 
{
	// TODO: Add your command handler code here
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("Domain_IP：%Y-%m-%d %H-%M-%S");
	
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
            strData.Format(_T("%s\t%s\r\n"), m_ListCtrl.GetItemText(i, 0), m_ListCtrl.GetItemText(i, 1));
			strData.MakeLower();
            file.Write(strData, strData.GetLength());
		}
        file.Close();
        ::MessageBox(0, "列表内容导出成功", "提示", MB_ICONINFORMATION);
    }
}

void CMainDlg::OnMenuitemDeleteSeleted() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos  ==  NULL)
		return;
	else
	{
		for (int i = m_ListCtrl.GetItemCount() - 1; i >= 0; i--)
		{
			if (m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				m_ListCtrl.DeleteItem(i);
		}
	}

	StatusTextOut(0, "");
	ShowRowNum();
}

void CMainDlg::OnMenuitemClearList() 
{
	// TODO: Add your command handler code here
	m_ListCtrl.DeleteAllItems();
	ShowRowNum();
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 23;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CMainDlg::OnMenuitemOpenUrl() 
{
	// TODO: Add your command handler code here
	int nItem;
    CString strText(_T(""));
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_ListCtrl.GetNextSelectedItem(pos);
        strText = "http://" + m_ListCtrl.GetItemText(nItem, 0);
		ShellExecute(0, "open", strText, NULL, NULL, SW_SHOW);
    }
}

void CMainDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nItem;
    CString strText(_T(""));
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_ListCtrl.GetNextSelectedItem(pos);
        strText = "http://" + m_ListCtrl.GetItemText(nItem, 0);
		ShellExecute(0, "open", strText, NULL, NULL, SW_SHOW);
    }

	*pResult = 0;
}

void CMainDlg::OnMenuitemCopyHttpDomain() 
{
	// TODO: Add your command handler code here
	int nItem;
    CString strText(_T(""));
    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_ListCtrl.GetNextSelectedItem(pos);
        strText += "http://" + m_ListCtrl.GetItemText(nItem, 0) + _T("/\r\n");
    }
	strText.MakeLower();
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

void CMainDlg::OnMenuitemSaveHttpDomain() 
{
	// TODO: Add your command handler code here
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("Http Domain：%Y-%m-%d %H-%M-%S");
	
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
            strData.Format(_T("http://%s/\r\n"), m_ListCtrl.GetItemText(i, 0));
			strData.MakeLower();
            file.Write(strData, strData.GetLength());
		}
        file.Close();
        ::MessageBox(0, "列表内容导出成功", "提示", MB_ICONINFORMATION);
    }
}

void CMainDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;	//主程序结束的时候销毁
	CDialog::PostNcDestroy();
}

void CMainDlg::SaveTreeCtrlData(CString strSavePath)
{
	UpdateData();
	SYSTEMTIME	SysTime;
	GetLocalTime(&SysTime);
	char strLogFileName[MAX_PATH] = {0};
	char strTime1[MAX_PATH] = {0};
	char strTime2[MAX_PATH] = {0};

	wsprintf
		(
		strTime1,
		"\\SubDomain：%d-%02d-%02d %02d^%02d^%02d.htm",
		SysTime.wYear, SysTime.wMonth, SysTime.wDay,
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
	CString strHostIP, strTmp1;

	strTmp1 = "</tr> \r\n      <tr>\r\n        <td class=sub width=\"20%\">域名</td>\r\n	<td class=sub width=\"15%\">IP</td>\r\n	<td class=sub width=\"15%\">主机名称</td>\r\n	<td class=sub width=\"20%\">服务器类型</td>\r\n	<td class=sub width=\"30%\">标题</td>\r\n</tr>\r\n      </tr>";
	file.Write(strTmp1, strTmp1.GetLength());
	
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		strHostIP = m_ListCtrl.GetItemText(i, 1);
		strHostIP.TrimRight(",");
		strHostIP.Replace(",", "<br>");

		strTmp1 = "            <tr>\r\n        <td class=\"default\" width=\"20%\"><a href=\"";
		strTmp1 += "http://" + m_ListCtrl.GetItemText(i, 0);
		strTmp1 += "/\" target=\"_blank\">";
		strTmp1 += m_ListCtrl.GetItemText(i, 0);
		strTmp1 += "</a></td>\r\n	<td class=default width=\"15%\">";
		strTmp1 += strHostIP;
		strTmp1 += "</td>\r\n	<td class=default width=\"15%\">";
		strTmp1 += m_ListCtrl.GetItemText(i, 2);
		strTmp1 += "</td>\r\n	<td class=default width=\"20%\">";
		strTmp1 += m_ListCtrl.GetItemText(i, 3);
		strTmp1 += "</td>\r\n	<td class=default width=\"30%\">";
		strTmp1 += m_ListCtrl.GetItemText(i, 4);
		strTmp1 += "</td></tr>";
		file.Write(strTmp1, strTmp1.GetLength());
	}

	file.Write(HTMLEnd, sizeof(HTMLEnd));
	file.Close();
}

void CMainDlg::OnMenuitemSaveHtml() 
{
	SYSTEMTIME	SysTime;
	GetLocalTime(&SysTime);
	char strFileName[MAX_PATH] = {0};
	wsprintf
		(
		strFileName,
		"SubDomain：%d-%02d-%02d %02d^%02d^%02d.htm",
		SysTime.wYear, SysTime.wMonth, SysTime.wDay,
		SysTime.wHour, SysTime.wMinute, SysTime.wSecond
		);
	
    CFileDialog dlg(FALSE, _T("htm"), strFileName, OFN_OVERWRITEPROMPT, _T("htm文件(*.htm)|*.htm||"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
	SaveTreeCtrlData(dlg.GetPathName());
	::MessageBox(0, "数据导出成功！", "提示", MB_ICONINFORMATION);
}
