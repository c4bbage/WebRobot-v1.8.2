// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__406A9D73_8473_479A_9B49_AB2633E57A54__INCLUDED_)
#define AFX_STDAFX_H__406A9D73_8473_479A_9B49_AB2633E57A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
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

#include "../common/EasySize.h"
#include "../common/ThreadTemplate.h"
#include "../common/Tools/ButtonXP.h"
#include "../common/Tools/ComboBoxXP.h"
#include "../common/CoolMenu/NewMenu.h"
#include "../common/IniFile/IniFile.h"
#include "../common/ListCtrlEx/ListCtrlEx.h"
#include "../common/base64.h"

#include <WINSOCK2.H>
#pragma comment (lib,"Ws2_32.lib")
#include <afxinet.h>
#include <Shlwapi.h>
#pragma comment (lib,"Shlwapi.lib")


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__406A9D73_8473_479A_9B49_AB2633E57A54__INCLUDED_)
