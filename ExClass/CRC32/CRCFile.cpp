#include "stdafx.h"
#include "CRCFile.h"

FileCRC32::FileCRC32()
{
	DataLen = totalsize = 0;
}
bool FileCRC32::Open(LPCSTR lpFileName)
{
	hFile = CreateFile(lpFileName, 
        GENERIC_READ, 
        FILE_SHARE_READ, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return false;
	}
	LARGE_INTEGER LI;
	BOOL bError = GetFileSizeEx(hFile, &LI);
	if(!bError)
		return false;
	DataLen = LI.QuadPart;
	return true;
}

bool FileCRC32::Open(LPCSTR lpFileName, __int64 start, __int64 end)
{
	hFile = CreateFile(lpFileName, 
        GENERIC_READ, 
        FILE_SHARE_READ, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return false;
	}
	DataLen = end - start;
	LARGE_INTEGER LI;
	LI.QuadPart = start;
	SetFilePointerEx(hFile, LI, NULL, FILE_BEGIN);
	return true;
}

void FileCRC32::Close()
{
	CloseHandle(hFile);
}

char * FileCRC32::GetFileCRCString()
{
	unsigned long len = 0;

	while (ReadFile(hFile, buffer, sizeof(buffer), &len, NULL))
	{
		if(!len)
			break;
		totalsize += len;
		if(totalsize > DataLen)//__int64 start, __int64 end
		{
			GetCrc32Str(buffer, (unsigned long)(len - (totalsize - DataLen)));
			break;
		}
		GetCrc32Str(buffer, len);
	}
	return CRCString;
}


bool FileCRC32::DoFileCRC()
//异步版，仍有数据未CRC 则返回true, 完成任务时返回false
/*
USAGE:
while(DoFileCRC())
{
	//your code to determine whether to end the loop.
}

*/
{
	unsigned long len = 0;
	if(! ReadFile(hFile, buffer, sizeof(buffer), &len, NULL))
		return false;
	if(!len)
		return false;

	totalsize += len;

	if(totalsize > DataLen)//__int64 start, __int64 end
	{
		GetCrc32Str(buffer, (unsigned long)(len - (totalsize - DataLen)));
		return false;
	}else
	{
		GetCrc32Str(buffer, len);
		return true;
	}
}

char *FileCRC32::GetCRCString()
{
	return CRCString;
}