#if !defined(AFX_INPLACEEDIT_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)
#define AFX_AUTOLOCK_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_
// InPlaceEdit.h : header file
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit window

class CInPlaceEdit : public CEdit
{
// Construction
public:
        CInPlaceEdit(int iItem, int iSubItem, CString sInitText);

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CInPlaceEdit)
        public:
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CInPlaceEdit();
		void CalculateSize();

        // Generated message map functions
protected:
        //{{AFX_MSG(CInPlaceEdit)
        afx_msg void OnKillFocus(CWnd* pNewWnd);
        afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
private:
        int m_iItem;
        int m_iSubItem;
        CString m_sInitText;
        BOOL    m_bESC;         // To indicate whether ESC key was pressed
};

#endif // !defined(AFX_INPLACEEDIT_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)
/////////////////////////////////////////////////////////////////////////////