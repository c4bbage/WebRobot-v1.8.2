#include "stdafx.h"
#include "CRC32.h"

CRC32::CRC32()
{
	CRCVAL = 0xffffffff;
	MakeTable();//初始化，生成CRC32的码表
	GetCrc32Str(0, 0);//初始化返回字符为00000000
}

void CRC32::MakeTable()
{
	int i,j;
	unsigned long crc;
	for (i = 0; i < 256; i++)
	{
		crc = i;
		for (j = 0; j < 8; j++)
		{
			if (crc & 1)
				crc = (crc >> 1) ^ 0xEDB88320;
			else
				crc >>= 1;
		}
		Crc32Table[i] = crc;
	}
}

int CRC32::GetCrc32(unsigned char *csData, unsigned long dwSize)
{
	unsigned long len;
	unsigned char* buffer;
	len = dwSize;
	buffer = (unsigned char*)csData;
	while(len--)
		CRCVAL = (CRCVAL >> 8) ^ Crc32Table[(CRCVAL & 0xFF) ^ *buffer++];
	return CRCVAL^0xffffffff;
}

char *CRC32::GetCrc32Str(unsigned char *csData, unsigned long dwSize)
{
	unsigned long CrcValue = GetCrc32(csData, dwSize);
	char *crc32_pstr = decToHex(CrcValue, CRCString, 8);//转换为字符形式

	return CRCString;
}
