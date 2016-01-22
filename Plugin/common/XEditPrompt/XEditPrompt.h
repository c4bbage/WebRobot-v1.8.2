// XEditPrompt.h  Version 1.1 - article available at www.codeproject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XEDITPROMPT_H
#define XEDITPROMPT_H

/////////////////////////////////////////////////////////////////////////////
// CXEditPrompt window

class CXEditPrompt : public CEdit
{
// Construction
public:
	CXEditPrompt();
	virtual ~CXEditPrompt();

// Operations
public:
	void Reset();

// Attributes
public:
	COLORREF	GetPromptColor()	{ return m_crPromptColor; }
	CString		GetPromptText()		{ return m_strPromptText; }
	int			GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
	void		GetWindowText(CString& rString) const;
	void		SetPromptColor(COLORREF crText);
	void		SetPromptText(LPCTSTR lpszPrompt);
	void		SetWindowText(LPCTSTR lpszString);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXEditPrompt)
protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
private:
	BOOL		m_bFirstTime;		// TRUE = user has not yet selected this control
	CString		m_strPromptText;	// prompt text to display initially
	COLORREF	m_crPromptColor;	// color to use for prompt text
	CBrush		m_brush;			// background brush for WM_CTLCOLOR

	// Generated message map functions
protected:
	//{{AFX_MSG(CXEditPrompt)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XEDITPROMPT_H
