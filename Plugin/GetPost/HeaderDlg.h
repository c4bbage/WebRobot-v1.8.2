#if !defined(AFX_HEADERDLG_H__3411F102_C477_4A6C_A5B4_2BA284CB3956__INCLUDED_)
#define AFX_HEADERDLG_H__3411F102_C477_4A6C_A5B4_2BA284CB3956__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderDlg dialog

class CHeaderDlg : public CDialog
{
// Construction
public:
	CHeaderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHeaderDlg)
	enum { IDD = IDD_HEADER_DLG };
	CButtonXP	m_btnClear;
	CButtonXP	m_btnAdd;
	CXEditPrompt	m_editHeader;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonClear();
	afx_msg void OnChangeEditHeader();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERDLG_H__3411F102_C477_4A6C_A5B4_2BA284CB3956__INCLUDED_)
