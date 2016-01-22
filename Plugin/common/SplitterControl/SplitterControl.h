#ifndef __CSplitterControl_H_
#define __CSplitterControl_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitterControl.h : header file
//


#include "AFXTEMPL.h"


/////////////////////////////////////////////////////////////////////////////
// Splitter style
#define SPS_HORIZONTAL      0x00000000          //  Horizontal look splitter								水平的分隔线
#define SPS_VERTICAL        0x00000001          //  Vertical look splitter(this is the default style)		垂直的分隔线
#define SPS_DELTA_NOTIFY    0x00000002          //  if need notify the parent to handle delta message


//  Linked side(used for Vertical style)
#define SPLS_LINKED_RIGHT   0x00000000          //  linked window at the right of the splitter(right pos change automatic)
#define SPLS_LINKED_LEFT    0x00000001          //  linked window at the left of the splitter(right pos change automatic)


//  Linked side(used for Horizontal style)
#define SPLS_LINKED_UP      SPLS_LINKED_LEFT    //  linked window at the top of the splitter(right pos change automatic)
#define SPLS_LINKED_DOWN    SPLS_LINKED_RIGHT   //  linked window at the bottom of the splitter(right pos change automatic)


//  Notify event : to get the max/min pos limit
//      usualy, the max/min pos depend on the parent window size, so we'd better get it from parent.
#define SPN_MAXMINPOS       (WM_USER + 1)
struct SPC_NM_MAXMINPOS
{
    NMHDR   hdr;
    LONG    lMax;
    LONG    lMin;
};


//  Notify event : tell the parent to do some special things
//      some times, the parent window can not register the child control for reason it does not created yet.
//      so, SPN_DELTA event give the parent window a chance to change the child control's pos.
#define SPN_DELTA           (WM_USER + 2)
struct SPC_NM_DELTA
{
    NMHDR   hdr;
    LONG    lDelta;
};



/////////////////////////////////////////////////////////////////////////////
//  CSplitterControl
//      
class CSplitterControl : public CStatic
{
// Attributes
protected:
    DWORD           m_dwSplitterStyle;      //  Splitter Style

    BOOL            m_bMouseIsDown;         //  Record the mouse is down or up
    CPoint          m_ptCurrent;            //  Current positon
    CPoint          m_ptOrigin;             //  Positon when mouse down
    HCURSOR         m_hCursor;              //  Cursor when mouse move

    COLORREF        m_clrSplitterColor;     //  Color of splitter
    LONG            m_lSplitterWidth;       //  line width of splitter

    int             m_iMaxPos;              //  Max pos the splitter
    int             m_iMinPos;              //  Min pos the splitter

    typedef CList<CWnd*, CWnd*>  CLinkedWndList;
    CLinkedWndList  m_listLinkedWnds[2];    //  Registered linked window

// Operations
public:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSplitterControl)
    //}}AFX_VIRTUAL

// Implementation
public:
    // Constructor/Destructor
    CSplitterControl();
    virtual     ~CSplitterControl();

    //  Create a splitter
    BOOL    Create(DWORD dwStaticStyle, const CRect& rect, CWnd* pParent, UINT nID, 
                   DWORD dwSplitterControlStyle = SPS_VERTICAL, 
                   COLORREF clrSplitterColor = RGB(120, 120, 120),
                   LONG     lSplitterLineWidth = 1);


    //  Register linked window
    BOOL    RegisterLinkedWindow(DWORD  dwLinkedSide, CWnd* pWnd);


    //  Usualy used in the SPN_DELTA notify event handle function
    void    ChangePos(CWnd* pWnd, DWORD dwLinkedSide, LONG lDelta);

	static void ChangeHeight(CWnd* pWnd, int dy);
	static void ChangeWidth(CWnd* pWnd, int dx);
    //  Relayout all linked windows
    //      you can call it in CDialog::OnInitDialog.
    //      if you have some special requirement, you can over write this function in sub class.
    virtual void    Relayout();


// Generated message map functions
protected:
    //  Draw the splitter lines
    //      if the default implement is not suit for you, over write it.
    virtual void    DrawSplitterLine(CDC* pDC);


    //  To get the left most and the right most pos
    //      this function called before you change the splitter pos.
    //      it just send message to parent window, but there are some many other ways, 
    virtual void    GetMaxMinPos(int& iMax, int& iMin);


    //  Move self to a new pos
    void            MoveSelfWindowToPoint(const CPoint& pt);

    //{{AFX_MSG(CSplitterControl)
    afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL    OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif//__CSplitterControl_H_
