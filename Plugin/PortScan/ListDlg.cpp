// ListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PortScan.h"
#include "ListDlg.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainDlg *g_pMainDlg;
/////////////////////////////////////////////////////////////////////////////
// CListDlg dialog


CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListDlg)
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	//{{AFX_MSG_MAP(CListDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListDlg message handlers

BOOL CListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CListDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString strTemp, strAll;
	int nCount = 0;
	for (int i = 0; i < m_edit.GetLineCount(); i++)
	{
		int len = m_edit.LineLength(m_edit.LineIndex(i));
		m_edit.GetLine(i, strTemp.GetBuffer(len), len);
		strTemp.ReleaseBuffer(len);
		strTemp.Replace(" ", "");
		strTemp.Replace("\t", "");
		
		if (strTemp.GetLength() >= 7)//长度大于7
		{
			strAll += strTemp;
			strAll += ",";
		}

		strTemp.ReleaseBuffer();
	}

	strAll.Replace(",,", ",");
	g_pMainDlg->m_comboIP.SetWindowText(strAll);
	CDialog::OnCancel();
}
