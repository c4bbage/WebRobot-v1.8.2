// Spider.h : main header file for the SPIDER DLL
//

#if !defined(AFX_SPIDER_H__CE4641F0_865C_42A7_9B19_1B14AFDF2A80__INCLUDED_)
#define AFX_SPIDER_H__CE4641F0_865C_42A7_9B19_1B14AFDF2A80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSpiderApp
// See Spider.cpp for the implementation of this class
//

class CSpiderApp : public CWinApp
{
public:
	CSpiderApp();
	CIniFile	m_IniFile;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpiderApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSpiderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPIDER_H__CE4641F0_865C_42A7_9B19_1B14AFDF2A80__INCLUDED_)
