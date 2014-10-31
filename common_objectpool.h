/*
 * common_objectpool.h
 *
 *  Created on: 2013Äê8ÔÂ22ÈÕ
 *      Author: 1854
 */

#ifndef COMMON_OBJECTPOOL_H_
#define COMMON_OBJECTPOOL_H_

#include "common_typedef.h"

#include <list>
using namespace std;

template<typename T, int CAPACITY>
class CObjectPool
{
public:
	CObjectPool(){Init();};
	~CObjectPool(){Destroy();};

	void Init()
	{
		T* obj = NULL;
		for(uint32_t i = 0; i < CAPACITY; i ++)
		{
			obj = new T;
			m_stObjectList.push_back(obj);
		}
	};

	void Destroy()
	{
		T* obj = NULL;
		while(!m_stObjectList.empty())
		{
			obj = m_stObjectList.front();
			if(obj != NULL)
			{
				delete obj;
				obj = NULL;
			}

			m_stObjectList.pop_front();
		}
	}

	void AllocObject(uint32_t number)
	{
		T* obj = NULL;
		for(uint32_t i = 0; i < number; i ++)
		{
			obj = new T;
			m_stObjectList.push_back(obj);
		}
	}

	void PushObj(T* obj)
	{
		if(obj == NULL)
			return ;

		m_stObjectList.push_back(obj);
	}

	T* PopObj()
	{
		if(m_stObjectList.empty())
		{
			AllocObject(CAPACITY / 2);
		}

		T* obj = m_stObjectList.front();
		m_stObjectList.pop_front();

		return obj;
	}

	size_t Size()
	{
		return m_stObjectList.size();
	};
private:
	list<T*>	m_stObjectList;
};

#endif /* COMMON_OBJECTPOOL_H_ */
