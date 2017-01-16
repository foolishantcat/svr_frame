

#ifndef __UNLOCK__QUE__H__20160607
#define __UNLOCK__QUE__H__20160607

#include "Global.h"
#include <vector>
#include <pthread.h>


/*
 * 该无锁队列针对应用场景对应为1对N，即一个线程取数据，N个存数据线程
 *
 */
template <class T0>
class CUnlockQue
{
	typedef struct
	{
		T0 Element;
		bool bIsEmpty;
	}ELEMENT;

	typedef CUnlockQue::ELEMENT T_ELEMENT;
public:
	CUnlockQue();
	virtual ~CUnlockQue();

public:
	BOOL InitQue(INT nMaxSize);
	BOOL Push(T0 &tValue, INT nWaitTime);
	BOOL Pop(T0 &tValue);

private:
	INT AtomAdd(INT &nElement, INT nValue);
	INT AtomDec(INT &nElement, INT nValue);

private:
	INT m_nPush;
	INT m_nAbleWriteCount;
	INT m_nPop;
	INT m_nAbleReadCount;

	std::vector<T_ELEMENT> m_VecElement;
	pthread_cond_t m_CondWait;
	CMutex m_Mutex;

	INT m_nMaxCap;
};

template<class T0>
CUnlockQue<T0>::CUnlockQue()
{
	m_nPush = 0;
	m_nAbleWriteCount = 0;
	m_nPop = 0;
	m_nAbleReadCount = 0;
	m_nMaxCap = 0;
}

template<class T0>
CUnlockQue<T0>::~CUnlockQue()
{
	//pthread_mutex_destroy(pMutex)
}

template<class T0>
BOOL CUnlockQue<T0>::InitQue(INT nMaxSize)
{
	// 暂时一定要默认构造函数
	T_ELEMENT tElement = {T0(), FALSE};

	m_nMaxCap 			= nMaxSize;
	m_nAbleWriteCount	= nMaxSize;
	m_VecElement.resize(nMaxSize, tElement);
	InitializeCriticalSection(&m_Mutex);
	pthread_cond_init(&m_CondWait, NULL);
}

template<class T0>
BOOL CUnlockQue<T0>::Push(T0 &tValue, INT nWaitTime)
{
	struct timespec TimeWait;

	//已经有足够多的push操作向列队不同位置写入数据
	while (0 >= AtomDec(m_nAbleWriteCount, 1))
	{
		AtomAdd(m_nAbleWriteCount, 1);

		TimeWait.tv_sec 	= time(NULL) + nWaitTime;
		TimeWait.tv_nsec 	= 0;

		// 等待唤醒
		pthread_mutex_lock(&m_Mutex);
	    if (ETIMEDOUT == pthread_cond_timedwait(&m_CondWait, &m_Mutex, &TimeWait)) // 超时
	    {
	    	pthread_mutex_unlock(&m_Mutex);
	    	return FALSE;
	    }

		pthread_mutex_unlock(&m_Mutex);
	}

	INT nPushPos = AtomAdd(m_nPush, 1);
	nPushPos = nPushPos % m_nMaxCap;

	/*
		只有在NPop并发情况下，因Pop无序完成，第一个位置的Pop未完成，后面的Pop就先完成提示有空位
		因为该类只允许1对N，所以必然是单线程Push，所以条件内push控制变量不需要原子操作
		进行不停的尝试，找到"Pop“完成
	 */
	while (!m_VecElement[nPushPos].bIsEmpty)
	{
		/*
		m_nPush --;
		m_nWriteAbleCount ++;
		return false;
		*/
	}

	m_VecElement[nPushPos].Element 	= tValue;
	m_VecElement[nPushPos].bIsEmpty = FALSE;

	AtomAdd(m_nAbleReadCount, 1);

	return TRUE;
}

template<class T0>
BOOL CUnlockQue<T0>::Pop(T0 &tValue)
{
	if (0 >= m_nAbleReadCount)
	{
		return FALSE;//空列队
	}
	if ( 0 >= AtomDec(m_nAbleReadCount, 1)) //已经有足够多的pop操作读取列队不同位置的数据
	{
		AtomAdd(m_nAbleReadCount, 1);
		return FALSE;
	}

	pthread_cond_broadcast(&m_CondWait);

	INT nPopPos = AtomAdd(m_nPop, 1);
	nPopPos = m_nPop % m_nMaxCap;

	while (m_VecElement[nPopPos].IsEmpty)
	{
		/*
		m_nAbleReadCount ++;
		m_nPop --;
		*/
	}

	tValue = m_VecElement[nPopPos].Element;
	m_VecElement[nPopPos].IsEmpty = true;

	AtomAdd(m_nAbleWriteCount, 1);

	return TRUE;
}

template<class T0>
INT CUnlockQue<T0>::AtomAdd(INT &nElement, INT nValue)
{
	return __sync_fetch_and_add(&nElement, nValue);
}

template<class T0>
INT CUnlockQue<T0>::AtomDec(INT &nElement, INT nValue)
{
	nValue *= -1;
	return __sync_fetch_and_add(&nElement, nValue);
}



