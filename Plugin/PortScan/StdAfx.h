// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8D3E3100_2F91_4992_B783_043B7282CDC6__INCLUDED_)
#define AFX_STDAFX_H__8D3E3100_2F91_4992_B783_043B7282CDC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x0500
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#undef _WIN32_WINNT
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../common/Tools/ButtonXP.h"
#include "../common/Tools/ComboBoxXP.h"
#include "../common/EasySize.h"
#include "../common/ThreadTemplate.h"
#include "../common/IniFile/IniFile.h"
#include "../common/CoolMenu/NewMenu.h"

#include <WINSOCK2.H>
#pragma comment (lib,"Ws2_32.lib")
#include <afxmt.h>
#include "process.h"
#include <vector>
using namespace std;

#define WM_ON_INSERT_RESULT (WM_USER+100)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8D3E3100_2F91_4992_B783_043B7282CDC6__INCLUDED_)
