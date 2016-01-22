// ScanCSection.h : main header file for the SCANCSECTION DLL
//

#if !defined(AFX_SCANCSECTION_H__B38F7640_3CF3_4C2E_A1CE_75C0C55B7D26__INCLUDED_)
#define AFX_SCANCSECTION_H__B38F7640_3CF3_4C2E_A1CE_75C0C55B7D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MainDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CScanCSectionApp
// See ScanCSection.cpp for the implementation of this class
//

class CScanCSectionApp : public CWinApp
{
public:
	CScanCSectionApp();
	CIniFile	m_IniFile;
	CMainDlg	*m_pMyMainDlg;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanCSectionApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CScanCSectionApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANCSECTION_H__B38F7640_3CF3_4C2E_A1CE_75C0C55B7D26__INCLUDED_)
