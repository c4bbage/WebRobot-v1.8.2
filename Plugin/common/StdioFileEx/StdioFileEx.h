// StdioFileEx.h: interface for the CStdioFileEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDIOFILEEX_H__C1F1F96B_9417_4388_8D24_892EDFA2A616__INCLUDED_)
#define AFX_STDIOFILEEX_H__C1F1F96B_9417_4388_8D24_892EDFA2A616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// --------------------------------------------------------------------------------------------
//程序用途：按行读取常见(包括ANSI、UNICODE、UNICODE  big endian、UTF-8)格式的文本文件    
//程序作者：湖北师范学院计算机科学与技术学院  王定桥                                 
//核心算法：CStdioFileEx继承自CStdioFile， 覆盖CStdioFile的 BOOL ReadString(CString& rString)方法, 
//          根据不同文件编码特征，寻找文件回车换行符判断读取行结束，文件结束符判断文件结束                               	  
//          检测不同文件编码头部，获取文件类型后调用不同的读取函数
//测试结果:在Windows7 VC6.0环境下测试上述四种格式的txt文件通过
//尚未完成:未重载CStdioFile的 virtual LPTSTR ReadString( LPTSTR lpsz, UINT nMax )方法
//		   未完成WriteString方法,未在VC UNICODE 环境下的测试         	  					          				  
//制作时间：2012-04-19											  
//代码版权:代码公开供学习交流使用  欢迎指正错误  改善算法								           		
// --------------------------------------------------------------------------------------------
#include "stdafx.h"
//文本文件类型枚举值
typedef enum TextCodeType
{
	UTF8=0,
	UNICODE =1,
	UNICODEBIGENDIAN=2,
	ANSI=3,
	FILEERROR=4
}TextCode;
class CStdioFileEx  :public CStdioFile
{
public:
	CStdioFileEx();
	CStdioFileEx(FILE* pOpenStream);
	CStdioFileEx(LPCTSTR lpszFileName, UINT nOpenFlags);
	virtual ~CStdioFileEx();
	virtual BOOL Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);
public:
	//文件类型值转换到字符串
	CString  FileTypeToString();
	//获取文件类型
	TextCode GetFileType();
	//按行读取文件
	BOOL     ReadString(CString& rString);
	//静态方法  获取文件类型
	static   TextCode GetFileType( LPCTSTR lpszFileName);
protected:
	TextCode m_FileType;//保存文件类型
	const static int  PREDEFINEDSIZE;//预定义一行文件所需空间
protected:
	//从UTF-8文件按行读取
	BOOL     ReadStringFromUTF8File(CString& rString);
	//从ANSI文件按行读取
    BOOL     ReadStringFromAnsiFile(CString& rString);
	//重UNCIDOE、UNICODE big endian文件读取
	BOOL     ReadStringFromUnicodeFile(CString& rString);
	//UTF-8字符串转换到UNICODE字符串
	CString  UTF8ToUnicode(byte  *szUTF8);
	//处理文件打开标志
	UINT     ProcessFlags(LPCTSTR lpszFileName, UINT& nOpenFlags,TextCode &tc);
};

#endif // !defined(AFX_STDIOFILEEX_H__C1F1F96B_9417_4388_8D24_892EDFA2A616__INCLUDED_)