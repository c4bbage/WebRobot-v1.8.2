#if !defined(AFX_MAINDLG_H__7BA201D4_B6E0_46AB_AD67_333FF5063522__INCLUDED_)
#define AFX_MAINDLG_H__7BA201D4_B6E0_46AB_AD67_333FF5063522__INCLUDED_

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
	CEdit	m_editDest;
	CEdit	m_editSrc;
	CButtonXP	m_btnDecode;
	CButtonXP	m_btnEncode;
	CComboBoxXP	m_comboMode;
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
	CSplitterControl m_wndSplitter;

	void MoveDlgItem(int nID, const CRect& rcPos, BOOL bRepaint);
	void Resize();

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonEncode();
	afx_msg void OnButtonDecode();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnSetfocusEditSrc();
	afx_msg void OnSetfocusEditDest();
	//}}AFX_MSG
	afx_msg void OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__7BA201D4_B6E0_46AB_AD67_333FF5063522__INCLUDED_)
