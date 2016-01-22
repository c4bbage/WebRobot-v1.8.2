#if !defined(AFX_MAINDLG_H__948EE26B_D539_42C9_A652_94A0F51A477D__INCLUDED_)
#define AFX_MAINDLG_H__948EE26B_D539_42C9_A652_94A0F51A477D__INCLUDED_

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
	void LoadTxt(CString strTxtPath);

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
	HHideListCtrl	m_ListCtrl;
	CListCtrlEx	m_ListCtrlResult;
	CButtonXP	m_btnScan;
	int		m_nTimeOut;
	int		m_nWebPageChangeRate;
	BOOL	m_bInjectionCheckNormal;
	int		m_nMaxThreadNum;
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
	CSplitterControl m_wndSplitter;
	CImageList m_ImageList;
	CImageList m_ImageListResult;
	CStatusBarCtrl m_wndStatusBar;

	CString m_strUserAgent;
	HANDLE m_hWorkThread;
	HANDLE m_hTerminateWorkThread;
	BOOL	m_bScanning;
	CStringArray strArrayErrorWords;
	CString m_strTempImportFilePath;

	CSize LoadMyBitmap(CImageList *imgList, UINT nID);
	CString GetKeyboardText();
	void Resize();
	void MoveDlgItem(int nD, const CRect& rcPos, BOOL bRepaint);
	void StatusTextOut(int iPane,LPCTSTR ptzFormat, ...);
	void ShowRowNum();
	void ShowResultRowNum();
	DWORD ScanInjectionThread();
	DWORD TerminateWorkThread();
	DWORD RemoveDuplicateThread();
	void ImportUrlFile(CString strPath);
	CString AddSingleQuotes(CString strURL);
	CString RemoveURLParameters(CString strURL);
	void DetectURL(ReturnData *lpReturnData);
	CString ModifyParameter(int Mode, CString strURL);
	int GetPageChangeRate(CString strURL1, CString strURL2, long &lWebPageSize1, long &lWebPageSize2);
	CString GetErrMethod(int nMethod);
	CString GetUseType(int nMethod);
	CString GetInjectType(int nMethod);

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonScanInjection();
	afx_msg void OnMenuitemVisit();
	afx_msg void OnMenuitemCopy();
	afx_msg void OnMenuitemSave();
	afx_msg void OnMenuitemAddurltolist();
	afx_msg void OnMenuitemImport();
	afx_msg void OnMenuitemDel();
	afx_msg void OnMenuitemClear();
	afx_msg void OnRclickListUrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListUrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemVisitResult();
	afx_msg void OnMenuitemCopyResult();
	afx_msg void OnMenuitemSaveResult();
	afx_msg void OnMenuitemDelResult();
	afx_msg void OnMenuitemClearResult();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMenuitemDeleteTimeoutUrls();
	afx_msg void OnMenuitemScanTimeoutUrls();
	//}}AFX_MSG
	afx_msg void OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSplitter1Delta(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMessageInsertInjectResult(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static	DWORD WINAPI DetectInjectURLThread(LPVOID lparam);

	HANDLE	m_hThread[999999];
	UINT	m_nThreadCount;
	int	m_nCurrentThreadCount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__948EE26B_D539_42C9_A652_94A0F51A477D__INCLUDED_)
