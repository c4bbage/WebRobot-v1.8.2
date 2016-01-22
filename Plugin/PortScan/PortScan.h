// PortScan.h : main header file for the PORTSCAN DLL
//

#if !defined(AFX_PORTSCAN_H__5261D648_0160_45CE_B26A_0BDDCB1ADEEE__INCLUDED_)
#define AFX_PORTSCAN_H__5261D648_0160_45CE_B26A_0BDDCB1ADEEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MainDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CPortScanApp
// See PortScan.cpp for the implementation of this class
//

class CPortScanApp : public CWinApp
{
public:
	CPortScanApp();
	CIniFile	m_IniFile;
	CMainDlg	*m_pMyMainDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortScanApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPortScanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTSCAN_H__5261D648_0160_45CE_B26A_0BDDCB1ADEEE__INCLUDED_)
