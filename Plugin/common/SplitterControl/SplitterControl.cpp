// SplitterControl.cpp : implementation file
//

#include "stdafx.h"
#include "SplitterControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSplitterControl
CSplitterControl::CSplitterControl()
{
    m_dwSplitterStyle   = SPS_VERTICAL;
    m_bMouseIsDown      = FALSE;
    m_ptCurrent         = CPoint(0, 0);
    m_ptOrigin          = CPoint(0, 0);
    m_hCursor           = NULL;
    m_iMinPos           = INT_MIN;
    m_iMaxPos           = INT_MAX;
    m_clrSplitterColor  = RGB(150, 120, 120);
    m_lSplitterWidth    = 3;
}

CSplitterControl::~CSplitterControl()
{
    m_listLinkedWnds[SPLS_LINKED_LEFT].RemoveAll();
    m_listLinkedWnds[SPLS_LINKED_RIGHT].RemoveAll();
}


BEGIN_MESSAGE_MAP(CSplitterControl, CStatic)
    //{{AFX_MSG_MAP(CSplitterControl)
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterControl message handlers
BOOL CSplitterControl::Create(DWORD dwStaticStyle, const CRect& rect, CWnd* pParent, UINT nID, 
                              DWORD dwSplitterControlStyle, COLORREF clrSplitterColor, LONG lSplitterLineWidth)
{
    //  Save styles
    m_dwSplitterStyle = dwSplitterControlStyle;

    //  Sace splitter color
    m_clrSplitterColor = clrSplitterColor;
    
    //  load the cursor
	m_hCursor = LoadCursor(NULL, m_dwSplitterStyle&SPS_VERTICAL?IDC_SIZEWE:IDC_SIZENS);

    //  just create the static(no title)
    return (CStatic::Create(_T(""), (dwStaticStyle | SS_NOTIFY | WS_CHILD), rect, pParent, nID));
}

void CSplitterControl::OnMouseMove(UINT nFlags, CPoint point) 
{
    if (m_bMouseIsDown)
    {
        //  erase the old splitter
        CWindowDC dc(NULL);
        this->DrawSplitterLine(&dc);
        
        //  calc the new pos of the splitter
        CPoint pt = point;
        this->ClientToScreen(&pt);

        CWnd* pParent = GetParent();
        ASSERT(pParent);
        ASSERT(IsWindow(pParent->m_hWnd));
        pParent->ScreenToClient(&pt);

        //  split position limit
        pt.x = (pt.x < m_iMinPos)?m_iMinPos:pt.x;
        pt.y = (pt.y < m_iMinPos)?m_iMinPos:pt.y;
        pt.x = (pt.x > m_iMaxPos)?m_iMaxPos:pt.x;
        pt.y = (pt.y > m_iMaxPos)?m_iMaxPos:pt.y;

        //  save the current pos, this value will be used when the mouse up
        pParent->ClientToScreen(&pt);
        m_ptCurrent = pt;

        //  repaint the splitter
        this->DrawSplitterLine(&dc);
    }

    CStatic::OnMouseMove(nFlags, point);
}

BOOL CSplitterControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    //  when the move is on the client of the splitter this event while be fired
    //      so just change the cursor
    if (nHitTest == HTCLIENT)
    {
        (void)::SetCursor(m_hCursor);
        return (TRUE);
    }

    return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitterControl::OnLButtonDown(UINT nFlags, CPoint point) 
{
    CStatic::OnLButtonDown(nFlags, point);

    //  get the max/min pos of the splitter first
    this->GetMaxMinPos(this->m_iMaxPos, this->m_iMinPos);

    //  Record the mouse status
    m_bMouseIsDown = TRUE;

    SetCapture();
    //  Get the move split start pos
    CRect rcWnd;
    GetWindowRect(rcWnd);
    m_ptOrigin = m_ptCurrent = rcWnd.CenterPoint();
    
    //  draw the splitter
    CWindowDC dc(NULL);
    this->DrawSplitterLine(&dc);
}

void CSplitterControl::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (m_bMouseIsDown)
    {
        //  erase the old picture
        this->ClientToScreen(&point);
        CWindowDC dc(NULL);
        this->DrawSplitterLine(&dc);

        //  move spliter control self to the new pos
        m_bMouseIsDown = FALSE;
        CWnd* pParent = GetParent();
        if (pParent && IsWindow(pParent->m_hWnd))
        {
            CPoint pt = m_ptCurrent;
            pParent->ScreenToClient(&pt);
            this->MoveSelfWindowToPoint(pt);

        }

        //  relayout all registerd windows
        this->Relayout();

        //  if need notify the parent
        if (m_dwSplitterStyle & SPS_DELTA_NOTIFY)
        {
            if (pParent && IsWindow(pParent->m_hWnd))
            {
                CPoint ptDelta = m_ptCurrent - m_ptOrigin;
                SPC_NM_DELTA tNotify;
                tNotify.hdr.hwndFrom = m_hWnd;
                tNotify.hdr.idFrom   = GetDlgCtrlID();
                tNotify.hdr.code     = SPN_DELTA;
                tNotify.lDelta       = (m_dwSplitterStyle&SPS_VERTICAL)?ptDelta.x:ptDelta.y;
                (void)pParent->SendMessage(WM_NOTIFY, tNotify.hdr.idFrom, (LPARAM)&tNotify);
            }
        }
    }

    CStatic::OnLButtonUp(nFlags, point);
    ReleaseCapture();
}

void CSplitterControl::DrawSplitterLine(CDC* pDC)
{
    CPoint      pt          = this->m_ptCurrent;
    COLORREF    clrSplitter = this->m_clrSplitterColor;


    int nRop = pDC->SetROP2(R2_NOTXORPEN);

    CRect rcWnd;
    GetWindowRect(rcWnd);

    CPen  pen(0, 1, clrSplitter);
    CPen* pOldPen = pDC->SelectObject(&pen);
    
    if (m_dwSplitterStyle&SPS_VERTICAL)
    {
        pDC->MoveTo(pt.x - m_lSplitterWidth, rcWnd.top);
        pDC->LineTo(pt.x - m_lSplitterWidth, rcWnd.bottom);

        pDC->MoveTo(pt.x + m_lSplitterWidth, rcWnd.top);
        pDC->LineTo(pt.x + m_lSplitterWidth, rcWnd.bottom);
    }
    else // m_dwSplitterStyle == SPS_HORIZONTAL
    {
        pDC->MoveTo(rcWnd.left,  pt.y - m_lSplitterWidth);
        pDC->LineTo(rcWnd.right, pt.y - m_lSplitterWidth);
        
        pDC->MoveTo(rcWnd.left,  pt.y + m_lSplitterWidth);
        pDC->LineTo(rcWnd.right, pt.y + m_lSplitterWidth);
    }

    pDC->SetROP2(nRop);
    pDC->SelectObject(pOldPen);
}

void CSplitterControl::MoveSelfWindowToPoint(const CPoint& pt)
{
    CWnd* pParent = GetParent();
    if (!pParent || !::IsWindow(pParent->m_hWnd))
    {
        return;
    }

    CRect rc;
    GetWindowRect(rc);
    pParent->ScreenToClient(rc);

    //  calc the new rect
    if (m_dwSplitterStyle&SPS_VERTICAL)
    {
        rc.OffsetRect((pt.x - (rc.left + rc.right) / 2), 0);
    }
    else
    {
        rc.OffsetRect(0, (pt.y - (rc.top + rc.bottom) / 2));
    }

    MoveWindow(rc);
}

void    CSplitterControl::GetMaxMinPos(int& iMax, int& iMin)
{
    CWnd* pParent = GetParent();
    ASSERT(pParent);
    ASSERT(IsWindow(pParent->m_hWnd));
    
    CRect rcParent;
    pParent->GetClientRect(rcParent);
    
    //  try to get the max/min pos limit from parent window
    SPC_NM_MAXMINPOS  nmMinmax;
    nmMinmax.hdr.hwndFrom   = m_hWnd;
    nmMinmax.hdr.idFrom     = GetDlgCtrlID();
    nmMinmax.hdr.code       = SPN_MAXMINPOS;
    nmMinmax.lMax           = (m_dwSplitterStyle&SPS_VERTICAL)?rcParent.right:rcParent.bottom;
    nmMinmax.lMin           = (m_dwSplitterStyle&SPS_VERTICAL)?rcParent.left:rcParent.top;
    (void)pParent->SendMessage(WM_NOTIFY, nmMinmax.hdr.idFrom, (LPARAM)&nmMinmax);
    
    //  return
    iMax = nmMinmax.lMax;
    iMin = nmMinmax.lMin;
}

BOOL CSplitterControl::RegisterLinkedWindow(DWORD  dwLinkSide, CWnd* pWnd)
{
    //  check parameters
    if (NULL == pWnd)
    {
        return (FALSE);
    }

    //  check parameters
    if ((SPLS_LINKED_LEFT != dwLinkSide) && (SPLS_LINKED_RIGHT != dwLinkSide))
    {
        return (FALSE);
    }

    //  make sure the spliiter and pWnd have same parent
    if (pWnd->GetParent()->m_hWnd != this->GetParent()->m_hWnd)
    {
        return (FALSE);
    }

    //  save it
    POSITION pos = m_listLinkedWnds[dwLinkSide].AddTail(pWnd);
    if (NULL == pos)
    {
        return (FALSE);
    }

    return (TRUE);
}

void CSplitterControl::Relayout()
{
    CWnd* pParent = GetParent();
    ASSERT(pParent);
    ASSERT(IsWindow(pParent->m_hWnd));

    CRect rcSelf;
    this->GetWindowRect(rcSelf);

    //  relayout all registered window
    for (int i = 0; i < 2; i++)
    {
        for (POSITION pos = this->m_listLinkedWnds[i].GetHeadPosition(); NULL != pos;)
        {
            //  get the window object
            CWnd* pWnd = this->m_listLinkedWnds[i].GetNext(pos);
            if (NULL == pWnd)
            {
                continue;
            }
    
            //  calc the new pos(the code is not very good)
            CRect rcLinked;
            pWnd->GetWindowRect(rcLinked);
            if (SPS_VERTICAL&m_dwSplitterStyle)
            {
                if (SPLS_LINKED_LEFT == i)
                {
                    rcLinked.right  = rcSelf.left;
                }
                else
                {
                    rcLinked.left   = rcSelf.right;
                }
            }
            else
            {
                if (SPLS_LINKED_LEFT == i)
                {
                    rcLinked.bottom = rcSelf.top;
                }
                else
                {
                    rcLinked.top    = rcSelf.bottom;
                }
            }

            //  move it to new pos and then update
            pParent->ScreenToClient(rcLinked);
            pWnd->MoveWindow(rcLinked, TRUE);
            pWnd->Invalidate();
        }
    }
}

void CSplitterControl::ChangePos(CWnd* pWnd, DWORD dwLinkedSide, LONG lDelta)
{
    if (NULL == pWnd)
    {
        return;
    }

    if (FALSE == ::IsWindow(pWnd->GetSafeHwnd()))
    {
        return;
    }

    CWnd* pParent = pWnd->GetParent();
    if (NULL == pParent)
    {
        return;
    }

    if (FALSE == ::IsWindow(pParent->m_hWnd))
    {
        return;
    }

    CRect rcWnd;
    pWnd->GetWindowRect(rcWnd);
    pParent->ScreenToClient(rcWnd);

    if (SPS_VERTICAL & m_dwSplitterStyle)
    {
        if (SPLS_LINKED_LEFT & dwLinkedSide)    //  Same as SPLS_LINKED_UP
        {
            rcWnd.right += lDelta;
        }
        else
        {
            rcWnd.left  += lDelta;
        }
    }
    else
    {
        if (SPLS_LINKED_UP & dwLinkedSide)      //  Same as SPLS_LINKED_LEFT
        {
            rcWnd.bottom += lDelta;
        }
        else
        {
            rcWnd.top    += lDelta;
        }
    }
    
    pWnd->MoveWindow(rcWnd, TRUE);
}

void CSplitterControl::ChangeHeight(CWnd *pWnd, int dy)
{
	CWnd* pParent = pWnd->GetParent();
	if (pParent && ::IsWindow(pParent->m_hWnd))
	{
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		pParent->ScreenToClient(rcWnd);
		rcWnd.bottom += dy;
		pWnd->MoveWindow(rcWnd);
	}
}

void CSplitterControl::ChangeWidth(CWnd *pWnd, int dx)
{
	CWnd* pParent = pWnd->GetParent();
	if (pParent && ::IsWindow(pParent->m_hWnd))
	{
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		pParent->ScreenToClient(rcWnd);
		rcWnd.right += dx;
		pWnd->MoveWindow(rcWnd);
	}
}