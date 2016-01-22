#ifndef	__HENGAI_CLASS_HIDELISTCTRL_H__
#define __HENGAI_CLASS_HIDELISTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HHideListCtrl.h : header file
//

#include "HHideHeaderCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// HHideListCtrl window

class HHideListCtrl : public CListCtrl
{
// Construction
public:
	HHideListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HHideListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	int SetColumnHide(int nCol, BOOL bHide);
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1 );
	int InsertColumn(int nCol, const LVCOLUMN *pColumn);
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, BOOL bHide, int nFormat, int nWidth, int nSubItem );
	int InsertColumn(int nCol, const LVCOLUMN *pColumn, BOOL bHide);
	BOOL DeleteColumn( int nCol );
	virtual ~HHideListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(HHideListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	HHideHeaderCtrl m_ctlHeader;
	int m_nHideColumCount, *m_pHideColumn;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__HENGAI_CLASS_HIDELISTCTRL_H__)
