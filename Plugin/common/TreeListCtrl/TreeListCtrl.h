//*****************************************************************************
//*
//*		
//*		TreeListCtrl.h
//*
//*
//*****************************************************************************
#ifndef 	__TREELISTCTRL_H__
#define 	__TREELISTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include	<AfxCmn.h>
#include	"TreeListWnd.h"

class CHeaderCtrl;



//*****************************************************************************
//*
//*		CTreeListCtrl
//*
//*****************************************************************************
class CTreeListCtrl : public CTreeCtrl
{

	DECLARE_DYNAMIC(CTreeListCtrl)

public:

	CTreeListCtrl(){};

	BOOL		 Create				 (DWORD dwStyle, const RECT &sRect, CWnd* pParentWnd, UINT nId);
	int  		 InsertColumn  		 (int nCol, LPCTSTR pColumnText, int nFormat=TVCFMT_LEFT, int nWidth=-1);
	int  		 InsertColumn  		 (int nCol, const TV_COLUMN* pColumn );
	BOOL		 SetColumn     		 (int nCol, const TV_COLUMN* pColumn );
	BOOL		 GetColumn     		 (int nCol, TV_COLUMN* pColumn ) const;
	int			 GetColumnWidth		 (int nCol) const;
	BOOL		 SetColumnWidth		 (int nCol, int iWidth);
	BOOL 		 DeleteColumn		 (int nCol);
	CHeaderCtrl *GetHeaderCtrl		 () const;
	CEdit		*EditLabel			 (HTREEITEM hItem,int nCol=0,int iFull=0);
	CComboBox	*EditLabelCb		 (HTREEITEM hItem,int nCol=0,int iFull=0,int iList=0);
	CImageList  *CreateDragImage	 (HTREEITEM hItem,int nCol=0); 	
	DWORD		 GetExtendedStyle	 () const;
	DWORD		 SetExtendedStyle	 (DWORD dwStyle);
	DWORD		 SetExtendedStyle	 (DWORD dwStyle,DWORD dwMask);
	COLORREF	 GetColor			 (int iIndex) const;
	COLORREF	 SetColor			 (int iIndex,COLORREF uColor);
	COLORREF	 GetItemBkColor		 (HTREEITEM hItem,int nCol) const;
	COLORREF	 SetItemBkColor		 (HTREEITEM hItem,int nCol,COLORREF uColor);
	COLORREF	 GetItemTextColor	 (HTREEITEM hItem,int nCol) const;
	COLORREF	 SetItemTextColor	 (HTREEITEM hItem,int nCol,COLORREF uColor);
	BOOL		 EnsureVisible		 (HTREEITEM hItem,int nCol);
	BOOL		 EnsureVisible		 (HTREEITEM hItem);
	BOOL		 SelectDropTarget	 (HTREEITEM hItem,int nCol=0);
	BOOL		 SelectItem			 (HTREEITEM hItem,int nCol=0);
	BOOL		 GetItemRect		 (HTREEITEM hItem,int nCol,RECT *pRect,int iCode);
	BOOL		 GetItemRect		 (HTREEITEM hItem,		   RECT *pRect,int iCode);
	BOOL		 GetItemText		 (HTREEITEM hItem,LPSTR pBuffer,int iMax,int nCol=0);
	LPCTSTR		 GetItemText		 (HTREEITEM hItem,                       int nCol=0);
	BOOL		 SetItemText		 (HTREEITEM hItem,LPCSTR pText          ,int nCol=0);
	UINT 		 GetItemState        (HTREEITEM hItem,UINT nStateMask       ,int nCol=0);
	BOOL		 SetItemState        (HTREEITEM hItem,int nCol,UINT nState,UINT nStateMask);
	BOOL		 SetItem			 (HTREEITEM hItem,int nCol,UINT nMask, LPCTSTR pText, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam );
	BOOL		 SetItem			 (TVITEM* pItem);
	BOOL		 SortChildrenCB		 (LPTVSORTCB pSort,BOOL bRecusive=FALSE);
	BOOL		 SortChildren		 (HTREEITEM hItem ,BOOL bRecusive=FALSE);
	LPVOID		 GetUserData		 (HTREEITEM hItem) const;
	INT			 SetUserDataSize     (int iSize);
	INT			 GetUserDataSize	 () const;

	HTREEITEM	 GetNextSelected	 (HTREEITEM hItem);
	HTREEITEM	 GetNextSelectedChild(HTREEITEM hItem);
	HTREEITEM	 GetFocus			 ();
	int 		 GetFocusColumn		 (); 
	int 		 GetSelectionColumn	 (); 
	int 		 GetDropHilightColumn();

	// ListCtrl Functions

	int			 GetRowOfItem		 (HTREEITEM hItem) const;
	HTREEITEM	 GetItemOfRow		 (int iRow) const;
	BOOL		 ListDeleteItem		 (int iRow);
	int 		 ListInsertItem		 (int iRow,			LPCTSTR pText,int nImage=-1,int nState=-1,int iMask=TVIS_BOLD|TVIS_UNTERLINE|TVIS_SELECTED|TVIS_OVERLAYMASK|TVIS_STATEIMAGEMASK);	
	BOOL		 ListSetItem		 (int iRow,			LPCTSTR pText,int nImage=-1,int nState=-1,int iMask=TVIS_BOLD|TVIS_UNTERLINE|TVIS_SELECTED|TVIS_OVERLAYMASK|TVIS_STATEIMAGEMASK);	
	BOOL		 ListSetItem		 (int iRow,int iCol,LPCTSTR pText,int nImage=-1,int nState=-1,int iMask=TVIS_BOLD|TVIS_UNTERLINE|TVIS_SELECTED|TVIS_OVERLAYMASK|TVIS_STATEIMAGEMASK);	
	BOOL		 ListSetColor		 (int iRow,int iCol,COLORREF  uBkColor=TV_NOCOLOR,COLORREF  uTextColor=TV_NOCOLOR); 
	BOOL		 ListGetColor		 (int iRow,int iCol,COLORREF &uBkColor           ,COLORREF &uTextColor);

	
};


//*****************************************************************************
//*
//*		Inline Funktions
//*
//*****************************************************************************


inline	LPVOID		 CTreeListCtrl::GetUserData(HTREEITEM hItem) const							{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetUserData(m_hWnd,hItem);}
inline	INT			 CTreeListCtrl::GetUserDataSize() const										{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetUserDataSize(m_hWnd);}
inline	INT			 CTreeListCtrl::SetUserDataSize(int iSize)									{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetUserDataSize(m_hWnd,iSize);}
inline	BOOL		 CTreeListCtrl::GetColumn(int nCol, TV_COLUMN *pColumn) const				{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetColumn(m_hWnd,nCol,pColumn);}
inline	BOOL		 CTreeListCtrl::SetColumn(int nCol, const TV_COLUMN *pColumn)				{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetColumn(m_hWnd,nCol,pColumn);}
inline	INT 		 CTreeListCtrl::InsertColumn(int nCol, const TV_COLUMN *pColumn)			{ ASSERT(::IsWindow(m_hWnd)); return TreeList_InsertColumn(m_hWnd,nCol,pColumn); }
inline	BOOL		 CTreeListCtrl::DeleteColumn(int nCol)										{ ASSERT(::IsWindow(m_hWnd)); return TreeList_DeleteColumn(m_hWnd,nCol); }
inline	BOOL		 CTreeListCtrl::SortChildren  (HTREEITEM hItem,BOOL bRecusive)				{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SortChildren  (m_hWnd,hItem,bRecusive); }
inline	BOOL		 CTreeListCtrl::SortChildrenCB(LPTVSORTCB pSort,BOOL bRecusive)				{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SortChildrenCB(m_hWnd,pSort,bRecusive); }
inline	INT			 CTreeListCtrl::GetColumnWidth(int nCol) const								{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetColumnWidth(m_hWnd,nCol); }
inline	BOOL		 CTreeListCtrl::SetColumnWidth(int nCol, int iWidth)						{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetColumnWidth(m_hWnd,nCol,iWidth); }
inline	HTREEITEM	 CTreeListCtrl::GetItemOfRow(int iRow) const								{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetItemOfRow(m_hWnd,iRow); }
inline	INT			 CTreeListCtrl::GetRowOfItem(HTREEITEM hItem) const							{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetRowOfItem(m_hWnd,hItem); }
inline	DWORD		 CTreeListCtrl::GetExtendedStyle() const									{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetExtendedStyle(m_hWnd); }
inline	DWORD		 CTreeListCtrl::SetExtendedStyle(DWORD dwStyle)								{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetExtendedStyle(m_hWnd,dwStyle); }
inline	DWORD		 CTreeListCtrl::SetExtendedStyle(DWORD dwStyle,DWORD dwMask)				{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetExtendedStyleEx(m_hWnd,dwStyle,dwMask); }
inline	COLORREF	 CTreeListCtrl::GetColor(int iIndex) const									{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetColor(m_hWnd,iIndex); }
inline	COLORREF	 CTreeListCtrl::SetColor(int iIndex,COLORREF uColor)						{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetColor(m_hWnd,iIndex,uColor); }
inline	COLORREF	 CTreeListCtrl::GetItemBkColor(HTREEITEM hItem,int nCol) const				{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetItemBkColor(m_hWnd,hItem,nCol); }
inline	COLORREF	 CTreeListCtrl::SetItemBkColor(HTREEITEM hItem,int nCol,COLORREF uColor)	{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetItemBkColor(m_hWnd,hItem,nCol,uColor); }
inline	COLORREF	 CTreeListCtrl::GetItemTextColor(HTREEITEM hItem,int nCol) const			{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetItemTextColor(m_hWnd,hItem,nCol); }
inline	COLORREF	 CTreeListCtrl::SetItemTextColor(HTREEITEM hItem,int nCol,COLORREF uColor)	{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SetItemTextColor(m_hWnd,hItem,nCol,uColor); }
inline	BOOL		 CTreeListCtrl::EnsureVisible(HTREEITEM hItem,int nCol)						{ ASSERT(::IsWindow(m_hWnd)); return TreeList_EnsureVisibleEx(m_hWnd,hItem,nCol); }
inline	BOOL		 CTreeListCtrl::SelectDropTarget(HTREEITEM hItem,int nCol)					{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SelectDropTargetEx(m_hWnd,hItem,nCol); }
inline	BOOL		 CTreeListCtrl::SelectItem(HTREEITEM hItem,int nCol)						{ ASSERT(::IsWindow(m_hWnd)); return TreeList_SelectSubItem(m_hWnd,hItem,nCol); }
inline	BOOL		 CTreeListCtrl::GetItemRect(HTREEITEM hItem,int nCol,RECT *pRect,int iCode)	{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetItemRect(m_hWnd,hItem,nCol,pRect,iCode); }			
inline	HTREEITEM	 CTreeListCtrl::GetNextSelectedChild(HTREEITEM hItem) 						{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetNextSelectedChild(m_hWnd,hItem); }	
inline	HTREEITEM	 CTreeListCtrl::GetNextSelected(HTREEITEM hItem) 							{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetNextSelected(m_hWnd,hItem); }
inline	HTREEITEM	 CTreeListCtrl::GetFocus()													{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetFocus(m_hWnd); }
inline	INT 		 CTreeListCtrl::GetFocusColumn() 											{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetFocusColumn(m_hWnd); }														
inline	INT 		 CTreeListCtrl::GetSelectionColumn() 										{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetSelectionColumn(m_hWnd); }
inline	INT 		 CTreeListCtrl::GetDropHilightColumn() 										{ ASSERT(::IsWindow(m_hWnd)); return TreeList_GetDropHilightColumn(m_hWnd); }	
inline	CEdit		*CTreeListCtrl::EditLabel(HTREEITEM hItem,int nCol,int iFull)				{ ASSERT(::IsWindow(m_hWnd)); return (CEdit      *)CEdit      ::FromHandle(TreeList_EditLabel(m_hWnd,hItem,(nCol&0x1FFFFFFF)|((iFull)? TVIR_EDITFULL:0))); }
inline	CComboBox	*CTreeListCtrl::EditLabelCb(HTREEITEM hItem,int nCol,int iFull,int iList)	{ ASSERT(::IsWindow(m_hWnd)); return (CComboBox  *)CEdit      ::FromHandle(TreeList_EditLabel(m_hWnd,hItem,(nCol&0x1FFFFFFF)|((iFull)? TVIR_EDITFULL:0)|((iList)? TVIR_EDITCOMBOLIST:TVIR_EDITCOMBOBOX))); }
inline	CImageList  *CTreeListCtrl::CreateDragImage(HTREEITEM hItem,int nCol)					{ ASSERT(::IsWindow(m_hWnd)); return (CImageList *)CImageList ::FromHandle(TreeList_CreateDragImageEx(m_hWnd,hItem,nCol)); }
inline	CHeaderCtrl *CTreeListCtrl::GetHeaderCtrl() const										{ ASSERT(::IsWindow(m_hWnd)); return (CHeaderCtrl*)CHeaderCtrl::FromHandle(TreeList_GetHeader(m_hWnd)); }


inline	BOOL		 CTreeListCtrl::EnsureVisible(HTREEITEM hItem)								{ return EnsureVisible(hItem,0xFFFFFFFF); }		
inline	BOOL		 CTreeListCtrl::GetItemRect(HTREEITEM hItem,RECT *pRect,int iCode)			{ return GetItemRect(hItem,pRect,iCode); }
inline	BOOL		 CTreeListCtrl::SetItem(TVITEM* pItem)										{ return CTreeCtrl::SetItem(pItem); }
inline	BOOL		 CTreeListCtrl::ListSetItem(int iRow,LPCTSTR pTxt,int nImg,int nSt,int iMsk){ return ListSetItem(iRow,0,pTxt,nImg,nSt,iMsk);	}

#endif 	
