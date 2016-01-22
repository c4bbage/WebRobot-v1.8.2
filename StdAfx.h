// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A03564E2_5B2A_4E34_A38C_873588350A87__INCLUDED_)
#define AFX_STDAFX_H__A03564E2_5B2A_4E34_A38C_873588350A87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#pragma warning(disable:4018)

#include <Mshtmhst.h>
#include <mshtml.h>
#include <atlbase.h>  //需要安装ATL库
#include <afxinet.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "ExClass/CoolTabCtrl/CoolTabCtrl.h"
#include "ExClass/SplitterControl/SplitterControl.h"
#include "ExClass/Tools/ButtonXP.h"
#include "ExClass/Tools/ComboBoxXP.h"
#include "ExClass/ListMoveItem/ListMoveItem.h"
#include "ExClass/CRC32/CRCFile.h"
#include "ExClass/XHTMLStatic/XHTMLStatic.h"
#include "ExClass/CoolMenu/NewMenu.h"
#include "ExClass/IniFile/IniFile.h"
#include "ExClass/TrayIcon/TrayIcon.h"
#include "ExClass/WebHost/GetWeb.h"
#include "ExClass/WebHost/WebHost.h"
#include "ExClass/CButtonST/BtnST.h"

typedef BOOL(_cdecl *PreTM)(MSG *pMsg);//导出函数
typedef void(_cdecl *PLoadTempTxt)(CString strTempTxtPath);

#define	WM_LIST_DROP_END	(WM_USER+100)

#define TEXT_FILTER "cache.baiducontent.com|.google.com.hk|www.youtube.com|265.com|microsofttranslator.com|bingj.com|bing.com|soso.com|msn.com|microsoft.com|live.com|google.cn|content4ads.com|googleusercontent.com|bdimg.com|miibeian.gov.cn|.google.com|.bdstatic.com|www.blogger.com|office.com|outlook.com|www.onenote.com|bing.net|fanyi.baidu.com|baike.baidu.com|music.baidu.com|play.baidu.com|www.hao123.com|"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A03564E2_5B2A_4E34_A38C_873588350A87__INCLUDED_)
