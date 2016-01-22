#if !defined(AFX_MAINDLG_H__B51CB741_C121_4A05_8824_9A9BD09DF2FA__INCLUDED_)
#define AFX_MAINDLG_H__B51CB741_C121_4A05_8824_9A9BD09DF2FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CDialog
{
DECLARE_EASYSIZE //Ìí¼ÓÕâ¸ö
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
	CTreeCtrl	m_TreeCtrl;
	CButtonXP	m_btnScan;
	CButtonXP	m_btnResolve;
	CComboBoxXP	m_comboPort;
	CComboBoxXP	m_comboIP;
	CComboBoxXP	m_comboIPMode;
	CString	m_Url;
	int		m_nMaxThread;
	BOOL	m_bGetBanner;
	int		m_nTimeOut;
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
	CImageList m_ImageList;
	CStatusBarCtrl m_wndStatusBar;
	CToolTipCtrl m_infotip;

	BOOL bStopScan;
	BOOL	m_bScanning;
	HANDLE m_hGetIpThread;
	HANDLE	hSemaphore;
	int	nIpScanned;
	DWORD	dwThreadsUsed;
	CRITICAL_SECTION cs;
	BOOL m_bIsCdnIp;
	vector<int> iPortList;
	vector<CString> sIPList;

	void StatusTextOut(int iPane,LPCTSTR ptzFormat, ...);
	CSize LoadMyBitmap(CImageList *imgList, UINT nID);
	void MyExpandTree(HTREEITEM hTreeItem, UINT nCode);

	CString GetKeyboardText();
	DWORD GetIpAddrThread();
	int startScan(vector<CString> sIPList, vector<int> iPortList, int nMaxThreads);
	HTREEITEM IsItemExist(HTREEITEM hItemParent, CString const& strItem);
	HTREEITEM InsertInfoToTreeCtrl(CString strIP, CString strInfo, BOOL bInsertNum = FALSE);
	void SockSelect(u_long uIpAddr, int nPort);
	void IsPortOpen(u_long uIpAddr, int nPort);


	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonResolve();
	afx_msg void OnSetfocusEditUrl();
	afx_msg void OnSelchangeComboIpMode();
	afx_msg void OnButtonScan();
	afx_msg void OnSelchangeComboIp();
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMenuitemCopy();
	afx_msg void OnMenuitemExpand();
	afx_msg void OnMenuitemCollapse();
	afx_msg void OnMenuitemEmpty();
	afx_msg void OnMenuitemSave();
	afx_msg void OnSelchangeComboPort();
	//}}AFX_MSG
	afx_msg LRESULT OnMessageInsertResult(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static	unsigned __stdcall tcpScanThread(LPVOID lparam);
	static	DWORD WINAPI ScanPortThread(LPVOID lparam);
	static DWORD WINAPI WorkerFunction(LPVOID vThreadParm);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__B51CB741_C121_4A05_8824_9A9BD09DF2FA__INCLUDED_)
