// Coding.h : main header file for the CODING DLL
//

#if !defined(AFX_CODING_H__2A4EFB28_0C5C_43F4_8558_C6AE6ABC224F__INCLUDED_)
#define AFX_CODING_H__2A4EFB28_0C5C_43F4_8558_C6AE6ABC224F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCodingApp
// See Coding.cpp for the implementation of this class
//

class CCodingApp : public CWinApp
{
public:
	CCodingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodingApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCodingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODING_H__2A4EFB28_0C5C_43F4_8558_C6AE6ABC224F__INCLUDED_)
