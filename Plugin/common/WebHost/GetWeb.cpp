//////////////////////////////////////////////////////////////////////
//
// GetWeb.h: interface for the GetWeb class.
// 
// Author : Huawen nie
//
// Email : nie173@sohu.com
//
// Data Time ：2004 .5 .31
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetWeb.h"

//////////////////////////////////////////////////////////////////////////
//Globals

const TCHAR szHeaders[] =
_T("Accept: text/*\r\nUser-Agent: Mozilla/5.0 (Windows NT 5.1; rv:6.0) Gecko/20100101 Firefox/6.0\r\n");
//////////////////////////////////////////////////////////////////////////

CGetWeb::CGetWeb()
{
	dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
};
//Get web
// return TRUE was success
int CGetWeb::OnGetTheWeb(CString& sURL, CString &fileContent)
{
	
	dwRetcode=-1;
	dwServiceType =0;
	//CInternetSession session;
	CInternetSession session(_T("session"), 0, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD);//设置不缓冲
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		if(!OnInitSession(session))
		{
			return FALSE;
		}
		if (sURL.CompareNoCase("about:blank") == 0)
		{
            str_Error ="空白页";
			return FALSE;
		}
		if (!AfxParseURL((LPCTSTR)sURL, dwServiceType, strServerName, strObject, nPort) ||
			(dwServiceType != INTERNET_SERVICE_HTTP && dwServiceType != AFX_INET_SERVICE_HTTPS))
		{
			str_Error = "非法的URL";
			return FALSE;
		}
		pServer = session.GetHttpConnection(strServerName, nPort);
		if(pServer==NULL)
		{
			str_Error = "无法与服务器建立连接";
			return FALSE;
		}
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		
		if(pFile==NULL)
		{
			str_Error = "无法与服务器建立连接";
			return FALSE;
		}
		//////////////////////////////////////////////////////////////////////////
        try
		{
			if(!pFile->AddRequestHeaders(szHeaders) || !pFile->SendRequest())
			{
				str_Error ="网络错误－无法发送请求报头";
				return FALSE;
			}
		}
		catch (CInternetException* ex) 
		{
			fileContent.Empty();
			str_Error = "无法发送http报头,可能网络状况有问题";
			ex->Delete();
			return FALSE;		
		}
		//////////////////////////////////////////////////////////////////////////
		
		if(!pFile->QueryInfoStatusCode(dwRetcode))
		{
			str_Error ="网络错误－无法查询反馈代码";
			return FALSE;
		}
		
		if(dwRetcode>=200 && dwRetcode < 300)
		{
			return OnGetData(session, pServer, pFile, fileContent);
		}
        else
		{
			//重新定向
			if( dwRetcode >= 300 && dwRetcode <= 306 )
			{
				
				pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);
				
				int nPlace = strNewLocation.Find(_T("Location: "));
				if (nPlace == -1)
				{
					str_Error ="Error: Site redirects with no new location";
					if (pFile != NULL)
						delete pFile;
					if (pServer != NULL)
						delete pServer;
					session.Close();
					return FALSE;
				}
				
				strNewLocation = strNewLocation.Mid(nPlace + 10);
				nPlace = strNewLocation.Find('\n');
				if (nPlace > 0)
				{
					strNewLocation = strNewLocation.Left(nPlace);
				}
				
				if(strNewLocation.Find("http://", 0) < 0)
				{
					sURL = OnConversionURL(sURL, strNewLocation);
				}
				else
				{
					sURL = strNewLocation;
				}
				fileContent.Empty();
				pFile->Close();
				delete pFile;
				pServer->Close();
				delete pServer;
				return OnGetTheWeb(sURL, fileContent);
			}
			else//网络一般错误
			{
				fileContent.Empty();
				return OnProcessError(dwRetcode,session,pServer,pFile);
			}
		}
	}
	catch (CInternetException* pEx)
	{
	/*TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);*/
		str_Error ="网络错误";
		
		pEx->Delete();   
		return FALSE;
	}
}
//////////////////////////////////////////////////////////////////////////
//查询网页的最后修改时间
BOOL CGetWeb::OnQuerLastMendTime(const COleDateTime& m_site_LSTime,COleDateTime& news_Time ,CHttpFile* pFile)
{
	///*OnFunctionsMoind("OnQuerLastMendTime");*/
	//SYSTEMTIME *ptime=NULL;
	//ptime = new SYSTEMTIME ;
	//try
	//{
	//	if(ptime!=NULL&&pFile->QueryInfo( HTTP_QUERY_LAST_MODIFIED  , ptime) )
	//	{
	//		news_Time.SetDateTime(ptime->wYear,ptime->wMonth,ptime->wDay,ptime->wHour,ptime->wMonth,ptime->wSecond );
	//	
	//		if(m_site_LSTime >= news_Time||odNowtime.GetDay()-news_Time.GetDay()>5)//no updata
	//		{
	//			if(ptime!=NULL)
	//			{
	//				delete ptime;
	//			}
	//			return FALSE;
	//		}
	//		else
	//		{
	//			if(ptime!=NULL)
	//			{
	//				delete ptime;
	//			}
	//			return TRUE;
	//		}
	//	}
	//	else
	//	{
	//		if(ptime!=NULL)
	//		{
	//			delete ptime;
	//		}
	//		return TRUE;
	//	}
	//}
	//catch(CInternetException* pEx)
	//{
	//	str_Error ="查询文件时间导致网络错误";
	//	pEx->Delete();  
	//	return FALSE;
	//}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//初始化CInternetSession
BOOL CGetWeb::OnInitSession(CInternetSession& session)
{
	
	//超时设置很重要！如果设置太小回引起服务器超时，如果设置太大则回引起线程挂起。
	//在重试连接之间的等待的延时值在毫秒级。
	//网络连接请求时间超时值在数毫秒级。如果连接请求时间超过这个超时值，请求将被取消。缺省的超时值是无限的。
	//在网络连接请求时的重试次数。如果一个连接企图在指定的重试次数后仍失败，则请求被取消。缺省值为5。
	try
	{
		if(!session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 10000)||
		!session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000)||
		!session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3)||
		!session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT,60000)||	
		!session.EnableStatusCallback(TRUE))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	catch(CInternetException* pEx)
	{
		pEx->GetErrorMessage(str_Error.GetBuffer(0),1023);
		pEx->Delete();
		return FALSE;
	}
}
//process error
int CGetWeb::OnProcessError(int dwRetcode,CInternetSession& session,CHttpConnection* pServer,CHttpFile* pFile)
{

    switch( dwRetcode )
		{
		case 100:
			str_Error ="客户方错误-继续 [Continue]";
			break;
			//
		case 101:
			str_Error ="客户方错误-交换协议 [witching Protocols]";
			break;
			//
		case 204:
			str_Error ="网页内容为空 [No Content]";
			break;
			//------------------------------------------------------------
		case 400:
			str_Error ="错误请求 [Bad Request]";
			break;
			//
		case 401:
			str_Error ="网页需要验证信息 [Unauthorized]";
			break;
			//
		case 402:
			str_Error ="网页需要付费 [Payment Required]";
			break;
			//
		case 403:
			str_Error ="禁止访问 [Forbidden]";
			break;
			//
		case 404://
			str_Error = "没有找到网页 [Not Found]";
			break;
			//
		case 405:
			str_Error ="不允许Http访问该文件 [Method Not Allowed]";
			break;
			//
		case 406:
			str_Error ="该文件不允许访问 [Not Acceptable]";
			break;
			//
		case 407:
			str_Error ="该文件需要代理认证 [Proxy Authentication Required]";
			break;
			//
		case 408:
			str_Error ="对该文件请求超时 [Request Time-out]";
			break;
			//
		case 409:
			str_Error ="对该文件访问冲突 [Conflict]";
			break;
			//
		case 410:
			str_Error ="对该文件访问失败 [Gone]";
			break;
			//
		case 411:
			str_Error ="该文件需要长度信息 [Length Required]";
			break;
			//
		case 412:
			str_Error ="请求条件失败 [Precondition Failed]";
			break;
			//
		case 413:
			str_Error ="请求文件实体太大 [Request Entity Too Large]";
			break;
			//
		case 414:
			str_Error ="请求的URI太长 [Request-URI Too Large]";
			break;
			//
		case 415:
			str_Error ="不支持媒体类型 [Unsupported Media Type]";
			break;
			//
		case 416:
			str_Error ="队列请求失败 [Requested range not satisfiable]";
			break;
			//
		case 417:
			str_Error ="预期失败 [Expectation Failed]";
			break;
			//--------------------------------------------------------------
		case 500:
			str_Error ="服务器内部错误 [Internal Server Error]";
			break;
			//
		case 501:
			str_Error ="未实现请求 [Not Implemented]";
			break;
			//
		case 502:
			str_Error ="网关失败 [Bad Gateway]";
			break;
			//
		case 503:
			str_Error ="没有找到服务器 [Service Unavailable]";
			break;
			//
		case 504:
			str_Error ="网关超时 [Gateway Time-out]";
			break;
			//
		case 505:
			str_Error ="服务器不支持系统使用的HTTP版本 [HTTP Version not supported]";
			break;
			//
		}
	try
	{
	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	session.Close();
	return FALSE;
	}
    catch (CInternetException* pEx)
	{
	pEx->GetErrorMessage(str_Error.GetBuffer(0),1024);
	pEx->Delete();
    return FALSE;
	}
}
//RXD function
int CGetWeb::OnGetData(CInternetSession& session,CHttpConnection* pServer,CHttpFile* pFile,CString &fileContent)
{
	try
	{
		while(pFile->ReadString(str_FlagTemp) > 0)
		{
			fileContent += str_FlagTemp;
			fileContent += "\r\n";
			if (fileContent.GetLength() > 5242880)//5Mb
				break;
		}
		
		if (pFile != NULL)
			delete pFile;
		if (pServer != NULL)
			delete pServer;
		session.Close();
		return TRUE;
	}
	catch (CInternetException* pEx)
	{
		str_Error ="接收数据错误";
		pEx->Delete();
		return 0;
	}
}
//====================================================
//
// ../表示向上一层
// /表示根目录下的
// XX.htm表示当前目录下的
//把URL转换成绝对地址
CString CGetWeb::OnConversionURL(CString sURL,CString str_fafURL)
{
	
	if(sURL.Find("/",8)<0)
	{
		sURL +="/";
	}
	CString str_activeURL;
	int int_j = 0;
	int i=0;
	str_activeURL = str_fafURL;
	if(str_fafURL.Find("../",0)!=-1&&str_fafURL[0]!='/')
	{
		while( i<=str_fafURL.GetLength() )
		{
			if( str_fafURL[i] == '.' && str_fafURL[i+1] == '.' && str_fafURL[i+2] == '/' )
			{ int_j++;}
			i++;
		}
		if(str_fafURL[0]=='/')
		{
			str_fafURL.Delete(0,1);
		}
		str_fafURL.Replace("../","");
		i=0;
		int int_i=0;
		while( i <= sURL.GetLength() )
		{
			if( sURL[i]=='/' )
			{ 
				int_i++;
			}
			i++;
		}
		int_i -= int_j;

		if( int_i<3 )
		{
			int_i = 3;
		}

		int int_cour=0;
		for( i=0; i<=sURL.GetLength(); i++)
		{
			if( sURL[i]=='/' )
			{ 
				int_cour++;
			}
			if( int_cour==int_i )
			{			 
				sURL= sURL.Left(i+1);
				break;
			}
		}
		//容错处理
		if( sURL[sURL.GetLength()-1]!='/' )
		{	
			sURL +="/";
		}
		sURL += str_fafURL;
		return sURL;
	}
	else
	{
		if( str_fafURL[0] =='/' )
		{
			int int_b = 0 ;
			for( int a=0; int_b<3 && a<sURL.GetLength(); a++)
			{
				if( sURL[a]=='/' )
			 {
				 int_b++;
			 }
			 if( int_b==3 )
			 {
				 sURL = sURL.Left(a);
				 break;
			 }
			}
			sURL += str_fafURL;
		}
		else
		{
			for( int i=sURL.GetLength() - 1; i> 0 ; i-- )
			{
				if( sURL[i] =='/' )
				{
					sURL = sURL.Left( i+1 );
					break;
				}
			}
			sURL += str_fafURL;
		}
		return sURL;
	}
}
//====================================================
//
//return errer content
CString CGetWeb::OnQueryErrer()
{
	return str_Error;
}
