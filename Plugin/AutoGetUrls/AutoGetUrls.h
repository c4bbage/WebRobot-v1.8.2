// AutoGetUrls.h : main header file for the AUTOGETURLS DLL
//

#if !defined(AFX_AUTOGETURLS_H__E004742E_062E_4ABC_9F61_CF460A144021__INCLUDED_)
#define AFX_AUTOGETURLS_H__E004742E_062E_4ABC_9F61_CF460A144021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoGetUrlsApp
// See AutoGetUrls.cpp for the implementation of this class
//

class CAutoGetUrlsApp : public CWinApp
{
public:
	CAutoGetUrlsApp();
	CIniFile	m_IniFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoGetUrlsApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAutoGetUrlsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOGETURLS_H__E004742E_062E_4ABC_9F61_CF460A144021__INCLUDED_)
