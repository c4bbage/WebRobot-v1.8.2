#if !defined(AFX_LISTCTRLEX_H__INCLUDED_)
#define AFX_LISTCTRLEX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//
#include <afxtempl.h>       // MFC core and standard components

#define _CCS_DES

#define WM_COLUMN_CHANGED		WM_USER+5	//改变列的宽度时发送该消息
#define WM_ITEM_CHANGED			WM_USER+8	//某一项被编辑改变内容后发送该消息
#define WM_LISTBUTTON_CLICKED	WM_USER+10	//列表中的按钮被单击后发送该消息

#define CS_NONE			0x00000000	//无控件
#define CS_EDIT			0x00000001	//编辑框
#define CS_COMBOBOX		0x00000002	//组合框
#define CS_BUTTON		0x00000004	//按钮


/*=========================================================================================
CListCtrlEx Class Version 3.00
Author:luxintian
Company:南宁创能科技开发有限公司
Tel:13737075336
E-Mail:luxintian@sina.com
Date:2005-05-07
All right reserved.
类的功能：
本类是继承于MFC类中的CListCtrl类，在该类中添加了一下功能
1，可以修改各项的字体颜色
2，可以修改各项的背景颜色
3，点击列表头的列标题可以以该列数据进行排序
4，可以用文本框或者组合框编辑各项，但是要使某一项能被编辑必须调用SetEdit函数(默认是不被编辑的)

修改历程：
1，去掉了CArray<COLORREF,COLORREF> m_aryBKColor和CArray<COLORREF,COLORREF> m_aryTextColor
2，改为ITEMCOLOR结构。
3，在插入项的时候把ITEMCOLOR结构实例的地址保存于LVITEM结构中的参数lParam
4，2005年6月22日重新修正了内存泄漏的Bug,添加了OnDestroy()函数在函数中添加
   了删除已经为ITEMCOLOR结构变量空间。
5，2006年12月27日添加了WM_COLUMN_CHANGED自定义消息，该消息是在改变列的宽度的时候发送该消息到主窗体
   在主窗体添加该消息的处理函数，就可以处理在改变列的宽度时的事件。
6，2007年3月28日添加了在InsertColumn函数中的nFormat参数的的可用性（之前只是居中可用）可以使用LVCFMT_LEFT、
   LVCFMT_CENTER和LVCFMT_RIGHT来使每一列的左对齐，居中和右对齐。
7，2007年6月3日添加了SetItemText和GetItemText函数的重载，该函数的第二个参数可以是列表头的名称，即
   LPCTSTR类型的参数
8，2007年6月4日添加了SetColumnWidth和GetColumnWidth函数的重载，该函数的第二个参数可以是列表头的名称，即
   LPCTSTR类型的参数
9，2007年6月7日修改了SetItemTextColor和SetItemBKColor和SetItemText中的内容，使其减少刷新次数
10，2007年9月25日删除了OnDestroy函数；删除了InsertItem函数使用了OnInsertitem函数添加附加代码；
    删除了DeleteItem和DeleteAllItems函数使用了OnDeleteitem函数添加附加代码；
    修改了tagITEMCOLOR结构为CListCtrlEx的成员结构
11，2007年9月7日在原来的基础上添加CListCellCb类和CListCellEd类，使列表控件以编辑。
12，2007年10月17日添加了列表打印功能，使用PrintReport函数和GetPrintPages函数，在对话框he视图中分别调用两个
    不同版本的PrintReport函数，在视图中把PrintReport函数放到OnPrint函数中，但是必须在OnBeginPrinting函数中
	调用GetPrintPages函数获得要打印的页数并调用pInfo->SetMaxPage函数设置页数,该打印功能只能用在固定结构形式,
	如果要打印其他样式,则不能使用该函数,只能自己写代码了
13，2008年1月3日添加了在列表中按钮的能力，在InsertColumn函数中使用CS_BUTTON样式即可实现，并在点击按钮时向
	父窗体发送WM_LISTBUTTON_CLICKED自定义消息
14，2008年3月24日修正了打印行的误差积累问题，每一行的高度用double类型的变量来表示。
15，2008年4月18日修正了在XP样式的界面下表头显示异常问题，原因是表头采用重绘的方法导致，修正方法：不采用重绘，
	而是加入“shell32.dll”中的位图作为表头排序图标
------------------------
注意：
WM_ITEM_CHANGED消息为列表被修改后发出的消息，WPARAM为被修改的的项索引；LPARAM为被修改子项索引
如果该控件是在设计时创建的要在文件开头添加宏定义：#define _CCS_DES
如果该控件是在运行时创建的要在文件开头添加宏定义：#define _CCS_RUN
*///=======================================================================================
//////////////////////////////////////////////////////////////////////////
// CListCellBT window

class CListCellBT : public CButton
{
	// Construction
public:
	CListCellBT(int nItem,int nSubItem);
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCellBT)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CListCellBT();
	
	// Generated message map functions
protected:
	int			m_nItem;
    int			m_nSubItem;
	
	//{{AFX_MSG(CListCellBT)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnClicked();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

// CListCellEd window

class CListCellEd : public CEdit
{
	// Construction
public:
	CListCellEd(int nItem, int nSubItem, CString strInitTex);
	void SetListItemText();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCellEd)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CListCellEd();
	
protected:
	int			m_nItem;
    int			m_nSubItem;
    CString		m_strInitText;
    BOOL		m_bEscape;
	
	// Generated message map functions
	//{{AFX_MSG(CListCellEd)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CListCellCb window

class CListCellCb : public CComboBox
{
	// Construction
public:
	CListCellCb();
	CListCellCb(int nItem,int nSubItem,CString strWindowText);
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCellCb)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CListCellCb();
	
	// Generated message map functions
protected:	
	int m_iRowIndex;// Index of the item in the list control	
	int m_iColumnIndex;// Index of the subitem in the list control		
	CString m_strWindowText;// Previous selected string value in the combo control
	
	//{{AFX_MSG(CListCellCb)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CListHeader window

class CListHeader : public CHeaderCtrl
{
	// Construction
public:
	CListHeader();
	
	// Attributes
public:
	
	// Operations
public:
	
protected:
	int m_nUpBit;
	int m_nDownBit;
	CImageList m_ImgList;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListHeader)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void SetSortArrow( const int iSortColumn, const BOOL bSortAscending );
	virtual ~CListHeader();
	
	// Generated message map functions
protected:
	virtual BOOL OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes);	
	BOOL m_bSortAscending;
	int m_iSortColumn;	
	//{{AFX_MSG(CListHeader)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
	//项数据
	typedef struct tagITEMCOLOR
	{
		tagITEMCOLOR(){dwData=0;crBK=NULL;crText=NULL;bEdit=FALSE;}
		DWORD		dwData;	//用于保存CLuListVCtrl::SetItemData/CLuListVCtrl::GetItemData的数据保存LVITEM结构中的参数lParam
		COLORREF	crBK;	//保存项的背景颜色
		COLORREF	crText;	//保存项的字体颜色
		BOOL		bEdit;	//是否可以编辑
	} ITEMCOLOR,*LPITEMCOLOR;
	//子项样式信息
	typedef struct tagSUBITEMINFO
	{
		tagSUBITEMINFO(){m_dwData=0;m_dwColumnType=0;m_parComboString=NULL;}
		DWORD			m_dwData;
		DWORD			m_dwColumnType;
		CStringArray*	m_parComboString;
	} SUBITEMINFO,*LPSUBITEMINFO;
	
// Construction
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();
		
// Operations
public:
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	//}}AFX_VIRTUAL
		
#ifdef _CCS_DES
	virtual void PreSubclassWindow();		
#endif

public:

#ifdef _CCS_RUN
	BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );	
#endif
	
	
// Implementation
public:
	//返回需要打印的页数
	int GetPrintPages(CDC* pDC);
	//打印列表内容,视图中适用(strTitle:报表标题,strHeader:页眉)	
	void PrintReport(CDC* pDC,CPrintInfo* pInfo,CString strTitle,CString strHeader=_T(""));
	//打印列表内容,对话框适用(strTitle:报表标题,strHeader:页眉)
	void PrintReport(CString strTitle,CString strHeader=_T(""));
	//插入列(nStyle:列样式;pStringArray:字符串数组指针;其他参数同MFC)
	int InsertColumn( int nCol, const LVCOLUMN* pColumn, DWORD nStyle = CS_NONE,CStringArray * pStringArray = NULL );
	//插入列(nStyle:列样式;pStringArray:字符串数组指针;其他参数同MFC)	
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, DWORD nStyle = CS_NONE ,CStringArray * pStringArray = NULL, int nSubItem = -1 );
	//删除列(nCol:列索引)
	BOOL DeleteColumn( int nCol );
	//显示子项控件(nItem:项索引;nSubItem:子项)
	void ShowSubCtrl(int nItem, int nSubItem);
	//获得指定项是否可以编辑(nItem:项索引,返回是否能被编辑)
	BOOL CanEdit(int nItem);
	//使指定项可否以编辑(nItem:项索引;bEdit:是否能被编辑，如果要使那一项被编辑必须调用此函数)
	void SetEdit(int nItem,BOOL bEdit = TRUE);
	//获取指定项的字体颜色(nItem:项索引)
	COLORREF GetItemTextColor(int nItem) const;
	//设置指定项的字体颜色(nItem:项索引;crTextColor:字体颜色)
	BOOL SetItemTextColor(int nItem,COLORREF crTextColor);
	//获取指定项的背景颜色(nItem:项索引)
	COLORREF GetItemBKColor(int nItem) const;
	//设置指定项的背景颜色(nItem:项索引;crBKColor:背景颜色)
	BOOL SetItemBKColor(int nItem,COLORREF crBKColor);
	//获取项的数据(nItem:项索引)
	DWORD GetItemData( int nItem ) const;
	//设置项的数据(nItem:项索引,dwData:项数据)
	BOOL SetItemData( int nItem, DWORD dwData );
	//设置项文本(nItem:项索引;nSubItem:子项;lpszText:文本)
	BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );
	//设置项文本(nItem:项索引;lpszSubItem:子项名称;lpszText:文本)	
	BOOL SetItemText( int nItem, LPCTSTR lpszSubItem, LPCTSTR lpszText );
	//获取项文本(nItem:项索引;nSubItem:子项;lpszText:文本;nLen:文本长度)
	int GetItemText( int nItem, int nSubItem, LPTSTR lpszText, int nLen );
	//获取项文本(nItem:项索引;lpszSubItem:子项列名称;lpszText:文本;nLen:文本长度)	
	int GetItemText( int nItem, LPCTSTR lpszSubItem, LPTSTR lpszText, int nLen );
	//获取项文本(nItem:项索引;nSubItem:子项;返回项的文本)
	CString GetItemText( int nItem, int nSubItem );
	//获取项文本(nItem:项索引;lpszSubItem:子项列名称;返回项的文本)	
	CString GetItemText( int nItem, LPCTSTR lpszSubItem );
	//设置列的宽度(nCol:列索引;cx:宽度)
	BOOL SetColumnWidth( int nCol, int cx );
	//设置列的宽度(lpszCol:子项列名称;cx:宽度)	
	BOOL SetColumnWidth( LPCTSTR lpszCol, int cx );
	//获取列的宽度(nCol:列索引;返回宽度)
	int GetColumnWidth( int nCol );
	//获取列的宽度(lpszCol:子项列名称;返回宽度)
	int GetColumnWidth( LPCTSTR lpszCol );	

	void DisableSort();

	// Generated message map functions
protected:
	void PrintPage(int nPage,CDC* pDC,CRect rcPrint, CString strTitle, CString strHeader=_T(""));	
	BOOL m_bSortAscending;
	CArray<LPSUBITEMINFO,LPSUBITEMINFO> m_arColumnInfo;	
	int m_iSortColumn;//所选的列
	CListHeader m_ctlHeader;
	BOOL m_bDisableSort;
	
	int HitTestEx (CPoint& Point, int& nSubItem);	
	static bool IsNumber( LPCTSTR pszText );
	static int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 );
	static bool IsDate( LPCTSTR pszText );
	static int DateCompare( const CString& strDate1, const CString& strDate2 );
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
	
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);		
	afx_msg void OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__INCLUDED_)
