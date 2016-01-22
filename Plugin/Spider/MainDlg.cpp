// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spider.h"
#include "MainDlg.h"
#include "../common/InputDlg/InputDlg.h"
#include "../common/AutoLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define COLUMN_STRING "待爬行链接"
#define COLUMN_STRING_1 "正在爬行的链接"
#define BUTTON_SCAN_1 "爬行网站"
#define	WM_UPDATEDATA	(WM_USER+100)
#define WM_ON_INSERT_CRAWL_RESULT (WM_USER + 103)
#define REPLACESTRING "))(($^@~#&"

typedef struct tagThreadParameter
{
	int nCurCrawlCount;
	CString strTempURL;
	CMainDlg	*pThis;
}ThreadParameter,*LPThreadParameter;

struct CrawlParamStruct
{
	int		nScanStatus;
	CString	strURL;
	CString strSrcUrl;
};

CCriticalSection g_csSpider;

CString strNotCrawl = ".jpg|.jpeg|.png|.ico|.css|.txt|.log|.gif|.rar|.zip|.exe|.xls|.doc|.ppt|.js|.mp3|.pdf|.swf|.flv|.iso|.bin|";
/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_nMaxThreadNum = ((CSpiderApp *)AfxGetApp())->m_IniFile.GetInt("Spider", "nMaxThreadNum", 5);
	m_bSimilarUrlNotClawl = ((CSpiderApp *)AfxGetApp())->m_IniFile.GetInt("Spider", "bSimilarUrlNotClawl", TRUE);
	//}}AFX_DATA_INIT
	m_pListEdit = 0;
	nListData	= 0;
	m_bScanning = FALSE;
	m_nThreadCount = 0;
	m_nCurrentThreadCount = 0;
}

CMainDlg::~CMainDlg()
{
	delete m_pListEdit;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_BUTTON_APPEARANCE, m_btnAppearance);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_btnScan);
	DDX_Control(pDX, IDC_COMBO_LIMITDIR, m_comboLimitDir);
	DDX_Control(pDX, IDC_TREE_URL, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST_URL, m_ListCtrl);
	DDX_Control(pDX, IDC_LIST_RESCANURL, m_ReScanURLListCtrl);
	DDX_Control(pDX, IDC_LIST_SITE, m_SiteListCtrl);
	DDX_Text(pDX, IDC_EDIT_MAXTHREADNUM, m_nMaxThreadNum);
	DDV_MinMaxUInt(pDX, m_nMaxThreadNum, 1, 200);
	DDX_Check(pDX, IDC_CHECK_SIMILAR_URL, m_bSimilarUrlNotClawl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_APPEARANCE, OnButtonAppearance)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SITE, OnRclickListSite)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RESCANURL, OnRclickListRescanurl)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_URL, OnRclickTreeUrl)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_URL, OnRclickListUrl)
	ON_COMMAND(ID_MENUITEM_COPY, OnMenuitemCopy)
	ON_COMMAND(ID_MENUITEM_DELETE, OnMenuitemDelete)
	ON_COMMAND(ID_MENUITEM_ADDURL, OnMenuitemAddurl)
	ON_COMMAND(ID_MENUITEM_IMPORT, OnMenuitemImport)
	ON_COMMAND(ID_MENUITEM_DELETEALLURLS, OnMenuitemDeleteallurls)
	ON_COMMAND(ID_MENUITEM_RIGHT_COPY, OnMenuitemRightCopy)
	ON_COMMAND(ID_MENUITEM_RIGHT_COPYSRCPAGE, OnMenuitemRightCopysrcpage)
	ON_COMMAND(ID_MENUITEM_RIGHT_DELSEL, OnMenuitemRightDelsel)
	ON_COMMAND(ID_MENUITEM_RIGHT_CLEARALL, OnMenuitemRightClearall)
	ON_COMMAND(ID_MENUITEM_RIGHT_SAVEALLURLS, OnMenuitemRightSaveallurls)
	ON_COMMAND(ID_MENUITEM_RIGHT_IMPORTFILE, OnMenuitemRightImportfile)
	ON_COMMAND(ID_MENUITEM_RIGHT_VISIT, OnMenuitemRightVisit)
	ON_COMMAND(ID_MENUITEM_RIGHT_TREE_COPY, OnMenuitemRightTreeCopy)
	ON_COMMAND(ID_MENUITEM_TREE_CLEAR, OnMenuitemTreeClear)
	ON_COMMAND(ID_MENUITEM_GOTO_SRCPAGE, OnMenuitemGotoSrcpage)
	ON_COMMAND(ID_MENUITEM_TREE_SAVEALLURLSTOFILE, OnMenuitemTreeSaveallurlstofile)
	ON_COMMAND(ID_MENUITEM_TREE_IMPORT_URLS, OnMenuitemTreeImportUrls)
	ON_COMMAND(ID_MENUITEM_EXPAND, OnMenuitemExpand)
	ON_COMMAND(ID_MENUITEM_COLLAPSE, OnMenuitemCollapse)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_URL, OnSelchangedTreeUrl)
	ON_WM_DROPFILES()
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_SITE, OnBeginlabeleditList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_SITE, OnEndlabeleditList)
	ON_COMMAND(ID_MENUITEM_CLEAR_TOP_LIST, OnMenuitemClearTopList)
	ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTER, OnMaxMinInfo)
	ON_NOTIFY(SPN_DELTA,     IDC_SPLITTER, OnSplitter1Delta)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDATEDATA, OnMessageUpdateData)
	ON_MESSAGE(WM_ON_INSERT_CRAWL_RESULT, OnMessageInsertCrawlResult)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comboLimitDir.SetCurSel(3);
	m_comboLimitDir.SendMessage(CB_SETITEMHEIGHT, 0, 17);
	m_comboLimitDir.SendMessage(CB_SETITEMHEIGHT, -1, 16);

	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1200001);
	int strPartDim[2]= {300, -1};
	m_wndStatusBar.SetParts(2, strPartDim);

	m_btnAppearance.SetBitmaps(IDB_BITMAP_C, RGB(0, 255, 0));

	m_infotip.Create(this);
	m_infotip.SetMaxTipWidth(450); 
	m_infotip.SetTipTextColor(RGB(0,170,64));
	m_infotip.AddTool(GetDlgItem(IDC_BUTTON_APPEARANCE), _T("列表显示"));
	m_infotip.AddTool(GetDlgItem(IDC_CHECK_SIMILAR_URL), _T("对URL文件一样但参数不一样的链接只爬行一次"));

	m_ImageList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	CBitmap bm1, bm2;
	bm1.LoadBitmap(IDB_BITMAP_A);
	bm2.LoadBitmap(IDB_BITMAP_B);
	m_ImageList.Add(&bm1, RGB(192, 192, 192));
	m_ImageList.Add(&bm2, RGB(192, 192, 192));
	
	ListView_SetExtendedListViewStyle(m_SiteListCtrl.m_hWnd, LVS_EX_FULLROWSELECT);
	m_SiteListCtrl.InsertColumn(0, COLUMN_STRING, LVCFMT_LEFT, 480);
	m_SiteListCtrl.InsertColumn(1, "爬行条件(待测URL必须包含以下字串)", LVCFMT_LEFT, 230);
	m_SiteListCtrl.InsertColumn(2, "hide column", LVCFMT_LEFT, 0);
	m_SiteListCtrl.SetExtendedStyle(m_SiteListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
	m_SiteListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_SiteListCtrl.PrepareControl(0);

	ListView_SetExtendedListViewStyle(m_ReScanURLListCtrl.m_hWnd, LVS_EX_FULLROWSELECT);
	m_ReScanURLListCtrl.InsertColumn(0, COLUMN_STRING_1, LVCFMT_LEFT, 410);
	m_ReScanURLListCtrl.InsertColumn(1, "来源页面", LVCFMT_LEFT, 300);
	m_ReScanURLListCtrl.SetExtendedStyle(m_ReScanURLListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
	m_ReScanURLListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_RightImageList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	CBitmap bmp1, bmp2, bmp3, bmp4, bmp5, bmp6, bmp7, bmp8, bmp9, bmp10, bmp11;
	bmp1.LoadBitmap(IDB_BITMAP_1);
	bmp2.LoadBitmap(IDB_BITMAP_2);
	bmp3.LoadBitmap(IDB_BITMAP_3);
	bmp4.LoadBitmap(IDB_BITMAP_4);
	bmp5.LoadBitmap(IDB_BITMAP_5);
	bmp6.LoadBitmap(IDB_BITMAP_6);
	bmp7.LoadBitmap(IDB_BITMAP_7);
	bmp8.LoadBitmap(IDB_BITMAP_8);
	bmp9.LoadBitmap(IDB_BITMAP_9);
	bmp10.LoadBitmap(IDB_BITMAP_10);
	bmp11.LoadBitmap(IDB_BITMAP_11);
	m_RightImageList.Add(&bmp1, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp2, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp3, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp4, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp5, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp6, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp7, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp8, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp9, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp10, RGB(192, 192, 192));
	m_RightImageList.Add(&bmp11, RGB(192, 192, 192));
	m_TreeCtrl.SetImageList(&m_RightImageList, TVSIL_NORMAL);
	
	ListView_SetExtendedListViewStyle(m_ListCtrl.m_hWnd, LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0, "URL", LVCFMT_LEFT, 300);
	m_ListCtrl.InsertColumn(1, "来源页面", LVCFMT_LEFT, 300);
	m_ListCtrl.InsertColumn(2, "无参URL", LVCFMT_LEFT, 0);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
	m_ListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	//Splitter
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_SPLITTER);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER, SPS_VERTICAL|SPS_DELTA_NOTIFY);
    //  register windows for splitter
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_LEFT,	GetDlgItem(IDC_LIST_SITE));
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_LEFT,	GetDlgItem(IDC_LIST_RESCANURL));
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_RIGHT,	GetDlgItem(IDC_TREE_URL));
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_RIGHT,	GetDlgItem(IDC_LIST_URL));
    m_wndSplitter.Relayout();
	m_ListCtrl.ShowWindow(SW_HIDE);

	//不爬行的文件的后缀初始化
	int nPos = 0;
	int nLastestPos = 0;
	while (1)
	{
		nPos = strNotCrawl.Find("|", nPos);
		if (nPos != -1)
		{
			CString strSingleExt = strNotCrawl.Mid(nLastestPos, nPos - nLastestPos);
			strNotCrawlList.AddTail(strSingleExt);
			nPos++;
			nLastestPos = nPos;
		}
		else
			break;
	}

	PostMessage(WM_SIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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
		m_wndStatusBar.MoveWindow(&rc);
		Resize();
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
	int nHeight = 0;
    CRect rcDlg;
	GetClientRect(rcDlg);
	
	if (m_SiteListCtrl.m_hWnd != NULL && m_ListCtrl.m_hWnd != NULL)
	{
		nHeight = (rcDlg.bottom - rcDlg.top)/2;
		
		CRect rcSplit1;
		m_wndSplitter.GetWindowRect(rcSplit1);
		ScreenToClient(rcSplit1);
		rcSplit1.top = rcDlg.top;
		rcSplit1.bottom = rcDlg.bottom - 23;
		m_wndSplitter.MoveWindow(rcSplit1, TRUE);

		CRect rcList;
		rcList.left    = rcDlg.left;
		rcList.right   = rcSplit1.left - 1;
		rcList.top     = rcDlg.top + 38;
		rcList.bottom  = nHeight;
		this->MoveDlgItem(IDC_LIST_SITE, rcList, TRUE);
		
		CRect rcBtnAppearance;
		rcBtnAppearance.left    = rcSplit1.right - 30;
		rcBtnAppearance.right   = rcSplit1.right - 5;
		rcBtnAppearance.top		= rcDlg.top + 8;
		rcBtnAppearance.bottom	= rcBtnAppearance.top + 25;
		this->MoveDlgItem(IDC_BUTTON_APPEARANCE, rcBtnAppearance, TRUE);
		
		CRect rcReList;
		rcReList.left    = rcDlg.left;
		rcReList.right   = rcSplit1.left - 1;
		rcReList.top     = rcList.bottom + 5;
		rcReList.bottom  = rcDlg.bottom - 23;
		this->MoveDlgItem(IDC_LIST_RESCANURL, rcReList, TRUE);

		CRect rcListResult;
		rcListResult.left    = rcSplit1.right + 2;
		rcListResult.right   = rcDlg.right;
		rcListResult.top     = rcDlg.top;
		rcListResult.bottom  = rcDlg.bottom - 23;
		this->MoveDlgItem(IDC_LIST_URL, rcListResult, TRUE);
		
		CRect rcTreeResult;
		rcTreeResult.left    = rcSplit1.right + 2;
		rcTreeResult.right   = rcDlg.right;
		rcTreeResult.top     = rcDlg.top;
		rcTreeResult.bottom  = rcDlg.bottom - 23;
		this->MoveDlgItem(IDC_TREE_URL, rcTreeResult, TRUE);

		//m_ListCtrlUrl.SetColumnWidth(0, (rcDlg.right - rcDlg.left)/2 - 25);
		//m_ReScanListCtrl.SetColumnWidth(0, (rcDlg.right - rcDlg.left)/2 - 25);
	}
}

void CMainDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;	//主程序结束的时候销毁
	CDialog::PostNcDestroy();
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

void CMainDlg::OnButtonAppearance() 
{
	if (m_ListCtrl.GetStyle() & WS_VISIBLE)
	{
		m_ListCtrl.ShowWindow(SW_HIDE);
		m_TreeCtrl.ShowWindow(SW_SHOW);
		m_btnAppearance.SetBitmaps(IDB_BITMAP_C, RGB(0, 255, 0));
		m_infotip.UpdateTipText(_T("列表显示"), GetDlgItem(IDC_BUTTON_APPEARANCE));
	}
	else
	{
		m_ListCtrl.ShowWindow(SW_SHOW);
		m_TreeCtrl.ShowWindow(SW_HIDE);
		m_btnAppearance.SetBitmaps(IDB_BITMAP_D, RGB(0, 255, 0));
		m_infotip.UpdateTipText(_T("树状显示"), GetDlgItem(IDC_BUTTON_APPEARANCE));
	}
	m_btnAppearance.RedrawWindow();
}

void CMainDlg::OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  Get current pos of the child controls
    CRect rcList;
    CRect rcList1;
    m_SiteListCtrl.GetWindowRect(rcList);
    m_ListCtrl.GetWindowRect(rcList1);
    ScreenToClient(rcList);
    ScreenToClient(rcList1);
    //  return the pos limit
    SPC_NM_MAXMINPOS* pNewMaxMinPos = (SPC_NM_MAXMINPOS*)pNMHDR;
    if (IDC_SPLITTER == pNMHDR->idFrom)
    {
        pNewMaxMinPos->lMin = rcList.left + 620;
        pNewMaxMinPos->lMax = rcList1.right - 5;
    }
}

void CMainDlg::OnSplitter1Delta(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  this function just want to show you how to use the delta event
    *pResult = 0;
	
    SPC_NM_DELTA* pDelta = (SPC_NM_DELTA*)pNMHDR;
    if (NULL == pDelta)
        return;
	PostMessage(WM_SIZE);
}

void CMainDlg::OnButtonScan() 
{
	UpdateData();
	try
	{
		if (!m_bScanning)
		{
			if (m_SiteListCtrl.GetItemCount() <= 0)
			{
				StatusTextOut(1, "待爬行URL列表为空，网站爬行终止！");
				return;
			}

			((CSpiderApp *)AfxGetApp())->m_IniFile.SetInt("Spider", "nMaxThreadNum", m_nMaxThreadNum);
			((CSpiderApp *)AfxGetApp())->m_IniFile.SetInt("Spider", "bSimilarUrlNotClawl", m_bSimilarUrlNotClawl);

			m_nThreadCount = 0;
			m_nCurrentThreadCount = 0;
			m_hWorkThread = ThreadTemplate::StartThread<CMainDlg, DWORD>(this, CrawlSiteThread);
			if (m_hWorkThread == NULL)
			{
				::MessageBox(NULL, "开启网站爬行线程失败！", "提示", MB_ICONINFORMATION);
				return;
			}
		}
		else
		{
			m_hTerminateWorkThread = ThreadTemplate::StartThread<CMainDlg, DWORD>(this, TerminateCrawlWorkThread);
			if (m_hTerminateWorkThread == NULL)
			{
				StatusTextOut(0, "结束网站爬行失败！");
				return;
			}
		}
	}
	catch (...){}
}

DWORD CMainDlg::CrawlSiteThread()
{
	try
	{
		//获取哪类后缀的文件不分析
		nLimitUrlLevel = m_comboLimitDir.GetCurSel();
		nLimitUrlLevel += 3;

		m_bScanning = TRUE;
		//m_GifPic.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_BUTTON_SCAN, "停止爬行");
		StatusTextOut(0, "正在爬行待检测URL...");
		StatusTextOut(1, "");
		
		int nDeleteItem[10000] = {0};//用来标志同一域名，以便加入扫描的时候把一个域名的全部项加入扫描
		//开始扫描
		while (1)
		{
			PostMessage(WM_UPDATEDATA);
			if (m_SiteListCtrl.GetItemCount() == 0 && m_ReScanURLListCtrl.GetItemCount() == 0)
				break;
			if (m_SiteListCtrl.GetItemCount() == 0)
			{
				Sleep(200);
				continue;
			}
			int nNum = 0;
			DWORD nListDataTemp = m_SiteListCtrl.GetItemData(0);
			
			for (DWORD nListCount = 0; nListCount < (DWORD)m_SiteListCtrl.GetItemCount(); nListCount++)
			{
				if (m_SiteListCtrl.GetItemData(nListCount) == nListDataTemp)
				{
					int nCurrentListCount = m_ReScanURLListCtrl.GetItemCount();
					m_ReScanURLListCtrl.InsertItem(nCurrentListCount, m_SiteListCtrl.GetItemText(nListCount, 0));
					m_ReScanURLListCtrl.SetItemText(nCurrentListCount, 1, "初始URL");
					nDeleteItem[nNum] = nListCount;
					nNum++;
				}
			}
			strMustInclude = m_SiteListCtrl.GetItemText(0, 1);
			for (int i = nNum - 1; i >= 0; i--)
			{
				m_SiteListCtrl.DeleteItem(nDeleteItem[i]);
			}
			CrawlSite();
			ShowRowNum();
		}
		m_bScanning = FALSE;
		StatusTextOut(0, "所有网站爬行完毕！");
		StatusTextOut(1, "");
		SetDlgItemText(IDC_BUTTON_SCAN, BUTTON_SCAN_1);
		MessageBeep(MB_ICONEXCLAMATION);
	}
	catch (...){}
	
	return 0;
}

DWORD CMainDlg::TerminateCrawlWorkThread()
{
	try
	{
		StatusTextOut(1, "请稍候，正在等待全部线程退出...");
		GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(FALSE);
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
		
		//扫描线程最长存在时间是：第一次连接超时时间+重连5次超时时间
		for(UINT i = 0; i < m_nThreadCount; i++)
		{
			if (m_hThread[i] != NULL)
			{
				DWORD dwExitCode;
				if(m_hThread[i] != NULL)
				{
					WaitForSingleObject(m_hThread[i], 100);
					if(GetExitCodeThread(m_hThread[i], &dwExitCode))
					{
						if(dwExitCode == STILL_ACTIVE)
							TerminateThread(m_hThread[i], dwExitCode);
					}
					m_hThread[i] = NULL;
				}
			}
		}
		m_nThreadCount = 0;
		m_bScanning = FALSE;
		SetDlgItemText(IDC_BUTTON_SCAN, BUTTON_SCAN_1);
		StatusTextOut(0, "网站爬行已停止！");
		StatusTextOut(1, "");
		GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(TRUE);
	}
	catch (...){}
	
	return 0;
}

void CMainDlg::InsertURLToTreeCtrl(CString strURL)
{
	try
	{
		CString strTempBuffer, strTemp;
		HTREEITEM hParent = NULL;
		HTREEITEM hChild = NULL;
		HTREEITEM hTempItem = NULL;
		
		int nStopPos, nStopPos1;
		int nPos = strURL.Find("://", 0);
		nPos += 3;
		nStopPos = strURL.Find("://", nPos);
		nStopPos1 = strURL.Find("?", nPos);
		nPos = strURL.Find("/", nPos);
		if (nPos == -1)
		{
			//添加类似I:\类的链接(除了“:\”没别的“\”了)
			strTempBuffer = strURL.Mid(0, strURL.GetLength());
			hTempItem = IsItemExist(TVI_ROOT, strTempBuffer);
			if (hTempItem != NULL)
			{
				hParent = hTempItem;
				m_TreeCtrl.Expand(hParent, TRUE?TVE_EXPAND:TVE_COLLAPSE);//展开/叠起结点
			}
			else
			{
				hParent = m_TreeCtrl.InsertItem(strTempBuffer, 0, 0, TVI_ROOT, TVI_LAST);
				m_TreeCtrl.SetItemData(hParent, 0);
			}
			return;
		}
		else
		{
			//添加类似"I:\file\61.93.252.221\*****"类的链接(除了“:\”还有别的“\”)
			strTempBuffer = strURL.Mid(0, nPos);
			hTempItem = IsItemExist(TVI_ROOT, strTempBuffer);
			if (hTempItem != NULL)
			{
				hParent = hTempItem;
				m_TreeCtrl.Expand(hParent, TRUE?TVE_EXPAND:TVE_COLLAPSE);//展开/叠起结点
			}
			else
			{
				if (nPos + 1 == strURL.GetLength())
				{
					return;
				}
				hParent = m_TreeCtrl.InsertItem(strTempBuffer, 0, 0, TVI_ROOT, TVI_LAST);
				m_TreeCtrl.SetItemData(hParent, 0);
			}
			nPos++;
		}
		hChild = hParent;
		int nPos3 = 0;
		while (1)
		{
			nPos3 = strURL.Find("/", nPos);
			if (nPos3 == -1 || (nStopPos1 != -1 && nPos3 > nStopPos1) || (nStopPos != -1 && nPos3 > nStopPos))
			{
				int nLastLenth = strURL.GetLength() - nPos;
				if (nLastLenth != 0)
				{
					strTemp = strURL.Mid(nPos, nLastLenth);
					if (strTemp.GetLength() == 0)
						break;
					
					CString strMail;
					strMail.Format("%s", strTemp.Mid(0, 7));
					strMail.MakeLower();
					if (strTemp.GetAt(0) == '#')
					{
						hChild = m_TreeCtrl.InsertItem(strTemp, 5, 5, hChild);
						m_TreeCtrl.SetItemData(hChild, 1);
						//ItemData == 0 -->目录；ItemData == 1 -->文件
					}
					else if (strMail.CompareNoCase("mailto:") == 0)//邮箱链接
					{
						hChild = m_TreeCtrl.InsertItem(strTemp, 6, 6, hChild);
						m_TreeCtrl.SetItemData(hChild, 1);
					}
					else if (strTemp.Find(".", 0) == -1 && strTemp.Find("?", 0) == -1)
					{
						hChild = m_TreeCtrl.InsertItem(strTemp, 7, 7, hChild);
						m_TreeCtrl.SetItemData(hChild, 1);
					}
					else
					{
						int nFirstQMarkPos = strTemp.Find("?", 0);
						if (nFirstQMarkPos != -1)
						{
							//URL包含参数
							strTempBuffer = strTemp.Mid(nFirstQMarkPos, strTemp.GetLength() - nFirstQMarkPos);
							strTemp.SetAt(nFirstQMarkPos, 0x00);
							hTempItem = IsItemExist(hChild, strTemp);
							if (hTempItem != NULL)
							{
								hChild = m_TreeCtrl.InsertItem(strTempBuffer, 9, 9, hTempItem);
							}
							else
							{
								hChild = m_TreeCtrl.InsertItem(strTemp, 10, 10, hChild);
								m_TreeCtrl.SetItemData(hChild, 1);
								hChild = m_TreeCtrl.InsertItem(strTempBuffer, 9, 9, hChild);
							}
							m_TreeCtrl.SetItemData(hChild, 1);
						}
						else
						{
							hChild = m_TreeCtrl.InsertItem(strTemp, 3, 4, hChild);
							m_TreeCtrl.SetItemData(hChild, 1);
						}
					}
				}
				break;
			}
			else if (nStopPos != -1 && nPos > nStopPos)
			{
				nPos -= 2;
				while (1)
				{
					if (strURL.GetAt(nPos) == '/')
					{
						nPos++;
						break;
					}
					nPos--;
				}
				continue;
			}
			
			strTemp = strURL.Mid(nPos, nPos3 - nPos);
			nPos3++;
			nPos = nPos3;
			hTempItem = IsItemExist(hChild, strTemp);
			if (hTempItem != NULL)
			{
				if (m_TreeCtrl.GetItemData(hTempItem) != 0)
				{
					hChild = m_TreeCtrl.InsertItem(strTemp, 1, 2, hChild, TVI_FIRST);
					m_TreeCtrl.SetItemData(hChild, 0);
				}
				else
				{
					hChild = hTempItem;
				}
			}
			else
			{
				hChild = m_TreeCtrl.InsertItem(strTemp, 1, 2, hChild, TVI_FIRST);
				m_TreeCtrl.SetItemData(hChild, 0);
			}
		}
	}
	catch (...){}
	ShowRowNum();
}

void CMainDlg::ListInsertItem(CString strTmpURL, CString strSrcPage, int nStatus)
{
	g_csSpider.Lock();
	try
	{
		if (m_bSimilarUrlNotClawl)//相似链接不爬行
		{
			CString strNoURLParameters = RemoveURLParameters(strTmpURL);
			if (FindSameItem(strNoURLParameters, 2) == -1)//没有相同项
			{
				int nCount = m_ListCtrl.GetItemCount();
				m_ListCtrl.InsertItem(nCount, strTmpURL, 0);
				m_ListCtrl.SetItemText(nCount, 1, strSrcPage);
				m_ListCtrl.SetItemText(nCount, 2, strNoURLParameters);
				m_ListCtrl.SetItemData(nCount, nStatus);
				InsertURLToTreeCtrl(strTmpURL);
			}
		}
		else
		{
			int nCount = m_ListCtrl.GetItemCount();
			m_ListCtrl.InsertItem(nCount, strTmpURL, 0);
			m_ListCtrl.SetItemText(nCount, 1, strSrcPage);
			m_ListCtrl.SetItemData(nCount, nStatus);
			InsertURLToTreeCtrl(strTmpURL);
		}
	}
	catch (...){}
	g_csSpider.Unlock();
}

//获取当前页面是在网站根目录下或者一级、二级...目录下
int CMainDlg::GetURLLevel(CString strURL)
{
	int nLevel = 0;
	int nPos = strURL.Find("?", 0);
	if (nPos != -1)
	{
		strURL.SetAt(nPos, 0x00);
		strURL.Format("%s", strURL);
	}
	int nLenth = strURL.GetLength();
	strURL.Replace("/", "");
	nLevel = nLenth - strURL.GetLength();
	return nLevel;
}

int CMainDlg::CrawlSite()
{
	ShowRowNum();
	CString strURLToScan, strCurSrcUrl;
	int nUnCrawledItem = -1;
	while (1)
	{
		try
		{
			PostMessage(WM_UPDATEDATA);
			if (m_ReScanURLListCtrl.GetItemCount() > 0)
			{
				strURLToScan = m_ReScanURLListCtrl.GetItemText(0, 0);
				strCurSrcUrl = m_ReScanURLListCtrl.GetItemText(0, 1);

				//URL第八个字母之后如果发现了“//”且非“://”就要去除掉
				strURLToScan.Replace("://", REPLACESTRING);
				strURLToScan.Replace("\\", "/");//把“\”转换成“/”
				strURLToScan.Replace("//", "/");//把“//”转换成“/”
				strURLToScan.Replace(REPLACESTRING, "://");
				strURLToScan.TrimRight(" ");
				strURLToScan.TrimRight("\t");
				
				m_ReScanURLListCtrl.DeleteItem(0);
				ShowReListRowNum();

				if (strURLToScan.GetLength() < 7)//“http://” -> 长度7
					continue;
				if (FindSameItem(strURLToScan) != -1)//判断列表里是否有重复项
					continue;
				
				//通过当前URL的后缀来判断其是否在扫描的范围内，若不需扫描，则直接添加到TreeCtrl里
				CString strURLExt = GetURLExt(strURLToScan);
				BOOL bCrawl = TRUE;
				CString strSingleExt;
				for (int nNotCrawlListCount = 0; nNotCrawlListCount < strNotCrawlList.GetCount(); nNotCrawlListCount++)
				{
					strSingleExt = strNotCrawlList.GetAt(strNotCrawlList.FindIndex(nNotCrawlListCount));
					if (strSingleExt.CompareNoCase(strURLExt) == 0)
					{
						bCrawl = FALSE;
					}
				}
				
				// 扫描时在ListCtrl插入URL
				if (!bCrawl)
				{
					ListInsertItem(strURLToScan, strCurSrcUrl, SCAN_FINISHED);
					continue;
				}
				int nURLLevel = GetURLLevel(strURLToScan);
				if (nURLLevel > nLimitUrlLevel && nLimitUrlLevel < 6)//nLimitUrlLevel大于等于6的时候，所有的url都要爬行了
				{
					ListInsertItem(strURLToScan, strCurSrcUrl, SCAN_FINISHED);
					continue;
				}
				ListInsertItem(strURLToScan, strCurSrcUrl, SCAN_UNWORK);
				if (m_ReScanURLListCtrl.GetItemCount() != 0)
					continue;
			}
			while (1)
			{
				nUnCrawledItem = GetUnCrawledItem();
				if (nUnCrawledItem == -1)//全部都是SCAN_FINISHED
				{
					if (IsCrawlFinished() && m_ReScanURLListCtrl.GetItemCount() == 0)//判断是否所有链接都已经爬行完毕
					{
						return 1;
					}
					else
					{
						Sleep(500);
						break;
					}
				}
				
				m_ListCtrl.SetItemData(nUnCrawledItem, SCAN_WORKING);
				
				strURLToScan = m_ListCtrl.GetItemText(nUnCrawledItem, 0);
				
				//线程传入参数
				ThreadParameter *lparam = new ThreadParameter;
				lparam->nCurCrawlCount = nUnCrawledItem;
				lparam->strTempURL = strURLToScan;
				lparam->pThis = this;
				//创建扫描线程
				m_hThread[m_nThreadCount++] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *))DetectURLThread, (LPVOID)lparam, 0, NULL);
				m_nCurrentThreadCount++;
				
				StatusTextOut(0, "线程数:%3d", m_nCurrentThreadCount);
				
				Sleep(100);
				while (m_nCurrentThreadCount >= m_nMaxThreadNum)
				{
					Sleep(500);
				}
			}
		}
		catch (...){}
	}

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

LRESULT CMainDlg::OnMessageInsertCrawlResult(WPARAM wParam, LPARAM lParam)
{
	CrawlParamStruct *par = (CrawlParamStruct *)lParam;
	
	int nCount = m_ReScanURLListCtrl.GetItemCount();
	m_ReScanURLListCtrl.InsertItem(nCount, par->strURL);
	m_ReScanURLListCtrl.SetItemText(nCount, 1, par->strSrcUrl);
	ShowReListRowNum();

	delete par;
	return 0;
}

BOOL CMainDlg::IsCrawlFinished()
{
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		if (m_ListCtrl.GetItemData(i) != SCAN_FINISHED)
		{
			return FALSE;
		}
	}
	return TRUE;
}

int CMainDlg::GetUnCrawledItem()
{
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		if (m_ListCtrl.GetItemData(i) == SCAN_UNWORK)
		{
			return i;
		}
	}
	return -1;
}

CString CMainDlg::GetURLExt(CString strURL)
{
	CString strTmp;
	CString strExt = "";
	try
	{
		strTmp.Format("%s", strURL);
		if (strTmp.GetAt(strTmp.GetLength() - 1) == '/')
		{
			return strExt;
		}
		int nMarkPos = strTmp.Find("?", 0);
		if (nMarkPos != -1)
		{
			strTmp.SetAt(nMarkPos, 0x00);
			strTmp.Format("%s", strTmp);
		}
		
		LPTSTR pszExtension = PathFindExtension(strTmp);
		strExt.Format("%s", pszExtension);
	}
	catch (...)
	{
		strExt = "";
	}
	return strExt;
}

int CMainDlg::FindSameItem(CString strText, int nSubItem)
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
			if (strText == m_ListCtrl.GetItemText(nCount, nSubItem))
				return nCount;
			else
				continue;
		}
		else
			break;
	}
	return -1;
}

void CMainDlg::InsertSimilarURL(int nListCount, int nListData, CString strURL, CString strTempDomain)
{
	CString strTemp;
	int nPos = strURL.Find("://", 0) + 3;
	int nPosCur = 0;
	while (1)
	{
		nPosCur = strURL.Find("/", nPos);
		if (nPosCur == -1)
		{
			break;
		}
		nPosCur++;
		nPos = nPosCur;
		strTemp.Format("%s", strURL.Mid(0, nPosCur));
		BOOL bAddToList = TRUE;
		for (int i = 0; i < m_SiteListCtrl.GetItemCount(); i++)
		{
			//if (strTemp.Compare(m_SiteListCtrl.GetItemText(i, 0)) == 0)
			if (strTemp == m_SiteListCtrl.GetItemText(i, 0))
			{
				bAddToList = FALSE;
			}	
		}
		if (bAddToList)
		{
			m_SiteListCtrl.InsertItem(nListCount, strTemp);
			m_SiteListCtrl.SetItemText(nListCount, 1, strTempDomain);
			m_SiteListCtrl.SetItemData(nListCount, nListData);
		}
	}
}

CString CMainDlg::RemoveURLParameters(CString strURL)
{
	int nPos = 0;
	strURL.TrimRight(" ");
	strURL.TrimRight("\t");
	int nLenth = strURL.GetLength();
	CString strTmpURL;
	strTmpURL.Format("%s", strURL);
	int nQMarkPos = strURL.Find("?", 0);
	if (nQMarkPos == -1)//判断是否存在问号
	{
		return strURL;
	}
	else
	{
		int nLenthChange = 0;
		int nConnectionSymbolPos = nQMarkPos;//连接符位置
		int nEqualMark = nQMarkPos;//等于号位置
		while (1)
		{
			nEqualMark = strTmpURL.Find("=", nConnectionSymbolPos);
			
			if (nEqualMark == -1)
			{
				strTmpURL.Format("%s", strTmpURL.Mid(0, nConnectionSymbolPos));
				break;
			}
			else
			{
				//找连接符
				nConnectionSymbolPos = strTmpURL.Find("&", nEqualMark);
				if (nConnectionSymbolPos == -1)//判断是否存在连接符
				{
					nConnectionSymbolPos = nEqualMark + 1;
					strTmpURL.Format("%s", strTmpURL.Mid(0, nEqualMark + 1));
					//goto END;
				}
				else
				{
					nLenthChange = strTmpURL.GetLength();
					strTmpURL.Format("%s%s", strTmpURL.Mid(0, nEqualMark + 1), strTmpURL.Mid(nConnectionSymbolPos, strTmpURL.GetLength()));
					nLenthChange -= strTmpURL.GetLength();
					nConnectionSymbolPos -= nLenthChange;
				}
			}
		}
	}
	
	return strTmpURL;
}

BOOL CMainDlg::GetUrlFile(CString url, CString& strRetBuff)
{
	if(!m_GetWeb.OnGetTheWeb(url, strRetBuff))
	{
		//AfxMessageBox(m_GetWeb.OnQueryErrer());
		//StatusTextOut(1, "%s", m_GetWeb.OnQueryErrer());
		return FALSE;
	}
	
	return TRUE;
}

unsigned __stdcall CMainDlg::DetectURLThread(LPVOID lparam)
{
	ThreadParameter *par = (ThreadParameter *)lparam;
	CMainDlg *pThis = par->pThis;
	CString strURL = par->strTempURL;
	int nCurCrawlCount = par->nCurCrawlCount;
	
	try
	{
		CString strBuffer;
		CString strTemp;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		vector<HyperLink>m_TempPageURL;
		
		CString strhtmlCode, strTempBuffer;
		if(pThis->GetUrlFile(strURL, strhtmlCode))
		{
			HyperLink m_thisURL;
			m_thisURL.str_Hyperlink= strURL;
			m_TempPageURL.empty();
			CWebHost m_host(strhtmlCode, m_TempPageURL, m_thisURL);
			for(int i = 0; i < m_TempPageURL.size(); i++)
			{
				if (!AfxParseURL((LPCTSTR)m_TempPageURL[i].str_Hyperlink, dwServiceType, strBuffer, strTemp, nPort))
					continue;
				if (dwServiceType != INTERNET_SERVICE_HTTP && dwServiceType != AFX_INET_SERVICE_HTTPS)
					continue;
				
				//必须包含指定url字符串
				strBuffer.Format("%s", m_TempPageURL[i].str_Hyperlink);
				strBuffer.MakeLower();
				if (strBuffer.Find(pThis->strMustInclude, 0) == -1)
					continue;
				strTemp.Format("%s", m_TempPageURL[i].str_Hyperlink);
				strTemp.Replace("./", "/");
				strTemp.Replace("://", ":/*/");
				strTemp.Replace("//", "/");
				strTemp.Replace(":/*/", "://");
				int nPos = strTemp.Find("#", 0);
				if (nPos != -1)
					strTemp.SetAt(nPos, 0x00);

				CrawlParamStruct *par = new CrawlParamStruct;
				par->strURL = strTemp;
				par->strSrcUrl = strURL;
				pThis->PostMessage(WM_ON_INSERT_CRAWL_RESULT, 0, (LPARAM)par);//OnMessageInsertCrawlResult
			}
		}
		pThis->m_ListCtrl.SetItemData(nCurCrawlCount, SCAN_FINISHED);
	}
	catch (...){}
	
	pThis->m_nCurrentThreadCount--;
	delete par;
	return -1;	
}

void CMainDlg::ShowRowNum()
{
	CString strColumnHeaderText;
	strColumnHeaderText.Format("%s - (%d)", COLUMN_STRING, m_SiteListCtrl.GetItemCount());
    LVCOLUMN lvCom = {0};
    char szBuffer[256] = {0};
    strcpy(szBuffer, strColumnHeaderText.GetBuffer(0));
    lvCom.mask = LVCF_TEXT;
    lvCom.cchTextMax = 256;
    lvCom.pszText = szBuffer;
    m_SiteListCtrl.SetColumn(0, &lvCom);
	strColumnHeaderText.ReleaseBuffer();
}

void CMainDlg::ShowReListRowNum()
{
	CString strColumnHeaderText;
	strColumnHeaderText.Format("%s - (%d)", COLUMN_STRING_1, m_ReScanURLListCtrl.GetItemCount());
    LVCOLUMN lvCom = {0};
    char szBuffer[256] = {0};
    strcpy(szBuffer, strColumnHeaderText.GetBuffer(0));
    lvCom.mask = LVCF_TEXT;
    lvCom.cchTextMax = 256;
    lvCom.pszText = szBuffer;
    m_ReScanURLListCtrl.SetColumn(0, &lvCom);
	strColumnHeaderText.ReleaseBuffer();
}

void CMainDlg::AddListCtrlURLs()
{
	int i = 0;
	CString strTemp;
	BOOL bAddToList = TRUE;
	for (int nListPos = 0; nListPos < strURLList.GetCount(); nListPos++)
	{
		bAddToList = TRUE;
		strTemp = strURLList.GetAt(strURLList.FindIndex(nListPos));
		
		CString strNoParametersURL = RemoveURLParameters(strTemp);
		for (i = 0; i < m_SiteListCtrl.GetItemCount(); i++)
		{
			//if (strNoParametersURL.Compare(m_SiteListCtrl.GetItemText(i, 2)) == 0)
			if (strNoParametersURL == m_SiteListCtrl.GetItemText(i, 2))
			{
				bAddToList = FALSE;
				break;
			}
		}
		if (bAddToList)
		{
			CString strTempDomain = strTemp;
			strTempDomain.MakeLower();
			strTempDomain.Replace("http://", "");
			strTempDomain.Replace("https://", "");
			
			int nPos1 = strTempDomain.Find("/", 0);
			if (nPos1 != -1)
			{
				strTempDomain = strTempDomain.Mid(0, nPos1);
			}
			else if (nPos1 == -1)
			{
				//m_Url = m_Url;
			}
			else
			{
				StatusTextOut(0, "URL格式错误！");
				continue;
			}
			strTempDomain.TrimLeft(" ");
			strTempDomain.TrimLeft("\t");
			strTempDomain.TrimRight(" ");
			strTempDomain.TrimRight("\t");
			strTempDomain.Insert(0, "://");
			strTempDomain.Insert(strTempDomain.GetLength(), "/");
			
			int nListCount = m_SiteListCtrl.GetItemCount();
			m_SiteListCtrl.InsertItem(nListCount, strTemp);
			m_SiteListCtrl.SetItemData(nListCount, nListData);
			m_SiteListCtrl.SetItemText(nListCount, 1, strTempDomain);
			m_SiteListCtrl.SetItemText(nListCount, 2, strNoParametersURL);
			InsertSimilarURL(nListCount, nListData, strTemp, strTempDomain);
			nListData++;
		}
	}
}

void CMainDlg::OnRclickListSite(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_SPIDER_TOP);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	int	count = pSubMenu->GetMenuItemCount();
	if (m_SiteListCtrl.GetSelectedCount() == 0)
	{
		for (int i = count - 1; i >= 0; i--)
		{
			if (i == count - 1 || i == count - 2)
			{
				continue;
			}
			pSubMenu->DeleteMenu(i, MF_BYPOSITION);
		}
	}
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	
	*pResult = 0;
}

void CMainDlg::OnRclickListRescanurl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_SPIDER_BOTTOM);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	int	count = pSubMenu->GetMenuItemCount();
	if (m_ReScanURLListCtrl.GetSelectedCount() == 0 || m_bScanning)
	{
		for (int i = 0; i < count; i++)
		{
			pSubMenu->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	
	*pResult = 0;
}

void CMainDlg::OnRclickTreeUrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_SPIDER_RIGHT_TREE);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	
	int	count = pSubMenu->GetMenuItemCount();
	if (m_TreeCtrl.GetCount() == 0)
	{
		for (int i = 0; i < count; i++)
		{
			if (i == count - 4)
			{
				continue;
			}
			pSubMenu->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}
	
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CMainDlg::OnRclickListUrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_SPIDER_RIGHT_LIST);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	
	int	count = pSubMenu->GetMenuItemCount();
	if (m_ListCtrl.GetItemCount() < 1)
	{
		for (int i = 0; i < count; i++)
		{
			if (i == count - 1)
			{
				continue;
			}
			pSubMenu->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}
	
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CMainDlg::OnMenuitemCopy() 
{
    int nItem;
    CString strText(_T(""));
    POSITION pos = m_SiteListCtrl.GetFirstSelectedItemPosition();
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_SiteListCtrl.GetNextSelectedItem(pos);
        strText += m_SiteListCtrl.GetItemText(nItem, 0) + _T("\r\n");
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
	// TODO: Add your command handler code here
	for (int i = m_SiteListCtrl.GetItemCount() - 1; i >= 0; i--)
	{
		if (m_SiteListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			m_SiteListCtrl.DeleteItem(i);
	}
	ShowRowNum();
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

void CMainDlg::OnMenuitemAddurl() 
{
	try
	{
		CInputDialog	dlg;
		dlg.Init(_T("添加URL"), _T("添加要爬行的URL到列表:"), this);
		dlg.m_str = GetKeyboardText();
		if (dlg.DoModal() != IDOK)   
			return;
		
		CString strUrl = dlg.m_str;
		strUrl.TrimLeft(" ");
		strUrl.TrimLeft("\t");
		strUrl.TrimRight(" ");
		strUrl.TrimRight("\t");
		
		if (strUrl.GetLength() < 1)
		{
			::MessageBox(NULL, "URL不能为空！", "提示", MB_ICONINFORMATION);
			return;
		}
		//URL开头不能有"/"；URL里至少有一个"."；URL里不能出现"///"；在URL的除了开头的"http://"里有"//",后面就不能再有"//"了；输入的URL里不能出现反斜杠
		if (strUrl.GetAt(0) == '//' || strUrl.Find(".", 0) == -1 || strUrl.Find("///", 0) != -1 || strUrl.Find("\\", 0) != -1)
		{
			::MessageBox(NULL, "URL格式错误！", "提示", MB_ICONINFORMATION);
			return;
		}
		CString strTemp = strUrl.Mid(0, 8);
		if (strTemp.Find("://", 0) == -1 || strTemp.Find("http", 0) == -1)//找到了"://",但没找到"http",URL也是不合法的
		{
			::MessageBox(NULL, "URL格式错误！", "提示", MB_ICONINFORMATION);
			return;
		}
		
		strURLList.RemoveAll();
		strURLList.AddTail(strUrl);
		AddListCtrlURLs();
		ShowRowNum();
	}
	catch (...){}
}

void CMainDlg::OnMenuitemImport() 
{
    CFileDialog dlg(TRUE, _T(""), "", OFN_OVERWRITEPROMPT, _T("*.txt|*.txt||"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
	
	try
	{
		strURLList.RemoveAll();
		CStdioFileEx file;
		CString strBuffer;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		while(file.ReadString(strBuffer) > 0)
		{
			if(strBuffer.GetLength() > 0)
			{
				strBuffer.Format("%s", strBuffer);
				if (strBuffer.Mid(0, 4).CompareNoCase("http") != 0)
					continue;
				
				//可以用“#”号在字典文件中作为注释字符用
				int nPos = strBuffer.Find('#');
				if(nPos != -1)
					strBuffer.SetAt(nPos, 0x00);
				strBuffer.TrimRight(" ");
				strBuffer.TrimRight("\t");
				strBuffer.Format("%s", strBuffer);
				strURLList.AddTail(strBuffer);
			}
		}
		file.Close();
		
		AddListCtrlURLs();
		ShowRowNum();
		
		::MessageBox(0, "数据导入完成！", "提示", MB_ICONINFORMATION);
	}
	catch (...)
	{
		::MessageBox(0, "数据导入出错！", "提示", MB_ICONINFORMATION);
	}
}

LRESULT CMainDlg::OnMessageUpdateData(WPARAM wParam, LPARAM lParam)
{
	UpdateData();
	return 0;
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE)//不加这个调试会崩溃
		m_infotip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CMainDlg::OnMenuitemDeleteallurls() 
{
	m_ReScanURLListCtrl.DeleteAllItems();
	ShowReListRowNum();
}

void CMainDlg::OnMenuitemRightCopy() 
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

void CMainDlg::OnMenuitemRightCopysrcpage() 
{
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

void CMainDlg::OnMenuitemRightDelsel() 
{
	StatusTextOut(0, "请稍等，正在删除选定链接...");
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
	
	m_TreeCtrl.DeleteAllItems();
	int nCount = m_ListCtrl.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		InsertURLToTreeCtrl(m_ListCtrl.GetItemText(i, 0));
	}
	StatusTextOut(0, "删除选定链接完毕,TREE视图URL同步更新完毕！");
	ShowRowNum();
}

void CMainDlg::OnMenuitemRightClearall() 
{
	if (MessageBox("确认要清空列表吗?", "提示", MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	m_ListCtrl.DeleteAllItems();
	m_TreeCtrl.DeleteAllItems();
	StatusTextOut(0, "");
	ShowRowNum();
}

void CMainDlg::OnMenuitemRightSaveallurls() 
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString strTime = time.Format("%Y-%m-%d %H-%M-%S");
	
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

void CMainDlg::OnMenuitemRightImportfile() 
{
	OnMenuitemTreeImportUrls();
}

void CMainDlg::OnMenuitemRightVisit() 
{
	try
	{
		ShellExecute(0, "open", strCompleteURL, NULL, NULL, SW_SHOW);
	}
	catch (...){}
}

void CMainDlg::OnMenuitemRightTreeCopy() 
{
    //将内容保存到剪贴板。
    if (!strCompleteURL.IsEmpty())
    {
        if (OpenClipboard())
        {
            EmptyClipboard();
            HGLOBAL hClipboardData = GlobalAlloc(GHND | GMEM_SHARE, (strCompleteURL.GetLength() + 1) * sizeof(TCHAR));
            if (hClipboardData)
            {
                TCHAR* pszData = (TCHAR *)GlobalLock(hClipboardData);
                _tcscpy(pszData, strCompleteURL);
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_TEXT, hClipboardData);
            }
            CloseClipboard();
        }
    }
}

void CMainDlg::OnMenuitemTreeClear() 
{
	if (MessageBox("确认要清空列表吗?", "提示", MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	m_TreeCtrl.DeleteAllItems();
	m_ListCtrl.DeleteAllItems();
	ShowRowNum();
}

void CMainDlg::OnMenuitemGotoSrcpage() 
{
	try
	{
		CString strTmp = _T("");
		strCompleteURL.Empty();
		HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
		HTREEITEM m_hTreeItemRoot = m_TreeCtrl.GetRootItem();
		int nCount = 0;//防止卡死
		while (hItem != m_hTreeItemRoot && hItem != NULL && nCount < 1000)
		{
			strTmp = m_TreeCtrl.GetItemText(hItem);
			if (m_TreeCtrl.GetItemData(hItem) == 0)
			{
				if (strTmp.Right(1) != _T("/"))
					strTmp += _T("/");
			}
			strCompleteURL = strTmp + strCompleteURL;
			
			hItem = m_TreeCtrl.GetParentItem(hItem);
			nCount++;
		}
		strTmp = m_TreeCtrl.GetItemText(hItem);
		if (m_TreeCtrl.GetItemData(hItem) == 0)
		{
			if (strTmp.Right(1) != _T("/"))
				strTmp += _T("/");
		}
		strCompleteURL = strTmp + strCompleteURL;
		
		m_ListCtrl.ShowWindow(SW_SHOW);
		m_TreeCtrl.ShowWindow(SW_HIDE);
		m_btnAppearance.SetBitmaps(IDB_BITMAP_C, RGB(0, 255, 0));

		int i = 0;
		for (i = 0; i < m_ListCtrl.GetItemCount(); i++)
		{
			m_ListCtrl.SetItemTextColor(i, RGB(0, 0, 0));
		}
		for (i = 0; i < m_ListCtrl.GetItemCount(); i++)
		{
			strTmp = m_ListCtrl.GetItemText(i, 0);
			if (strTmp == strCompleteURL)
			{
				m_ListCtrl.EnsureVisible(i, TRUE);
				m_ListCtrl.SetItemTextColor(i, RGB(0, 200, 0));
				break;
			}
		}
	}
	catch (...){}
}

void CMainDlg::OnMenuitemTreeSaveallurlstofile() 
{
	OnMenuitemRightSaveallurls();
}

void CMainDlg::OnMenuitemTreeImportUrls() 
{
    CFileDialog dlg(TRUE, _T(""), "", OFN_OVERWRITEPROMPT, _T("*.txt|*.txt||"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
	
	try
	{
		CStdioFile file;
		CString strBuffer;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		while(file.ReadString(strBuffer))
		{
			if(strBuffer.GetLength() > 0)
			{
				strBuffer.Format("%s", strBuffer);
				if (strBuffer.Mid(0, 4).CompareNoCase("http") != 0)
					continue;
				
				strBuffer.Replace("://", "$^@");
				strBuffer.Replace("//", "/");
				while (strBuffer.Find("//", 0) != -1)
				{
					strBuffer.Replace("//", "/");
				}
				strBuffer.Replace("$^@", "://");
				
				strBuffer.TrimRight(" ");
				strBuffer.TrimRight("\t");
				strBuffer.Format("%s", strBuffer);
				
				BOOL bAddToList = TRUE;
				for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
				{
					CString strTemp = m_ListCtrl.GetItemText(i, 0);
					//if (strBuffer.Compare(strTemp) == 0)
					if (strBuffer == strTemp)
					{
						bAddToList = FALSE;
						break;
					}
				}
				if (bAddToList)
				{
					int nCount = m_ListCtrl.GetItemCount();
					m_ListCtrl.InsertItem(nCount, strBuffer, 8);
					m_ListCtrl.SetItemText(nCount, 1, "外部导入");
					InsertURLToTreeCtrl(strBuffer);
				}
			}
		}
		file.Close();
		
		try
		{
			MyExpandTree(TVI_ROOT, TVE_COLLAPSE);
		}
		catch (...){}
		
		ShowRowNum();
		::MessageBox(0, "数据导入完成！", "提示", MB_ICONINFORMATION);
	}
	catch (...)
	{
		::MessageBox(0, "数据导入出错！", "提示", MB_ICONINFORMATION);
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
	try
	{
		MyExpandTree(TVI_ROOT, TVE_EXPAND);
	}
	catch (...){}
}

void CMainDlg::OnMenuitemCollapse() 
{
	try
	{
		MyExpandTree(TVI_ROOT, TVE_COLLAPSE);
	}
	catch (...){}
}

void CMainDlg::OnSelchangedTreeUrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	try
	{
		CString strTmp = _T("");
		strCompleteURL.Empty();
		HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
		HTREEITEM m_hTreeItemRoot = m_TreeCtrl.GetRootItem();
		int nCount = 0;//防止卡死
		while (hItem != m_hTreeItemRoot && hItem != NULL && nCount < 1000)
		{
			strTmp = m_TreeCtrl.GetItemText(hItem);
			if (m_TreeCtrl.GetItemData(hItem) == 0)
			{
				if (strTmp.Right(1) != _T("/"))
					strTmp += _T("/");
			}
			strCompleteURL = strTmp + strCompleteURL;
			if (hItem == TVI_ROOT)
				break;
			hItem = m_TreeCtrl.GetParentItem(hItem);
			nCount++;
		}
		strTmp = m_TreeCtrl.GetItemText(hItem);

		if (hItem != NULL)
		{
			if (m_TreeCtrl.GetItemData(hItem) == 0)
			{
				if (strTmp.Right(1) != _T("/"))
					strTmp += _T("/");
			}
		}

		strCompleteURL = strTmp + strCompleteURL;
		StatusTextOut(1, "%s", strCompleteURL);
	}
	catch (...){}

	*pResult = 0;
}

void CMainDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	UINT count;
	char filePath[MAX_PATH];
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{
		int pathLen = DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));

		try
		{
			strURLList.RemoveAll();
			CStdioFileEx file;
			CString strBuffer;
			file.Open(filePath, CFile::modeRead);
			while(file.ReadString(strBuffer) > 0)
			{
				if(strBuffer.GetLength() > 0)
				{
					strBuffer.Format("%s", strBuffer);
					if (strBuffer.Mid(0, 4).CompareNoCase("http") != 0)
						continue;
					
					//可以用“#”号在字典文件中作为注释字符用
					int nPos = strBuffer.Find('#');
					if(nPos != -1)
						strBuffer.SetAt(nPos, 0x00);
					strBuffer.TrimRight(" ");
					strBuffer.TrimRight("\t");
					strBuffer.Format("%s", strBuffer);
					strURLList.AddTail(strBuffer);
				}
			}
			file.Close();
			
			AddListCtrlURLs();
			ShowRowNum();
			
			::MessageBox(0, "数据导入完成！", "提示", MB_ICONINFORMATION);
		}
		catch (...)
		{
			::MessageBox(0, "数据导入出错！", "提示", MB_ICONINFORMATION);
		}
	}
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

void CMainDlg::OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	CString str = pDispInfo->item.pszText;
	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;
	// Construct and create the custom multiline edit control.
	// We could just as well have used a combobox, checkbox, 
	// rich text control, etc.
	m_pListEdit = new CInPlaceEdit( item, subitem, str );
	// Start with a small rectangle.  We'll change it later.
	CRect  rect( 0,0,1,1 );
	DWORD dwStyle = ES_LEFT;
	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL;
	m_pListEdit->Create( dwStyle, rect, &m_SiteListCtrl, 103 );
	// Have the Grid position and size the custom edit control
	m_SiteListCtrl.PositionControl( m_pListEdit, item, subitem );
	// Have the edit box size itself to its content.
	m_pListEdit->CalculateSize();
	// Return TRUE so that the list control will hnadle NOT edit label itself. 
	*pResult = 1;
}

void CMainDlg::OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;
	// This is coming from the grid list control notification.
	if( m_pListEdit )
	{
		CString str;
		if( pDispInfo->item.pszText )
			m_SiteListCtrl.SetItemText(item, subitem, pDispInfo->item.pszText);
		delete m_pListEdit;
		m_pListEdit = 0;
	}
	*pResult = 0;
}

void CMainDlg::OnMenuitemClearTopList() 
{
	// TODO: Add your command handler code here
	m_SiteListCtrl.DeleteAllItems();
	ShowRowNum();
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 23;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
