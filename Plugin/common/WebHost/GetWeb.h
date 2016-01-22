//////////////////////////////////////////////////////////////////////
//
// GetWeb.h: catch web by GetWeb class.
// 
// Author : Huawen nie
//
// Email : nie173@sohu.com
//
// Data Time ：2004 .5 .31
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_GETWEB_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)
#define AFX_GETWEB_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_

#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>
#pragma warning(push)
#pragma warning(disable: 4100 4667)
#include <iostream>
#pragma warning(pop)
using namespace std;
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////
class CGetWeb
{
public:
   //Get web
	CGetWeb();
	
	~CGetWeb()
	{
	};

	// return TRUE was success
   int OnGetTheWeb(CString& sURL, CString &fileContent);
   //return errer content
   CString OnQueryErrer();
 
private:
   CString OnConversionURL(CString sURL,CString str_fafURL);
private:
	//初始化CInternetSession
	BOOL OnInitSession(CInternetSession& session);
	//查询网页的最后修改时间
	BOOL OnQuerLastMendTime(const COleDateTime& m_site_LSTime,COleDateTime& news_Time ,CHttpFile* pFile);
	//RXD function
	int OnGetData(CInternetSession& session,CHttpConnection* pServer,CHttpFile* pFile,CString &fileContent);
	//process error
	int OnProcessError(int dwRetcode,CInternetSession& session,CHttpConnection* pServer,CHttpFile* pFile);
	
	//error info
    CString str_Error;
	//reauest parmater
	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	//request handers
	DWORD dwHttpRequestFlags;
	// status ocde
	DWORD dwRetcode;
	//new location
	CString strNewLocation;
	//flag temp
	CString str_FlagTemp;
};
#endif // !defined(AFX_GETWEB_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)