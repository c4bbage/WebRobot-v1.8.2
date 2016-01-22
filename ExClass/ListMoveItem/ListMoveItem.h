#if !defined(AFX_LISTMOVEITEM_H__48880351_42B0_4F55_B19C_2D3974E4C883__INCLUDED_)
#define AFX_LISTMOVEITEM_H__48880351_42B0_4F55_B19C_2D3974E4C883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListMoveItem.h : header file
//
typedef struct {
	LVITEM* plvi;
	CString sCol2;
	CString sCol3;
	CString sCol4;
	CString sCol5;
	DWORD dwID;
} lvItem, *plvItem;
/////////////////////////////////////////////////////////////////////////////
// CListMoveItem window

class CListMoveItem : public CListCtrl
{
// Construction
public:
	CListMoveItem();

// Attributes
public:
    CImageList * m_pDragImage;
    BOOL m_bDragging;
	int			m_nDragIndex;	//Index of selected item in the List we are dragging FROM
	int			m_nDropIndex;	//Index at which to drop item in the List we are dropping ON
	CListCtrl* m_pDragList;		//Which ListCtrl we are dragging FROM
	CListCtrl* m_pDropList;		//Which ListCtrl we are dropping ON
	CWnd*		m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListMoveItem)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);
	virtual ~CListMoveItem();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListMoveItem)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTMOVEITEM_H__48880351_42B0_4F55_B19C_2D3974E4C883__INCLUDED_)
