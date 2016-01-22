#if !defined(AFX_MAINDLG_H__5D17FE99_2FF3_4131_B31A_5C2B38403357__INCLUDED_)
#define AFX_MAINDLG_H__5D17FE99_2FF3_4131_B31A_5C2B38403357__INCLUDED_

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
	DECLARE_DYNCREATE(CMainDlg)
	void LoadCustomPlugin(CString strName, CString strTempTxtPath);

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
	CXHTMLStatic	m_static;
	CListMoveItem	m_ListCtrl;
	CTreeCtrl	m_TreeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;
	CImageList m_ImageList1;
	CSplitterControl m_wndSplitter;

	CSize LoadMyBitmap(CImageList *imgList, UINT nID);
	void Resize();
	void MoveDlgItem(int nD, const CRect& rcPos, BOOL bRepaint);
	void InsertInfoToTreeCtrl(CString strName, int nData);
	void InsertInfoToListCtrl(CString strName, CString strDescription, CString strVersion, CString strFileName, CString strPath, CString strCrc32);
	HTREEITEM IsItemExist(HTREEITEM hItemParent, CString const& strItem);
	void LoadPlugins(LPCTSTR lpszPath);
	void LoadPlugin(CString strName, CString strPath, CString strTempTxtPath = "");
	void InsertPluginInfo(LPCTSTR lpszPath, LPCTSTR lpszFileName);
	void InsertPluginInfoEx(CStringArray &sArray);
	CString GetPluginPath(CString strName);

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemAutoload();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemOpenPluginDir();
	afx_msg void OnMenuitemRefreshPluginList();
	afx_msg void OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	afx_msg void OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSplitter1Delta(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMessageShowDlg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__5D17FE99_2FF3_4131_B31A_5C2B38403357__INCLUDED_)
