/*
 * common_singleton.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_SINGLETON_H_
#define COMMON_SINGLETON_H_

#include <stdio.h>
#include "common_mutex.h"
#include "common_export.h"

template<typename T>
class CSingleton
{
public:
	//创建实例
    static T& CreateInstance()
	{
		return GetInstance();
	}
	//获取实例
	EXPORT static T& GetInstance()
	{
		if (NULL == ms_pInstance)
		{
			MUTEX_GUARD(lock, m_stLock);
			if(NULL == ms_pInstance)
			{
				ms_pInstance = new T();
			}
		}

		return *ms_pInstance;
	}
	//销毁实例
	static void DestroyInstance()
	{
		if (NULL != ms_pInstance)
		{
			MUTEX_GUARD(lock, m_stLock);
			if(NULL != ms_pInstance)
			{
				delete ms_pInstance;
				ms_pInstance = NULL;
			}
		}
	}

	//将用新的实例将旧的实例换出来
	static T* SwapInstance(T* new_one)
	{
		MUTEX_GUARD(lock, m_stLock);

		T* old_one = ms_pInstance;
		ms_pInstance = new_one;
		return old_one;
	}

protected:
	EXPORT static T	*ms_pInstance;
	EXPORT static CriticalSection	m_stLock;

protected:
	CSingleton()
	{
	}
	virtual ~CSingleton()
	{
	}

private:
    // Prohibited actions
    CSingleton(const CSingleton &);
    CSingleton& operator = (const CSingleton &);

};

template<typename T>
T* CSingleton<T>::ms_pInstance = NULL;

template<typename T>
CriticalSection CSingleton<T>::m_stLock;


#endif /* COMMON_SINGLETON_H_ */
