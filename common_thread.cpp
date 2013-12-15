/*
 * common_thread.cpp
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#include "common_thread.h"
#include "common_api.h"
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#ifdef WIN32

#else
pthread_key_t CThread::ms_private_key_log = 0;
#endif

bool CThread::ms_bIsSetPrivateKeyLog = false;

#ifdef WIN32
void ThreadProc(void *pParam)
{
    if (NULL == pParam)
	{
        return;
	}

	//初始化随机种子
	srand((unsigned)time(NULL));

    CThread *p = (CThread*)pParam;

    p->Execute();
}
#else
void* ThreadProc(void* pParam)
{
    if (NULL == pParam)
	{
		return NULL;
	}

    CThread *p = (CThread*)pParam;
    p->Execute();

	return NULL;
}
#endif

CThread::CThread()
{
#ifdef WIN32
	m_nTimeOut = THREAD_TERMINATE_TIMEOUT;
	m_hThread = INVALID_HANDLE_VALUE;
#else
	m_thread = 0;
#endif


	m_bTerminated = false;

	m_szName[0] = '\0';

	m_szLogName[0] = '\0';

	m_nThreadIndex = -1;

	pthread_key_create(&m_stThreadDataKey, NULL);

	SetThreadData(NULL);
}

CThread::~CThread()
{
	Terminate();
}

int32_t CThread::Start()
{
	m_bTerminated = false;

#ifdef WIN32
	m_hThread = (HANDLE)_beginthread(ThreadProc, 0, this);
#else
	pthread_create(&m_thread, NULL, ThreadProc, this);
#endif

	return 0;
}

int32_t CThread::Terminate()
{
	if (m_bTerminated)
	{
		return 0;
	}

    m_bTerminated = true;

#ifdef WIN32
    if(m_hThread != INVALID_HANDLE_VALUE)
    {
	WaitForSingleObject(m_hThread, m_nTimeOut);
    }
#else
    if(m_thread != 0)
    {
    	pthread_join(m_thread, NULL);
    }
#endif

	return 0;
}

void CThread::Execute()
{
	uint32_t n = 0;
	while (!m_bTerminated)
	{
		if ((++n) % 1000 == 0)
		{
			printf("%d\n", n);
		}
		Delay(1000);
	}

#ifdef WIN32
	_endthread();
#else
	pthread_exit(NULL);
#endif

}

bool CThread::GetTerminated()
{
	return m_bTerminated;
}

void CThread::SetTerminated(bool bTerminated)
{
	m_bTerminated = bTerminated;
}

bool CThread::SetPriority(int nPriority)
{
#ifdef WIN32
	if(m_hThread != INVALID_HANDLE_VALUE)
	{
		//return SetThreadPriority(m_hThread, nPriority);
	}
#endif
	return true;
}

int CThread::GetPriority()
{
#ifdef WIN32
	if(m_hThread != INVALID_HANDLE_VALUE)
	{
		//return GetThreadPriority(m_hThread);
	}
#endif
	return 1;
}

int32_t	CThread::SetThreadData(const void *pData)
{
	return pthread_setspecific(m_stThreadDataKey, pData);
}

void *CThread::GetThreadData()
{
	return pthread_getspecific(m_stThreadDataKey);
}

const char* CThread::GetName()
{
	return m_szName;
}

void CThread::SetName(const char* name)
{
	strncpy(m_szName, name, sizeof(m_szName));
	m_szName[sizeof(m_szName) - 1] = '\0';
}

const char* CThread::GetLogName()
{
	return m_szLogName;
}

void CThread::SetLogName(const char* log_name)
{
	strncpy(m_szLogName, log_name, sizeof(m_szLogName));
	m_szLogName[sizeof(m_szLogName) - 1] = '\0';
}

#if defined(WIN32) || defined(_WIN32)
CThread* CThread::GetLogCtxByPrivateKey()
{
	return NULL;
}

int32_t CThread::InitPrivateKeyLog()
{
	return 0;
}
#else
CThread* CThread::GetLogCtxByPrivateKey()
{
	void* data = pthread_getspecific(ms_private_key_log);
	return (CThread*)data;
}
int32_t CThread::InitPrivateKeyLog()
{
	int ret = pthread_key_create(&ms_private_key_log, NULL);
	return ret;
}
#endif

bool CThread::IsSetPrivateKeyLog()
{
	return ms_bIsSetPrivateKeyLog;
}
void CThread::SetPrivateKeyLog()
{
	ms_bIsSetPrivateKeyLog = true;
}




