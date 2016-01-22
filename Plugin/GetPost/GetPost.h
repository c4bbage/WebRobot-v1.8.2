// GetPost.h : main header file for the GETPOST DLL
//

#if !defined(AFX_GETPOST_H__0D3CBE55_9F08_4AA5_A729_2A7E0E285D14__INCLUDED_)
#define AFX_GETPOST_H__0D3CBE55_9F08_4AA5_A729_2A7E0E285D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MainDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CGetPostApp
// See GetPost.cpp for the implementation of this class
//

class CGetPostApp : public CWinApp
{
public:
	CGetPostApp();
	CMainDlg 	*pMainDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetPostApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGetPostApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETPOST_H__0D3CBE55_9F08_4AA5_A729_2A7E0E285D14__INCLUDED_)
