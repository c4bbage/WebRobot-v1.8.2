#if !defined(AFX_FileCRC32_H__29F90AAD_D373_46E4_850B_EE258BAFAE30__INCLUDED_)
#define AFX_FileCRC32_H__29F90AAD_D373_46E4_850B_EE258BAFAE30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRC32.h"
#include "windows.h"

class FileCRC32: public CRC32
{
private:
	HANDLE hFile;
	__int64 DataLen;
	__int64 totalsize;
	unsigned char buffer[8192];

public:
	FileCRC32();
	bool Open(LPCSTR lpFileName);
	bool Open(LPCSTR lpFileName, __int64 start, __int64 end);
	void Close();
	char * GetFileCRCString();
	char * GetCRCString();
	bool DoFileCRC();
};


#endif // !defined(AFX_FileCRC32_H__29F90AAD_D373_46E4_850B_EE258BAFAE30__INCLUDED_)
