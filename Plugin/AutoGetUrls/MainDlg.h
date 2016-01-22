#if !defined(AFX_MAINDLG_H__C3BB003A_0234_4D6A_AAEC_46898313BC7A__INCLUDED_)
#define AFX_MAINDLG_H__C3BB003A_0234_4D6A_AAEC_46898313BC7A__INCLUDED_

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
	CListCtrl	m_ListCtrl;
	CButtonXP	m_btnSearch;
	CComboBoxXP	m_comboSearch;
	CComboBoxXP	m_comboSearchEngine;
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
	CStringArray strUrlFilterArray;
	CString m_strUserAgent;
	HANDLE	m_hWorkThread;
	BOOL m_bStop;

	void StatusTextOut(int iPane,LPCTSTR ptzFormat, ...);
	void ShowRowNum();
	int FindSameItem(CString strText);

	void InitConfig();
	void SearchFunc();
	void InsertUrlToList(int nSearchEngine, CString strUrl, CString strHtml);
	CString getURLContext(int nSearchEngine, CString strUrl, CString strHeader);
	BOOL GetBaiduSearchResultRedirectUrl(CString &strReturn);


	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSearch();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemCopy();
	afx_msg void OnMenuitemDelete();
	afx_msg void OnMenuitemClear();
	afx_msg void OnMenuitemSave();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static	DWORD WINAPI SearchThread(LPVOID lparam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__C3BB003A_0234_4D6A_AAEC_46898313BC7A__INCLUDED_)
