///////////////////////////////////////////////////////////////////////////////
//
// ComboBoxXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComboBoxXP.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
CComboBoxXP::CComboBoxXP () :
    m_nVisibleLines (7), m_nDefaultHeight (-1), m_nEditHeight (-1)
{
}

///////////////////////////////////////////////////////////////////////////////
BOOL CComboBoxXP::Create (DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
    if ( !CComboBox::Create (dwStyle, rect, pParentWnd, nID) )
    {
        return false;
    }
    SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
    m_MouseMgr.Init (m_hWnd);

    HWND hEdit = ::GetWindow (m_hWnd, GW_CHILD);

    if ( hEdit != NULL )
    {
        m_Edit.SubclassWindow (hEdit);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::PreSubclassWindow ()
{
    m_MouseMgr.Init (m_hWnd);

    HWND hEdit = ::GetWindow (GetSafeHwnd(), GW_CHILD);

    if ( hEdit != NULL )
    {
        m_Edit.SubclassWindow (hEdit);
    }
    CWindowRect rc (this);

    SetWindowPos (NULL, 0, 0, rc.Width(), 100, SWP_NOMOVE|SWP_NOZORDER);
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CComboBoxXP, CComboBox)
    //{{AFX_MSG_MAP(CComboBoxXP)
    ON_WM_GETDLGCODE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_WINDOWPOSCHANGED()
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEOUT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////////
BOOL CComboBoxXP::PreTranslateMessage (MSG* pMsg)
{
    if ( ChildOfDockedToolbar (this) && pMsg->message == WM_KEYDOWN )
    {
        switch ( pMsg->wParam )
        {
        case VK_RETURN:
            GetParent()->SendMessage (WM_COMMAND, GetDlgCtrlID());
            break;

        case VK_ESCAPE:
            if ( GetDroppedState() )
            {
                ShowDropDown (false);
            }
            else
            {
                GetParentFrame()->GetActiveFrame()->SetFocus();
            }
            break;
        }
    }
    return CComboBox::PreTranslateMessage (pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnWindowPosChanging (WINDOWPOS* pWndPos)
{
    if ( !(pWndPos->flags & SWP_NOSIZE) && m_nEditHeight == -1 )
    {
        if ( m_nDefaultHeight == -1 )
        {
            m_nDefaultHeight = GetItemHeight (-1);
        }
        m_nEditHeight = pWndPos->cy;

        pWndPos->cy = m_nDefaultHeight + GetItemHeight(0)*m_nVisibleLines + GetSystemMetrics(SM_CYBORDER)*7;
    }
    CComboBox::OnWindowPosChanging (pWndPos);
}

///////////////////////////////////////////////////////////////////////////////
UINT CComboBoxXP::OnGetDlgCode ()
{
    if ( KEYDOWN (VK_RETURN) )
    {
        return DLGC_WANTALLKEYS|CComboBox::OnGetDlgCode();
    }
    return CComboBox::OnGetDlgCode();
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnWindowPosChanged (WINDOWPOS* pWndPos)
{
    CComboBox::OnWindowPosChanged (pWndPos);

    if ( !(pWndPos->flags & SWP_NOSIZE) && m_nEditHeight != -1 )
    {
        SetItemHeight (-1, m_nEditHeight-6);
        m_nEditHeight = -1;
    }
    HWND hEdit = ::GetWindow (m_hWnd, GW_CHILD);

    // Del the selection
    if ( hEdit != NULL && hEdit != ::GetFocus() )
    {
        ::SendMessage (hEdit, EM_SETSEL, (WPARAM)-1, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnPaint ()
{
    CClientRect rc (this);
    CRect rcBtn (rc.right-::GetSystemMetrics (SM_CXHTHUMB), rc.top, rc.right, rc.bottom);

    ValidateRect (rcBtn);
    Default();

    CClientDC cDC (this);
    DWORD wStyle = DB_FLAT|DB_DOWN|(IsWindowEnabled() ? DB_ENABLED : DB_DISABLED);
    bool bInToolbar = ::ChildOfDockedToolbar (this);
    bool bDropped = DefWindowProc (CB_GETDROPPEDSTATE, 0, 0L) != 0;
    bool bFocus = (::GetFocus() == m_hWnd || ::GetParent (::GetFocus()) == m_hWnd) && bInToolbar;

    if ( !bInToolbar )
    {
        wStyle |= DB_BORDER;
    }
    if ( bFocus || m_MouseMgr.MouseOver() )
    {
        wStyle |= DB_OVER;
    }
    if ( bDropped )
    {
        wStyle |= DB_OVER|DB_PRESSED;
    }
    CDrawButton (wStyle, rcBtn).Draw (&cDC);
    CPenDC pen (cDC, ::GetSysColor (( bFocus || bDropped || m_MouseMgr.MouseOver() )
                                    ? COLOR_HIGHLIGHT
                                    : (bInToolbar ? COLOR_WINDOW : COLOR_3DSHADOW)));

    cDC.SelectObject (::GetStockObject (NULL_BRUSH));
    cDC.Rectangle (rc);

    pen.Color (::GetSysColor (IsWindowEnabled() ? COLOR_WINDOW : COLOR_3DFACE));
    rc.InflateRect (-1, -1);
    rc.right -= ::GetSystemMetrics (SM_CXHTHUMB);
    cDC.Rectangle (rc);
    cDC.MoveTo (rc.right, rc.top);
    cDC.LineTo (rc.right, rc.bottom);
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnSetFocus (CWnd* pOldWnd)
{
    CComboBox::OnSetFocus (pOldWnd);

    if ( ::ChildOfDockedToolbar (this) )
    {
        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnKillFocus (CWnd* pNewWnd)
{
    CComboBox::OnKillFocus (pNewWnd);

    if ( ::ChildOfDockedToolbar (this) )
    {
        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnMouseMove (UINT, CPoint)
{
    Default();
    m_MouseMgr.OnMouseMove();
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnMouseOut ()
{
    m_MouseMgr.OnMouseOut();
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CComboEditXP, CEdit)
    //{{AFX_MSG_MAP(CComboEditXP)
    ON_WM_KILLFOCUS()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEOUT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
void CComboEditXP::OnKillFocus (CWnd* pNewWnd)
{
    CEdit::OnKillFocus (pNewWnd);

    if ( ::ChildOfDockedToolbar (&GetComboBox()) )
    {
        GetComboBox().Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CComboEditXP::OnMouseMove (UINT, CPoint)
{
    Default();
    GetComboBox().m_MouseMgr.OnMouseMove (m_hWnd);
}

///////////////////////////////////////////////////////////////////////////////
void CComboEditXP::OnMouseOut ()
{
    GetComboBox().m_MouseMgr.OnMouseOut (m_hWnd);
}
