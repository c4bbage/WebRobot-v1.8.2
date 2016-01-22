// SubDomain.h : main header file for the SUBDOMAIN DLL
//

#if !defined(AFX_SUBDOMAIN_H__3260529B_3818_4FFC_B38E_59DAA99B355C__INCLUDED_)
#define AFX_SUBDOMAIN_H__3260529B_3818_4FFC_B38E_59DAA99B355C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSubDomainApp
// See SubDomain.cpp for the implementation of this class
//

class CSubDomainApp : public CWinApp
{
public:
	CSubDomainApp();
	CIniFile	m_IniFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDomainApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSubDomainApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDOMAIN_H__3260529B_3818_4FFC_B38E_59DAA99B355C__INCLUDED_)
