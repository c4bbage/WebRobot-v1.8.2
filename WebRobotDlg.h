// WebRobotDlg.h : header file
//

#if !defined(AFX_WEBROBOTDLG_H__586EEF10_E3C4_43F5_BC2B_7732F7ACC429__INCLUDED_)
#define AFX_WEBROBOTDLG_H__586EEF10_E3C4_43F5_BC2B_7732F7ACC429__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWebRobotDlg dialog

class CWebRobotDlg : public CDialog
{
// Construction
public:
	CWebRobotDlg(CWnd* pParent = NULL);	// standard constructor
	CCoolTabCtrl	m_TabCtrl;
	CTrayIcon m_TrayIcon;

// Dialog Data
	//{{AFX_DATA(CWebRobotDlg)
	enum { IDD = IDD_WEBROBOT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebRobotDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	void MySetTooltipText(CString strText, UINT nMilliseconds = 1000);

	// Generated message map functions
	//{{AFX_MSG(CWebRobotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuitemShow();
	afx_msg void OnAppSuspend();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBROBOTDLG_H__586EEF10_E3C4_43F5_BC2B_7732F7ACC429__INCLUDED_)
