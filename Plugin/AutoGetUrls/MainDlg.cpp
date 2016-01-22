// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoGetUrls.h"
#include "MainDlg.h"

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
	//}}AFX_DATA_INIT
	m_bStop = TRUE;
	m_hWorkThread = NULL;
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_comboSearch);
	DDX_Control(pDX, IDC_COMBO_SEARCH_ENGINE, m_comboSearchEngine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_COMMAND(ID_MENUITEM_COPY, OnMenuitemCopy)
	ON_COMMAND(ID_MENUITEM_DELETE, OnMenuitemDelete)
	ON_COMMAND(ID_MENUITEM_CLEAR, OnMenuitemClear)
	ON_COMMAND(ID_MENUITEM_SAVE, OnMenuitemSave)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CMainDlg)
EASYSIZE(IDC_LIST, ES_BORDER, IDC_COMBO_SEARCH, ES_BORDER, ES_BORDER, 0)//左上右下
END_EASYSIZE_MAP
/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	INIT_EASYSIZE;
	// TODO: Add extra initialization here
	m_comboSearchEngine.SendMessage(CB_SETITEMHEIGHT, -1, 16);
	m_comboSearchEngine.SendMessage(CB_SETITEMHEIGHT, 0, 16);
	m_comboSearch.SendMessage(CB_SETITEMHEIGHT, -1, 16);
	m_comboSearch.SendMessage(CB_SETITEMHEIGHT, 0, 16);
	m_comboSearchEngine.SetCurSel(0);

	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1200001);
	int strPartDim[1]= {-1};
	m_wndStatusBar.SetParts(1, strPartDim);

	m_strUserAgent = ((CAutoGetUrlsApp *)AfxGetApp())->m_IniFile.GetString("WebRobot", "UserAgent", "User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5");

	CBitmap bm1;
	bm1.LoadBitmap(IDB_BITMAP_URL);
	m_ImageList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	m_ImageList.Add(&bm1, RGB(255, 255, 255));
	m_ListCtrl.InsertColumn(0, _T("URL"), LVCFMT_LEFT, 650);
	m_ListCtrl.InsertColumn(1, _T("Title"), LVCFMT_LEFT, 300);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	InitConfig();

	PostMessage(WM_SIZE);

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

void CMainDlg::ShowRowNum()
{
	CString strColumnHeaderText;
	strColumnHeaderText.Format("URL - (%d)", m_ListCtrl.GetItemCount());
    LVCOLUMN lvCom = {0};
    char szBuffer[256] = {0};
    strcpy(szBuffer, strColumnHeaderText.GetBuffer(0));
    lvCom.mask = LVCF_TEXT;
    lvCom.cchTextMax = 256;
    lvCom.pszText = szBuffer;
    m_ListCtrl.SetColumn(0, &lvCom);
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

CString CMainDlg::getURLContext(int nSearchEngine, CString strUrl, CString strHeader)
{
	DWORD dwFlags;
	CString strTemp, s, strHtml;
	
	try
	{
		DWORD dwType = 0;
		CString strServer, strObject, indata;
		INTERNET_PORT wPort = 80;
		if (!AfxParseURL(strUrl, dwType, strServer, strObject, wPort))
			return "";
		
		CInternetSession Session(_T("session"), 0, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD);//设置不缓冲
		Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5 * 1000, 0);
		Session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 5 * 1000, 0);
		Session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 5 * 1000, 0);
		
		CHttpConnection *pHttpConnect = NULL;
		if (wPort == 443)
			pHttpConnect = Session.GetHttpConnection(strServer, INTERNET_FLAG_SECURE, wPort);
		else
			pHttpConnect = Session.GetHttpConnection(strServer, wPort);
		
		if(pHttpConnect)
		{
			CHttpFile* pFile = NULL; 
			if (wPort == 443)
			{
				/*
				pFile = (CHttpFile*)pHttpConnect->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1,
					NULL, NULL,INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_SECURE | INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_COOKIES |
					INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_DONT_CACHE
					);
				//get web server option
				pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
				dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
				//set web server option
				pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);*/

				pFile = (CHttpFile*)pHttpConnect->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1,
					NULL, NULL,INTERNET_FLAG_SECURE | INTERNET_FLAG_EXISTING_CONNECT |
					INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_DONT_CACHE
					);
				//get web server option
				pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
				dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
				//set web server option
				pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
			}
			else
			{
				pFile = pHttpConnect->OpenRequest( CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL,
					INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);//不加这个INTERNET_FLAG_NO_COOKIES就无法自定义cookie
			}
			if ( pFile )
			{
				if (nSearchEngine == 1)//Google
					pFile->AddRequestHeaders("User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5");
				else
					pFile->AddRequestHeaders(m_strUserAgent);

				pFile->AddRequestHeaders(strHeader);
				pFile->SendRequest();
				
				// 返回的HTML
				while (pFile->ReadString(s))
				{
					strHtml += s;
					strHtml += "\r\n";
				}
				Utf8ToAnsi(strHtml);
				
				pFile->Close();
				delete pFile ;
				pFile = NULL;
			}
			
			pHttpConnect->Close() ;
			delete pHttpConnect ;
		}
	}
	catch( CInternetException *e )
	{
		e->Delete();
		strHtml = "";
	}
	return strHtml;
}


BOOL CMainDlg::GetBaiduSearchResultRedirectUrl(CString &strReturn)
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

		CInternetSession Session(_T("session"), 0, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD);//设置不缓冲
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
	
	return sOut;
}

CString URLEncode(CString sIn)
{
	CString sOut;
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
	
	return sOut;
}

int CMainDlg::FindSameItem(CString strText)
{
	int nCount = -1;
	LVFINDINFO info;
	info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.psz = strText;
	
	while (1)
	{
		nCount = m_ListCtrl.FindItem(&info, nCount);
		if (nCount != -1)//找到了类似的，判断是否相同
		{
			if (strText == m_ListCtrl.GetItemText(nCount, 0))
				return nCount;
			else
				continue;
		}
		else
			break;
	}
	return -1;
}

void CMainDlg::InsertUrlToList(int nSearchEngine, CString strUrl, CString strHtml)
{
	try
	{
		HyperLink m_thisURL;
		// URL Vector
		vector<HyperLink>m_pageURL;
		m_thisURL.str_Hyperlink = strUrl;
		CWebHost m_host(strHtml, m_pageURL, m_thisURL);
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
				if (!GetBaiduSearchResultRedirectUrl(strTemp))
					continue;
				strTemp.Format("%s", URLDecode(strTemp));
			}
			else if (strTemp.Find(".google.", 0) != -1 || strTemp.Find("s.bt.gg", 0) != -1)//处理谷歌搜索的数据
			{
				int nPosGoogleUrlStart = strTemp.Find("/url?q=", 0);
				if (nPosGoogleUrlStart == -1)
					continue;
				int nPosGoogleUrlEnd = strTemp.Find("&sa=", nPosGoogleUrlStart);
				if (nPosGoogleUrlEnd == -1)
				{
					nPosGoogleUrlEnd = strTemp.Find("&amp;", nPosGoogleUrlStart);
					if (nPosGoogleUrlEnd == -1)
						continue;
				}
				strTemp = strTemp.Mid(nPosGoogleUrlStart + 7, nPosGoogleUrlEnd - nPosGoogleUrlStart - 7);
				strTemp.Format("%s", URLDecode(strTemp));
			}
			
			// 判断url里是否包含被过滤的字串
			for (int nStringListCount = 0; nStringListCount < strUrlFilterArray.GetSize(); nStringListCount++)
			{
				strFilter = strUrlFilterArray.GetAt(nStringListCount);
				if (strTemp.Find(strFilter, 0) != -1)
				{
					bFindFilterString = TRUE;
					break;
				}
			}
			if (bFindFilterString)
				continue;
			
			if (FindSameItem(strTemp) != -1)//判断列表里是否有重复项
				continue;
			int nCount = m_ListCtrl.GetItemCount();
			m_ListCtrl.InsertItem(nCount, strTemp);
			m_ListCtrl.SetItemText(nCount, 1, m_pageURL[i].str_HyperlinkText);
			ShowRowNum();
		}
	}
	catch (...){}
}

void CMainDlg::InitConfig()
{
	strUrlFilterArray.RemoveAll();
	CString strAllFilters = ((CAutoGetUrlsApp *)AfxGetApp())->m_IniFile.GetString("GetUrls", "FilterKeyWord", TEXT_FILTER);
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
}

DWORD WINAPI CMainDlg::SearchThread(LPVOID lparam)
{
	CMainDlg *pThis = (CMainDlg *)lparam;
	pThis->SearchFunc();
	return -1;	
}

void WriteLog(char *fmt,...)
{
	FILE *fp;
	va_list args;
	if((fp =fopen("c:\\ginalog.log", "a")) !=NULL)
	{		
		va_start(args,fmt);
		
		vfprintf(fp, fmt, args);
		fprintf(fp, "\n");
		fclose(fp);
		
		va_end(args);
	}
}

void MySleep(int nMinTime, int nMaxIncrease)
{
	Sleep(nMinTime + rand()%nMaxIncrease);
}

void CMainDlg::SearchFunc()
{
	CString strSearch, strUrl, strHtml;
	m_comboSearch.GetWindowText(strSearch);
	if (strSearch.GetLength() == 0)
		return;

	m_btnSearch.SetWindowText("停止");

	CString strHeader;
	int nSearchEngine = m_comboSearchEngine.GetCurSel();
	if (nSearchEngine == 0)//Baidu
	{
		strHeader += "Cookie: BAIDUID=19168E8FF8AD2E0ED77152AA2E38EEC8:SL=0:NR=50:FG=1;";
		//http://www.baidu.com/s?wd=test&pn=0~750	+=50	Cookie: BAIDUID=19168E8FF8AD2E0ED77152AA2E38EEC8:SL=0:NR=50:FG=1;
		for (int i = 0; i <= 15; i++)
		{
			if (m_bStop)
				break;

			try
			{
				strUrl.Format("http://www.baidu.com/s?wd=%s&pn=%d", strSearch, 50 * i);
				StatusTextOut(0, "%s", strUrl);
				strHtml = getURLContext(nSearchEngine, strUrl, strHeader);
				if (strHtml.Find("/link?url=", 0) == -1)
				{
					::MessageBox(NULL, _T("获取网页源码失败，可能是需要验证码，若需要验证码，请更换IP来解决。"), _T("提示"), MB_ICONINFORMATION);
					break;
				}
				strHtml.Replace(" =", "=");//百度搜索会出现这种异常的东西
				InsertUrlToList(nSearchEngine, strUrl, strHtml);
			}
			catch (...){}
		}
	}
	else if (nSearchEngine == 1)//Google
	{
		BOOL ping = InternetCheckConnection("http://www.google.com.hk/", 1, 0);
		if (!ping)
        {
			::MessageBox(NULL, _T("连接到Google服务器失败，请使用境外代理或VPN访问！"), _T("提示"), MB_ICONINFORMATION);
		}
		else
		{
			strHeader += "Cookie: PREF=ID=1111111111111111:FF=1:LD=zh-CN:NR=100:TM=1433517672:LM=1434556436:SG=2:S=VEmoR-mP3auPQ7Gh";
			//http://www.google.com.hk/search?q=test&num=100&filter=0&start=0~900	+=100	Cookie: PREF=ID=1111111111111111:FF=1:LD=en:NR=100:TM=1433517672:LM=1434556436:SG=2:S=VEmoR-mP3auPQ7Gh
			for (int i = 0; i <= 9; i++)
			{
				if (m_bStop)
					break;
				
				try
				{
					strUrl.Format("http://www.google.com.hk/search?q=%s&num=100&filter=0&start=%d&gws_rd=ssl", strSearch, 100 * i);
					StatusTextOut(0, "%s", strUrl);
					strHtml = getURLContext(nSearchEngine, strUrl, strHeader);
					if (strHtml != "" && strHtml.Find("/url?q=", 0) == -1 && strHtml.Find("resultStats", 0) == -1)
					{
						if (strHtml.Find("/sorry/image", 0) != -1)
						{
							::MessageBox(NULL, _T("需要验证码，请更换IP来解决。"), _T("提示"), MB_ICONINFORMATION);
							break;
						}
						else
						{
							::MessageBox(NULL, _T("获取网页源码失败，可能是需要验证码，若需要验证码，请更换IP来解决。"), _T("提示"), MB_ICONINFORMATION);
							break;
						}
					}
					InsertUrlToList(nSearchEngine, strUrl, strHtml);
				}
				catch (...){}
			}
		}
	}
	else if (nSearchEngine == 2)//Bing
	{
		strHeader += "Cookie: SRCHUSR=AUTOREDIR=0&GEOVAR=&DOB=20150607; _EDGE_V=1; SCRHDN=ASD=0&DURL=#; SRCHD=D=3923568&AF=QBRE; _FS=mkt=zh-hk; _EDGE_S=mkt=zh-hk&SID=27EC5EEBA33A630526325922A2286261; SRCHHPGUSR=CW=1903&CH=963&DPR=1&NEWWND=0&NRSLT=50";
		//http://www.bing.com/search?q=test&first=1~301	+=100	Cookie: PREF=ID=1111111111111111:FF=1:LD=en:NR=100:TM=1433517672:LM=1434556436:SG=2:S=VEmoR-mP3auPQ7Gh
		for (int i = 0; i <= 6; i++)
		{
			if (m_bStop)
				break;
			
			try
			{
				strUrl.Format("http://cn.bing.com/search?q=%s&first=%d&qs=bs&form=QBRE", strSearch, 50 * i + 1);
				StatusTextOut(0, "%s", strUrl);
				strHtml = getURLContext(nSearchEngine, strUrl, strHeader);
				if (strHtml != "" && strHtml.Find("challenge?reload", 0) != -1)
				{
					::MessageBox(NULL, _T("需要验证码，请更换IP来解决。"), _T("提示"), MB_ICONINFORMATION);
					break;
				}
				InsertUrlToList(nSearchEngine, strUrl, strHtml);
			}
			catch (...){}
			Sleep(1000);
			StatusTextOut(0, "%s", "休息中...");
			MySleep(2000, 3000);
		}
	}
	/*
	else if (nSearchEngine == 3)//Google镜像
	{
		strHeader += "Cookie: PREF=ID=1111111111111111:FF=1:LD=zh-CN:NR=100:TM=1433517672:LM=1434556436:SG=2:S=VEmoR-mP3auPQ7Gh";
		//http://www.google.com.hk/search?q=test&num=100&filter=0&start=0~900	+=100	Cookie: PREF=ID=1111111111111111:FF=1:LD=en:NR=100:TM=1433517672:LM=1434556436:SG=2:S=VEmoR-mP3auPQ7Gh
		for (int i = 0; i <= 9; i++)
		{
			if (m_bStop)
				break;
			
			try
			{
				//strUrl.Format("https://s.bt.gg/search?q=%s&num=100&filter=0&start=%d&gws_rd=ssl", strSearch, 100 * i);
				strUrl.Format("https://s.bt.gg/search?q=%s&start=%d&num=100&newwindow=1&biw=1920&bih=963&filter=0", strSearch, 100 * i);
				StatusTextOut(0, "%s", strUrl);
				strHtml = getURLContext(nSearchEngine, strUrl, strHeader);
				WriteLog("%s", strHtml);
				
				if (strHtml != "" && strHtml.Find("/url?q=", 0) == -1 && strHtml.Find("resultStats", 0) == -1)
				{
					if (strHtml.Find("/sorry/image", 0) != -1)
					{
						::MessageBox(NULL, _T("需要验证码，请更换IP来解决。"), _T("提示"), MB_ICONINFORMATION);
						break;
					}
					else
					{
						::MessageBox(NULL, _T("获取网页源码失败，可能是需要验证码，若需要验证码，请更换IP来解决。"), _T("提示"), MB_ICONINFORMATION);
						break;
					}
				}
				InsertUrlToList(nSearchEngine, strUrl, strHtml);
			}
			catch (...){}
		}
	}*/

	StatusTextOut(0, "%s", "抓取链接完毕！");
	m_btnSearch.SetWindowText("搜索");
	m_btnSearch.EnableWindow(TRUE);
}

void CMainDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	CString strBtn;
	m_btnSearch.GetWindowText(strBtn);
	if (strBtn == "停止")
	{
		m_btnSearch.EnableWindow(FALSE);
		m_bStop = TRUE;
		DWORD dwExitCode;
		if(m_hWorkThread != NULL)
		{
			//主线程直到扫描结束才会退出，所以必须强制结束
			WaitForSingleObject(m_hWorkThread, 200);
			if(GetExitCodeThread(m_hWorkThread, &dwExitCode))
			{
				if(dwExitCode == STILL_ACTIVE)
					TerminateThread(m_hWorkThread, dwExitCode);
			}
			m_hWorkThread = NULL;
		}
		m_btnSearch.SetWindowText("搜索");
		return;
	}

	m_bStop = FALSE;
	m_hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SearchThread, this, 0, NULL);
	WaitForSingleObject(m_hWorkThread, 100);
	CloseHandle(m_hWorkThread);
}

void CMainDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
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

	*pResult = 0;
}

void CMainDlg::OnMenuitemCopy() 
{
    int nItem;
    CString strText;
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

void CMainDlg::OnMenuitemDelete() 
{
	for (int i = m_ListCtrl.GetItemCount() - 1; i >= 0; i--)
	{
		if (m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			m_ListCtrl.DeleteItem(i);
	}
	ShowRowNum();
}

void CMainDlg::OnMenuitemClear() 
{
	m_ListCtrl.DeleteAllItems();
	
	ShowRowNum();
}

void CMainDlg::OnMenuitemSave() 
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("%Y-%m-%d %H-%M-%S");
	strTime.Insert(0, "URL：");
	
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
        ::MessageBox(0, "列表内容导出成功", "提示", MB_ICONINFORMATION);
    }
}

void CMainDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU);
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

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 23;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CMainDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;	//主程序结束的时候销毁
	CDialog::PostNcDestroy();
}
