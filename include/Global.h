
#ifndef __GLOBAL__H__20140611
#define __GLOBAL__H__20140611

#define CMutex pthread_mutex_t
#define InitializeCriticalSection(pMutex)		pthread_mutex_init(pMutex, NULL)
#define EntryCriticalSection(pMutex)			pthread_mutex_lock(pMutex)
#define LeaveCriticalSection(pMutex)			pthread_mutex_unlock(pMutex)
#define DestroyCriticalSection(pMutex)			pthread_mutex_destroy(pMutex)

// max and min
#define MAX(m, n) ((m) > (n)? (m): (n))
#define MIN(m, n) ((m) < (n)? (m): (n))

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#define TRUE	1
#define FALSE	0
#define CONST const


#if defined(WIN_PLAFORM_32)
	#ifndef _WIN32
		typedef int INT;
		typedef short SHORT;
		typedef char CHAR;
		typedef unsigned char UCHAR;
		typedef __int64 INT64, _INT64;
		typedef long LONG;

		typedef unsigned int DWORD;
		typedef unsigned char BYTE;
		typedef unsigned short USHORT;
		typedef unsigned short WORD;
		typedef unsigned __int64 QWORD;
		typedef void VOID;
		typedef INT	BOOL;
		typedef const CHAR * LPCSTR;
		typedef double DOUBLE;
		typedef float FLOAT;
		typedef void *HANDLE;
	#else
		#include <windows.h>
	#endif

#elif defined(WIN_PLAFORM_64)
	typedef int INT;
	typedef short SHORT;
	typedef char CHAR;
	typedef unsigned char UCHAR;
	typedef long INT64, _INT64;
	typedef long LONG;

	typedef unsigned int DWORD;
	typedef unsigned char BYTE;
	typedef unsigned short USHORT;
	typedef unsigned short WORD;
	typedef unsigned long QWORD;
	typedef void VOID;
	typedef INT BOOL;
	typedef const CHAR * LPCSTR;
	typedef double DOUBLE;
	typedef void *HANDLE;

#else // linuxƽ̨(��ʱ)
	typedef int INT;
	typedef short SHORT;
	typedef char CHAR;
	typedef unsigned char UCHAR;
	typedef long long INT64, _INT64;
	typedef long LONG;

	typedef unsigned int DWORD;
	typedef unsigned char BYTE;
	typedef unsigned short USHORT;
	typedef unsigned short WORD;
	typedef unsigned long long QWORD;

	typedef void VOID;
	typedef INT BOOL;
	typedef const CHAR * LPCSTR;
	typedef CHAR *LPSTR;
	typedef double DOUBLE;
	typedef float FLOAT;

	typedef void *HANDLE;

#endif
//#else
//	#include <windows.h>
//#endif

#define CUR_TIME_FORMAT(szDateTime) do{ \
	struct timeval TimeNow; \
	struct tm *tmBlock; \
	INT nMillSeconds; \
	gettimeofday(&TimeNow, NULL); \
	tmBlock = localtime(&TimeNow.tv_sec); \
	nMillSeconds = TimeNow.tv_usec / 1000; \
	\
	sprintf(szDateTime, "%4d-%02d-%02d %02d:%02d:%02d.%03d", \
				tmBlock->tm_year + 1900, \
				tmBlock->tm_mon + 1, \
				tmBlock->tm_mday, \
				tmBlock->tm_hour, \
				tmBlock->tm_min, \
				tmBlock->tm_sec,\
				nMillSeconds); \
				\
	}while (0)
		
#define CUR_DATE_FORMAT(szDateTime) do{ \
	struct timeval TimeNow; \
	struct tm *tmBlock; \
	gettimeofday(&TimeNow, NULL); \
	tmBlock = localtime(&TimeNow.tv_sec); \
	\
	sprintf(szDateTime, "%4d-%02d-%02d", \
				tmBlock->tm_year + 1900, \
				tmBlock->tm_mon + 1, \
				tmBlock->tm_mday); \
\
} while (0)


// ASSERT宏类似windows中的assert�?
#ifdef __DEBUG
#define ASSERT(n)	do{ \
	if (!(n)) \
	{ \
		FILE *lpFile; \
		CHAR szMessageLog[1024]; \
		\
		lpFile = fopen("assert.txt", "ab"); \
		CUR_DATE_TIME_FORMAT(szMessageLog); \
		fprintf(lpFile, "%s\r\n", szMessageLog); \
		fprintf(lpFile, "%s,%d:%s()\r\n", __FILE__, __LINE__, __FUNCTION__); \
		fprintf(lpFile, "%s\r\n", #n); \
		fflush(lpFile); \
		fclose(lpFile); \
		abort();\
	} \
} while (0)
#else
#define ASSERT(n) (VOID)0
#endif

#define DISALLOW_DESTRUCT(ClassName) \
	protected: \
	virtual ~ClassName(){}\

#define SINGLE_MODE_CLASS(ClassName) \
	private: \
	ClassName(); \
	virtual ~ClassName(); \
	\
	private: \
	static ClassName* m_lpInstance; \
	\
	public:\
	static ClassName *CreateInstance() \
	{ \
		ASSERT(m_lpInstance == NULL); \
		m_lpInstance = new ClassName; \
		return m_lpInstance; \
	} \
	static ClassName *GetInstance() \
	{ \
		ASSERT(m_lpInstance != NULL); \
		return m_lpInstance; \
	}

#define SINGLE_INITSTANCE(ClassName) \
		ClassName *ClassName::m_lpInstance = NULL;

// 管道通信消息
typedef struct tagSES_DATA
{
	WORD wSesId;
	CHAR szSesBody[0];

}SES_DATA;

// 使用系统消息，NetApp消息队列
enum
{
	SYS_MSG_NULL 	= 0x00,
	SYS_MSG_SIGNAL 	= 0x01,		// "信号"消息
	SYS_MSG_IDLE,				// 空闲消息，当NetApp消息队列空闲时，发�?�此消息
	SYS_MSG_QUIT,				// 系统�?出消息，发�?�此消息，NetApp�?出消息线�?
};
#define MSG_USER_ID  0x90		// 用户自己定义消息的基�?，基于MSG_USER

#define OFFSET_STRUCT(T, Parameter) (DWORD)(&(((T *)0)->Parameter))

// ��ͣ��������غ궨��
/*
#define va_list	CHAR *
#define va_start(v, s) do \
{ \
	v = (va_list)&s + sizeof(s); \
	\
} while (0)

#define va_arg(v, t) (*((t*)(v += sizeof(va_list))))
#define va_end(v)
*/

// UDP Destination
typedef struct tagREMOTE_DATA
{
	CHAR szRemoteIp[100];
	WORD wPort;

}REMOTE_DATA;


#endif



