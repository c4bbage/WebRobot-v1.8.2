#if !defined(AFX_BINGKEYDLG_H__B684C77E_71D0_42AB_A56B_24E39D8BFBBE__INCLUDED_)
#define AFX_BINGKEYDLG_H__B684C77E_71D0_42AB_A56B_24E39D8BFBBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BingKeyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBingKeyDlg dialog

class CBingKeyDlg : public CDialog
{
// Construction
public:
	CBingKeyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBingKeyDlg)
	enum { IDD = IDD_BINGKEY_DLG };
	CListCtrlEx	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBingKeyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;

	void RefreshList(CString strCurrentKey);

	// Generated message map functions
	//{{AFX_MSG(CBingKeyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemAddKey();
	afx_msg void OnMenuitemDeleteKey();
	afx_msg void OnMenuitemUseKey();
	afx_msg void OnMenuitemCopyKey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINGKEYDLG_H__B684C77E_71D0_42AB_A56B_24E39D8BFBBE__INCLUDED_)
