///////////////////////////////////////////////////////////////////////////////
//
// ComboBoxXP.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Tools.h"

class CComboBoxXP;
///////////////////////////////////////////////////////////////////////////////
class CComboEditXP : public CEdit
{
public:
    CComboBoxXP& GetComboBox ();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CComboEditXP)
    public:
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CComboEditXP)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnMouseMove(UINT, CPoint);
    afx_msg void OnMouseOut();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////
class CComboBoxXP : public CComboBox
{
public:
    CComboBoxXP ();

// Operations
public:
    BOOL Create (DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

    // Not implemented !!!
    void SetVisibleLines (int nLines);

// Implementation
protected:
    int m_nVisibleLines;
    int m_nDefaultHeight;
    int m_nEditHeight;

    CComboEditXP m_Edit;
    CMouseMgr m_MouseMgr;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CComboBoxXP)
    public:
    virtual void PreSubclassWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CComboBoxXP)
    afx_msg UINT OnGetDlgCode();
    afx_msg void OnWindowPosChanging(WINDOWPOS*);
    afx_msg void OnWindowPosChanged(WINDOWPOS*);
    afx_msg void OnPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

friend CComboEditXP;
};


///////////////////////////////////////////////////////////////////////////////
inline CComboBoxXP& CComboEditXP::GetComboBox ()
{
    return *((CComboBoxXP*)GetParent());
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
