/*****************************************************************************
Module :     URLEncode.H
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: H URL Encoder
*****************************************************************************/
#ifndef __CURLENCODE_H_
#define __CURLENCODE_H_

#include <string>
using namespace std;

class CURLEncode
{
private:
	static CString csUnsafeString;
	CString decToHex(char num, int radix);
	bool isUnsafe(char compareChar);
	CString convert(char val);

public:
	CURLEncode() { };
	virtual ~CURLEncode() { };
	CString URLEncode(CString vData);
	CString URLEncodeAll(CString vData);
	std::string UrlDecode(const std::string& src);
};

#endif //__CURLENCODE_H_