// ListMoveItem.cpp : implementation file
//

#include "stdafx.h"
#include "../../WebRobot.h"
#include "ListMoveItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListMoveItem

CListMoveItem::CListMoveItem()
{
	//初始化
	m_bDragging=FALSE;
	m_pDragImage = NULL;
	m_nDragIndex = -1;
	m_nDropIndex = -1;
}

CListMoveItem::~CListMoveItem()
{
}


BEGIN_MESSAGE_MAP(CListMoveItem, CListCtrl)
	//{{AFX_MSG_MAP(CListMoveItem)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListMoveItem message handlers

void CListMoveItem::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging) 
	{
		//// Move the drag image
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window

		//// If we drag outside current window we need to adjust the highlights displayed
		if (pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight
			{
				TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
				TRACE("m_nDropIndex is not -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				int i = 0;
				int nCount = pList->GetItemCount();
				for(i = 0; i < nCount; i++)
				{
					pList->SetItemState(i, 0, LVIS_DROPHILITED);
				}
				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))
		{			
			//Note that we can drop here
			//SetCursor(LoadCursor(NULL, IDC_ARROW));
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LISTMOVE));

			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			// Turn off hilight for previous drop target
			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// Redraw previous item
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);
			
			// Get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
			// Highlight it
			pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
			// Redraw item
			pList->RedrawItems(m_nDropIndex, m_nDropIndex);
			pList->UpdateWindow();
		}
		else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}
	CListCtrl::OnMouseMove(nFlags, point);
}

void CListMoveItem::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;
		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();
		delete m_pDragImage; //must delete it because it was created at the beginning of the drag

		CPoint pt (point); //Get current mouse coordinates
		ClientToScreen (&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer
		// If window is CListCtrl, we perform the drop
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
		{
			m_pDropList = (CListCtrl*)pDropWnd; //Set pointer to the list we are dropping on
			DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop
		}
	}
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CListMoveItem::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
		m_nDragIndex = pNMListView->iItem;
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
	if(this->GetSelectedCount() > 1) //more than one item is selected
	pt.x = nOffset;
	pt.y = nOffset;
	m_pDragImage = this->CreateDragImage(m_nDragIndex, &pt);
	ASSERT(m_pDragImage); //make sure it was created
	//////////////////////////////////////////////////////////////////////////
	CBitmap bitmap;
	if(this->GetSelectedCount() > 1) //more than 1 item in list is selected
		//bitmap.LoadBitmap(IDB_BITMAP_MULTI);
		bitmap.LoadBitmap(IDB_BITMAP_BOX_SINGLE);
	else
		bitmap.LoadBitmap(IDB_BITMAP_BOX_SINGLE);
	m_pDragImage->Replace(0, &bitmap, &bitmap);
	//////////////////////////////////////////////////////////////////////////
	
	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
	//// Set dragging flag and others
	m_bDragging = TRUE;	//we are in a drag and drop operation
	m_nDropIndex = -1;	//we don't have a drop index yet
	m_pDragList = this; //make note of which list we are dragging from
	m_pDropWnd = this;	//at present the drag list is the drop list
	//// Capture all mouse messages
	SetCapture ();

	*pResult = 0;
}

void CListMoveItem::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
	pDropList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);

	//Set up the LV_ITEM for retrieving item from pDragList and adding the new item to the pDropList
	char szLabel[256];
	LVITEM lviT;
	LVITEM* plvitem;

	ZeroMemory(&lviT, sizeof (LVITEM)); //allocate and clear memory space for LV_ITEM
	lviT.iItem		= m_nDragIndex;
	lviT.mask		= LVIF_TEXT;
	lviT.pszText		= szLabel;
	lviT.cchTextMax	= 255;

	lvItem* pItem;
	lvItem lvi;
	lvi.plvi = &lviT;
	lvi.plvi->iItem = m_nDragIndex;
	lvi.plvi->mask = LVIF_TEXT;
	lvi.plvi->pszText = szLabel;
	lvi.plvi->cchTextMax = 255;

	if(pDragList->GetSelectedCount() == 1)
	{
		// Get item that was dragged
		pDragList->GetItem (lvi.plvi);
		lvi.sCol2 = pDragList->GetItemText(lvi.plvi->iItem, 1);
        lvi.sCol3 = pDragList->GetItemText(lvi.plvi->iItem, 2);
        lvi.sCol4 = pDragList->GetItemText(lvi.plvi->iItem, 3);
        lvi.sCol5 = pDragList->GetItemText(lvi.plvi->iItem, 4);
		lvi.dwID =  pDragList->GetItemData(lvi.plvi->iItem);
		// Delete the original item (for Move operation)
		// This is optional. If you want to implement a Copy operation, don't delete.
		// This works very well though for re-arranging items within a CListCtrl.
		// It is written at present such that when dragging from one list to the other
		//  the item is copied, but if dragging within one list, the item is moved.
		if(pDragList == pDropList)
		{
			pDragList->DeleteItem (m_nDragIndex);
			if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; //decrement drop index to account for item
															//being deleted above it
		}

		// Insert item into pDropList
		// if m_nDropIndex == -1, iItem = GetItemCount() (inserts at end of list), else iItem = m_nDropIndex
		lvi.plvi->iItem = (m_nDropIndex == -1) ? pDropList->GetItemCount () : m_nDropIndex;
		pDropList->InsertItem (lvi.plvi);
		pDropList->SetItemText(lvi.plvi->iItem, 1, (LPCTSTR)lvi.sCol2);
	    pDropList->SetItemText(lvi.plvi->iItem, 2, (LPCTSTR)lvi.sCol3);
	    pDropList->SetItemText(lvi.plvi->iItem, 3, (LPCTSTR)lvi.sCol4);
	    pDropList->SetItemText(lvi.plvi->iItem, 4, (LPCTSTR)lvi.sCol5);
		pDropList->SetItemData(lvi.plvi->iItem, lvi.dwID);
		// Select the new item we just inserted
		pDropList->SetItemState (lvi.plvi->iItem, LVIS_SELECTED, LVIS_SELECTED);
	}
	else //more than 1 item is being dropped
	{
		//We have to parse through all of the selected items from the DragList
		//1) Retrieve the info for the items and store them in memory
		//2) If we are reordering, delete the items from the list
		//3) Insert the items into the list (either same list or different list)

		CList<lvItem*, lvItem*> listItems;
		POSITION listPos;
			
		//Retrieve the selected items
		POSITION pos = pDragList->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
		while(pos) //so long as we have a valid POSITION, we keep iterating
		{
			plvitem = new LVITEM;
			ZeroMemory(plvitem, sizeof(LVITEM));
			pItem = new lvItem;
			//ZeroMemory(pItem, sizeof(lvItem)); //If you use ZeroMemory on the lvItem struct, it creates an error when you try to set sCol2
			pItem->plvi = plvitem;
			pItem->plvi->iItem = m_nDragIndex;
			pItem->plvi->mask = LVIF_TEXT;
			pItem->plvi->pszText = new char; //since this is a pointer to the string, we need a new pointer to a new string on the heap
			pItem->plvi->cchTextMax = 255;

			m_nDragIndex = pDragList->GetNextSelectedItem(pos);

			//Get the item
			pItem->plvi->iItem = m_nDragIndex; //set the index in the drag list to the selected item
			pDragList->GetItem(pItem->plvi); //retrieve the information
			pItem->sCol2 = pDragList->GetItemText(pItem->plvi->iItem, 1);
            pItem->sCol3 = pDragList->GetItemText(pItem->plvi->iItem, 2);
            pItem->sCol4 = pDragList->GetItemText(pItem->plvi->iItem, 3);
            pItem->sCol5 = pDragList->GetItemText(pItem->plvi->iItem, 4);
			lvi.dwID =  pDragList->GetItemData(lvi.plvi->iItem);
			//Save the pointer to the new item in our CList
			listItems.AddTail(pItem);
		} //EO while(pos) -- at this point we have deleted the moving items and stored them in memory

		if(pDragList == pDropList) //we are reordering the list (moving)
		{
			//Delete the selected items
			pos = pDragList->GetFirstSelectedItemPosition();
			while(pos)
			{
				pos = pDragList->GetFirstSelectedItemPosition();
				m_nDragIndex = pDragList->GetNextSelectedItem(pos);

				pDragList->DeleteItem(m_nDragIndex); //since we are MOVING, delete the item
				if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; //must decrement the drop index to account
																//for the deleted items
			} //EO while(pos)
		} //EO if(pDragList...

		//Iterate through the items stored in memory and add them back into the CListCtrl at the drop index
		listPos = listItems.GetHeadPosition();
		while(listPos)
		{
			pItem = listItems.GetNext(listPos);

			m_nDropIndex = (m_nDropIndex == -1) ? pDropList->GetItemCount() : m_nDropIndex;
			pItem->plvi->iItem = m_nDropIndex;
			pDropList->InsertItem(pItem->plvi); //add the item
			pDropList->SetItemText(pItem->plvi->iItem, 1, pItem->sCol2);
            pDropList->SetItemText(pItem->plvi->iItem, 2, pItem->sCol3);
            pDropList->SetItemText(pItem->plvi->iItem, 3, pItem->sCol4);
            pDropList->SetItemText(pItem->plvi->iItem, 4, pItem->sCol5);
			pDropList->SetItemData(lvi.plvi->iItem, lvi.dwID);
			pDropList->SetItemState(pItem->plvi->iItem, LVIS_SELECTED, LVIS_SELECTED); //highlight/select the item we just added

			m_nDropIndex++; //increment the index we are dropping at to keep the dropped items in the same order they were in in the Drag List
			//If we dont' increment this, the items are added in reverse order
		} //EO while(listPos)
	}
	GetParent()->PostMessage(WM_LIST_DROP_END, 0, 0);//发送消息给主窗体，通知拖拽完毕
}

void CListMoveItem::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//CString s;
	//s.Format("%d,%d",point.x,point.y);
	//this->SetWindowText(s);
	CListCtrl::OnLButtonDown(nFlags, point);
}
