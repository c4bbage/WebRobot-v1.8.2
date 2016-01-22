// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1FD97875_E90A_4CD2_817E_C83F3B02B6FC__INCLUDED_)
#define AFX_STDAFX_H__1FD97875_E90A_4CD2_817E_C83F3B02B6FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x0500	// 用AutoLock.h必须要有这个
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


#include <afxmt.h>// 用AutoLock.h必须要有这个
#include <afxinet.h>
#include <vector>
using namespace std;
#include <Shlwapi.h>
#pragma comment (lib,"Shlwapi.lib")
#include <process.h>

#include "../common/SplitterControl/SplitterControl.h"
#include "../common/GridListCtrl/GridListCtrl.h"
#include "../common/GridListCtrl/InPlaceEdit.h"
#include "../common/ThreadTemplate.h"
#include "../common/IniFile/IniFile.h"
#include "../common/WebHost/GetWeb.h"
#include "../common/WebHost/WebHost.h"
#include "../common/Tools/ButtonXP.h"
#include "../common/Tools/ComboBoxXP.h"
#include "../common/CButtonST/BtnST.h"
#include "../common/ListCtrlEx/ListCtrlEx.h"
#include "../common/CoolMenu/NewMenu.h"
#include "../common/StdioFileEx/StdioFileEx.h"

enum
{
	SCAN_UNWORK,	//未爬行
	SCAN_WORKING,	//正在爬行
	SCAN_FINISHED,	//爬行完毕
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1FD97875_E90A_4CD2_817E_C83F3B02B6FC__INCLUDED_)
