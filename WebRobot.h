// WebRobot.h : main header file for the WEBROBOT application
//

#if !defined(AFX_WEBROBOT_H__DC7B8C50_BBD1_4A97_8291_F5DCF6D6DA0E__INCLUDED_)
#define AFX_WEBROBOT_H__DC7B8C50_BBD1_4A97_8291_F5DCF6D6DA0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWebRobotApp:
// See WebRobot.cpp for the implementation of this class
//

class CWebRobotApp : public CWinApp
{
public:
	CWebRobotApp();
	CIniFile	m_IniFile;
	CDialog *m_pMainDlg;
	CDialog *m_pWebRobotDlg;

	BOOL AlreadyRunning();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebRobotApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWebRobotApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBROBOT_H__DC7B8C50_BBD1_4A97_8291_F5DCF6D6DA0E__INCLUDED_)
