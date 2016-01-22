#if !defined(AFX_MAINDLG_H__7D9DC88B_BE4E_4E8C_B87B_26090F5140FF__INCLUDED_)
#define AFX_MAINDLG_H__7D9DC88B_BE4E_4E8C_B87B_26090F5140FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
	CButtonST	m_btnAppearance;
	CButtonXP	m_btnScan;
	CComboBoxXP	m_comboLimitDir;
	CTreeCtrl	m_TreeCtrl;
	CListCtrlEx	m_ListCtrl;
	CListCtrl	m_ReScanURLListCtrl;
	CGridListCtrl	m_SiteListCtrl;
	UINT	m_nMaxThreadNum;
	BOOL	m_bSimilarUrlNotClawl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_infotip;
	CImageList m_ImageList;
	CImageList m_RightImageList;
	CSplitterControl m_wndSplitter;
	CStatusBarCtrl m_wndStatusBar;
	CInPlaceEdit *m_pListEdit;

	CGetWeb m_GetWeb;
	int nLimitUrlLevel;
	BOOL	m_bScanning;
	HANDLE m_hWorkThread;
	HANDLE m_hTerminateWorkThread;
	CStringList strNotCrawlList;
	CString strMustInclude;
	CStringList strURLList;
	DWORD nListData;
	CString strCompleteURL;

	virtual ~CMainDlg();

	CString GetKeyboardText();
	void MoveDlgItem(int nID, const CRect& rcPos, BOOL bRepaint);
	void Resize();
	void StatusTextOut(int iPane,LPCTSTR ptzFormat, ...);
	void ShowRowNum();
	void ShowReListRowNum();
	void MyExpandTree(HTREEITEM hTreeItem, UINT nCode);

	CString RemoveURLParameters(CString strURL);
	void InsertSimilarURL(int nListCount, int nListData, CString strURL, CString strTempDomain);
	int CrawlSite();
	DWORD CrawlSiteThread();
	DWORD TerminateCrawlWorkThread();
	BOOL GetUrlFile(CString url, CString& strRetBuff);
	CString GetURLExt(CString strURL);
	int FindSameItem(CString strText, int nSubItem = 0);
	int GetUnCrawledItem();
	BOOL IsCrawlFinished();
	void ListInsertItem(CString strTmp, CString strSrcPage, int nStatus);
	void InsertURLToTreeCtrl(CString strURL);
	HTREEITEM IsItemExist(HTREEITEM hItemParent, CString const& strItem);
	int GetURLLevel(CString strURL);
	void AddListCtrlURLs();

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonScan();
	afx_msg void OnButtonAppearance();
	afx_msg void OnRclickListSite(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListRescanurl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTreeUrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListUrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemCopy();
	afx_msg void OnMenuitemDelete();
	afx_msg void OnMenuitemAddurl();
	afx_msg void OnMenuitemImport();
	afx_msg void OnMenuitemDeleteallurls();
	afx_msg void OnMenuitemRightCopy();
	afx_msg void OnMenuitemRightCopysrcpage();
	afx_msg void OnMenuitemRightDelsel();
	afx_msg void OnMenuitemRightClearall();
	afx_msg void OnMenuitemRightSaveallurls();
	afx_msg void OnMenuitemRightImportfile();
	afx_msg void OnMenuitemRightVisit();
	afx_msg void OnMenuitemRightTreeCopy();
	afx_msg void OnMenuitemTreeClear();
	afx_msg void OnMenuitemGotoSrcpage();
	afx_msg void OnMenuitemTreeSaveallurlstofile();
	afx_msg void OnMenuitemTreeImportUrls();
	afx_msg void OnMenuitemExpand();
	afx_msg void OnMenuitemCollapse();
	afx_msg void OnSelchangedTreeUrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemClearTopList();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	afx_msg void OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSplitter1Delta(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMessageUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageInsertCrawlResult(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditEnd(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private:
	HANDLE	m_hThread[200000];
	UINT	m_nThreadCount;
	UINT	m_nCurrentThreadCount;
	static unsigned __stdcall  DetectURLThread(LPVOID lparam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__7D9DC88B_BE4E_4E8C_B87B_26090F5140FF__INCLUDED_)
