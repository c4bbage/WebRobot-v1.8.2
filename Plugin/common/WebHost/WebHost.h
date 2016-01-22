/*########################################################################
	【文件名】: 	WebHost.h
	【名  称】:		解析web连接的封装类.
	----------------------------------------------------
	Remarks:	...
	----------------------------------------------------
	Author:		huawenNie
	Email:		nie173@vip.sina.com
				nie173@msn.com
	Created:	03/05/2004
  ########################################################################*/
#if !defined(AFX_WEBHOST_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)
#define AFX_WEBHOST_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_
#include <vector>
using namespace std;
#include "URLSturct_.h"

class CWebHost
{
public:
   //构造函数 参数说明
   //[页面代码],[查重URL容器]
   //[返回的URL容器],本页的URL]
   //[该页的标题]
   CWebHost( const CString& m_str_webcode ,  vector<HyperLink>& m_vec_All_URL  ,HyperLink& str_URL);
   //是否有框架或跳转
   BOOL mb_ifFream;
protected:

	//返回所有合法链接 参数说明
	//[页面代码],[查重URL容器],[返回的URL容器],[抓取URL比较模板容器],[分析URL比较模板容器]
    void OnRetrunWebContent(const  CString& m_str_webcode ,  vector<HyperLink>& m_vec_All_URL );
    //获取前头有指定标志的链接
	void OnGetWebPageURL(const CString& str_htmlcode, vector<HyperLink>& m_vec_All_URL, int pagesize, CString strFlagString, int nFlagStringLenth);
	//获得嵌套代码的URL
	void OnReturnFrameURL(const CString& str_htmlcode ,vector<HyperLink>& m_vec_All_URL) ;

	//把URL转换成绝对地址
	CString OnConversionURL(CString SeedURL,CString RelativeURL) ;
    
	//返回Javascript代码里面合要求的URL 参数说明
    //[页面代码],[返回的URL容器]
	void OnAnalyseJavascrript(const CString& m_str_webcode ,  vector<HyperLink>& m_vec_All_URL  );
	
    //取URL连接文字
	CString OnGetLinkText(int iIn,const CString& htmlcode);
	
protected:
	 //该页的URL
     CString str_Page_URL;
	 //该页的标题
	 CString str_pagetitle;
     HyperLink m_HyperLink;
	 //变量中转
	 CString str_link;
};
#endif // !defined(AFX_WEBHOST_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)