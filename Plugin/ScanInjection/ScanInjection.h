// ScanInjection.h : main header file for the SCANINJECTION DLL
//

#if !defined(AFX_SCANINJECTION_H__3B8DEE58_55AD_4906_A730_D6A47C67F245__INCLUDED_)
#define AFX_SCANINJECTION_H__3B8DEE58_55AD_4906_A730_D6A47C67F245__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MainDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CScanInjectionApp
// See ScanInjection.cpp for the implementation of this class
//

class CScanInjectionApp : public CWinApp
{
public:
	CScanInjectionApp();
	CIniFile	m_IniFile;
	CMainDlg	*m_pMyMainDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanInjectionApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CScanInjectionApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANINJECTION_H__3B8DEE58_55AD_4906_A730_D6A47C67F245__INCLUDED_)
