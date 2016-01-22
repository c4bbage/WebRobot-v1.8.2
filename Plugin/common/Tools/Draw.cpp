///////////////////////////////////////////////////////////////////////////////
//
// Draw.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tools.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
HLSCOLOR RGB2HLS (COLORREF rgb)
{
    unsigned char minval = min(GetRValue(rgb), min(GetGValue(rgb), GetBValue(rgb)));
    unsigned char maxval = max(GetRValue(rgb), max(GetGValue(rgb), GetBValue(rgb)));
    float mdiff  = float(maxval) - float(minval);
    float msum   = float(maxval) + float(minval);
   
    float luminance = msum / 510.0f;
    float saturation = 0.0f;
    float hue = 0.0f; 

    if ( maxval != minval )
    { 
        float rnorm = (maxval - GetRValue(rgb)  ) / mdiff;      
        float gnorm = (maxval - GetGValue(rgb)) / mdiff;
        float bnorm = (maxval - GetBValue(rgb) ) / mdiff;   

        saturation = (luminance <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

        if (GetRValue(rgb) == maxval) hue = 60.0f * (6.0f + bnorm - gnorm);
        if (GetGValue(rgb) == maxval) hue = 60.0f * (2.0f + rnorm - bnorm);
        if (GetBValue(rgb) == maxval) hue = 60.0f * (4.0f + gnorm - rnorm);
        if (hue > 360.0f) hue = hue - 360.0f;
    }
    return HLS ((hue*255)/360, luminance*255, saturation*255);
}

///////////////////////////////////////////////////////////////////////////////
static BYTE _ToRGB (float rm1, float rm2, float rh)
{
  if      (rh > 360.0f) rh -= 360.0f;
  else if (rh <   0.0f) rh += 360.0f;
 
  if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
  else if (rh < 180.0f) rm1 = rm2;
  else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
                   
  return (BYTE)(rm1 * 255);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF HLS2RGB (HLSCOLOR hls)
{
    float hue        = ((int)HLS_H(hls)*360)/255.0f;
    float luminance  = HLS_L(hls)/255.0f;
    float saturation = HLS_S(hls)/255.0f;

    if ( saturation == 0.0f )
    {
        return RGB (HLS_L(hls), HLS_L(hls), HLS_L(hls));
    }
    float rm1, rm2;
     
    if ( luminance <= 0.5f ) rm2 = luminance + luminance * saturation;  
    else                     rm2 = luminance + saturation - luminance * saturation;
    rm1 = 2.0f * luminance - rm2;   
    BYTE red   = _ToRGB (rm1, rm2, hue + 120.0f);   
    BYTE green = _ToRGB (rm1, rm2, hue);
    BYTE blue  = _ToRGB (rm1, rm2, hue - 120.0f);

    return RGB (red, green, blue);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S)
{
    HLSCOLOR hls = RGB2HLS (rgb);
    BYTE h = HLS_H(hls);
    BYTE l = HLS_L(hls);
    BYTE s = HLS_S(hls);

    if ( percent_L > 0 )
    {
        l = BYTE(l + ((255 - l) * percent_L) / 100);
    }
    else if ( percent_L < 0 )
    {
        l = BYTE((l * (100+percent_L)) / 100);
    }
    if ( percent_S > 0 )
    {
        s = BYTE(s + ((255 - s) * percent_S) / 100);
    }
    else if ( percent_S < 0 )
    {
        s = BYTE((s * (100+percent_S)) / 100);
    }
    return HLS2RGB (HLS(h, l, s));
}

/////////////////////////////////////////////////////////////////////////////
HBITMAP WINAPI GetScreenBitmap (LPCRECT pRect)
{
    HDC     hDC;
    HDC     hMemDC;
    HBITMAP hNewBitmap = NULL;

    if ( (hDC = ::GetDC (NULL)) != NULL )
    {
        if ( (hMemDC = ::CreateCompatibleDC (hDC)) != NULL )
        {
            if ( (hNewBitmap = ::CreateCompatibleBitmap (hDC, pRect->right - pRect->left, pRect->bottom - pRect->top)) != NULL )
            {
                HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hNewBitmap);
                ::BitBlt (hMemDC, 0, 0, pRect->right - pRect->left, pRect->bottom - pRect->top,
                          hDC, pRect->left, pRect->top, SRCCOPY);
                ::SelectObject (hMemDC, (HGDIOBJ) hOldBitmap);
            }
            ::DeleteDC (hMemDC);
        }
        ::ReleaseDC (NULL, hDC);
    }
    return hNewBitmap;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CBufferDC::CBufferDC (HDC hDestDC, const CRect& rcPaint) : m_hDestDC (hDestDC)
{
    if ( rcPaint.IsRectEmpty() )
    {
        ::GetClipBox (m_hDestDC, m_rect);
    }
    else
    {
        m_rect = rcPaint;
    }
    VERIFY(Attach (::CreateCompatibleDC (m_hDestDC)));
    m_bitmap.Attach (::CreateCompatibleBitmap (m_hDestDC, m_rect.right, m_rect.bottom));
    m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);

    if ( m_rect.top > 0 )
    {
        ExcludeClipRect (0, 0, m_rect.right, m_rect.top);
    }
    if ( m_rect.left > 0 )
    {
        ExcludeClipRect (0, m_rect.top, m_rect.left, m_rect.bottom);
    }
}

///////////////////////////////////////////////////////////////////////////////
CBufferDC::~CBufferDC ()
{
    VERIFY(::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY));
    ::SelectObject (m_hDC, m_hOldBitmap);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CPenDC::CPenDC (HDC hDC, COLORREF crColor) : m_hDC (hDC)
{
    VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

///////////////////////////////////////////////////////////////////////////////
CPenDC::~CPenDC ()
{
    ::SelectObject (m_hDC, m_hOldPen);
}

///////////////////////////////////////////////////////////////////////////////
void CPenDC::Color (COLORREF crColor)
{
    ::SelectObject (m_hDC, m_hOldPen);
    VERIFY(m_pen.DeleteObject());
    VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
    m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF CPenDC::Color () const
{
    LOGPEN logPen;

    ((CPenDC*)this)->m_pen.GetLogPen (&logPen);

    return logPen.lopnColor;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CBrushDC::CBrushDC (HDC hDC, COLORREF crColor) : m_hDC (hDC)
{
    if ( crColor == CLR_NONE ) VERIFY(m_brush.CreateStockObject (NULL_BRUSH));
    else                       VERIFY(m_brush.CreateSolidBrush (crColor));
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

///////////////////////////////////////////////////////////////////////////////
CBrushDC::~CBrushDC ()
{
    ::SelectObject (m_hDC, m_hOldBrush);
}

///////////////////////////////////////////////////////////////////////////////
void CBrushDC::Color (COLORREF crColor)
{
    ::SelectObject (m_hDC, m_hOldBrush);
    VERIFY(m_brush.DeleteObject());
    if ( crColor == CLR_NONE ) VERIFY(m_brush.CreateStockObject (NULL_BRUSH));
    else                       VERIFY(m_brush.CreateSolidBrush (crColor));
    m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF CBrushDC::Color () const
{
    LOGBRUSH logBrush;

    ((CBrushDC*)this)->m_brush.GetLogBrush (&logBrush);

    return logBrush.lbColor;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CFontDC::CFontDC (HDC hDC, LPCTSTR sFaceName, COLORREF crText)
        : m_hDC (hDC), m_hFont (NULL), m_hDefFont (NULL), m_crTextOld (CLR_NONE)
{
    *this = sFaceName;

    if ( crText != CLR_DEFAULT )
    {
        *this = crText;
    }
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::CFontDC (HDC hDC, BYTE nStockFont, COLORREF crText)
        : m_hDC (hDC), m_hFont (NULL), m_hDefFont (NULL), m_crTextOld (CLR_NONE)
{
    *this = nStockFont;

    if ( crText != CLR_DEFAULT )
    {
        *this = crText;
    }
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::CFontDC (HDC hDC, HFONT hFont, COLORREF crText)
        : m_hDC (hDC), m_hFont (NULL), m_hDefFont (NULL), m_crTextOld (CLR_NONE)
{
    *this = hFont;

    if ( crText != CLR_DEFAULT )
    {
        *this = crText;
    }
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::~CFontDC ()
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
        DeleteObject (m_hFont);
    }
    if ( m_crTextOld != CLR_NONE )
    {
        ::SetTextColor (m_hDC, m_crTextOld);
    }
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (LPCTSTR sFaceName)
{
    LOGFONT lf;

	::GetObject (::GetCurrentObject (m_hDC, OBJ_FONT), sizeof(LOGFONT), &lf);

    if ( _tcsicmp (sFaceName, lf.lfFaceName) )
    {
        if ( m_hDefFont != NULL )
        {
            ::SelectObject (m_hDC, m_hDefFont);
            DeleteObject (m_hFont);
        }
        _tcscpy (lf.lfFaceName, sFaceName);
        m_hFont = ::CreateFontIndirect (&lf);
        m_hDefFont = (HFONT)::SelectObject (m_hDC, m_hFont);
    }
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (BYTE nStockFont)
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
        DeleteObject (m_hFont);
    }
    m_hFont = (HFONT)::GetStockObject (nStockFont);
    m_hDefFont = (HFONT)::SelectObject (m_hDC, m_hFont);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (HFONT hFont)
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
        DeleteObject (m_hFont);
    }
    m_hFont = hFont;
    m_hDefFont = (HFONT)::SelectObject (m_hDC, m_hFont);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
const CFontDC& CFontDC::operator = (COLORREF crText)
{
    if ( m_crTextOld == CLR_NONE )
    {
        m_crTextOld = ::GetTextColor (m_hDC);
    }
    ::SetTextColor (m_hDC, crText);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::operator LPCTSTR ()
{
    LOGFONT lf;

	::GetObject (::GetCurrentObject (m_hDC, OBJ_FONT), sizeof(LOGFONT), &lf);

    return lf.lfFaceName;
}

/////////////////////////////////////////////////////////////////////////////
CFontDC::operator COLORREF ()
{
    return ::GetTextColor (m_hDC);
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CBoldDC::CBoldDC (HDC hDC, bool bBold) : m_hDC (hDC), m_hDefFont (NULL)
{
    LOGFONT lf;

    CFont::FromHandle((HFONT)::GetCurrentObject(m_hDC, OBJ_FONT))->GetLogFont (&lf);

    if ( ( bBold && lf.lfWeight != FW_BOLD) ||
         (!bBold && lf.lfWeight == FW_BOLD) )
    {
        lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

        m_fontBold.CreateFontIndirect (&lf);
        m_hDefFont = (HFONT)::SelectObject (m_hDC, m_fontBold);
    }
}

/////////////////////////////////////////////////////////////////////////////
CBoldDC::~CBoldDC ()
{
    if ( m_hDefFont != NULL )
    {
        ::SelectObject (m_hDC, m_hDefFont);
    }
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define DB_GETTYPE(Style)       ((Style)&0x00FF)
#define DB_ISENABLED(Style)     (!((Style)&DB_DISABLED))
#define DB_ISBORDER(Style)      (((Style)&DB_BORDER)==DB_BORDER)
#define DB_ISWINDOWDC(Style)    (((Style)&DB_WINDOWDC)==DB_WINDOWDC)
#define DB_ISFLAT(Style)        (((Style)&DB_FLAT)==DB_FLAT)
#define DB_ISPRESSED(Style)     (((Style)&(DB_PRESSED|DB_PRESSED2))!=0)
#define DB_ISPRESSED1(Style)    (((Style)&DB_PRESSED)==DB_PRESSED)
#define DB_ISPRESSED2(Style)    (((Style)&DB_PRESSED2)==DB_PRESSED2)
#define DB_ISOVER(Style)        (((Style)&DB_OVER)==DB_OVER)
#define DB_ISTRANSPARENT(Style) (((Style)&DB_TRANSPARENT)==DB_TRANSPARENT)

/////////////////////////////////////////////////////////////////////////////
static void _DrawTriangle (CDC* pDC, int x, int y, int nSize, bool bDown)
{
    for ( int i = 0; i < nSize; i++ )
    {
        pDC->MoveTo (x-i, bDown ? y-i : y+i);
        pDC->LineTo (x+i+1, bDown ? y-i : y+i);
    }
}

/////////////////////////////////////////////////////////////////////////////
void CDrawButton::Draw (CDC* pDC, DWORD wStyle) const
{
    if ( wStyle == DB_DEFAULT )
    {
        wStyle = m_wStyle;
    }
    ASSERT (pDC != NULL);
    CRect rc (m_Rect);
    CPenDC pen (pDC->m_hDC, ::GetSysColor (COLOR_3DFACE));
    CBrushDC brush (pDC->m_hDC, ::GetSysColor (COLOR_3DFACE));

    if ( DB_ISBORDER(m_wStyle) && !DB_ISFLAT(wStyle) )
    {
        pDC->MoveTo (rc.right-1, rc.top);
        pDC->LineTo (rc.left, rc.top);
        pDC->LineTo (rc.left, rc.bottom);
        pDC->SetPixel (rc.right-1, rc.top, ::GetSysColor (COLOR_3DDKSHADOW));
        rc.left++;
        rc.top++;
    }
    COLORREF crBorder = 0, crPressed = 0, crOver = 0, crText = 0;

    if ( DB_ISFLAT(wStyle) )
    {
        crBorder = ::GetSysColor (COLOR_HIGHLIGHT);
        crPressed = HLS_TRANSFORM (crBorder, +50, -50);
        crOver = HLS_TRANSFORM (crBorder, +70, -57);
    }
    if ( DB_GETTYPE(wStyle) == DB_UPDOWN )
    {
        if ( !DB_ISPRESSED2(wStyle) )
        {
            if ( DB_ISFLAT(wStyle) )
            {
                CRect rcBtn (rc.left, rc.top, rc.right, (rc.top+rc.bottom)/2+1);

                if ( DB_ISENABLED(wStyle) && DB_ISOVER(wStyle) )
                {
                    pen.Color (crBorder);
                    brush.Color (DB_ISPRESSED(wStyle) ? crPressed : crOver);
                    pDC->Rectangle (rcBtn);
                }
                else if ( !DB_ISTRANSPARENT(wStyle) )
                {
                    pen.Color (::GetSysColor (DB_ISBORDER(m_wStyle) ? COLOR_3DSHADOW : COLOR_WINDOW));
                    pDC->Rectangle (rcBtn);
                }
            }
            else
            {
                CRect rcBtn (rc.left, rc.top, rc.right, (rc.top+rc.bottom)/2);

                pDC->DrawFrameControl (rcBtn, DFC_BUTTON, DFCS_BUTTONPUSH|(DB_ISPRESSED1(wStyle) ? DFCS_PUSHED : 0));
            }
        }
        if ( !DB_ISPRESSED(wStyle) && !DB_ISFLAT(wStyle) )
        {
            pDC->MoveTo (rc.left, (rc.top+rc.bottom)/2);
            pDC->LineTo (rc.right, (rc.top+rc.bottom)/2);
        }
        if ( !DB_ISPRESSED1(wStyle) )
        {
            if ( DB_ISFLAT(wStyle) )
            {
                CRect rcBtn (rc.left, (rc.top+rc.bottom)/2/*+rc.Height()%2*/, rc.right, rc.bottom);

                if ( DB_ISENABLED(wStyle) && DB_ISOVER(wStyle) )
                {
                    pen.Color (crBorder);
                    brush.Color (DB_ISPRESSED(wStyle) ? crPressed : crOver);
                    pDC->Rectangle (rcBtn);
                }
                else if ( !DB_ISTRANSPARENT(wStyle) )
                {
                    pen.Color (::GetSysColor (DB_ISBORDER(m_wStyle) ? COLOR_3DSHADOW : COLOR_WINDOW));
                    pDC->Rectangle (rcBtn);
                }
            }
            else
            {
                CRect rcBtn (rc.left, (rc.top+rc.bottom)/2+rc.Height()%2, rc.right, rc.bottom);

                pDC->DrawFrameControl (rcBtn, DFC_BUTTON, DFCS_BUTTONPUSH|(DB_ISPRESSED2(wStyle) ? DFCS_PUSHED : 0));
            }
        }
        if ( DB_ISFLAT(wStyle) )
        {
            crText = DB_ISENABLED(wStyle) && pDC->GetPixel (rc.left+1, rc.top+1) == crOver ? RGB(0,0,0) : ::GetSysColor (COLOR_BTNTEXT);
            pen.Color (DB_ISPRESSED(wStyle) ? RGB(240,240,240) : DB_ISENABLED(wStyle) ? crText : ::GetSysColor (COLOR_GRAYTEXT));
        }
        else
        {
            pen.Color (::GetSysColor (DB_ISENABLED(wStyle) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
        }
        if ( !DB_ISPRESSED2(wStyle) )
        {
            _DrawTriangle (pDC, (rc.left+rc.right)/2 + ((DB_ISPRESSED1(wStyle)&&!DB_ISFLAT(wStyle)) ? 1 : 0),
                                (rc.top*3+rc.bottom)/4 + ((DB_ISPRESSED1(wStyle)||DB_ISFLAT(wStyle)) ? 1 : 0) - 2, 3, false);
        }
        if ( !DB_ISPRESSED1(wStyle) )
        {
            _DrawTriangle (pDC, (rc.left+rc.right)/2 + ((DB_ISPRESSED2(wStyle)&&!DB_ISFLAT(wStyle)) ? 1 : 0),
                                (rc.top+rc.bottom*3)/4 + ((DB_ISPRESSED2(wStyle)&&!DB_ISFLAT(wStyle)) ? 1 : 0) + 1, 3, true);
        }
    }
    else
    {
        if ( DB_ISFLAT(wStyle) )
        {
            if ( DB_ISENABLED(wStyle) && DB_ISOVER(wStyle) )
            {
                pen.Color (crBorder);
                brush.Color (DB_ISPRESSED(wStyle) ? crPressed : crOver);
                pDC->Rectangle (rc);
            }
            else if ( !DB_ISTRANSPARENT(wStyle) )
            {
                pen.Color (::GetSysColor (DB_ISBORDER(m_wStyle) ? COLOR_3DSHADOW : COLOR_WINDOW));
                pDC->Rectangle (rc);
            }
        }
        else
        {
            pDC->DrawFrameControl (rc, DFC_BUTTON, DFCS_BUTTONPUSH|(DB_ISPRESSED(wStyle) ? DFCS_PUSHED : 0));
        }
        switch ( DB_GETTYPE(wStyle) )
        {
            case DB_3POINTS:
                if ( DB_ISFLAT(wStyle) )
                {
                    crText = DB_ISENABLED(wStyle) && pDC->GetPixel (rc.left+1, rc.top+1) == crOver ? RGB(0,0,0) : ::GetSysColor (COLOR_BTNTEXT);
                    crText = DB_ISPRESSED(wStyle) ? RGB(240,240,240) : DB_ISENABLED(wStyle) ? crText : ::GetSysColor (COLOR_GRAYTEXT);
                }
                else
                {
                    crText = ::GetSysColor (DB_ISENABLED(wStyle) ? COLOR_BTNTEXT : COLOR_GRAYTEXT);
                }
                crText = pDC->SetTextColor (crText);
                pDC->SetBkMode (TRANSPARENT);
                pDC->DrawText (_T("..."), 3, CRect(rc.left + ((DB_ISPRESSED(wStyle)||DB_ISFLAT(wStyle)) ? 2 : 1),
                                                   rc.top, rc.right, rc.bottom + ((DB_ISPRESSED(wStyle)&&!DB_ISFLAT(wStyle)) ? 1 : 0)),
                               DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
                pDC->SetTextColor (crText);
                break;

            case DB_UP:
            case DB_DOWN:
                if ( DB_ISFLAT(wStyle) )
                {
                    crText = DB_ISENABLED(wStyle) && pDC->GetPixel (rc.left+1, rc.top+1) == crOver ? RGB(0,0,0) : ::GetSysColor (COLOR_BTNTEXT);
                    pen.Color (DB_ISPRESSED(wStyle) ? RGB(240,240,240) : DB_ISENABLED(wStyle) ? crText : ::GetSysColor (COLOR_GRAYTEXT));
                }
                else
                {
                    pen.Color (::GetSysColor (DB_ISENABLED(wStyle) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
                }
                _DrawTriangle (pDC, (rc.left+rc.right)/2 + ((DB_ISPRESSED(wStyle)&&!DB_ISFLAT(wStyle)) ? 1 : 0),
                                    (rc.top+rc.bottom)/2 + ((DB_ISPRESSED(wStyle)||DB_ISFLAT(wStyle)) ? 2 : 1) - ((DB_GETTYPE(wStyle) == DB_UP) ? 3 : 0),
                                    4, DB_GETTYPE(wStyle) == DB_DOWN);
                break;

            case DB_CROSS:
                {
                    int x1 = (rc.left*2+rc.right)/3-1;
                    int x2 = (rc.left+rc.right*2)/3;
                    int y1 = (rc.top*2+rc.bottom)/3-1;
                    int y2 = (rc.top+rc.bottom*2)/3-1;

                    if ( DB_ISFLAT(wStyle) )
                    {
                        x2++; y2++;
                    }
                    else if ( DB_ISPRESSED(wStyle) )
                    {
                        x1++; y1++;
                        x2++; y2++;
                    }
                    if ( DB_ISFLAT(wStyle) )
                    {
                        crText = DB_ISENABLED(wStyle) && pDC->GetPixel (x1, y1+1) == crOver ? RGB(0,0,0) : ::GetSysColor (COLOR_BTNTEXT);
                        pen.Color (DB_ISPRESSED(wStyle) ? RGB(240,240,240) : DB_ISENABLED(wStyle) ? crText : ::GetSysColor (COLOR_GRAYTEXT));
                    }
                    else
                    {
                        pen.Color (::GetSysColor (DB_ISENABLED(wStyle) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
                    }
                    pDC->MoveTo (x1, y1);
                    pDC->LineTo (x2, y2+1);
                    pDC->MoveTo (x1+1, y1);
                    pDC->LineTo (x2+1, y2+1);
                    pDC->MoveTo (x2-1, y1);
                    pDC->LineTo (x1-1, y2+1);
                    pDC->MoveTo (x2, y1);
                    pDC->LineTo (x1, y2+1);
                }
                break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
static WORD s_wNotify = 0;
void CALLBACK EXPORT CDrawButton_TimerProcRepeat (HWND hWnd, UINT, UINT, DWORD)
{
    ::SendMessage (::GetParent (hWnd), WM_COMMAND, MAKELONG (GetDlgCtrlID (hWnd), s_wNotify), (LPARAM)hWnd);
}

/////////////////////////////////////////////////////////////////////////////
DWORD CDrawButton::Click (CWnd* pWnd, CPoint pt, UINT nIDRepeat) const
{
    // don't handle if capture already set
    if ( ::GetCapture() != NULL || !DB_ISENABLED(m_wStyle) )
    {
        return DB_DEFAULT;
    }
    // set capture to the window which received this message
    pWnd->SetCapture();
    ASSERT(pWnd == CWnd::GetCapture());

    bool  bDown = false, bClick = false;
    int   nSpinType = DB_PRESSED;
    CRect rcBtn(m_Rect);

    if ( DB_GETTYPE(m_wStyle) == DB_UPDOWN )
    {
        if ( pt.y >= (rcBtn.top+rcBtn.bottom)/2 )
        {
            nSpinType = DB_PRESSED2;
            rcBtn.top = (rcBtn.top+rcBtn.bottom)/2;
        }
        else
        {
            rcBtn.bottom = (rcBtn.top+rcBtn.bottom)/2;
        }
        if ( nIDRepeat != 0 )
        {
            s_wNotify = (nSpinType == DB_PRESSED) ? LOWORD(nIDRepeat) : HIWORD(nIDRepeat);

            if ( !pWnd->SetTimer (nIDRepeat, 100, &CDrawButton_TimerProcRepeat) )
            {
                nIDRepeat = 0;
            }
        }
    }
    // get messages until capture lost or cancelled/accepted
    while ( true )
    {
        bool bNewDown = ::PtInRect (rcBtn, pt) != 0;

        if ( bNewDown != bDown )
        {
            bDown = bNewDown;

            if ( DB_ISWINDOWDC(m_wStyle) )
            {
                CWindowDC cDC (pWnd);
                Draw (&cDC, m_wStyle|(bDown ? nSpinType : DB_DEFAULT));
            }
            else
            {
                CClientDC cDC (pWnd);
                Draw (&cDC, m_wStyle|(bDown ? nSpinType : DB_DEFAULT));
            }
        }
        MSG msg;
        VERIFY(::GetMessage(&msg, NULL, 0, 0));

        if ( CWnd::GetCapture() != pWnd )
        {
            DispatchMessage (&msg);
            goto ExitLoop;
        }
        switch ( msg.message )
        {
            // handle movement messages
            case WM_MOUSEMOVE:
                pt.x = (short signed)LOWORD(msg.lParam);
                pt.y = (short signed)HIWORD(msg.lParam);

                if ( DB_ISWINDOWDC(m_wStyle) )
                {
                    pWnd->ClientToScreen (&pt);
                    pt -= CWindowRect(pWnd).TopLeft();
                }
                break;

            // handle accept messages
            case WM_LBUTTONUP:
                bClick = bDown;
                goto ExitLoop;

            // handle cancel messages
            case WM_KEYDOWN:
                if ( msg.wParam != VK_ESCAPE )
                {
                    break;
                }
                // continue...

            case WM_CANCELMODE:
            case WM_RBUTTONDOWN:
                goto ExitLoop;

            case WM_TIMER:
                if ( msg.wParam == nIDRepeat && !bDown )
                {
                    break;
                }
                // continue...

            default:
                // just dispatch rest of the messages
                DispatchMessage (&msg);
                break;
        }
    }
ExitLoop:
    ReleaseCapture();
    if ( DB_GETTYPE(m_wStyle) == DB_UPDOWN && nIDRepeat != 0 )
    {
        pWnd->KillTimer (nIDRepeat);
    }
    if ( bDown )
    {
        if ( DB_ISWINDOWDC(m_wStyle) )
        {
            CWindowDC cDC (pWnd);
            Draw (&cDC);
        }
        else
        {
            CClientDC cDC (pWnd);
            Draw (&cDC);
        }
    }
    return bClick ? (DB_GETTYPE(m_wStyle)|nSpinType) : DB_DEFAULT;
}

/////////////////////////////////////////////////////////////////////////////
void CALLBACK EXPORT CDrawButton_TimerProcMouseOver (HWND hWnd, UINT, UINT, DWORD)
{
    CPoint pt;

    ::GetCursorPos (&pt);

    // WM_MOUSEMOVE isn't necessary if pt is over the window !
    
    if ( ::WindowFromPoint (pt) != hWnd )
    {
        ::ScreenToClient (hWnd, &pt);
        ::PostMessage (hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
    }
}

/////////////////////////////////////////////////////////////////////////////
void CDrawButton::CheckForMouseOver (CWnd* pWnd, CPoint pt)
{
    if ( DB_ISFLAT(m_wStyle) )
    {
        if ( !DB_ISOVER(m_wStyle) && PtInRect (pt) )
        {
            if ( pWnd->SetTimer (54321, 300, &CDrawButton_TimerProcMouseOver) )
            {
                m_wStyle |= DB_OVER;

                CClientDC cDC (pWnd);

                Draw (&cDC);
            }
        }
        else if ( DB_ISOVER(m_wStyle) && !PtInRect (pt) )
        {
            pWnd->KillTimer (54321);

            m_wStyle &= ~DB_OVER;

            pWnd->InvalidateRect (m_Rect);
        }
    }
}
