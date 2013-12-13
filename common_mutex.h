/*
 * common_mutex.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_MUTEX_H_
#define COMMON_MUTEX_H_

#include <pthread.h>

class CriticalSection
{
public:
	CriticalSection()
	{
		pthread_mutexattr_t mutex_attribute;
		pthread_mutexattr_init(&mutex_attribute);
		pthread_mutexattr_settype(&mutex_attribute, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_stMutex, &mutex_attribute);
		pthread_mutexattr_destroy(&mutex_attribute);
	};

	~CriticalSection()
	{
		pthread_mutex_destroy(&m_stMutex);
	};

	void enter()
	{
		pthread_mutex_lock(&m_stMutex);
	};

	bool try_enter()
	{
		return pthread_mutex_trylock(&m_stMutex) == 0 ? true : false;
	};

	void leave()
	{
		pthread_mutex_unlock(&m_stMutex);
	};

private:
	pthread_mutex_t m_stMutex;
};


class CThreadMutex
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


#define MUTEX_GUARD(M, C)				CThreadMutex M(C);
#define MUTEX_GUARD_RETURN(M, C, R)		if(!C.try_enter()){return (R);};\
										C.leave();\
										MUTEX_GUARD(M, C);

#endif /* COMMON_MUTEX_H_ */
