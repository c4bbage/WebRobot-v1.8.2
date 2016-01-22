///////////////////////////////////////////////////////////////////////////////
//
// ButtonXP.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Tools.h"

///////////////////////////////////////////////////////////////////////////////
class CButtonXP : public CButton
{
// Construction
public:
    CButtonXP();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CButtonXP)
    public:
    virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CButtonXP();

protected:
    CMouseMgr m_MouseMgr;

    // Generated message map functions
protected:
    //{{AFX_MSG(CButtonXP)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
