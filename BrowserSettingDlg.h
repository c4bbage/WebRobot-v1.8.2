#if !defined(AFX_BROWSERSETTINGDLG_H__B689E080_2F7F_4724_9CD7_31FFF7518FAA__INCLUDED_)
#define AFX_BROWSERSETTINGDLG_H__B689E080_2F7F_4724_9CD7_31FFF7518FAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrowserSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrowserSettingDlg dialog

class CBrowserSettingDlg : public CDialog
{
// Construction
public:
	CBrowserSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrowserSettingDlg)
	enum { IDD = IDD_BROWSER_SETTING_DLG };
	CButtonXP	m_btnGoogleMirror;
	CListCtrl	m_ListCtrl;
	BOOL	m_bCheckQMark;
	BOOL	m_bBeepSound;
	BOOL	m_bCheckVSCrollToEnd;
	CString	m_strGoogleMirror;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowserSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrowserSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckQmark();
	afx_msg void OnCheckBeep();
	afx_msg void OnCheckVscrolltoend();
	afx_msg void OnMenuitemInsert();
	afx_msg void OnMenuitemDelete();
	afx_msg void OnRclickListUrlFilter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERSETTINGDLG_H__B689E080_2F7F_4724_9CD7_31FFF7518FAA__INCLUDED_)
