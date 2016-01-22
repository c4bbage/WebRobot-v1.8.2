//*****************************************************************************
//*
//*
//*		TreeListCtrl.cpp
//*
//*
//*****************************************************************************
//
//	(C) Copyright Anton Zechner 2007
//
//	This code creates a Tree-List-Control for MFC
//
#include "StdAfx.h"
#include	<Afx.h>
#include	<tchar.h>
#include	"TreeListCtrl.h"


IMPLEMENT_DYNAMIC(CTreeListCtrl, CWnd)



//*****************************************************************************
//*
//*		Create
//*
//*****************************************************************************
//	Creates a new Tree-List-Control
//	dwStyle		: Style of the Window
//					TVS_CHECKBOXES
//					TVS_DISABLEDRAGDROP
//					TVS_EDITLABELS
//					TVS_FULLROWSELECT
//					TVS_HASBUTTONS
//					TVS_HASLINES
//					TVS_INFOTIP
//					TVS_LINESATROOT
//					TVS_NONEVENHEIGHT
//					TVS_NOSCROLL
//					TVS_NOTOOLTIPS
//					TVS_RTLREADING
//					TVS_SHOWSELALWAYS
//					TVS_SINGLEEXPAND
//					TVS_TRACKSELECT
//	sRect		: Rectangle wher the window should be shown
//	pParentWnd	: Pointer to parent window
//	nId			: Is the ID of the window
//	Returns TRUE if the window was created or FALSE if an error occurs
BOOL CTreeListCtrl::Create(DWORD dwStyle, const RECT &sRect, CWnd* pParentWnd,UINT nId)
{

	TreeListRegister();


return CWnd::Create("TreeList",NULL,dwStyle,sRect,pParentWnd,nId);
}

//*****************************************************************************
//*
//*		InsertColumn
//*
//*****************************************************************************
//	Inserts a column in the header of the Tree-List-Control
//	nCol		: Position where the column should be inserted (use -1 for end)
//	pColumnText	: Text for the column
//	nFormat		: Ist the format for the text
//					TVCFMT_CENTER
//					TVCFMT_IMAGE
//					TVCFMT_RIGHT
//					TVCFMT_LEFT
//	nWidth		: Is the width for the column in pixels (use -1 for an auto expanding column)
//	Retuns the position of the column or -1 if an error occurs
int CTreeListCtrl::InsertColumn(int nCol,LPCTSTR pColumnText,int nFormat,int nWidth)
{
TV_COLUMN	sItem;


	sItem.mask = 0;

	if(pColumnText)
		{
		sItem.mask		|= LVCF_TEXT;
		sItem.pszText	 = (LPTSTR)pColumnText;
		sItem.cchTextMax = 256;
		}

	if(nFormat>=0)
		{
		sItem.mask		|= LVCF_FMT;
		sItem.fmt		 = nFormat;
		}

	if(nWidth>=0)
		{
		sItem.mask		|= LVCF_WIDTH;
		sItem.cx		 = nWidth;
		}

return InsertColumn(nCol,&sItem);
}

//*****************************************************************************
//*
//*		SetItem
//*
//*****************************************************************************
//	Changes some properties of an item
//	hItem			: is the handle of the item
//	nCol			: is the column for the text (0=tree column)
//	nMask			: defies which propaties should be set (TVIF_????)
//	pText			: is the new text of the item (add TVIF_TEXT to nMask) 
//	nImage			: is the new image for the item (add TVIF_IMAGE to nMask) 
//	nSelectedImage	: is the new image for the item (add TVIF_SELECTEDIMAGE to nMask) 
//	nState			: is the new state for the item (add TVIF_STATE to nMask) 
//	nStateMask		: is a mask for the states 
//	lParam			: is the new LPARAM parameter for the item (add TVIF_PARAM to nMask) 
//	Returns TRUE if the text was changed or FALSE if an error occurs
BOOL CTreeListCtrl::SetItem(HTREEITEM hItem,int nCol,UINT nMask,LPCTSTR pText,int nImage,int nSelectedImage,UINT nState, UINT nStateMask,LPARAM lParam)
{
TV_ITEM		sItem;


	sItem.mask				= nMask|TVIF_SUBITEM;
    sItem.hItem				= hItem;
    sItem.state				= nState;
    sItem.stateMask			= nStateMask;
    sItem.pszText			= (LPTSTR)pText;
    sItem.cchTextMax		=(pText)? 256:0;
    sItem.iImage			= nImage;
    sItem.iSelectedImage	= nSelectedImage;
    sItem.cChildren			= nCol;
	sItem.lParam			= lParam;

return SetItem(&sItem);
}



//*****************************************************************************
//*
//*		SetItemText
//*
//*****************************************************************************
//	Set the text of an item
//	hItem		: Is the handle of the item
//	pText		: Is the new text of the item 
//	nCol		: Is the column for the text (0=tree column)
//	Returns TRUE if the text was changed or FALSE if an error occurs
BOOL CTreeListCtrl::SetItemText(HTREEITEM hItem,LPCSTR pText,int nCol)
{
TV_ITEM		sItem;


	sItem.mask				= TVIF_SUBITEM|TVIF_TEXT;
    sItem.hItem				= hItem;
    sItem.stateMask			= 0;
    sItem.pszText			= (LPTSTR)pText;
    sItem.cchTextMax		= (pText)? 256:0;
    sItem.cChildren			= nCol;


return SetItem(&sItem);
}

//*****************************************************************************
//*
//*		ListDeleteItem
//*
//*****************************************************************************
//	Deletes an item in a Tree-List-Control which is used as List-Control
//	iRow	: Is the row of the item
//	Returns TRUE if the item was delleted or FALSE if an error occurs
BOOL CTreeListCtrl::ListDeleteItem(int iRow)
{
return DeleteItem(GetItemOfRow(iRow));
}


//*****************************************************************************
//*
//*		ListInsertItem
//*
//*****************************************************************************
//	Inserts an item in a Tree-List-Control which is used as List-Control
//	iRow	: Is the row of the item
//	pText	: Is the text for the item
//	nImage	: Is the number for the icon (use TV_NOIMAGE for no icon)
//	nState	: Is the state of the image
//				TVIS_BOLD		 	= text is bolded
//				TVIS_UNTERLINE	 	= text is unterlined
//				TVIS_SELECTED		= item is selected
//				TVIS_OVERLAYMASK	= overlay bist for image
//				TVIS_STATEIMAGEMASK = image for state icons
//	iMask	: Is the mask of bist which are used in the nState parameter
//				TVIS_BOLD		 	= text is bolded
//				TVIS_UNTERLINE	 	= text is unterlined
//				TVIS_SELECTED	 	= item is selected
//				TVIS_OVERLAYMASK	= overlay bist for image
//				TVIS_STATEIMAGEMASK = image for state icons
//	Returns the insert position of the item or -1 if an error ocurrs
int CTreeListCtrl::ListInsertItem(int iRow,LPCTSTR pText,int nImage,int nState,int iMask)
{
HTREEITEM		hItem;
TVINSERTSTRUCT	sData;



	sData.item.mask	= 0;

	if(pText)
		{
		sData.item.mask		 |= TVIF_TEXT;
		sData.item.pszText	  = (LPTSTR)pText;
		sData.item.cchTextMax = 256;
		}

	if(nImage>=0)
		{
		sData.item.mask		     |= TVIF_SELECTEDIMAGE|TVIF_IMAGE ;
		sData.item.iImage		  = nImage;
		sData.item.iSelectedImage = nImage;
		}

	if(nState>=0)
		{
		sData.item.mask	    |= TVIF_STATE;
		sData.item.state	 = nState;
		sData.item.stateMask = iMask&(TVIS_BOLD|TVIS_UNTERLINE|TVIS_SELECTED|TVIS_OVERLAYMASK|TVIS_STATEIMAGEMASK);
		}


		hItem = GetItemOfRow(iRow);
	if(!hItem)
		{
		sData.hInsertAfter	= TVI_LAST;
		}
	else{
			hItem = GetPrevSiblingItem(hItem);
		if(!hItem)
				sData.hInsertAfter	= TVI_FIRST;
		else	sData.hInsertAfter	= hItem;
		}


	sData.hParent = TVI_ROOT;

	hItem = InsertItem(&sData);


return GetRowOfItem(hItem);
}

//*****************************************************************************
//*
//*		ListSetItem
//*
//*****************************************************************************
//	Changes an item in a Tree-List-Control which is used as List-Control
//	iRow	: Is the row of the item
//	nCol	: Is the column of the item
//	pText	: Is the text for the item
//	nImage	: Is the number for the icon (use TV_NOIMAGE for no icon)
//	nState	: Is the state of the image
//				TVIS_BOLD			= text is bolded
//				TVIS_UNTERLINE		= text is unterlined
//				TVIS_SELECTED		= item is selected
//				TVIS_OVERLAYMASK	= overlay bist for image
//				TVIS_STATEIMAGEMASK = image for state icons	(only for colum 0)
//	iMask	: Is the mask of bist which are used in the nState parameter
//				TVIS_BOLD			= text is bolded
//				TVIS_UNTERLINE		= text is unterlined
//				TVIS_SELECTED		= item is selected
//				TVIS_OVERLAYMASK	= overlay bist for image
//				TVIS_STATEIMAGEMASK = image for state icons (only for colum 0)
//	Returns TRUE if ok or FALSE if an error occurs
BOOL CTreeListCtrl::ListSetItem(int iRow,int nCol,LPCTSTR pText,int nImage,int nState,int iMask)
{
TV_ITEM		sData;



	sData.mask		= TVIF_HANDLE|TVIF_SUBITEM;
	sData.hItem		= GetItemOfRow(iRow);
	sData.cChildren	= nCol;

	if(!sData.hItem)return FALSE;

	if(pText)
		{
		sData.mask		 |= TVIF_TEXT;
		sData.pszText	  = (LPTSTR)pText;
		sData.cchTextMax = 256;
		}

	if(nImage>=0)
		{
		sData.mask		     |= TVIF_SELECTEDIMAGE|TVIF_IMAGE ;
		sData.iImage		  = nImage;
		sData.iSelectedImage = nImage;
		}

	if(nState>=0)
		{
		sData.mask	    |= TVIF_STATE;
		sData.state	 = nState;
		sData.stateMask = iMask&(TVIS_BOLD|TVIS_UNTERLINE|TVIS_SELECTED);
		}



return SetItem(&sData);
}


//*****************************************************************************
//*
//*		ListSetColor
//*
//*****************************************************************************
//	Changes the colors of an item in a Tree-List-Control which is used as List-Control
//	iRow		: Is the row of the item
//	nCol		: Is the column of the item
//	uBkColor	: Is the new background color (use TV_NOCOLOR for the default color)
//	uTextColor	: Is the new background color (use TV_NOCOLOR for the default color)
//	Returns TRUE if ok or FALSE if an error occurs
BOOL CTreeListCtrl::ListSetColor(int iRow,int nCol,COLORREF uBkColor,COLORREF uTextColor)
{
HTREEITEM	hItem;


		hItem = GetItemOfRow(iRow);
	if(!hItem)return FALSE;


	SetItemBkColor  (hItem,nCol,uBkColor);
	SetItemTextColor(hItem,nCol,uTextColor);


return TRUE;
}

//*****************************************************************************
//*
//*		ListGetColor
//*
//*****************************************************************************
//	Gets the colors of an item in a Tree-List-Control which is used as List-Control
//	iRow		: Is the row of the item
//	nCol		: Is the column of the item
//	uBkColor	: Is the new background color (TV_NOCOLOR for the default color)
//	uTextColor	: Is the new background color (TV_NOCOLOR for the default color)
//	Returns TRUE if ok or FALSE if an error occurs
BOOL CTreeListCtrl::ListGetColor(int iRow,int nCol,COLORREF &uBkColor,COLORREF &uTextColor)
{
HTREEITEM	hItem;


		hItem = GetItemOfRow(iRow);
	if(!hItem)
		{
		uBkColor   = TV_NOCOLOR;
		uTextColor = TV_NOCOLOR;
		return FALSE;
		}


	uBkColor   = GetItemBkColor  (hItem,nCol);
	uTextColor = GetItemTextColor(hItem,nCol);


return TRUE;
}



//*****************************************************************************
//*
//*		GetItemState
//*
//*****************************************************************************
//	Gets the state of an Item
//	hItem		: Is the handle of the item
//	nStateMask	: Is the state mask of the requested states
//	nCol		: Is the column of the item
//	Returns the state bits of the item
UINT CTreeListCtrl::GetItemState(HTREEITEM hItem,UINT uMask,int nCol)
{
TV_ITEM		sItem;


	sItem.mask		 = TVIF_STATE|TVIF_HANDLE|TVIF_SUBITEM;
	sItem.stateMask	 = uMask;
	sItem.hItem		 = hItem;
	sItem.cChildren	 = nCol;

	if(!GetItem(&sItem))return 0;


return sItem.state;
}


//*****************************************************************************
//*
//*		SetItemState
//*
//*****************************************************************************
//	Sets the state of an Item
//	hItem		: Is the handle of the item
//	nCol		: Is the column of the item
//	nState		: New state bist
//	nStateMask	: Mask of the bits which should be chaned
//	Returns the state bits of the item
BOOL CTreeListCtrl::SetItemState(HTREEITEM hItem,int nCol,UINT nState,UINT nStateMask)
{
TV_ITEM		sItem;


	sItem.mask		 = TVIF_STATE|TVIF_HANDLE|TVIF_SUBITEM;
	sItem.hItem		 = hItem;
	sItem.cChildren	 = nCol;
	sItem.stateMask	 = nStateMask;
	sItem.state		 = nState;


return SetItem(&sItem);
}

//*****************************************************************************
//*
//*		GetItemText
//*
//*****************************************************************************
//	Gets the text of an item
//	hItem		: Is the handle of the item
//	nCol		: Is the column of the item
//	Returns a pointer to the text or NULL if an error occurs
LPCSTR CTreeListCtrl::GetItemText(HTREEITEM hItem,int nCol)
{
TV_ITEM		sItem;


	sItem.mask		 = TVIF_TEXT|TVIF_SUBITEM|TVIF_TEXTPTR;
	sItem.hItem		 = hItem;
	sItem.cChildren	 = nCol;

	if(!GetItem(&sItem))return 0;


return sItem.pszText;
}

//*****************************************************************************
//*
//*		GetItemText
//*
//*****************************************************************************
//	Gets the text of an item and stores it in an buffer
//	hItem		: Is the handle of the item
//	pBuffer		: Is the text buffer where the text will be saved
//	iMax		: Is the size of the text buffer in chars
//	nCol		: Is the column of the item
//	Returns a pointer to the text or NULL if an error occurs
BOOL CTreeListCtrl::GetItemText(HTREEITEM hItem,LPSTR pBuffer,int iMax,int nCol)
{
TV_ITEM		sItem;


	if(iMax<=0)return FALSE;

	pBuffer[0]		 = 0;
	sItem.mask		 = TVIF_TEXT|TVIF_SUBITEM|TVIF_TEXTPTR;
	sItem.hItem		 = hItem;
	sItem.pszText	 = pBuffer;
	sItem.cchTextMax = iMax;
	sItem.cChildren	 = nCol;


return GetItem(&sItem);
}
