///////////////////////////////////////////////////////////////////////////////
//
// ToolBarXP.cpp: implementation of the CToolBarXP class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBarXP.h"
#include "Tools.h"
#include "Draw.h"
#include "MenuXP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// From <afximpl.h>
struct AUX_DATA
{
	int _unused1, _unused2;
	int _unused3, _unused4;
	int cxBorder2, cyBorder2;
};

extern __declspec(dllimport) AUX_DATA afxData;

class INIT_afxData
{
public:
    INIT_afxData ()
    {
        afxData.cxBorder2 = afxData.cyBorder2 = 0;
    }
} g_afxData2;

using namespace std;
///////////////////////////////////////////////////////////////////////////////
CToolBarXP::CToolBarXP () : m_bCheckVisibility (false)
{
	m_vecStr.clear();
}

///////////////////////////////////////////////////////////////////////////////
BOOL CToolBarXP::LoadToolBar (UINT nIDResource)
{
    if ( !CToolBar::LoadToolBar (nIDResource) )
    {
        return false;
    }
    SetBorders (CRect(1,3,6,4));
    SendMessage (TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
bool CToolBarXP::InsertControl (int nIndex, CWnd& Ctrl)
{
    TBBUTTON tbbutton;
    int nCount = DefWindowProc (TB_BUTTONCOUNT, 0, 0);
    CWindowRect rcCtrl (Ctrl);

    tbbutton.iBitmap = rcCtrl.Width();
    tbbutton.idCommand = Ctrl.GetDlgCtrlID();
    tbbutton.fsState = 0;
    tbbutton.fsStyle = TBSTYLE_SEP;
    tbbutton.bReserved[0] = 0;
    tbbutton.bReserved[1] = 0;
    tbbutton.iString = -1;
    tbbutton.dwData = NULL;

    if ( nIndex < 0 || nIndex > nCount )
    {
        nIndex = nCount;
    }
    if ( GetToolBarCtrl().InsertButton (nIndex, &tbbutton) )
    {
        m_bDelayedButtonLayout = true;

        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void CToolBarXP::TrackPopupMenu (int nID, CMenu* pMenu)
{
    CRect rcTBItem;
    int nTBIndex = CommandToIndex (nID);
    TPMPARAMS tpmp = { sizeof TPMPARAMS };
    
    GetItemRect (nTBIndex, rcTBItem);
    ClientToScreen (rcTBItem);
    CopyRect (&tpmp.rcExclude, rcTBItem);
    rcTBItem.OffsetRect (4, 0);
    CMenuXP::SetMRUMenuBarItem (rcTBItem);
    SetButtonStyle (nTBIndex, GetButtonStyle (nTBIndex)|TBBS_INDETERMINATE);
    UpdateWindow();
    ::TrackPopupMenuEx (pMenu->GetSafeHmenu(), TPM_LEFTBUTTON, tpmp.rcExclude.left, tpmp.rcExclude.bottom, GetParentFrame()->GetSafeHwnd(), &tpmp);
    SetButtonStyle (nTBIndex, GetButtonStyle (nTBIndex)&~TBBS_INDETERMINATE);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT CToolBarXP::DefWindowProc (UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = CToolBar::DefWindowProc (nMsg, wParam, lParam);

    if ( m_bCheckVisibility && nMsg == TB_GETBUTTON )
    {
        TBBUTTON* pButton = (TBBUTTON*)lParam;

        if ( IS_CONTROL(*pButton) )
        {
            if ( m_bCheckVisibility == 1 ) pButton->fsState |=  TBSTATE_HIDDEN;
            else                           pButton->fsState &= ~TBSTATE_HIDDEN;
        }
    }
    return lRes;
}

///////////////////////////////////////////////////////////////////////////////
CSize CToolBarXP::CalcDynamicLayout (int nLength, DWORD dwMode)
{
    bool bHideControls = (dwMode & LM_VERTDOCK) == LM_VERTDOCK;
    m_bCheckVisibility = (BYTE)(bHideControls ? 1 : 2);
    CSize size = CToolBar::CalcDynamicLayout (nLength, dwMode);
    m_bCheckVisibility = false;

    if ( dwMode & LM_COMMIT )
    {
        TBBUTTON tbbutton;
        int nCount = DefWindowProc (TB_BUTTONCOUNT, 0, 0);

        for ( int i = 0; i < nCount; i++ )
        {
            VERIFY(DefWindowProc (TB_GETBUTTON, i, (LPARAM)&tbbutton));

            if ( !IS_CONTROL(tbbutton) )
            {
                continue;
            }
            CWnd* pWnd = GetDlgItem (tbbutton.idCommand);

            if ( pWnd == NULL )
            {
                continue;
            }
            if ( bHideControls )
            {
                GetToolBarCtrl().HideButton (tbbutton.idCommand, true);
                pWnd->ShowWindow (SW_HIDE);
            }
            else
            {
                GetToolBarCtrl().HideButton (tbbutton.idCommand, false);

                // Update control position
                CRect rcControl;

                VERIFY (DefWindowProc (TB_GETITEMRECT, i, (LPARAM)&rcControl));
                rcControl.DeflateRect (1, 1);
                
                CWindowRect rcCtrl (pWnd);
                int         nNoSize = 0;

                if ( rcControl.Width() == rcCtrl.Width() )
                {
                    nNoSize = SWP_NOSIZE;
                }
                pWnd->SetWindowPos (NULL, rcControl.left, rcControl.top, rcControl.Width(), rcControl.Height(),
                                    nNoSize|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
            }
        }
    }
    return size;
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CToolBarXP, CToolBar)
    //{{AFX_MSG_MAP(CToolBarXP)
    ON_WM_PAINT()
    ON_NOTIFY_REFLECT(TBN_DROPDOWN, OnButtonDropDown)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CToolBarXP, CToolBar)

///////////////////////////////////////////////////////////////////////////////
// Paint the toolbar
void CToolBarXP::OnPaint ()
{
    if ( m_bDelayedButtonLayout )
    {
        Layout();
    }
    CPaintDC cpDC (this);
    CBufferDC cDC (cpDC);
    CRect rcClip;
    cDC.GetClipBox (rcClip);
    cDC.SetBkMode (TRANSPARENT);
    cDC.SelectObject (CFont::FromHandle ((HFONT)GetStockObject (DEFAULT_GUI_FONT)));

    cDC.FillSolidRect (rcClip, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), 20, 0));

    CPoint ptCursor;
    ::GetCursorPos (&ptCursor);
    ScreenToClient (&ptCursor);

    CClientRect rcClient (this);
    HIMAGELIST m_hImageList = (HIMAGELIST)DefWindowProc (TB_GETIMAGELIST, 0, 0);
    TBBUTTON tbbutton;
    int nCount = DefWindowProc (TB_BUTTONCOUNT, 0, 0);
    int nHotItem = GetToolBarCtrl().GetHotItem();

    for ( int i = 0; i < nCount; i++ )
    {
        VERIFY(DefWindowProc (TB_GETBUTTON, i, (LPARAM)&tbbutton));

        if ( !IS_VISIBLE(tbbutton) )
        {
            continue;
        }
        CRect rcButton;
        VERIFY(DefWindowProc (TB_GETITEMRECT, i, (LPARAM)&rcButton));

        if ( !CRect().IntersectRect (rcClip, rcButton) )
        {
            continue;
        }
        bool bOver = nHotItem == i && IS_ENABLED(tbbutton);
        bool bPressed = false;

        if ( IS_INDETERMINATE(tbbutton) )
        {
            CPenDC pen (cDC, ::GetSysColor (COLOR_3DDKSHADOW));

            cDC.MoveTo (rcButton.left, rcButton.bottom);
            cDC.LineTo (rcButton.left, rcButton.top);
            cDC.LineTo (rcButton.right-1, rcButton.top);
            cDC.LineTo (rcButton.right-1, rcButton.bottom-1);
            cDC.LineTo (rcButton.left, rcButton.bottom-1);
            bOver = true;
        }
        else if ( bOver || IS_CHECKED(tbbutton) )
        {
            bPressed = KEYDOWN(VK_LBUTTON) && rcButton.PtInRect (ptCursor);

            if ( IS_DROPDOWN(tbbutton) && bPressed )
            {
                bPressed = ptCursor.x < rcButton.right-13;

                if ( bPressed )
                {
                    rcButton.right -= 13;
                }
            }
            COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
            CPenDC pen (cDC, crHighLight);
            CBrushDC brush (cDC, bPressed||(bOver&&IS_CHECKED(tbbutton)) ? HLS_TRANSFORM (crHighLight, +50, -50) : (bOver ? HLS_TRANSFORM (crHighLight, +70, -57) : HLS_TRANSFORM (crHighLight, +80, -66)));

            cDC.Rectangle (&rcButton);

            if ( IS_DROPDOWN(tbbutton) )
            {
                if ( bPressed )
                {
                    int nLeft = rcButton.left;

                    rcButton.left = rcButton.right-1;
                    rcButton.right += 13;

                    brush.Color (HLS_TRANSFORM (crHighLight, +70, -66));
                    cDC.Rectangle (&rcButton);

                    rcButton.left = nLeft;
                }
                else
                {
                    cDC.MoveTo (rcButton.right-14, rcButton.top);
                    cDC.LineTo (rcButton.right-14, rcButton.bottom);
                }
            }
        }
        if ( IS_SEPARATOR(tbbutton) )
        {
            CPenDC pen (cDC, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -15, 0));

            if ( IS_WRAP(tbbutton) )
            {
                cDC.MoveTo (rcClient.left+2, rcButton.bottom-4);
                cDC.LineTo (rcClient.right-2, rcButton.bottom-4);
            }
            else
            {
                cDC.MoveTo ((rcButton.right+rcButton.left)/2-1, rcButton.top+2);
                cDC.LineTo ((rcButton.right+rcButton.left)/2-1, rcButton.bottom-2);
            }
        }
        else if ( !IS_CONTROL(tbbutton) )
        {
            if ( IS_DROPDOWN(tbbutton) )
            {
                CPenDC pen (cDC, ( bOver && !IS_INDETERMINATE(tbbutton) ) ? RGB(0,0,0) : ::GetSysColor (IS_ENABLED(tbbutton) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));

                cDC.MoveTo (rcButton.right-9, (rcButton.top+rcButton.bottom)/2-1);
                cDC.LineTo (rcButton.right-4, (rcButton.top+rcButton.bottom)/2-1);
                cDC.MoveTo (rcButton.right-8, (rcButton.top+rcButton.bottom)/2);
                cDC.LineTo (rcButton.right-5, (rcButton.top+rcButton.bottom)/2);
                cDC.SetPixel (rcButton.right-7, (rcButton.top+rcButton.bottom)/2+1, pen.Color());

                rcButton.right -= 14;
            }
            if ( tbbutton.iBitmap >= 0 )
            {
                if ( !IS_ENABLED(tbbutton) || (bOver && !bPressed) )
                {
                    HICON hIcon = ImageList_ExtractIcon (NULL, m_hImageList, tbbutton.iBitmap);
                    cDC.DrawState (CPoint (rcButton.left + ( bOver ? 4 : 3 ), rcButton.top + ( bOver ? 4 : 3 )), m_sizeImage, hIcon, DSS_MONO, CBrush (bOver ? (IS_INDETERMINATE(tbbutton) ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -20, 0) : HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +50, -66)) : HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0)));
                    DestroyIcon (hIcon);
                }
                if ( IS_ENABLED(tbbutton) )
                {
                    ::ImageList_Draw (m_hImageList, tbbutton.iBitmap, cDC.m_hDC,
                                      rcButton.left + ( (bOver && !bPressed) ? 2 : 3 ), rcButton.top + ( (bOver && !bPressed) ? 2 : 3 ), ILD_TRANSPARENT);
					CRect rcText = rcButton;
					rcText.left += 23;
					rcText.top += 5;
					
					if(i < m_vecStr.size())
						cDC.DrawText(m_vecStr[i], rcText, DT_VCENTER);
					if(i == 7) {
				/*		HICON hIcon = reinterpret_cast<HICON>(
							::LoadImage ( AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_NAVIGATE_GOTO),
								IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ));
						cDC.DrawState (CPoint (rcButton.left + ( bOver ? 4 : 3 ), rcButton.top + ( bOver ? 4 : 3 )), m_sizeImage, hIcon, DSS_MONO, CBrush (bOver ? (IS_INDETERMINATE(tbbutton) ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -20, 0) : HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +50, -66)) : HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0)));
						CImageList imglist;
						imglist.Create(16, 16, ILC_COLOR32, 1, 1);
						imglist.Add(hIcon);
						::ImageList_Draw (imglist.m_hImageList, tbbutton.iBitmap, cDC.m_hDC,
                                      rcButton.left + ( (bOver && !bPressed) ? 2 : 3 ), rcButton.top + ( (bOver && !bPressed) ? 2 : 3 ), ILD_TRANSPARENT);
						DestroyIcon (hIcon);*/
						cDC.DrawText(m_vecStr[4], rcText, DT_VCENTER);
					}
                }
            }
        }
    }
}

void CToolBarXP::SetButtonTextArray(std::vector<CString>& vecStr)
{
	for(vector<CString>::iterator iter = vecStr.begin();
			iter != vecStr.end(); ++iter)
	{
		m_vecStr.push_back(*iter);
	}
}

///////////////////////////////////////////////////////////////////////////////
void CToolBarXP::OnButtonDropDown (NMHDR* lpnmhdr, LRESULT* plResult)
{
    LPNMTOOLBAR lpnmtb = (LPNMTOOLBAR)lpnmhdr;

    *plResult = TBDDRET_DEFAULT;

    if ( !m_pDockSite->SendMessage (WM_COMMAND, DROPDOWN(lpnmtb->iItem)) )
    {
        *plResult = TBDDRET_TREATPRESSED;
    }
}
