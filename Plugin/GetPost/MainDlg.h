#if !defined(AFX_MAINDLG_H__AD308323_48EA_4A8F_92D2_C81D6047D245__INCLUDED_)
#define AFX_MAINDLG_H__AD308323_48EA_4A8F_92D2_C81D6047D245__INCLUDED_

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
	void GetPost();

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
	CButtonXP	m_btnInsert;
	CButtonXP	m_btnExec;
	CEdit	m_editPost;
	CXEditPrompt	m_editHeader;
	CComboBoxXP	m_ComboType;
	CString	m_strUrl;
	BOOL	m_bDecodeUtf8;
	CString	m_strPostValue;
	BOOL	m_bMutiThread;
	int		m_nThread;
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

	CRITICAL_SECTION cs;
	int m_nCurrentThreadCount;

    void Resize();
    void MoveDlgItem(int nD, const CRect& rcPos, BOOL bRepaint);
	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeComboMothod();
	afx_msg void OnButtonExec();
	afx_msg void OnButtonInsert();
	afx_msg void OnKillfocusEditPost();
	afx_msg void OnChangeEditHeader();
	afx_msg void OnChangeEditPost();
	afx_msg void OnChangeEditReturn();
	afx_msg void OnSetfocusEditPost();
	afx_msg void OnMenuitemCookie();
	afx_msg void OnMenuitemReferer();
	afx_msg void OnMenuitemUaBaidu();
	afx_msg void OnMenuitemUaGoogle();
	afx_msg void OnMenuitemUaIe();
	afx_msg void OnMenuitemUaChrome();
	afx_msg void OnMenuitemUaAndroid();
	afx_msg void OnMenuitemUaIphone();
	afx_msg void OnMenuitemFakeip();
	afx_msg void OnStaticHeader();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStaticLoadSave();
	afx_msg void OnMenuitemSave();
	afx_msg void OnMenuitemLoad();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMenuitemCopyall();
	afx_msg void OnMenuitemBrowser();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMenuitemClear();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnCheckMutiThread();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static	DWORD WINAPI MainThread(LPVOID lparam);
	static	DWORD WINAPI MutiMainThread(LPVOID lparam);
	static	DWORD WINAPI MutiSubThread(LPVOID lparam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__AD308323_48EA_4A8F_92D2_C81D6047D245__INCLUDED_)
