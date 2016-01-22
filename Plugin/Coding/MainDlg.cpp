// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Coding.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define URLDECODEENCODE			0
#define URLALLDECODEENCODE		1
#define CONVERTASCII			2
#define BASE64DECODEENCODE		3
#define GETMD5					4
#define SQLVARCHARENCODE		5
#define SQLDECLAREEXECENCODE	6
#define XORSTRORFILE			7

#define FUNC_URLDECODEENCODE		"URL编码解码"
#define FUNC_URLALLDECODEENCODE		"URL全编码"
#define FUNC_CONVERTASCII			"ASCII码转换"
#define FUNC_BASE64DECODEENCODE		"Base64编码解码"
#define FUNC_GETMD5					"MD5加密解密"
#define FUNC_SQLVARCHARENCODE		"SQL VarChar编码"
#define FUNC_SQLDECLAREEXECENCODE	"SQL编码为Declare Exec形式"
#define FUNC_XOR					"XOR加密解密"

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	//}}AFX_DATA_INIT
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_EDIT_DEST, m_editDest);
	DDX_Control(pDX, IDC_EDIT_SRC, m_editSrc);
	DDX_Control(pDX, IDC_BUTTON_DECODE, m_btnDecode);
	DDX_Control(pDX, IDC_BUTTON_ENCODE, m_btnEncode);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ENCODE, OnButtonEncode)
	ON_BN_CLICKED(IDC_BUTTON_DECODE, OnButtonDecode)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_EN_SETFOCUS(IDC_EDIT_SRC, OnSetfocusEditSrc)
	ON_EN_SETFOCUS(IDC_EDIT_DEST, OnSetfocusEditDest)
	//}}AFX_MSG_MAP
	ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTER, OnMaxMinInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comboMode.AddString(FUNC_URLDECODEENCODE);
	m_comboMode.AddString(FUNC_URLALLDECODEENCODE);
	m_comboMode.AddString(FUNC_CONVERTASCII);
	m_comboMode.AddString(FUNC_BASE64DECODEENCODE);
	m_comboMode.AddString(FUNC_GETMD5);
	m_comboMode.SetCurSel(0);
	m_comboMode.SendMessage(CB_SETITEMHEIGHT, -1, 16);
	m_comboMode.SendMessage(CB_SETITEMHEIGHT, 0, 18);

	//Splitter
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_SPLITTER);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER, SPS_HORIZONTAL|SPS_DELTA_NOTIFY);
    //  register windows for splitter
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_UP,    GetDlgItem(IDC_EDIT_SRC));
    m_wndSplitter.RegisterLinkedWindow(SPLS_LINKED_DOWN,    GetDlgItem(IDC_EDIT_DEST));
    m_wndSplitter.Relayout();

	PostMessage(WM_SIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (GetParent()->m_hWnd != NULL && m_comboMode.m_hWnd != NULL)
	{
		RECT rc;
		GetParent()->GetClientRect(&rc);
		MoveWindow(&rc);

		Resize();
	}
}

void CMainDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;	//主程序结束的时候销毁
	CDialog::PostNcDestroy();
}

void CMainDlg::MoveDlgItem(int nID, const CRect& rcPos, BOOL bRepaint)
{
    CWnd* pWnd = this->GetDlgItem(nID);
    if (NULL == pWnd)
        return;
    pWnd->MoveWindow(rcPos, bRepaint);
}

void CMainDlg::Resize()
{
	int nHeight = 0;
    CRect rcDlg;
	GetClientRect(rcDlg);

	nHeight = (rcDlg.bottom - rcDlg.top)/2;
	
	CRect rcSplit1;
	m_wndSplitter.GetWindowRect(rcSplit1);
	ScreenToClient(rcSplit1);
	rcSplit1.left = rcDlg.left;
	rcSplit1.right = rcDlg.right;
	rcSplit1.top = nHeight;
	rcSplit1.bottom = nHeight + 3;
	m_wndSplitter.MoveWindow(rcSplit1, TRUE);
	
	CRect rcSrc;
	rcSrc.left    = rcDlg.left;
	rcSrc.right   = rcDlg.right;
	rcSrc.top     = rcDlg.top + 38;
	rcSrc.bottom  = rcSplit1.top;
	this->MoveDlgItem(IDC_EDIT_SRC, rcSrc, TRUE);
	
	CRect rcDest;
	rcDest.left    = rcDlg.left;
	rcDest.right   = rcDlg.right;
	rcDest.top		= rcSplit1.bottom;
	rcDest.bottom	= rcDlg.bottom;
	this->MoveDlgItem(IDC_EDIT_DEST, rcDest, TRUE);
}

void CMainDlg::OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  Get current pos of the child controls
    CRect rcsrc;
    CRect rcDest;
    m_editSrc.GetWindowRect(rcsrc);
    GetWindowRect(rcDest);
    ScreenToClient(rcsrc);
    ScreenToClient(rcDest);
    //  return the pos limit
    SPC_NM_MAXMINPOS* pNewMaxMinPos = (SPC_NM_MAXMINPOS*)pNMHDR;
    if (IDC_SPLITTER == pNMHDR->idFrom)
    {
        pNewMaxMinPos->lMin = rcsrc.top + 50;
        pNewMaxMinPos->lMax = rcDest.bottom - 50;
    }
}

void CMainDlg::OnButtonEncode() 
{
	UpdateData();
	CString strBuffer;
	CURLEncode url_encode;
	m_editSrc.GetWindowText(strBuffer);
	CString strAll, strSingle;
	int i = 0, nLenth = 0;
	BYTE *lpBuffer = NULL;
	char strMd5[256] = {0};
	switch(m_comboMode.GetCurSel())
	{
	case URLDECODEENCODE:
		strBuffer = url_encode.URLEncode(strBuffer);
		m_editDest.SetWindowText(strBuffer);
		break;
	case URLALLDECODEENCODE:
		strBuffer = url_encode.URLEncodeAll(strBuffer);
		m_editDest.SetWindowText(strBuffer);
		break;
	case CONVERTASCII:
		for (i = 0; i < strBuffer.GetLength(); i++)
		{
			strSingle.Format("%d ", strBuffer.GetAt(i));
			strAll += strSingle;
		}
		strAll.TrimRight(" ");
		strBuffer = strAll;
		strAll.Insert(0, "以空格作为分隔符:\r\n");
		strAll += "\r\n\r\n";
		strBuffer.Replace(" ", ",");
		strBuffer.Insert(0, "以逗号作为分隔符:\r\n");
		strAll += strBuffer;
		m_editDest.SetWindowText(strAll);
		break;
	case BASE64DECODEENCODE:
		try
		{
			m_editDest.SetWindowText(Base64Encode(strBuffer.GetBuffer(0)));
		}
		catch (...){}
		break;
	case SQLVARCHARENCODE:
		GetDlgItemText(IDC_EDIT_SRC, strSingle);
		nLenth = strSingle.GetLength();
		lpBuffer = new BYTE[nLenth];
		memcpy(lpBuffer, strSingle, nLenth);
		strAll.Empty();
		strAll += "0x";
		for (i = 0; i < nLenth; i++)
		{
			strSingle.Format("%02X", lpBuffer[i]);
			strAll += strSingle;
		}
		delete []lpBuffer;
		SetDlgItemText(IDC_EDIT_DEST, strAll);
		break;
	case SQLDECLAREEXECENCODE:
		GetDlgItemText(IDC_EDIT_SRC, strSingle);
		nLenth = strSingle.GetLength();
		lpBuffer = new BYTE[nLenth];
		memcpy(lpBuffer, strSingle, nLenth);
		strAll.Empty();
		strAll += "dEcLaRe @s vArChAr(8000) sEt @s=0x";
		for (i = 0; i < nLenth; i++)
		{
			strSingle.Format("%02X", lpBuffer[i]);
			strAll += strSingle;
		}
		strAll += " eXeC(@s)";
		delete []lpBuffer;
		SetDlgItemText(IDC_EDIT_DEST, strAll);
		break;
	case GETMD5:
		GetDlgItemText(IDC_EDIT_SRC, strSingle);
		lstrcpy(strMd5, MD5String(strSingle.GetBuffer(0)));
		strSingle.Format("%s", strMd5);
		strAll += "16位加密:\r\n";
		strAll += strSingle.Mid(8, 16);
		strAll += "\r\n\r\n";
		strAll += "32位加密:\r\n";
		strAll += strMd5;
		SetDlgItemText(IDC_EDIT_DEST, strAll);
		break;
	default:
		break;
	}
}

void CMainDlg::OnButtonDecode() 
{
	UpdateData();
	CURLEncode url_decode;
	CString strBuffer;
	m_editSrc.GetWindowText(strBuffer);
	std::string s(strBuffer.GetBuffer(0));
	
	CString strAll, strSingle;
	int i = 0, nLenth = 0;
	BYTE *lpBuffer = NULL;
	BSTR bstr;
	switch(m_comboMode.GetCurSel())
	{
	case URLDECODEENCODE:
		if (strBuffer.GetLength() < 1)
			return;
		m_editDest.SetWindowText(URLDecode(strBuffer));
		break;
	case CONVERTASCII:
		strBuffer.TrimLeft(" ");
		strBuffer.TrimLeft("\t");
		strBuffer.TrimRight(" ");
		strBuffer.TrimRight("\t");
		strBuffer.Replace("  ", " ");// 两个空格变为一个空格
		strBuffer.Replace(" ", ",");
		strBuffer.Replace(",,", ",");
		if (strBuffer.Right(1) != ",")
			strBuffer += ",";
		while (1)
		{
			nLenth = strBuffer.Find(",", i);
			if (nLenth == -1)
				break;
			strSingle = strBuffer.Mid(i, nLenth - i);
			i = nLenth + 1;
			strSingle.Format("%c", atoi(strSingle));
			strAll += strSingle;
		}
		m_editDest.SetWindowText(strAll);
		break;
	case BASE64DECODEENCODE:
		if (strBuffer.GetLength() < 1)
			return;
		try
		{
			strBuffer.Replace("\r\n", "");
			strBuffer.Replace(" ", "");
			m_editDest.SetWindowText(Base64Decode(strBuffer.GetBuffer(0)));
		}
		catch (...){}
		
		break;
	case SQLVARCHARENCODE:
		if (strBuffer.GetLength() < 1)
			return;
		GetDlgItemText(IDC_EDIT_SRC, strSingle);
		bstr = strSingle.AllocSysString();
		nLenth = wcslen(bstr) * 2;
		lpBuffer = new BYTE[nLenth];
		memcpy(lpBuffer, bstr, nLenth);
		strAll.Empty();
		strAll += "0x";
		for (i = 0; i < nLenth; i++)
		{
			strSingle.Format("%02X", lpBuffer[i]);
			strAll += strSingle;
		}
		delete []lpBuffer;
		SysFreeString(bstr);
		SetDlgItemText(IDC_EDIT_DEST, strAll);
		break;
	case SQLDECLAREEXECENCODE:
		if (strBuffer.GetLength() < 1)
			return;
		GetDlgItemText(IDC_EDIT_SRC, strSingle);
		bstr = strSingle.AllocSysString();
		nLenth = wcslen(bstr) * 2;
		lpBuffer = new BYTE[nLenth];
		memcpy(lpBuffer, bstr, nLenth);
		strAll.Empty();
		strAll += "dEcLaRe @s nvArChAr(4000) sEt @s=0x";
		for (i = 0; i < nLenth; i++)
		{
			strSingle.Format("%02X", lpBuffer[i]);
			strAll += strSingle;
		}
		strAll += " eXeC(@s)";
		delete []lpBuffer;
		SysFreeString(bstr);
		SetDlgItemText(IDC_EDIT_DEST, strAll);
		break;
	case GETMD5:
		ShellExecute(0, "open", "http://www.cmd5.com/", NULL, NULL, SW_SHOW);
		break;
	default:
		break;
	}
}

void CMainDlg::OnSelchangeComboMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_comboMode.GetCurSel() == 1)
		m_btnDecode.ShowWindow(SW_HIDE);
	else
		m_btnDecode.ShowWindow(SW_SHOW);
}

void CMainDlg::OnSetfocusEditSrc() 
{
	m_editSrc.SetSel(0, -1);
}

void CMainDlg::OnSetfocusEditDest() 
{
	m_editDest.SetSel(0, -1);	
}
