#if !defined(AFX_MAINDLG_H__2B6832D9_94C8_4026_9B14_3FA04947AD78__INCLUDED_)
#define AFX_MAINDLG_H__2B6832D9_94C8_4026_9B14_3FA04947AD78__INCLUDED_

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
	CButtonXP	m_btnScan;
	CListCtrlEx	m_ListCtrl;
	int		m_nMaxThread;
	CString	m_strDomain;
	BOOL	m_checkGetDetailInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;
	CStatusBarCtrl m_wndStatusBar;

	BOOL bStopScan;
	BOOL	m_bScanning;
	CStringArray sDomainArray;
	CRITICAL_SECTION cs;
	DWORD	dwThreadsUsed;
	HANDLE	hSemaphore;
	CString m_strDictionaryPath;
	int	m_nIpScanned;

	void SaveTreeCtrlData(CString strSavePath);
	CSize LoadMyBitmap(CImageList *imgList, UINT nID);
	void StatusTextOut(int iPane,LPCTSTR ptzFormat, ...);
	void ShowRowNum();

	int startScanSubDomain(CStringArray *sArray);

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonScan();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemCopyDomain();
	afx_msg void OnMenuitemCopyIp();
	afx_msg void OnMenuitemSaveDomain();
	afx_msg void OnMenuitemSaveDomainIp();
	afx_msg void OnMenuitemDeleteSeleted();
	afx_msg void OnMenuitemClearList();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMenuitemOpenUrl();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemCopyHttpDomain();
	afx_msg void OnMenuitemSaveHttpDomain();
	afx_msg void OnMenuitemSaveHtml();
	//}}AFX_MSG
	afx_msg LRESULT OnMessageInsertResult(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static	DWORD WINAPI DomainScanThread(LPVOID lparam);
	static	DWORD WINAPI ScanThread(LPVOID lparam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__2B6832D9_94C8_4026_9B14_3FA04947AD78__INCLUDED_)
