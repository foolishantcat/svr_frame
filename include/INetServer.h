
#ifndef __INET__SERVER__H__20160117
#define __INET__SERVER__H__20160117

#include "Global.h"
#include "NetData.h"

struct INetListener
{
	virtual BOOL OnInitHandle(void **ppvTaskParams) = 0;
	virtual BOOL OnNetParsingData(QWORD qwUserId, ADDR_INF &AddInf, const CHAR *lpszDataBuf, INT *lpnRemindSize, VOID **ppIoParams) 		= 0;
	virtual void OnNetErr(QWORD qwUserId, ADDR_INF &AddInf, VOID **ppIoParams) 																= 0;
	virtual BOOL OnConnParsingData(QWORD qwUserId, ADDR_INF &AddInf, const CHAR *lpszDataBuf, INT *lpnRemindSize, VOID **ppvContext) 		= 0;
	virtual void OnConnSendOver(INT nErrCode, ADDR_INF &AddrInf, void *lpvSendParams)														= 0;
	virtual void OnHandleData(QWORD qwUserId, const CHAR *lpszPacketBuf, INT nSize, BOOL bNetClient, VOID *lpvParams) 						= 0;
	DISALLOW_DESTRUCT(INetListener)
};

struct INetServer
{
	virtual BOOL AddConnectPool(const CHAR *lpszServerName, const CHAR *lpszRemoteIp, WORD wPort, CONN_POOL_ATTR *lpConnAttr) = 0;
	virtual void RemoveConnectPool(const CHAR *lpszServerName, const CHAR *lpszRemoteIp, WORD wPort)		= 0;
	virtual BOOL Start(const CHAR *lpszConfgFilePath, INetListener *lpNetListener) 							= 0;
	virtual BOOL Start(INetListener *lpNetListener)															= 0;
	virtual BOOL AddListener(const CHAR *lpszIP, WORD wPort)												= 0;
	virtual void RemoveListener(const CHAR *lpszIp, WORD wPort)												= 0;
	virtual void Close(QWORD qwUserId)																		= 0;
	virtual void Dispatch(QWORD qwUserId, const CHAR *lpszPacket, INT nSize, INT nHashkey = 0) 				= 0;
	virtual BOOL Send(QWORD qwUserId, const CHAR *lpszBuf, INT nSize, BOOL bShutDown = FALSE)		 		= 0;
	virtual BOOL Send(const CHAR *lpszRemoteIP, WORD wPort, const CHAR *lpszBuf, INT nSize, VOID *lpvParams = NULL) = 0;	// connect-pool send data
	virtual void Release()																					= 0;
	DISALLOW_DESTRUCT(INetServer)
};

#ifdef __cplusplus
extern "C"
{
#endif
INetServer *CreateNetServerObject();

#ifdef __cplusplus
};
#endif

#endif
