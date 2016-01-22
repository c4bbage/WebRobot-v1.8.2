#if !defined(AFX_PLUGINDLG_H__A07B3009_116C_4CAC_8366_F20A0203BD61__INCLUDED_)
#define AFX_PLUGINDLG_H__A07B3009_116C_4CAC_8366_F20A0203BD61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPluginDlg dialog

class CPluginDlg : public CDialog
{
// Construction
public:
	CPluginDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CPluginDlg)

// Dialog Data
	//{{AFX_DATA(CPluginDlg)
	enum { IDD = IDD_PLUGIN_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPluginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLUGINDLG_H__A07B3009_116C_4CAC_8366_F20A0203BD61__INCLUDED_)
