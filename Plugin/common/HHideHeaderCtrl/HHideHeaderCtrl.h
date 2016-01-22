#ifndef	__HENGAI_CLASS_HIDEHEADERCTRL_H__
#define __HENGAI_CLASS_HIDEHEADERCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HHideHeaderCtrl.h : header file
//
#ifndef	__AFXTEMPL_H__
#pragma message ("如果你在 stdafx.h 文件中使用了 #include<afxtempl.h> 则不会出现这个消息提示")
	#include <afxtempl.h>
#endif
struct SColumn{			//记录了每列的信息。
	BOOL bHide;
	int nItemWidth;		//在隐藏前的宽度。因为隐藏的时候需要将宽度设为0
	SColumn(){
		bHide = FALSE;
		nItemWidth = -1;
	}
};

/////////////////////////////////////////////////////////////////////////////
// HHideHeaderCtrl window

class HHideHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	HHideHeaderCtrl();

// Attributes
public:

// Operations
public:
	CArray<SColumn, SColumn&> m_aColum;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HHideHeaderCtrl)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~HHideHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(HHideHeaderCtrl)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bUseHideCursor;		//如果该列隐藏，那么鼠标指针不变
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__HENGAI_CLASS_HIDEHEADERCTRL_H__)
