// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRobot.h"
#include "MainDlg.h"
#include "WebRobotDlg.h"
#include "PluginDlg.h"
#include "BrowserDlg.h"
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TREECTRL_ABOUTME		_T("关于程序")
#define TREECTRL_GETURLS		_T("抓取链接")
#define TREECTRL_FUNCTION		_T("功能插件")
#define TREECTRL_SETTING		_T("程序设置")

char m_strPluginPath[MAX_PATH] = {0};
CMainDlg* g_pMainDlg = NULL;
extern CWebRobotDlg* g_pWebRobotDlg;

typedef struct
{
	TCHAR	*pName;
	TCHAR	*pDescription;
	TCHAR	*pAbout;
	TCHAR	*pVersion;
}DLLINFO;

typedef	  char* (*GetInfo)();
extern PreTM m_PreTM;
CString strAboutWebRobot;
CString strGetUrlsText;
CString strSettingText;
/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog
IMPLEMENT_DYNCREATE(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	((CWebRobotApp *)AfxGetApp())->m_pMainDlg = this;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_STATIC_TEXT, m_static);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_NOTIFY(NM_CLICK, IDC_TREE, OnClickTree)
	ON_COMMAND(ID_MENUITEM_AUTOLOAD, OnMenuitemAutoload)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_COMMAND(ID_MENUITEM_OPEN_PLUGIN_DIR, OnMenuitemOpenPluginDir)
	ON_COMMAND(ID_MENUITEM_REFRESH_PLUGIN_LIST, OnMenuitemRefreshPluginList)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, OnItemexpandingTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTER, OnMaxMinInfo)
	ON_NOTIFY(SPN_DELTA,     IDC_SPLITTER, OnSplitter1Delta)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LIST_DROP_END, OnMessageShowDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

//判断文件是否存在
BOOL FileExists(LPCTSTR lpszFileName)
{
	//得到文件属性
	DWORD dwAttributes=GetFileAttributes(lpszFileName);
	//函数调用成功则文件存在
	if(dwAttributes==0xffffffff)
		return false;
	//否则文件不存在
	else
		return true;
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
	g_pMainDlg = (CMainDlg *)((CWebRobotApp *)AfxGetApp())->m_pMainDlg;
	// TODO: Add extra initialization here
	m_ImageList.Create(16, 16,ILC_COLOR32,2,2); 
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_ABOUT));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_BROWSER));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_PLUGIN));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SETTING));
	::SendMessage(m_TreeCtrl.m_hWnd, TVM_SETITEMHEIGHT, 23, 0);
	
	m_ImageList1.Create(24, 24, ILC_COLORDDB | ILC_MASK, 0, 1);
	LoadMyBitmap(&m_ImageList1, IDB_BITMAP_PLUGIN);
	m_ListCtrl.InsertColumn(0, _T("插件名称"), LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn(1, _T("说明(插件支持拖拽排序)"), LVCFMT_LEFT, 350);
	m_ListCtrl.InsertColumn(2, _T("加载"), LVCFMT_LEFT, 50);
	m_ListCtrl.InsertColumn(3, _T("版本"), LVCFMT_LEFT, 70);
	m_ListCtrl.InsertColumn(4, _T("文件名"), LVCFMT_LEFT, 120);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.SetImageList(&m_ImageList1,LVSIL_SMALL);

	strAboutWebRobot = "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"app:WM_APP_COMMAND1\"><font size=\"+13\" color=\"green\">WebRobot</font></a><br><br><br>";
	strAboutWebRobot += "&nbsp;&nbsp;&nbsp;&nbsp;本程序是一款插件模式的网络安全检测程序，主要功能由插件来实现。插件存放在主程序目录下的plugin文件夹里，所有插件的<br>&nbsp;配置文件及字典等其他文件也存放在这个文件夹内。<br><br>";
	strAboutWebRobot += "&nbsp;&nbsp;&nbsp;&nbsp;免责声明：WebRobot仅用于检测Web安全性能，请勿用作非法用途！非法使用本工具所带来的直接或间接损失与本程序开发者无<br>&nbsp;关。从您使用本工具的一刻起，将视为您已经接受本免责声明。<br><br>";
	strAboutWebRobot += "&nbsp;&nbsp;&nbsp;&nbsp;在您使用本程序的过程中遇到任何问题或有什么好的意见建议，欢迎骚扰。<br><br><br>";
	strAboutWebRobot += "&nbsp;BY:洪流&nbsp;&nbsp;<a href=\"http://wpa.qq.com/msgrd?v=3&uin=243107&site=qq&menu=yes\">QQ:243107</a>";
	strAboutWebRobot += "<br><br>&nbsp;<a href=\"https://www.t00ls.net/index.php?from=WebRobot\"><font color=\"blue\">T00ls论坛</font></a>";
	strAboutWebRobot += "<br><br>&nbsp;<a href=\"http://webrobot.blog.51cto.com/\"><font color=\"blue\">My Blog</font></a><br>";
	m_static.SetWindowText(strAboutWebRobot);

	strGetUrlsText	= _T("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"green\">抓取链接</font><br><br><br>&nbsp;&nbsp;&nbsp;\
						 主程序自带插件。<br><br>&nbsp;&nbsp;&nbsp;&nbsp;先开启读取网页链接功能，然后用该插件自带的浏览器打开网页，就可以抓取该页面的所有链接。<br><br>&nbsp;&nbsp;&nbsp;&nbsp;可用该功能抓取搜索结果或者网站链接。");

	strSettingText	= _T("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"green\">程序设置</font><br><br><br>&nbsp;&nbsp;&nbsp;\
						 程序基本设置。");

	GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
	//Splitter
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_SPLITTER);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER, SPS_VERTICAL|SPS_DELTA_NOTIFY);
    //  register windows for splitter
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_LEFT,     GetDlgItem(IDC_TREE));
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_RIGHT,    GetDlgItem(IDC_LIST));
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_RIGHT,    GetDlgItem(IDC_STATIC_TEXT));
    m_wndSplitter.Relayout();

	GetModuleFileName(NULL, m_strPluginPath, sizeof(m_strPluginPath));
	PathRemoveFileSpec(m_strPluginPath);
	lstrcat(m_strPluginPath, "\\plugin\\");
	LoadPlugins(m_strPluginPath);

	PostMessage(WM_SIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (IsWindow(m_wndSplitter.GetSafeHwnd()))
	{
		LockWindowUpdate();
		if (m_ListCtrl.m_hWnd != NULL)
			Resize();
		UnlockWindowUpdate();
	}
}

void CMainDlg::MoveDlgItem(int nID, const CRect& rcPos, BOOL bRepaint)
{
    CWnd* pWnd = GetDlgItem(nID);
    if (NULL == pWnd)
        return;
    pWnd->MoveWindow(rcPos, bRepaint);
    CRect rcsp;
    m_wndSplitter.GetWindowRect(rcsp);
    ScreenToClient(rcsp);
}

void CMainDlg::Resize()
{
    CRect rcDlg;
    GetClientRect(rcDlg);
	
    CRect rcSplit;
    m_wndSplitter.GetWindowRect(rcSplit);
    ScreenToClient(rcSplit);
	rcSplit.top = rcDlg.top + 1;
    rcSplit.bottom = rcDlg.bottom;
    m_wndSplitter.MoveWindow(rcSplit, TRUE);
	
    CRect rcTreeCtrl;
    rcTreeCtrl.left    = rcDlg.left;
    rcTreeCtrl.right   = rcSplit.left - 1;
    rcTreeCtrl.top     = rcDlg.top + 1;
    rcTreeCtrl.bottom  = rcDlg.bottom;
    MoveDlgItem(IDC_TREE, rcTreeCtrl, TRUE);
	
	CRect rcList;
	rcList.left     = rcSplit.right + 1;
	rcList.right    = rcDlg.right;
	rcList.top      = rcDlg.top + 1;
	rcList.bottom   = rcDlg.bottom;
	MoveDlgItem(IDC_LIST, rcList, TRUE);

	CRect rcText;
	rcText.left     = rcSplit.right + 1;
	rcText.right    = rcDlg.right;
	rcText.top      = rcDlg.top + 1;
	rcText.bottom   = rcDlg.bottom;
	MoveDlgItem(IDC_STATIC_TEXT, rcText, TRUE);
	m_static.Reset();

	UpdateWindow();
}

int nIcon = 3;//插件图标开始的地方，跟下面的nTreeIcon = 2;是一个值
void CMainDlg::InsertInfoToTreeCtrl(CString strName, int nData)														//插入到TreeCtrl
{
	HTREEITEM hRoot = NULL;
	HTREEITEM hTempItem = NULL;
	if (strName == TREECTRL_ABOUTME || strName == TREECTRL_FUNCTION || strName == TREECTRL_SETTING)
	{
		hRoot = IsItemExist(TVI_ROOT, strName);//主分支
		if (hRoot == NULL)
		{
			int nTreeIcon = 0;
			if (strName == TREECTRL_ABOUTME)
				nTreeIcon = 0;
			else if (strName == TREECTRL_FUNCTION)
				nTreeIcon = 2;
			else
				nTreeIcon = 3;

			hRoot = m_TreeCtrl.InsertItem(strName, nTreeIcon, nTreeIcon, TVI_ROOT, TVI_LAST);//主分支
			m_TreeCtrl.SetItemData(hRoot, nData);
		}
	}
	else
	{
		hRoot = IsItemExist(TVI_ROOT, TREECTRL_FUNCTION);//主分支
		if (hRoot != NULL)
		{
			hTempItem = IsItemExist(hRoot, strName);
			if (hTempItem == NULL)
			{
				if (strName == TREECTRL_GETURLS)
					hTempItem = m_TreeCtrl.InsertItem(strName, 1, 1, hRoot, TVI_LAST);
				else
					hTempItem = m_TreeCtrl.InsertItem(strName, nIcon, nIcon, hRoot, TVI_LAST);

				if (hTempItem != NULL)
					m_TreeCtrl.SetItemData(hTempItem, nData);//插件
			}
			m_TreeCtrl.Expand(hRoot, TVE_EXPAND);//展开结点
		}
	}
}

void CMainDlg::InsertInfoToListCtrl(CString strName, CString strDescription, CString strVersion, CString strFileName, CString strPath, CString strCrc32)		//插入到ListCtrl
{
	nIcon++;
	int nCount = m_ListCtrl.GetItemCount();
	long nData = strtol(strCrc32, NULL, 16);

	m_ListCtrl.InsertItem(nCount, strName, 0);
	m_ListCtrl.SetItemText(nCount, 1, strDescription);

	if (GetFileAttributes(strPath) & FILE_ATTRIBUTE_READONLY)
		m_ListCtrl.SetItemText(nCount, 2, "×");
	else
	{
		m_ListCtrl.SetItemText(nCount, 2, "是");
		InsertInfoToTreeCtrl(strName, nData);
	}

	m_ListCtrl.SetItemText(nCount, 3, strVersion);
	m_ListCtrl.SetItemText(nCount, 4, strFileName);

	m_ListCtrl.SetItemData(nCount, nData);
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

CString CMainDlg::GetPluginPath(CString strName)
{
	CString strPath;
	strPath.Format("%s%s", m_strPluginPath, strName);
	return strPath;
}

void CMainDlg::LoadPlugins(LPCTSTR lpszPath)
{
	m_TreeCtrl.DeleteAllItems();
	m_ListCtrl.DeleteAllItems();
	InsertInfoToTreeCtrl(TREECTRL_ABOUTME, -1);
	InsertInfoToTreeCtrl(TREECTRL_FUNCTION, -2);
	InsertInfoToTreeCtrl(TREECTRL_GETURLS, -3);
	CStringArray sArrayPluginName;
	BOOL bExistSortInfo = TRUE;
	if (((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("Config", "PluginSort", "") == "")
		bExistSortInfo = FALSE;

	TCHAR szFind[MAX_PATH];
	lstrcpy(szFind, lpszPath);
	lstrcat(szFind, _T("*.*")); // 找所有文件
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szFind, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
		return;
	do  
	{
		if (wfd.cFileName[0] == '.')
			continue; // 过滤这两个目录
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){}else
		{
			TCHAR szFile[MAX_PATH];
			wsprintf(szFile, _T("%s%s"), lpszPath, wfd.cFileName);
			LPTSTR pszExtension = PathFindExtension(szFile);
			if (stricmp(pszExtension, _T(".dll")) == 0)
			{
				if (!bExistSortInfo)//没有发现排序信息，直接按默认的来
				{
					InsertPluginInfo(szFile, wfd.cFileName);
				}
				else
				{
					sArrayPluginName.Add(wfd.cFileName);
				}
			}
		}
	}while (FindNextFile(hFind, &wfd));  
	FindClose(hFind);

	if (bExistSortInfo)//发现排序信息，按信息上的排序来
		InsertPluginInfoEx(sArrayPluginName);
	
	InsertInfoToTreeCtrl(TREECTRL_SETTING, -4);
	m_TreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);
}

void CMainDlg::InsertPluginInfo(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	CString strName, strDescription, strVersion;
	HMODULE hModule = LoadLibrary(lpszPath);
	if (!hModule)
		return;

	GetInfo info = (GetInfo)GetProcAddress(hModule, "GetInfo");
	DLLINFO *pInfo = (DLLINFO*)info();
	strName = pInfo->pName;
	strDescription = pInfo->pDescription;
	strVersion = pInfo->pVersion;
	FreeLibrary(hModule);
	if (pInfo)
		HeapFree(GetProcessHeap(), 0, pInfo);//释放dll里用GlobalAlloc申请的内存

	HICON hicon = ExtractIcon(AfxGetInstanceHandle(), lpszPath, 0);
	m_ImageList.Add(hicon); 

	FileCRC32		m_CRC32;
	m_CRC32.Open(lpszPath);
	CString strCRC32  = m_CRC32.GetFileCRCString();
	m_CRC32.Close();

	strCRC32 = strCRC32.Left(5);

	InsertInfoToListCtrl(strName, strDescription, strVersion, lpszFileName, lpszPath, strCRC32);
}

void CMainDlg::InsertPluginInfoEx(CStringArray &sArray)
{
	CString strName, strPath;
	CStringArray sArrayPlugin, sArrayNewPlugin;
	CString strInfo = ((CWebRobotApp *)AfxGetApp())->m_IniFile.GetString("Config", "PluginSort", "");
	int nPos = 0;
	while (1)
	{
		int nPos1 = strInfo.Find("|", nPos);
		if (nPos1 == -1)
			break;
		strName = strInfo.Mid(nPos, nPos1 - nPos);
		if (strName.GetLength() != 0)
			sArrayPlugin.Add(strName);
		nPos = nPos1 + 1;
	}

	//获取新添加的插件
	BOOL bFind = FALSE;
	for (int i = 0; i < sArray.GetSize(); i++)
	{
		bFind = FALSE;
		strName = sArray.GetAt(i);
		for (int j = 0; j < sArrayPlugin.GetSize(); j++)
		{
			if (strName == sArrayPlugin.GetAt(j))
			{
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)//是新添加进来的插件
			sArrayNewPlugin.Add(strName);
	}
	//插入按顺序排序的插件
	for (i = 0; i < sArrayPlugin.GetSize(); i++)
	{
		strName = sArrayPlugin.GetAt(i);
		strPath = GetPluginPath(strName);
		if (FileExists(strPath))
		{
			InsertPluginInfo(strPath, strName);
		}
	}
	//最后插入新添加进来的插件
	for (i = 0; i < sArrayNewPlugin.GetSize(); i++)
	{
		strName = sArrayNewPlugin.GetAt(i);
		InsertPluginInfo(GetPluginPath(strName), strName);
	}
}

void CMainDlg::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hTreeSel = m_TreeCtrl.GetSelectedItem();
	if (hTreeSel == NULL)
		return;

	DWORD dwData = m_TreeCtrl.GetItemData(hTreeSel);
	if (dwData <= 0)//功能插件/未知命令
	{
		return;
	}
	if (dwData == -3)//抓取链接
	{
		CCoolTabCtrl::CPageItem* pItem = g_pWebRobotDlg->m_TabCtrl.AddPage(RUNTIME_CLASS(CBrowserDlg), IDD_BROWSER_DLG, " 抓取链接 ", IDI_ICON_BROWSER);
		g_pWebRobotDlg->m_TabCtrl.SetActivePage(g_pWebRobotDlg->m_TabCtrl.GetPageCount() - 1);
	}
	if (dwData == -4)//程序设置
	{
		CCoolTabCtrl::CPageItem* pItem = g_pWebRobotDlg->m_TabCtrl.AddPage(RUNTIME_CLASS(CSettingDlg), IDD_SETTING_DLG, " 程序设置 ", IDI_ICON_SETTING);
		g_pWebRobotDlg->m_TabCtrl.SetActivePage(g_pWebRobotDlg->m_TabCtrl.GetPageCount() - 1);
	}


	CString strPath;
	int nItem = -1;
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		if (dwData == m_ListCtrl.GetItemData(i))
		{
			nItem = i;
			break;
		}
	}
	if (nItem == -1)
		return;

	strPath.Format("%s\\%s", m_strPluginPath, m_ListCtrl.GetItemText(i, 4));
	strPath.Replace("\\\\", "\\");
	LoadPlugin(m_ListCtrl.GetItemText(i, 0), strPath);

	*pResult = 0;
}

void CMainDlg::LoadCustomPlugin(CString strName, CString strTempTxtPath)
{
	CString strPluginPath;
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		if (m_ListCtrl.GetItemText(i, 0) == strName)
		{
			strPluginPath.Format("%s\\%s", m_strPluginPath, m_ListCtrl.GetItemText(i, 4));
			strPluginPath.Replace("\\\\", "\\");
			break;
		}
	}
	
	if (FileExists(strPluginPath))
	{
		LoadPlugin("注入检测", strPluginPath, strTempTxtPath);
	}
}

void CMainDlg::LoadPlugin(CString strName, CString strPath, CString strTempTxtPath)
{
	BOOL bLoadTmpTxt = FALSE;
	if (strTempTxtPath != "")
		bLoadTmpTxt = TRUE;

	//判断插件是否打开过
	strName += " ";
	strName.Insert(0, " ");

	HINSTANCE hModule = NULL;
	typedef CDialog* (*lpFun)(CWnd *p);
	hModule = LoadLibrary(strPath);
	if (NULL == hModule)
	{
		AfxMessageBox(_T("dll加载失败"));
		return ;
	}
	PLoadTempTxt pLoadTxt = NULL;
	lpFun pShowDlg = (lpFun)GetProcAddress(hModule, "ShowDlg");
	m_PreTM = (PreTM)GetProcAddress(hModule, "PreTranslateMessage");
	if (bLoadTmpTxt)
	{
		pLoadTxt = (PLoadTempTxt)GetProcAddress(hModule, "LoadTempTxt");
		if (NULL == pLoadTxt)
		{
			AfxMessageBox(_T("dll中函数LoadTempTxt寻找失败"));
			return;
		}
	}
	if (NULL == pShowDlg)
	{
        AfxMessageBox(_T("dll中函数ShowDlg寻找失败"));
		return;
	}
	if (NULL == m_PreTM)
	{
        AfxMessageBox(_T("dll中函数PreTranslateMessage寻找失败"));
		return;
	}

	HICON hIcon = ExtractIcon(AfxGetInstanceHandle(), strPath, 0);
	CCoolTabCtrl::CPageItem* pItem = g_pWebRobotDlg->m_TabCtrl.AddPage(RUNTIME_CLASS(CPluginDlg), IDD_PLUGIN_DLG, strName, hIcon);
	g_pWebRobotDlg->m_TabCtrl.SetActivePage(g_pWebRobotDlg->m_TabCtrl.GetPageCount() - 1);
	
	if (pShowDlg(pItem->m_pWnd))
	{
		if (bLoadTmpTxt)
		{
			pLoadTxt(strTempTxtPath);
		}
	}
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg) 
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

void CMainDlg::OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  Get current pos of the child controls
    CRect rcTree;
    CRect rcList;
    m_TreeCtrl.GetWindowRect(rcTree);
    m_ListCtrl.GetWindowRect(rcList);
    ScreenToClient(rcTree);
    ScreenToClient(rcList);
    //  return the pos limit
    SPC_NM_MAXMINPOS* pNewMaxMinPos = (SPC_NM_MAXMINPOS*)pNMHDR;
    if (IDC_SPLITTER == pNMHDR->idFrom)
    {
        pNewMaxMinPos->lMin = rcTree.left + 150;
        pNewMaxMinPos->lMax = rcList.right - 250;
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

void CMainDlg::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UINT uFlags = 0;
	CPoint pt = GetCurrentMessage()->pt;   //获取当前鼠标点击消息的坐标点
	m_TreeCtrl.ScreenToClient(&pt);   //将鼠标的屏幕坐标，转换成树形控件的客户区坐标
	HTREEITEM hItem = m_TreeCtrl.HitTest(pt, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))//没有点击在按钮上
	{
		DWORD dwData = m_TreeCtrl.GetItemData(hItem);
		if (dwData == -1)//关于程序
		{
			m_static.SetWindowText(strAboutWebRobot);

			if (!(m_static.GetStyle() & WS_VISIBLE))
			{
				GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TEXT)->ShowWindow(SW_SHOW);
			}
		}
		else if (dwData == -2)//功能插件
		{
			if (m_static.GetStyle() & WS_VISIBLE)
			{
				GetDlgItem(IDC_LIST)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_TEXT)->ShowWindow(SW_HIDE);
			}
		}
		if (dwData == -3)//抓取链接
		{
			m_static.SetWindowText(strGetUrlsText);
			
			if (!(m_static.GetStyle() & WS_VISIBLE))
			{
				GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TEXT)->ShowWindow(SW_SHOW);
			}
		}
		if (dwData == -4)//程序设置
		{
			m_static.SetWindowText(strSettingText);
			
			if (!(m_static.GetStyle() & WS_VISIBLE))
			{
				GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TEXT)->ShowWindow(SW_SHOW);
			}
		}
		else if (dwData > 0)
		{
			int nItemList = -1;
			for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
			{
				if (dwData == m_ListCtrl.GetItemData(i))
				{
					nItemList = i;
					break;
				}
			}
			if (nItemList != -1)
			{
				CString strAbout, strPath;
				strPath.Format("%s\\%s", m_strPluginPath, m_ListCtrl.GetItemText(i, 4));
				strPath.Replace("\\\\", "\\");
				HMODULE hModule = LoadLibrary(strPath);
				if (!hModule)
					return;
				GetInfo info = (GetInfo)GetProcAddress(hModule, "GetInfo");
				DLLINFO *pInfo = (DLLINFO*)info();
				strAbout = pInfo->pAbout;
				FreeLibrary(hModule);
				if (pInfo)
					HeapFree(GetProcessHeap(), 0, pInfo);//释放dll里用GlobalAlloc申请的内存
				
				m_static.SetWindowText(strAbout);
				
				if (!(m_static.GetStyle() & WS_VISIBLE))
				{
					GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_STATIC_TEXT)->ShowWindow(SW_SHOW);
				}
			}
		}
	}

	*pResult = 0;
}

void CMainDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CNewMenu OnLineMenu, *pSubMenu;
	OnLineMenu.LoadMenu(IDR_MENU_PLUGIN);
	OnLineMenu.SetMenuDrawMode(CNewMenu::STYLE_XP_2003);
	pSubMenu = (CNewMenu *)OnLineMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	int	count = pSubMenu->GetMenuItemCount();
	if (m_ListCtrl.GetSelectedCount() == 0)
	{
		pSubMenu->DeleteMenu(1, MF_BYPOSITION);
	}
	else if (m_ListCtrl.GetSelectedCount() == 1)
	{
		POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
		int iCurrSel= m_ListCtrl.GetNextSelectedItem(pos);
		if (iCurrSel < 0)
			return;//未选中含内容项
		CString strPath;
		strPath.Format("%s\\%s", m_strPluginPath, m_ListCtrl.GetItemText(iCurrSel, 4));
		strPath.Replace("\\\\", "\\");
		if (FileExists(strPath))
		{
			if (m_ListCtrl.GetItemText(iCurrSel, 2) == "×")
				pSubMenu->ModifyODMenu(ID_MENUITEM_AUTOLOAD, MF_BYCOMMAND, ID_MENUITEM_AUTOLOAD, "设为自动加载");
			else
				pSubMenu->ModifyODMenu(ID_MENUITEM_AUTOLOAD, MF_BYCOMMAND, ID_MENUITEM_AUTOLOAD, "设为不自动加载");
		}
	}

	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CMainDlg::OnMenuitemAutoload() 
{
	UpdateData();
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int iCurrSel= m_ListCtrl.GetNextSelectedItem(pos);
	if (iCurrSel < 0)
		return;//未选中含内容项
	CString strStatus, strPath;
	strStatus = m_ListCtrl.GetItemText(iCurrSel, 2);
	strPath.Format("%s\\%s", m_strPluginPath, m_ListCtrl.GetItemText(iCurrSel, 4));
	strPath.Replace("\\\\", "\\");
	if (FileExists(strPath))
	{
		if (strStatus == "×")
			SetFileAttributes(strPath, FILE_ATTRIBUTE_NORMAL);
		else
			SetFileAttributes(strPath, FILE_ATTRIBUTE_READONLY);

		LoadPlugins(m_strPluginPath);
	}
}

void CMainDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData();
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int iCurrSel= m_ListCtrl.GetNextSelectedItem(pos);
	if (iCurrSel < 0)
		return;//未选中含内容项

	CString strPath;
	strPath.Format("%s\\%s", m_strPluginPath, m_ListCtrl.GetItemText(iCurrSel, 4));
	strPath.Replace("\\\\", "\\");
	LoadPlugin(m_ListCtrl.GetItemText(iCurrSel, 0), strPath);
	*pResult = 0;
}

void CMainDlg::OnMenuitemOpenPluginDir() 
{
	::ShellExecute(NULL, _T("open"), m_strPluginPath, NULL, NULL, SW_SHOW);
}

void CMainDlg::OnMenuitemRefreshPluginList() 
{
	LoadPlugins(m_strPluginPath);
}

void CMainDlg::OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMTreeView->hdr.code==TVN_ITEMEXPANDING
        && pNMTreeView->action==TVE_COLLAPSE)//禁止节点收缩
    {
        *pResult = 1;
		return;
    }
	*pResult = 0;
}

LRESULT CMainDlg::OnMessageShowDlg(WPARAM wParam, LPARAM lParam)
{
	CString strInfo;
	for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		strInfo += m_ListCtrl.GetItemText(i, 4);
		strInfo += "|";
	}
	((CWebRobotApp *)AfxGetApp())->m_IniFile.SetString("Config", "PluginSort", strInfo);
	LoadPlugins(m_strPluginPath);

	return 0;
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

	*pResult = 0;
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = 23;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
