// XEditPrompt.cpp  Version 1.1 - article available at www.codeproject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// History
//     Version 1.1 - 2007 July 20
//     - Tabbing no longer erases edit prompt
//
//     Version 1.0 - 2007 July 4
//     - Initial public release
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

#include "stdafx.h"
#include "XEditPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXEditPrompt

BEGIN_MESSAGE_MAP(CXEditPrompt, CEdit)
	//{{AFX_MSG_MAP(CXEditPrompt)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXEditPrompt::CXEditPrompt()
  :	m_bFirstTime(TRUE),
	m_strPromptText(_T("<Enter text here>")),
	m_crPromptColor(RGB(119,121,118))	// RAL 9023 (Pearl dark gray)
										// see http://www.highplains.net/pixelcolor.html
{

}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXEditPrompt::~CXEditPrompt()
{
	m_brush.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CXEditPrompt::PreSubclassWindow()
{
	m_brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	SetWindowText(m_strPromptText);
	SetSel(-1, 0);		// get rid of standard highlighting
	CEdit::PreSubclassWindow();
}

///////////////////////////////////////////////////////////////////////////////
// Reset
void CXEditPrompt::Reset()
{
	m_bFirstTime = TRUE;
	SetWindowText(m_strPromptText);
	SetSel(m_strPromptText.GetLength(), m_strPromptText.GetLength());
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
// SetPromptColor
void CXEditPrompt::SetPromptColor(COLORREF crText)
{
	m_crPromptColor = crText; 
	//if (m_bFirstTime)
	//	Invalidate(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// SetPromptText
void CXEditPrompt::SetPromptText(LPCTSTR lpszPrompt)
{ 
	m_strPromptText = lpszPrompt;
	if (m_strPromptText.IsEmpty())
		m_bFirstTime = FALSE;
	if (m_bFirstTime)
		SetWindowText(m_strPromptText);
}

///////////////////////////////////////////////////////////////////////////////
// OnSetFocus
void CXEditPrompt::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	if (m_bFirstTime)
	{
		// get rid of standard highlighting
		//SetSel(m_strPromptText.GetLength(), m_strPromptText.GetLength());
		m_bFirstTime = FALSE;
		SetWindowText(_T(""));
	}
}

///////////////////////////////////////////////////////////////////////////////
// CtlColor
HBRUSH CXEditPrompt::CtlColor(CDC* pDC, UINT /*nCtlColor*/) 
{
	if (m_bFirstTime)
		pDC->SetTextColor(m_crPromptColor);

	if (GetStyle() & ES_READONLY)
	{
		/*
		CRect rect;
		GetClientRect(rect);
		COLORREF clr = RGB(230, 230, 230);
		CBrush brush(clr);
		pDC->FillRect(rect, &brush);*/
		pDC->SetBkMode(TRANSPARENT);
	}
	else
	{
		//pDC->SetBkMode(TRANSPARENT);//用这个不好，会重叠
		LOGBRUSH lb;
		m_brush.GetLogBrush(&lb);
		pDC->SetBkColor(lb.lbColor);
	}
	return m_brush;		// setting text color will have no effect unless 
						// we return a valid brush
}

///////////////////////////////////////////////////////////////////////////////
// OnKeyDown
void CXEditPrompt::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_bFirstTime)
	{
		// key down includes keys like shift and ctrl
		m_bFirstTime = FALSE;
		SetWindowText(_T(""));
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

///////////////////////////////////////////////////////////////////////////////
// OnLButtonDown
void CXEditPrompt::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bFirstTime)
	{
		m_bFirstTime = FALSE;
		SetWindowText(_T(""));
	}
	
	CEdit::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnMButtonDown
void CXEditPrompt::OnMButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bFirstTime)
	{
		m_bFirstTime = FALSE;
		SetWindowText(_T(""));
	}
	
	CEdit::OnMButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnRButtonDown
void CXEditPrompt::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bFirstTime)
	{
		m_bFirstTime = FALSE;
		SetWindowText(_T(""));
	}
	
	CEdit::OnRButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// GetWindowText
int CXEditPrompt::GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const
{
	if (m_bFirstTime)
	{
		if (lpszStringBuf && (nMaxCount > 0))
			lpszStringBuf[0] = _T('\0');
		return 0;
	}

	return CEdit::GetWindowText(lpszStringBuf, nMaxCount);
}

///////////////////////////////////////////////////////////////////////////////
// GetWindowText
void CXEditPrompt::GetWindowText(CString& rString) const
{
	if (m_bFirstTime)
		rString = _T("");
	else
		CEdit::GetWindowText(rString);
}

///////////////////////////////////////////////////////////////////////////////
// SetWindowText
void CXEditPrompt::SetWindowText(LPCTSTR lpszString)
{
	// the control is being initialized, just ignore
	if (m_bFirstTime && (lpszString[0] == _T('\0')))
		return;

	// if this is not prompt string, then no need to prompt
	if (lpszString && 
		(m_strPromptText.Compare(lpszString) != 0))
		m_bFirstTime = FALSE;

	CEdit::SetWindowText(lpszString);
}

///////////////////////////////////////////////////////////////////////////////
// DefWindowProc
LRESULT CXEditPrompt::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SETTEXT)
	{
		TCHAR *cp = (TCHAR *) lParam;

		if (!cp)
			return TRUE;

		// the control is being initialized, just ignore
		if (m_bFirstTime && (cp[0] == _T('\0')))
			return TRUE;

		// if this is not prompt string, then no need to prompt
		if (m_strPromptText.Compare(cp) != 0)
			m_bFirstTime = FALSE;
	}
	else if (message == WM_GETTEXT)
	{
		if (m_bFirstTime)
		{
			TCHAR *cp = (TCHAR *) lParam;

			if (cp && (wParam != 0))
				*cp = _T('\0');

			return 0;
		}
	}

	return CEdit::DefWindowProc(message, wParam, lParam);
}

void CXEditPrompt::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	CString str;
	GetWindowText(str);
	if (str.GetLength() == 0)
		Reset();
}
