// HeaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetPost.h"
#include "HeaderDlg.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainDlg *g_pMainDlg;
/////////////////////////////////////////////////////////////////////////////
// CHeaderDlg dialog


CHeaderDlg::CHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHeaderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeaderDlg)
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_EDIT_HEADER, m_editHeader);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeaderDlg, CDialog)
	//{{AFX_MSG_MAP(CHeaderDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_EN_CHANGE(IDC_EDIT_HEADER, OnChangeEditHeader)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderDlg message handlers

BOOL CHeaderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editHeader.SetLimitText(MAXDWORD); // 设置最大长度
	if (m_editHeader.GetWindowTextLength() == 0)
		m_btnClear.ShowWindow(SW_HIDE);

	m_editHeader.SetPromptColor(RGB(188,188,188));
	m_editHeader.SetPromptText(_T("格式例如：\r\n\r\nGET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: keep-alive\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1;)\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: zh-CN,zh;q=0.8,en;q=0.6\r\nCookie: BAIDUID=BCF38D27D56494FFA01DA7C35C5A14A9:FG=1; PSTM=1435023979; \r\n\r\n"));
	
	GetDlgItem(IDC_EDIT_FOCUS)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHeaderDlg::OnButtonAdd() 
{
	UpdateData();
	g_pMainDlg->SetDlgItemText(IDC_EDIT_HEADER, "");
	g_pMainDlg->SetDlgItemText(IDC_EDIT_POST, "");
	
	CString strHost, strUrl, strTemp, strOtherHeader;
	CStringList sHeaderList;
	
	for (int i = 0; i < m_editHeader.GetLineCount(); i++)
	{
		int len = m_editHeader.LineLength(m_editHeader.LineIndex(i));
		m_editHeader.GetLine(i, strTemp.GetBuffer(len), len);
		strTemp.ReleaseBuffer(len);
		strTemp.TrimLeft(" ");
		strTemp.TrimRight(" ");
		strTemp.TrimLeft("\t");
		strTemp.TrimRight("\t");
		
		if (strTemp.GetLength() > 0)
		{
			if (strTemp.Find(": ", 0) != -1 || strTemp.Find("HTTP/1.1", 0) != -1 || strTemp.Find("HTTP/1.0", 0) != -1 || strTemp.Find("HTTP/1.2", 0) != -1)
				sHeaderList.AddTail(strTemp);	
		}
		strTemp.ReleaseBuffer();
	}
	for (int nHeaderCount = 0; nHeaderCount < sHeaderList.GetCount(); nHeaderCount++)
	{
		strTemp = sHeaderList.GetAt(sHeaderList.FindIndex(nHeaderCount));
		if (strTemp.Find("Host: ", 0) != -1)
		{
			strTemp.Replace("Host: ", "http://");
			strHost = strTemp;
		}
		else if (strTemp.Left(3) == "GET" || strTemp.Left(4) == "POST")
		{
			strTemp.TrimLeft("GET");
			strTemp.TrimLeft("POST");
			strTemp.TrimLeft(" ");
			strTemp.TrimRight(" ");
			strTemp.TrimRight("HTTP/1.0");
			strTemp.TrimRight("HTTP/1.1");
			strTemp.TrimRight("HTTP/1.2");
			strTemp.TrimRight(" ");
			strUrl = strTemp;
		}
		else
		{
			strOtherHeader += strTemp;
			strOtherHeader += "\r\n";
		}
	}
	strTemp = strHost;
	strTemp += strUrl;
	g_pMainDlg->SetDlgItemText(IDC_EDIT_URL, strTemp);
	strOtherHeader.TrimLeft("\r\n");
	strOtherHeader.TrimRight("\r\n");
	g_pMainDlg->SetDlgItemText(IDC_EDIT_HEADER, strOtherHeader);

	CDialog::OnCancel();
}

void CHeaderDlg::OnButtonClear() 
{
	m_editHeader.SetWindowText("");
	m_btnClear.ShowWindow(SW_HIDE);
}

void CHeaderDlg::OnChangeEditHeader() 
{
	if (m_editHeader.GetWindowTextLength() != 0)
		m_btnClear.ShowWindow(SW_SHOW);
}
