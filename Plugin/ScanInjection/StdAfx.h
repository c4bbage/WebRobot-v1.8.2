// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__765621D4_66DD_4652_99D9_33CF5C33B060__INCLUDED_)
#define AFX_STDAFX_H__765621D4_66DD_4652_99D9_33CF5C33B060__INCLUDED_

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
#pragma warning(disable:4244)

#include <afxmt.h>
#include <afxinet.h>
#include<afxtempl.h>
#include <Shlwapi.h>
#pragma comment (lib,"Shlwapi.lib")

#include "../common/ThreadTemplate.h"
#include "../common/HHideHeaderCtrl/HHideListCtrl.h"
#include "../common/Tools/ButtonXP.h"
#include "../common/SplitterControl/SplitterControl.h"
#include "../common/CoolMenu/NewMenu.h"
#include "../common/IniFile/IniFile.h"
#include "../common/StdioFileEx/StdioFileEx.h"
#include "../common/ListCtrlEx/ListCtrlEx.h"
#include "../common/HttpClient/HttpClient.h"

typedef struct tagReturnData
{
	CString strUrl;
	BOOL bResult;
	CString strErrWord;
	int nMethod;
	int nRate;
	CString strURL1;
	CString strURL2;
	long lWebPageSize1;
	long lWebPageSize2;
}ReturnData,*LPReturnData;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__765621D4_66DD_4652_99D9_33CF5C33B060__INCLUDED_)
