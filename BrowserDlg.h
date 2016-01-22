//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_BROWSERDLG_H__CD2C9B13_1BCC_4A67_8FB1_00A9F90BB813__INCLUDED_)
#define AFX_BROWSERDLG_H__CD2C9B13_1BCC_4A67_8FB1_00A9F90BB813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrowserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrowserDlg dialog

class CBrowserDlg : public CDialog
{
// Construction
public:
	CBrowserDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CBrowserDlg)
// Dialog Data
	//{{AFX_DATA(CBrowserDlg)
	enum { IDD = IDD_BROWSER_DLG };
	CListCtrl	m_ListCtrlSearchKeyword;
	CListCtrl	m_ListCtrl;
	CComboBoxXP	m_ComboUrl;
	CButtonST	m_checkUrls;
	CButtonST	m_btnRefresh;
	CButtonST	m_btnBackward;
	CButtonST	m_btnForward;
	CButtonST	m_btnGo;
	CButtonST	m_btnSetting;
	CWebBrowser2	m_WebBrowser;
	BOOL	m_bGetUrls;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowserDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSplitterControl m_wndSplitter;
	CToolTipCtrl m_infotip;
	CImageList m_ImageList;
	int m_nListHeight;
	CStatusBarCtrl m_wndStatusBar;
	LPDISPATCH m_lpDisp;//用于判断网页加载是否真正完毕
	
	CString m_strGoogleMirror;
	CString m_strUserAgent;
	CString		strCurrentUrl;
	CString strNewWindowUrl;
	BOOL m_bNeedUpdateWindow;
	CGetWeb m_GetWeb;
	BOOL m_bBeepSound;
	BOOL m_bCheckQMark;
	CStringArray strUrlFilterArray;
	
	void Resize();
	void MoveDlgItem(int nD, const CRect& rcPos, BOOL bRepaint);
	void StatusTextOut(int iPane,LPCTSTR ptzFormat, ...);
	bool ScrollToEnd();
	BOOL GetUrlFile(CString url, CString& refString);
	BOOL IsQMarkNotExist(CString strUrl);
	BOOL GetBaiduSearchResultRedirectUrl(CString &strReturn);

	// Generated message map functions
	//{{AFX_MSG(CBrowserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSetting();
	afx_msg void OnButtonGo();
	afx_msg void OnButtonBackward();
	afx_msg void OnButtonForward();
	afx_msg void OnButtonRefresh();
	afx_msg void OnCheckUrls();
	afx_msg void OnNavigateComplete2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnDocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnStatusTextChangeExplorer(LPCTSTR Text);
	afx_msg void OnNewWindow2Explorer(LPDISPATCH FAR* ppDisp, BOOL FAR* Cancel);
	afx_msg void OnMenuitemOpenurl();
	afx_msg void OnMenuitemCopyurl();
	afx_msg void OnMenuitemSaveurls();
	afx_msg void OnMenuitemDeleteurl();
	afx_msg void OnMenuitemClearlist();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDblclkListSearchKeyword(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemSendtoInjection();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg void OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSplitter1Delta(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMessageGetUrls(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	static	DWORD WINAPI GetURLThread(LPVOID lparam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERDLG_H__CD2C9B13_1BCC_4A67_8FB1_00A9F90BB813__INCLUDED_)
