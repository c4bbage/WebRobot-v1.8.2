// HttpClient.h: interface for the CHttpClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPCLIENT_H__D22146A9_5234_40FE_8AAA_2A9AF7E2130D__INCLUDED_)
#define AFX_HTTPCLIENT_H__D22146A9_5234_40FE_8AAA_2A9AF7E2130D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>

class CHttpClient  
{
public:
	CHttpClient();
	virtual ~CHttpClient();

	BOOL GetData(CString strUrl, int nMode, CString strPostData, CString strCookie, CString strUserAgent, DWORD dwMillisecondsTimeout, DWORD &dwStatusCode, DWORD &dwWebPageSize, BOOL bGetHtml, BOOL bDecodeUtf8, CString &strHtml);//nMode -> 0:HTTP_VERB_POST	1:HTTP_VERB_GET	2:HTTP_VERB_HEAD

protected:
	DWORD m_dwStatusCode;
	DWORD m_dwWebPageSize;
	CString m_strHtml;
	void Utf8ToAnsi(CString &strUTF8);

private:
	static DWORD WINAPI WorkerFunction(LPVOID vThreadParm);
};

#endif // !defined(AFX_HTTPCLIENT_H__D22146A9_5234_40FE_8AAA_2A9AF7E2130D__INCLUDED_)
