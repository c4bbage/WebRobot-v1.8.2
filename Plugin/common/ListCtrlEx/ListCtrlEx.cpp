// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CListCellBT

CListCellBT::CListCellBT(int nItem,int nSubItem)
{
	m_nItem=nItem;
	m_nSubItem=nSubItem;
}

CListCellBT::~CListCellBT()
{
}


BEGIN_MESSAGE_MAP(CListCellBT, CButton)
//{{AFX_MSG_MAP(CListCellBT)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CListCellBT::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	DestroyWindow();
}

void CListCellBT::OnNcDestroy() 
{
	CButton::OnNcDestroy();
	
	// TODO: Add your message handler code here
	delete this;
}

int CListCellBT::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);	
	SetFocus();

	return 0;
}

void CListCellBT::OnClicked() 
{
	// TODO: Add your control notification handler code here
	GetParent()->GetParent()->SendMessage(WM_LISTBUTTON_CLICKED,(WPARAM)m_nItem,(LPARAM)m_nSubItem);
	
}

/////////////////////////////////////////////////////////////////////////////
// CListCellEd

CListCellEd::CListCellEd(int nItem, int nSubItem, CString strInitText)
{
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_strInitText = strInitText;
	m_bEscape=FALSE;
}

CListCellEd::~CListCellEd()
{
}


BEGIN_MESSAGE_MAP(CListCellEd, CEdit)
	//{{AFX_MSG_MAP(CListCellEd)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCellEd message handlers

void CListCellEd::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	SetListItemText();

    DestroyWindow();
}

void CListCellEd::OnNcDestroy() 
{
	CEdit::OnNcDestroy();
	
	// TODO: Add your message handler code here
    delete this;
}

BOOL CListCellEd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| GetKeyState( VK_CONTROL)
			)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

void CListCellEd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if( nChar == VK_ESCAPE )
			m_bEscape = TRUE;
		GetParent()->SetFocus();

		return;
	}

	// Resize edit control if needed
	// Get text extent
	CString str;

	GetWindowText( str );
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject( pFont );
	CSize size = dc.GetTextExtent( str );
	dc.SelectObject( pFontDC );
	size.cx += 5;			   	// add some extra buffer

	// Get client rect
	CRect rect, parentrect;
	GetClientRect( &rect );
	GetParent()->GetClientRect( &parentrect );

	// Transform rect to parent coordinates
	ClientToScreen( &rect );
	GetParent()->ScreenToClient( &rect );

	// Check whether control needs to be resized
	// and whether there is space to grow
	if( size.cx > rect.Width() )
	{
		if( size.cx + rect.left < parentrect.right )
			rect.right = rect.left + size.cx;
		else
			rect.right = parentrect.right;
		MoveWindow( &rect );
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

int CListCellEd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText( m_strInitText );
	SetFocus();
	SetSel( 0, -1 );
	
	return 0;
}

void CListCellEd::SetListItemText()
{
    CString Text;
    GetWindowText (Text);

    // Send Notification to parent of ListView ctrl
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
    dispinfo.hdr.idFrom = GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = m_nItem;
    dispinfo.item.iSubItem = m_nSubItem;
    dispinfo.item.pszText = m_bEscape ? NULL : LPTSTR ((LPCTSTR) Text);
    dispinfo.item.cchTextMax = Text.GetLength();
    GetParent()->GetParent()->SendMessage (WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM) &dispinfo);
	//如果已经修改则向主窗体发送消息
	if(Text.Compare(m_strInitText) !=0 )
	{
		GetParent()->GetParent()->SendMessage(WM_ITEM_CHANGED,(WPARAM)m_nItem,(LPARAM)m_nSubItem);
	}

}
/////////////////////////////////////////////////////////////////////////////
// CListCellCb

CListCellCb::CListCellCb()
{
}

CListCellCb::CListCellCb(int nItem,int nSubItem,CString strWindowText)
{
	m_iRowIndex=nItem;
	m_iColumnIndex=nSubItem;
	m_strWindowText=strWindowText;
}

CListCellCb::~CListCellCb()
{
}


BEGIN_MESSAGE_MAP(CListCellCb, CComboBox)
	//{{AFX_MSG_MAP(CListCellCb)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCellCb message handlers

BOOL CListCellCb::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// If the message if for "Enter" or "Esc"
	// Do not process
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			// DO NOT process further
			return TRUE;				
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CListCellCb::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	CString str;
	GetWindowText(str);
	
	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;
	
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex;
	dispinfo.item.pszText = LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
	//如果已经修改则向主窗体发送消息
	if(str.Compare(m_strWindowText) !=0 )
	{
		GetParent()->GetParent()->SendMessage(WM_ITEM_CHANGED,(WPARAM)m_iRowIndex,(LPARAM)m_iColumnIndex);
	}	
	// Close the control
	PostMessage(WM_CLOSE);
	
}

int CListCellCb::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);	
	SetFocus();
	
	return 0;
}

void CListCellCb::OnNcDestroy() 
{
	CComboBox::OnNcDestroy();
	
	// TODO: Add your message handler code here
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CListHeader

CListHeader::CListHeader()
{
	m_iSortColumn=-1;	
	m_bSortAscending=TRUE;
	m_ImgList.Create(14,14, ILC_COLOR24| ILC_MASK,2,0);
	HMODULE h=LoadLibrary(_T("shell32.dll"));
	HBITMAP hU=LoadBitmap(h,MAKEINTRESOURCE(133));
	HBITMAP hD=LoadBitmap(h,MAKEINTRESOURCE(134));
	m_nUpBit=m_ImgList.Add(CBitmap::FromHandle(hU),RGB(192,192,192));
	m_nDownBit=m_ImgList.Add(CBitmap::FromHandle(hD),RGB(192,192,192));
	FreeLibrary(h);
}

CListHeader::~CListHeader()
{
	m_ImgList.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CListHeader, CHeaderCtrl)
	//{{AFX_MSG_MAP(CListHeader)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListHeader message handlers
BOOL CListHeader::OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes)
{
	NMHDR& nmh = *(NMHDR*)lp;
	
	GetParent()->GetParent()->SendMessage(WM_COLUMN_CHANGED);
	// otherwise, pass to header control for default processing
	return CHeaderCtrl::OnChildNotify(msg, wp, lp, pRes);
}

void CListHeader::SetSortArrow(const int iSortColumn, const BOOL bSortAscending)
{
	m_iSortColumn=iSortColumn;
	m_bSortAscending=bSortAscending;
	SetImageList(&m_ImgList);
	
	int nCount=GetItemCount();
	for(int i=0;i<nCount;i++)
	{
		HD_ITEM hditem;
		hditem.mask = HDI_FORMAT | HDI_IMAGE;	
		
		VERIFY( GetItem(i, &hditem));
		if(i==iSortColumn && m_bSortAscending)
		{
			hditem.iImage = m_nUpBit;
			hditem.fmt = hditem.fmt&HDF_JUSTIFYMASK|HDF_IMAGE|HDF_STRING|HDF_BITMAP_ON_RIGHT;	
		}
		else if(i==iSortColumn && !m_bSortAscending)
		{
			hditem.iImage = m_nDownBit;
			hditem.fmt = hditem.fmt&HDF_JUSTIFYMASK|HDF_IMAGE|HDF_STRING|HDF_BITMAP_ON_RIGHT;
		}
		else
		{
			hditem.fmt = hditem.fmt & HDF_JUSTIFYMASK | HDF_STRING;
		}
		VERIFY( SetItem(i, &hditem));
	}
}
/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
	m_iSortColumn=-1;
	m_bSortAscending=TRUE;
	m_bDisableSort = FALSE;
}

CListCtrlEx::~CListCtrlEx()
{
	
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)	
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, OnInsertitem)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CListCtrlEx::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (m_bDisableSort)
		return;
	
	if(pNMListView->iSubItem==m_iSortColumn)
		m_bSortAscending=!m_bSortAscending;
	else
	{
		m_bSortAscending=TRUE;
		m_iSortColumn=pNMListView->iSubItem;
	}
	m_ctlHeader.SetSortArrow(m_iSortColumn,m_bSortAscending);
	
	SortItems(CompareFunction,(LPARAM)this);
	
	*pResult = 0;
}

void CListCtrlEx::DisableSort()
{
	m_bDisableSort = TRUE;
}

bool CListCtrlEx::IsNumber( LPCTSTR pszText )
{	
	for( int i = 0; i < lstrlen( pszText ); i++ )
	{
		if(pszText[i]=='.')
			continue;
		if( !_istdigit( pszText[ i ] ) )
			return false;		
	}
	return true;	
}

void CListCtrlEx::OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	LPITEMCOLOR pIC= new ITEMCOLOR;
	pIC->crBK=GetBkColor();
	pIC->crText=GetTextColor();
	CListCtrl::SetItemData(pNMListView->iItem,(DWORD)pIC);
	*pResult = 0;
}

void CListCtrlEx::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(pNMListView->iItem);
	ASSERT(pIC);
	delete pIC;
	
	*pResult = 0;
}

void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetFocus() != this) SetFocus();
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetFocus() != this) SetFocus();
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM	*plvItem = &pDispInfo->item;
	
    if (plvItem->pszText != NULL)
    {
		SetItemText (plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
    }
	
	*pResult = 0;
}

void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnLButtonDown(nFlags, point);
	
    int nSubItem=0;
	int nItem= HitTestEx (point, nSubItem);
    if (nItem==-1) return;
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	if(!pIC->bEdit) return;
	if(m_arColumnInfo[nSubItem]->m_dwColumnType==CS_NONE)
	{
		return;
	}
	ShowSubCtrl(nItem,nSubItem);
}

void CListCtrlEx::OnDestroy() 
{
	CListCtrl::OnDestroy();
	int nCount=m_arColumnInfo.GetSize();
	for(int i=0;i<nCount;i++)
	{
		delete m_arColumnInfo.GetAt(i);
	}
	m_arColumnInfo.RemoveAll();
	
	// TODO: Add your message handler code here
	
}

int CListCtrlEx::NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
	double iNumber1 = atof( pszNumber1 );
	double iNumber2 = atof( pszNumber2 );
	
	if( iNumber1 < iNumber2 )
		return -1;
	
	if( iNumber1 > iNumber2 )
		return 1;
	
	return 0;
}

bool CListCtrlEx::IsDate( LPCTSTR pszText )
{
	// format should be 99/99/9999.
	
	if( lstrlen( pszText ) != 10 )
		return false;
	
	return _istdigit( pszText[ 0 ] )
		&& _istdigit( pszText[ 1 ] )
		&& pszText[ 2 ] == _T('/')
		&& _istdigit( pszText[ 3 ] )
		&& _istdigit( pszText[ 4 ] )
		&& pszText[ 5 ] == _T('/')
		&& _istdigit( pszText[ 6 ] )
		&& _istdigit( pszText[ 7 ] )
		&& _istdigit( pszText[ 8 ] )
		&& _istdigit( pszText[ 9 ] );
}

int CListCtrlEx::DateCompare( const CString& strDate1, const CString& strDate2 )
{
	const int iYear1 = atoi( strDate1.Mid( 6, 4 ) );
	const int iYear2 = atoi( strDate2.Mid( 6, 4 ) );
	
	if( iYear1 < iYear2 )
		return -1;
	
	if( iYear1 > iYear2 )
		return 1;
	
	const int iMonth1 = atoi( strDate1.Mid( 3, 2 ) );
	const int iMonth2 = atoi( strDate2.Mid( 3, 2 ) );
	
	if( iMonth1 < iMonth2 )
		return -1;
	
	if( iMonth1 > iMonth2 )
		return 1;
	
	const int iDay1 = atoi( strDate1.Mid( 0, 2 ) );
	const int iDay2 = atoi( strDate2.Mid( 0, 2 ) );
	
	if( iDay1 < iDay2 )
		return -1;
	
	if( iDay1 > iDay2 )
		return 1;
	
	return 0;
}

int CALLBACK CListCtrlEx::CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamData)
{
	CListCtrlEx* pListCtrl=(CListCtrlEx*)lParamData;

	   int nItem1=0,nItem2=0;

	   LVFINDINFO FindInfo; 
	   FindInfo.flags = LVFI_PARAM; 
	   FindInfo.lParam=lParam1; 
	   nItem1=pListCtrl->FindItem(&FindInfo, -1);
	   FindInfo.lParam=lParam2;
	   nItem2=pListCtrl->FindItem(&FindInfo, -1);

	   if((nItem1==-1)||(nItem2==-1))
	   { 
		   TRACE("Not find!\n"); 
		   return 0; 
	   }
	   
	   CString strItem1,strItem2; 
	   strItem1 = pListCtrl->GetItemText(nItem1, pListCtrl->m_iSortColumn); 
	   strItem2 = pListCtrl->GetItemText(nItem2, pListCtrl->m_iSortColumn); 
	   
	if(CListCtrlEx::IsNumber(strItem1) && CListCtrlEx::IsNumber(strItem2))
		return pListCtrl->m_bSortAscending ? CListCtrlEx::NumberCompare(strItem1,strItem2) : CListCtrlEx::NumberCompare(strItem2,strItem1);
	else if(CListCtrlEx::IsDate(strItem1) && CListCtrlEx::IsDate(strItem2))
		return pListCtrl->m_bSortAscending ? CListCtrlEx::DateCompare(strItem1,strItem2) : CListCtrlEx::DateCompare(strItem2,strItem1);
	else
		return pListCtrl->m_bSortAscending ? lstrcmp(strItem1,strItem2) : lstrcmp(strItem2,strItem1);
	
}

void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//draw each item.set txt color,bkcolor....
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.
		
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.
		LPITEMCOLOR pIC=(LPITEMCOLOR)pLVCD->nmcd.lItemlParam;		
		ASSERT(pIC);
		pLVCD->clrText=pIC->crText;
		pLVCD->clrTextBk=pIC->crBK;		
	}		
}	

int CListCtrlEx::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, DWORD nStyle,CStringArray * pStringArray, int nSubItem)
{
	int nIndex=CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat ,nWidth ,nSubItem );
	if(nIndex!=-1)
	{
		LPSUBITEMINFO pSII=new SUBITEMINFO;
		pSII->m_dwColumnType=nStyle;
		pSII->m_parComboString=pStringArray;
		m_arColumnInfo.InsertAt(nIndex,pSII);
	}
	return nIndex;
}

int CListCtrlEx::InsertColumn( int nCol, const LVCOLUMN* pColumn, DWORD nStyle,CStringArray * pStringArray)
{
	int nIndex=CListCtrl::InsertColumn(nCol,pColumn);
	if(nIndex!=-1)
	{
		LPSUBITEMINFO pSII=new SUBITEMINFO;
		pSII->m_dwColumnType=nStyle;
		pSII->m_parComboString=pStringArray;
		m_arColumnInfo.InsertAt(nIndex,pSII);
	}
	return nIndex;
}

BOOL CListCtrlEx::DeleteColumn(int nCol )
{
	BOOL bResult=CListCtrl::DeleteColumn(nCol);
	if(bResult)
	{
		delete m_arColumnInfo.GetAt(nCol);
		m_arColumnInfo.RemoveAt(nCol);
	}
	return bResult;
}

int CListCtrlEx::HitTestEx (CPoint& Point, int& nSubItem)
{
	nSubItem = 0;
	int ColumnNum = 0;
    int Row = HitTest (Point, NULL);
	
    // Make sure that the ListView is in LVS_REPORT
    if ((GetWindowLong (m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return Row;
	
    // Get the top and bottom row visible
    Row = GetTopIndex();
    int Bottom = Row + GetCountPerPage();
    if (Bottom > GetItemCount())
		Bottom = GetItemCount();
    
    // Get the number of columns
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
	
    // Loop through the visible rows
    for(; Row <= Bottom; Row++)
    {
		// Get bounding rect of item and check whether point falls in it.
		CRect Rect;
		GetItemRect (Row, &Rect, LVIR_BOUNDS);
		if (Rect.PtInRect (Point))
		{
			// Now find the column
			for (ColumnNum = 0; ColumnNum < nColumnCount; ColumnNum++)
			{
				int ColWidth = GetColumnWidth (ColumnNum);
				if (Point.x >= Rect.left && Point.x <= (Rect.left + ColWidth))
				{
					nSubItem = ColumnNum;
					return Row;
				}
				Rect.left += ColWidth;
			}
		}
    }
	
    return -1;
}

BOOL CListCtrlEx::CanEdit(int nItem)
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	return pIC->bEdit;	
}

void CListCtrlEx::SetEdit(int nItem,BOOL bEdit/* = TRUE*/)
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);
	pIC->bEdit=bEdit;
}

BOOL CListCtrlEx::SetItemBKColor(int nItem,COLORREF crBKColor)
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	if(pIC->crBK==crBKColor)
	{
		return TRUE;
	}
	pIC->crBK=crBKColor;
	CRect rcSubRect;
	GetItemRect(nItem,&rcSubRect,LVIR_BOUNDS);
	InvalidateRect(&rcSubRect);
	return TRUE;
}

BOOL CListCtrlEx::SetItemTextColor(int nItem,COLORREF crTextColor)
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	if(pIC->crText==crTextColor)
	{
		return TRUE;
	}
	pIC->crText=crTextColor;
	CRect rcSubRect;
	GetItemRect(nItem,&rcSubRect,LVIR_BOUNDS);
	InvalidateRect(&rcSubRect);
	return TRUE;	
}

COLORREF CListCtrlEx::GetItemBKColor(int nItem) const
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	return (COLORREF)pIC->crBK;
}

COLORREF CListCtrlEx::GetItemTextColor(int nItem) const
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	return (COLORREF)pIC->crText;
	
}

BOOL CListCtrlEx::SetItemData(int nItem, DWORD dwData)
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	pIC->dwData=dwData;
	return TRUE;
}

DWORD CListCtrlEx::GetItemData(int nItem) const
{
	ASSERT(nItem>-1 && nItem<GetItemCount());
	LPITEMCOLOR pIC=(LPITEMCOLOR)CListCtrl::GetItemData(nItem);	
	return pIC->dwData;	
}

BOOL CListCtrlEx::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText )
{
	CString strText=CListCtrl::GetItemText(nItem,nSubItem);
	if(strText.Compare(lpszText)==0)
	{
		return TRUE;
	}
	return CListCtrl::SetItemText(nItem,nSubItem,lpszText);
}

BOOL CListCtrlEx::SetItemText(int nItem, LPCTSTR lpszSubItem, LPCTSTR lpszText )
{
	HDITEM hdi;
	TCHAR  lpBuffer[256];
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;
	for (int i=0;i < m_ctlHeader.GetItemCount();i++)
	{
		m_ctlHeader.GetItem(i, &hdi);	
		if (strcmp(hdi.pszText, lpszSubItem) == 0)
		{
			CString strText=CListCtrl::GetItemText(nItem,i);
			if(strText.Compare(lpszText)==0)
			{
				return TRUE;
			}			
			return CListCtrl::SetItemText(nItem,i,lpszText);
		}
	}
	return FALSE;
}

int CListCtrlEx::GetItemText( int nItem, int nSubItem, LPTSTR lpszText, int nLen )
{
	return CListCtrl::GetItemText(nItem,nSubItem,lpszText,nLen);
}

int CListCtrlEx::GetItemText( int nItem, LPCTSTR lpszSubItem, LPTSTR lpszText, int nLen )
{
	HDITEM hdi;
	TCHAR  lpBuffer[256];
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;
	for (int i=0;i < m_ctlHeader.GetItemCount();i++)
	{
		m_ctlHeader.GetItem(i, &hdi);	
		if (strcmp(hdi.pszText, lpszSubItem) == 0)
		{
			return CListCtrl::GetItemText(nItem,i,lpszText,nLen);
		}
	}
	return -1;	
}

CString CListCtrlEx::GetItemText(int nItem, int nSubItem )
{
	return CListCtrl::GetItemText(nItem,nSubItem);
}

CString CListCtrlEx::GetItemText(int nItem, LPCTSTR lpszSubItem )
{
	HDITEM hdi;
	TCHAR  lpBuffer[256];
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;
	for (int i=0;i < m_ctlHeader.GetItemCount();i++)
	{
		m_ctlHeader.GetItem(i, &hdi);	
		if (strcmp(hdi.pszText, lpszSubItem) == 0)
		{
			return CListCtrl::GetItemText(nItem,i);
		}
	}
	return _T("");
	
}

BOOL CListCtrlEx::SetColumnWidth(int nCol, int cx )
{
	return CListCtrl::SetColumnWidth(nCol,cx);
}

BOOL CListCtrlEx::SetColumnWidth(LPCTSTR lpszCol, int cx )
{
	HDITEM hdi;
	TCHAR  lpBuffer[256];
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;
	for (int i=0;i < m_ctlHeader.GetItemCount();i++)
	{
		m_ctlHeader.GetItem(i, &hdi);	
		if (strcmp(hdi.pszText, lpszCol) == 0)
		{
			return CListCtrl::SetColumnWidth(i,cx);
		}
	}
	return FALSE;
}

int CListCtrlEx::GetColumnWidth(int nCol)
{
	return CListCtrl::GetColumnWidth(nCol);
}

int CListCtrlEx::GetColumnWidth(LPCTSTR lpszCol )
{
	HDITEM hdi;
	TCHAR  lpBuffer[256];
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;
	for (int i=0;i < m_ctlHeader.GetItemCount();i++)
	{
		m_ctlHeader.GetItem(i, &hdi);	
		if (strcmp(hdi.pszText, lpszCol) == 0)
		{
			return CListCtrl::GetColumnWidth(i);
		}
	}
	return -1;
}

void CListCtrlEx::ShowSubCtrl(int nItem, int nSubItem)
{
    // The returned pointer should not be saved
	
    // Make sure that the item is visible
    if (!EnsureVisible (nItem, TRUE)) return;
	
    // Make sure that nCol is valid
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
    if (nSubItem >= nColumnCount || GetColumnWidth (nSubItem) < 5)
	{
		return;
	}
	
    // Get the column offset
    int Offset = 0;
    for (int iColumn = 0; iColumn < nSubItem; iColumn++)
		Offset += GetColumnWidth (iColumn);
	
    CRect Rect;
    GetItemRect (nItem, &Rect, LVIR_BOUNDS);
	
    // Now scroll if we need to expose the column
    CRect ClientRect;
    GetClientRect (&ClientRect);
    if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
    {
		CSize Size;
		if (Offset + Rect.left > 0)
			Size.cx = -(Offset - Rect.left);
		else
			Size.cx = Offset - Rect.left;
		Size.cy = 0;
		Scroll (Size);
		Rect.left -= Size.cx;
    }
	
    // Get nSubItem alignment
    LV_COLUMN lvCol;
    lvCol.mask = LVCF_FMT;
    GetColumn (nSubItem, &lvCol);
    DWORD dwStyle;
    if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
    else dwStyle = ES_CENTER;
	
    Rect.left += Offset+4;
    Rect.right = Rect.left + GetColumnWidth (nSubItem) - 3;
    if (Rect.right > ClientRect.right)
		Rect.right = ClientRect.right;
	
    dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	
	CString strCulText=GetItemText (nItem, nSubItem);
	if(m_arColumnInfo[nSubItem]->m_dwColumnType==CS_BUTTON)
	{
		//显示按钮
		CButton * pButton = new CListCellBT(nItem,nSubItem);
		m_arColumnInfo[nSubItem]->m_dwData=(DWORD)pButton;
		pButton->Create(strCulText,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,Rect,this,102);		
	}
	if(m_arColumnInfo[nSubItem]->m_dwColumnType==CS_EDIT)
	{
		//显示编辑框
		CEdit *pEdit = new CListCellEd (nItem, nSubItem, strCulText);
		pEdit->Create (dwStyle, Rect, this, 101);
	}
	if(m_arColumnInfo[nSubItem]->m_dwColumnType==CS_COMBOBOX)
	{
		//显示组合框
		CRect rc(Rect.left-2,Rect.top-2,Rect.right,Rect.bottom+100);
		CComboBox * pCombo = new CListCellCb(nItem,nSubItem,strCulText);
		pCombo->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,	rc,this, 1);
		if(m_arColumnInfo[nSubItem]->m_parComboString)
		{
			for(int i=0;i<m_arColumnInfo[nSubItem]->m_parComboString->GetSize();i++)
			{
				pCombo->AddString((*(m_arColumnInfo[nSubItem]->m_parComboString)).GetAt(i));
			}
		}
		int nIndex=pCombo->FindString(0,strCulText);
		if(nIndex!=CB_ERR)
		{
			pCombo->SetCurSel(nIndex);
		}
		else
		{
			pCombo->SetCurSel(-1);
		}
	}	
}

#ifdef _CCS_DES
void CListCtrlEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT( GetStyle()&LVS_REPORT);
	VERIFY( m_ctlHeader.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd()));
	CListCtrl::PreSubclassWindow();
}
#endif

#ifdef _CCS_RUN
BOOL CListCtrlEx::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	if(!CListCtrl::Create(dwStyle | LVS_REPORT,rect,pParentWnd,nID))
	{
		AfxMessageBox("Failed to create CListCtrlEx!");
		return FALSE;
	}
	if(!m_ctlHeader.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd()))
	{
		AfxMessageBox("Failed to Subclassing CListHeader!");
		return FALSE;
	}
	return TRUE;
}
#endif

void CListCtrlEx::PrintReport(CString strTitle,CString strHeader/*=_T("")*/)
{
	CDC dc;
	//把打印设备环境附加到DC对象
	CPrintDialog dlg(FALSE);
	if(dlg.DoModal()!=IDOK) return;
	dc.Attach(dlg.GetPrinterDC());
	//取打印纸矩形区域
	CRect rectDraw(0,0,dc.GetDeviceCaps(HORZRES),dc.GetDeviceCaps(VERTRES));
	
	//设置所需打印纸张数目
	DOCINFO di;
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = "Print Doc"; 
	di.lpszOutput = (LPTSTR) NULL; 
	di.lpszDatatype = (LPTSTR) NULL; 
	di.fwType = 0; 
		
	//开始打印
	dc.StartDoc(&di);
	//算出总共页数(第一页比其他页少3行)
	int nPages=GetPrintPages(&dc);
	for(int nPage=1;nPage<nPages+1;nPage++)
	{
		dc.StartPage();
		PrintPage(nPage,&dc,rectDraw,strTitle,strHeader);
		dc.EndPage();
	}
	dc.EndDoc();
	//打印结束
	//最后不要忘记将字体还原，这一句是必需的
	DeleteDC(dc.Detach()); 
}

void CListCtrlEx::PrintReport(CDC *pDC, CPrintInfo *pInfo, CString strTitle, CString strHeader/*=_T("")*/)
{
	PrintPage(pInfo->m_nCurPage,pDC,pInfo->m_rectDraw,strTitle,strHeader);
}

int CListCtrlEx::GetPrintPages(CDC *pDC)
{
	//创建字体
	CFont fnText;
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);	
	lf.lfHeight=-((pDC->GetDeviceCaps(LOGPIXELSY)*lf.lfHeight)/72);
	fnText.CreateFontIndirect(&lf);
	//计算每行的高度(由字体的高度增加20%)
	TEXTMETRIC TextM;
	CFont * pOldFont=pDC->SelectObject(&fnText);
	pDC->GetTextMetrics(&TextM);
	//取打印纸矩形区域
	CRect rectDraw(0,0,pDC->GetDeviceCaps(HORZRES),pDC->GetDeviceCaps(VERTRES));
	//矩形以页边距缩小,缩小的数值按照宽的1/10,高1/15来缩小	
	rectDraw.DeflateRect(pDC->GetDeviceCaps(HORZRES)/10,pDC->GetDeviceCaps(VERTRES)/15,pDC->GetDeviceCaps(HORZRES)/10,pDC->GetDeviceCaps(VERTRES)/15);
	//每页最大行数
	int nMaxCol=(int)(rectDraw.Height()/(TextM.tmHeight*6/5));
	//每一行的高度
	double dColH=(double)rectDraw.Height()/nMaxCol;
	
	//算出总共页数(第一页比其他页少3行)
	int nPages=0;
	if(GetItemCount()<nMaxCol-3)
		nPages=1;
	else
		nPages=(GetItemCount()+3)/nMaxCol+1;
	
	pDC->SelectObject(pOldFont);
	fnText.DeleteObject();

	return nPages;
}

void CListCtrlEx::PrintPage(int nPage, CDC *pDC, CRect rcPrint, CString strTitle, CString strHeader/*=_T("")*/)
{
	//创建字体
	CFont fnTitle,fnText;
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	int nFH=lf.lfHeight;
	lf.lfHeight=-((pDC->GetDeviceCaps(LOGPIXELSY)*nFH*3/2)/72);
	fnTitle.CreateFontIndirect(&lf);
	lf.lfHeight=-((pDC->GetDeviceCaps(LOGPIXELSY)*nFH)/72);
	fnText.CreateFontIndirect(&lf);
	//矩形以页边距缩小,缩小的数值按照宽的1/10,高1/15来缩小	
	rcPrint.DeflateRect(pDC->GetDeviceCaps(HORZRES)/10,pDC->GetDeviceCaps(VERTRES)/15,pDC->GetDeviceCaps(HORZRES)/10,pDC->GetDeviceCaps(VERTRES)/15);
	//计算每行的高度(由字体的高度增加20%)
	TEXTMETRIC TextM;
	CFont * pOldFont=pDC->SelectObject(&fnText);
	pDC->GetTextMetrics(&TextM);
	//每页最大行数
	int nMaxCol=(int)(rcPrint.Height()/(TextM.tmHeight*6/5));
	//每一行的高度
	double dColH=(double)rcPrint.Height()/nMaxCol;
	//列表总宽度
	int nRow=0,nCol=0,nTotal=0;
	CString strText="";	
	int nCols=m_ctlHeader.GetItemCount();
	//每一列的左边距\表头内容\样式
	CUIntArray arVer;//每一个垂直线的的位置
	CStringArray arStr;//每一列表头标题
	CUIntArray arStyle;//每一列的对齐方式
	HDITEM  hditem;
	TCHAR   szBuf[255];	
	hditem.mask       = HDI_TEXT | HDI_FORMAT;
	hditem.pszText    = szBuf;
	hditem.cchTextMax = sizeof(szBuf);
	
	arVer.RemoveAll();
	arStr.RemoveAll();
	arStyle.RemoveAll();	
	arVer.Add(0);
	for(nCol=0;nCol<nCols;nCol++)
	{
		nTotal+=GetColumnWidth(nCol);
		arVer.Add(nTotal);
		GetHeaderCtrl()->GetItem(nCol,&hditem);
		arStr.Add((LPCTSTR)hditem.pszText);
		arStyle.Add(hditem.fmt & 0x2);
	}
	////打印宽度与列表的比例(因为打印区域的单位与列表的单位不同)
	double dScale=(double)rcPrint.Width()/(double)nTotal;	
	//向下偏移量	
	int nMove=nPage==1 ? rcPrint.top+int(3*dColH) : rcPrint.top;	
	//循环数
	int nLoop=nPage==1 ? nMaxCol-3 : nMaxCol;
	//写标题
	if(nPage==1)
	{
		pDC->SelectObject(&fnTitle);
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		pDC->TextOut(rcPrint.CenterPoint().x,rcPrint.top,strTitle);
	}
	//写页眉
	pDC->SelectObject(&fnText);
	pDC->SetTextAlign(TA_BOTTOM|TA_RIGHT);
	pDC->TextOut(rcPrint.right,nMove,strHeader);
	
	//画表格=====================================================
	for(nRow=0;nRow<nLoop+1;nRow++)
	{
		pDC->MoveTo(rcPrint.left,int(nRow*dColH)+nMove);
		pDC->LineTo(rcPrint.right,int(nRow*dColH)+nMove);
	}			
	for(nCol=0;nCol<nCols+1;nCol++)
	{
		pDC->MoveTo(int(arVer.GetAt(nCol)*dScale)+rcPrint.left,nMove);
		pDC->LineTo(int(arVer.GetAt(nCol)*dScale)+rcPrint.left,rcPrint.bottom);
	}
	//写内容==============================================================
	for(nRow=0;nRow<nLoop;nRow++)
	{		
		for(nCol=0;nCol<nCols;nCol++)
		{
			CRect rcText(int(arVer.GetAt(nCol)*dScale)+rcPrint.left,//left
				int(nRow*dColH)+nMove,//top
				int((arVer.GetAt(nCol)+GetColumnWidth(nCol))*dScale)+rcPrint.left,//right
				int((nRow+1)*dColH)+nMove);//bottom
			rcText.DeflateRect(10,5,10,5);
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			
			if(nRow==0)
				strText=arStr[nCol];
			else
				strText=GetItemText((nPage-1)*(nMaxCol-1)+nRow-(nPage==1 ? 1 : 4),nCol);
			
			if(arStyle[nCol]==0)
				pDC->DrawText(strText,&rcText,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			else if(arStyle[nCol]==1)
				pDC->DrawText(strText,&rcText,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			else
				pDC->DrawText(strText,&rcText,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}
	//写页脚
	pDC->SetTextAlign(TA_TOP|TA_RIGHT);
	strText.Format("%d/%d",nPage,GetPrintPages(pDC));
	pDC->TextOut(rcPrint.right,rcPrint.bottom+5,strText);
	
	//打印结束
	//最后不要忘记将字体还原，这一句是必需的
	pDC->SelectObject(pOldFont);	
	fnTitle.DeleteObject(); 
	fnText.DeleteObject();
}
