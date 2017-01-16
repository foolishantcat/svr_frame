
#ifndef __NET_DATA__H__20160524
#define __NET_DATA__H__20160524

#include "Global.h"

typedef struct
{
	//DWORD dwLastElaspedTime; // 上一次访问时间
	DWORD dwHeartBitIntvalTime;
	CHAR *lpszHeartBit;
	INT nSize;
	INT nReferCounts; // 引用计数
	INT nMaxPacketSize;

}CONN_POOL_ATTR;

typedef struct
{
	CHAR szRemoteIP[64];
	WORD wPort;
}ADDR_INF;

#endif
