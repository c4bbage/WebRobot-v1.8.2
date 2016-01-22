#if !defined(AFX_MAINDLG_H__E12A7118_93F8_4B06_8101_064815CB7F07__INCLUDED_)
#define AFX_MAINDLG_H__E12A7118_93F8_4B06_8101_064815CB7F07__INCLUDED_

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
	CString m_strCurrentKey;
	

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
	CEdit	m_editShow;
	CButtonXP	m_btnScan;
	CButtonXP	m_btnParseServer;
	CComboBoxXP	m_comboIP;
	CComboBoxXP	m_comboInterface;
	CComboBoxXP	m_comboMode;
	CTreeCtrl	m_TreeCtrl;
	CString	m_Url;
	BOOL	m_bGetIpAddress;
	BOOL	m_bGetDetailServerInfo;
	int		m_nCScanBegin;
	int		m_nCScanEnd;
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
	CStringArray sArrayBingKey;
	BOOL m_bScanning;
	HANDLE m_hGetIpThread;
	HANDLE m_hWorkThread;
	CString	m_IpAddr;
	int nThreadNum;
	CStatusBarCtrl m_wndStatusBar;
	BOOL m_bCurrentScanModeIsSingleServerScan;
	CImageList m_ImageList;
	CToolTipCtrl m_infotip;
	BOOL m_bIsCdnIp;

	CString GetKeyboardText();
    void Resize();
    void MoveDlgItem(int nD, const CRect& rcPos, BOOL bRepaint);
	void StatusTextOut(int iPane, LPCTSTR ptzFormat, ...);
	void MyExpandTree(HTREEITEM hTreeItem, UINT nCode);
	CSize LoadMyBitmap(CImageList *imgList, UINT nID);
	HTREEITEM IsItemExist(HTREEITEM hItemParent, CString const& strItem);
	HTREEITEM InsertInfoToTreeCtrl(CString strIP, CString strInfo, BOOL bInsertNum = FALSE);
	void ShowCScanCtrl(BOOL bShow);
	void SaveTreeCtrlData(CString strSavePath);
	void ReSetChildItemNum(HTREEITEM hParent, int nNum);
	void ReSetAllItemNum();

	DWORD GetIpAddrThread();
	DWORD BingAPIScanThread();
	DWORD BingWebScanThread();
	CString GetBingApiData(CString strURL);
	void OnWorkEnd();

	void InsertIP(HTREEITEM hItem);
	CString GetFirstChildItemUrl(HTREEITEM hItem);
	void QuChongFu(CStringArray &sArray);//»•≥˝÷ÿ∏¥”Ú√˚


	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonParseUrl();
	afx_msg void OnButtonScan();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnMenuitemExpand();
	afx_msg void OnMenuitemCollapse();
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemSaveHtml();
	afx_msg void OnMenuitemSaveTxt();
	afx_msg void OnMenuitemSaveUrl();
	afx_msg void OnMenuitemCopySelected();
	afx_msg void OnMenuitemClear();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnCheckGetServerType();
	afx_msg void OnCheckGetDomainIp();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSelchangeComboInterface();
	afx_msg void OnStaticInterface();
	afx_msg void OnStaticIp();
	afx_msg void OnSetfocusEditUrl();
	afx_msg void OnMenuitemDeleteSelected();
	afx_msg void OnSelchangeComboIp();
	afx_msg void OnMenuitemDeleteErrorUrl();
	afx_msg void OnSetfocusComboIp();
	//}}AFX_MSG
	afx_msg LRESULT OnMessageShowDlg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static	DWORD WINAPI BingApiGetData(LPVOID lparam);
	static	DWORD WINAPI BingWebApiGetData(LPVOID lparam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__E12A7118_93F8_4B06_8101_064815CB7F07__INCLUDED_)
