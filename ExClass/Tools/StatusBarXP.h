///////////////////////////////////////////////////////////////////////////////
//
// StatusBarXP.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
class CStatusBarXP : public CStatusBar
{
public:
    BOOL SetIndicators (const UINT* lpIDArray, int nIDCount);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CStatusBarXP)
public:
    //}}AFX_VIRTUAL

    // Generated message map functions
protected:
    //{{AFX_MSG(CStatusBarXP)
    afx_msg void OnPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
    DECLARE_DYNCREATE(CStatusBarXP)
};
