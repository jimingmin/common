/*
 * common_mutex.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_MUTEX_H_
#define COMMON_MUTEX_H_

#ifdef WIN32
#include <WinSock2.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "common_export.h"

class EXPORT CriticalSection
{
public:
	CriticalSection()
	{
#ifdef WIN32
		m_stMutex = CreateMutex(NULL, FALSE, NULL);
#else
		//pthread_mutexattr_t mutex_attribute;
		//pthread_mutexattr_init(&mutex_attribute);
		//pthread_mutexattr_settype(&mutex_attribute, PTHREAD_MUTEX_TIMED_NP);
		//pthread_mutex_init(&m_stMutex, &mutex_attribute);
		//pthread_mutexattr_destroy(&mutex_attribute);
		pthread_mutex_init(&m_stMutex, NULL);
#endif
	};

	~CriticalSection()
	{
#ifdef WIN32
		CloseHandle(m_stMutex);
#else
		pthread_mutex_destroy(&m_stMutex);
#endif
	};

	void enter()
	{
#ifdef WIN32
		WaitForSingleObject(m_stMutex, INFINITE);
#else
		pthread_mutex_lock(&m_stMutex);
#endif
	};

	bool try_enter()
	{
#ifdef WIN32
		return WaitForSingleObject(m_stMutex, INFINITE) == WAIT_OBJECT_0 ? true : false;
#else
		return pthread_mutex_trylock(&m_stMutex) == 0 ? true : false;
#endif
	};

	void leave()
	{
#ifdef WIN32
		ReleaseMutex(m_stMutex);
#else
		pthread_mutex_unlock(&m_stMutex);
#endif
	};

private:

#ifdef WIN32
	HANDLE m_stMutex;
#else
	pthread_mutex_t m_stMutex;
#endif
};


class EXPORT CThreadMutex
{
public:
	CThreadMutex(CriticalSection &cirt) : m_stCS(cirt)
	{
		cirt.enter();
	};

	~CThreadMutex()
	{
		m_stCS.leave();
	};

private:
	CriticalSection& m_stCS;
};


#define MUTEX_GUARD(M, C)				//CThreadMutex M(C);
#define MUTEX_GUARD_RETURN(M, C, R)		/*if(!C.try_enter()){return (R);};\
										C.leave();\
										MUTEX_GUARD(M, C);*/

#endif /* COMMON_MUTEX_H_ */
