/*
 * common_cyclebuffer.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_CYCLEBUFFER_H_
#define COMMON_CYCLEBUFFER_H_

#include "common_mutex.h"
#include "common_memmgt.h"
#include "common_api.h"
#include <stdlib.h>
#include <string.h>

template<int32_t SIZE, bool AUTOGROW = false>
class EXPORT CycleBuffer
{
public:
	CycleBuffer()
	{
		m_bQueue = false;
		Reset();
		m_pBuf = MALLOC(SIZE);
		m_nBufSize = SIZE;
	}

	virtual ~CycleBuffer()
	{
		if(m_pBuf != NULL)
		{
			FREE(m_pBuf);
		}
	}

	void AsQueue()
	{
		if(AUTOGROW)
		{
			ASSERT(1);
		}
		m_bQueue = true;
	}

	int32_t Grow(int32_t nSize)
	{
		if(nSize <= 0)
		{
			return 0;
		}

		MUTEX_GUARD(lock, m_stLock);
		uint8_t *pMem = MALLOC(nSize + m_nBufSize);
		if(pMem == NULL)
		{
			return 0;
		}

		int32_t nWriteIndex = 0;
		do
		{
			int32_t nReadBytes = Read(pMem + nWriteIndex, 4096);
			if(nReadBytes <= 0)
			{
				break;
			}

			nWriteIndex += nReadBytes;
		}while(m_nDataSize > 0);

		if(m_nDataSize > 0)
		{
			return -1;
		}

		if(m_pBuf != NULL)
		{
			FREE(m_pBuf);
		}
		m_pBuf = pMem;

		m_nReadIndex = 0;
		m_nWriteIndex = nWriteIndex;
		m_nDataSize = nWriteIndex;
		m_nBufSize += nSize;

		return nSize;
	}

	//写到写列表的头部内存
	int32_t WriteToHead(const uint8_t *pBuf, int32_t nDataSize)
	{
		if(nDataSize <= 0 || pBuf == NULL)
		{
			return 0;
		}

		if(m_bQueue)
		{
			ASSERT(1);
			return 0;
		}

		MUTEX_GUARD(lock, m_stLock);
		//剩余空间不足
		if(m_nDataSize + nDataSize > m_nBufSize)
		{
			if(!AUTOGROW)
			{
				return -1;
			}

			if(Grow((m_nDataSize + nDataSize) - m_nBufSize) <= 0)
			{
				return 0;
			}
			//return -1;
		}

		if(m_nWriteIndex < m_nReadIndex)
		{
			memcpy(&m_pBuf[m_nReadIndex - nDataSize], pBuf, nDataSize);
			m_nReadIndex -= nDataSize;
		}
		//出现这中情况一般都是初始值的时候
		else if(m_nWriteIndex == m_nReadIndex)
		{
			return Write(pBuf, nDataSize);
		}
		else
		{
			int32_t nLeftDataSize = (m_nReadIndex - nDataSize < 0) ? m_nReadIndex : nDataSize;
			if(nLeftDataSize < nDataSize)
			{
				int32_t nTailDataSize = nDataSize - nLeftDataSize;
				int32_t nWriteIndex = m_nBufSize - nTailDataSize;
				memcpy(&m_pBuf[nWriteIndex], pBuf, nTailDataSize);
				memcpy(&m_pBuf[0], pBuf + nTailDataSize, nLeftDataSize);
				m_nReadIndex = nWriteIndex;
			}
			else
			{
				if(m_nReadIndex - nDataSize < 0)
				{
					return 0;
				}

				memcpy(&m_pBuf[m_nReadIndex - nDataSize], pBuf, nDataSize);
				m_nReadIndex -= nDataSize;
			}
		}

		m_nDataSize += nDataSize;

		return nDataSize;
	}

	//写到写列表的尾部内存
	int32_t Write(const uint8_t *pBuf, int32_t nDataSize)
	{
		if(nDataSize <= 0 || pBuf == NULL)
		{
			return 0;
		}

		if(!m_bQueue)
		{
			MUTEX_GUARD(lock, m_stLock);
		}

		//剩余空间不足
		if(m_nDataSize + nDataSize > m_nBufSize)
		{
			if(!AUTOGROW)
			{
				return -1;
			}
			if(Grow((m_nDataSize + nDataSize) - m_nBufSize) <= 0)
			{
				return 0;
			}
			//return -1;
		}

		if(m_nWriteIndex < m_nReadIndex)
		{
			memcpy(&m_pBuf[m_nWriteIndex], pBuf, nDataSize);
			m_nWriteIndex += nDataSize;
		}
		else
		{
			int32_t nLeftDataSize = m_nBufSize - m_nWriteIndex;
			if(nLeftDataSize < nDataSize)
			{
				memcpy(&m_pBuf[m_nWriteIndex], pBuf, nLeftDataSize);
				memcpy(&m_pBuf[0], pBuf + nLeftDataSize, nDataSize - nLeftDataSize);
				m_nWriteIndex = nDataSize - nLeftDataSize;
			}
			else
			{
				memcpy(&m_pBuf[m_nWriteIndex], pBuf, nDataSize);
				m_nWriteIndex += nDataSize;
			}
		}

		m_nDataSize += nDataSize;

		return nDataSize;
	}

	//读取读列表的尾部内存
	int32_t ReadFromTail(uint8_t *pBuf, const int32_t nWantSize)
	{
		if(nWantSize <= 0 || pBuf == NULL)
		{
			return 0;
		}

		if(m_bQueue)
		{
			ASSERT(1);
			return 0;
		}

		MUTEX_GUARD(lock, m_stLock);

		int32_t nDataSize = ((m_nDataSize < nWantSize) ? m_nDataSize : nWantSize);
		if(m_nReadIndex < m_nWriteIndex)
		{
			if(m_nWriteIndex - nDataSize < 0)
			{
				return 0;
			}

			memcpy(pBuf, &m_pBuf[m_nWriteIndex - nDataSize], nDataSize);
			m_nWriteIndex -= nDataSize;
		}
		else if(m_nReadIndex == m_nWriteIndex)
		{
			return 0;
		}
		else
		{
			if(m_nWriteIndex - nWantSize >= 0)
			{
				memcpy(pBuf, &m_pBuf[m_nWriteIndex - nWantSize], nWantSize);
				m_nWriteIndex -= nWantSize;
			}
			else
			{
				int32_t nTailDataSize = nDataSize - m_nWriteIndex;
				int32_t nWriteIndex = m_nBufSize - nTailDataSize;
				memcpy(pBuf, &m_pBuf[nWriteIndex], nTailDataSize);
				memcpy(&pBuf[nTailDataSize], &m_pBuf[0], m_nWriteIndex);
				m_nWriteIndex = nWriteIndex;
			}
		}

		m_nDataSize -= nDataSize;

		return nDataSize;
	}

	//读取读列表的头部内存
	int32_t Read(uint8_t *pBuf, const int32_t nWantSize)
	{
		if(nWantSize <= 0 || pBuf == NULL)
		{
			return 0;
		}

		if(!m_bQueue)
		{
			MUTEX_GUARD(lock, m_stLock);
		}

		int32_t nDataSize = ((m_nDataSize < nWantSize) ? m_nDataSize : nWantSize);
		if(nDataSize <= 0)
		{
			return 0;
		}

		if(m_nReadIndex < m_nWriteIndex)
		{
			memcpy(pBuf, &m_pBuf[m_nReadIndex], nDataSize);
			m_nReadIndex += nDataSize;
		}
		else
		{
			int32_t nLeftDataSize = m_nBufSize - m_nReadIndex;
			if(nLeftDataSize < nDataSize)
			{
				memcpy(pBuf, &m_pBuf[m_nReadIndex], nLeftDataSize);
				memcpy(pBuf + nLeftDataSize, &m_pBuf[0], nDataSize - nLeftDataSize);
				m_nReadIndex = nDataSize - nLeftDataSize;
			}
			else
			{
				memcpy(pBuf, &m_pBuf[m_nReadIndex], nDataSize);
				m_nReadIndex += nDataSize;
			}
		}

		m_nDataSize -= nDataSize;

		return nDataSize;
	}

	//读取数据但是不修改读索引
	int32_t PeekRead(uint8_t *pBuf, const int32_t nWantSize)
	{
		if(nWantSize <= 0 || pBuf == NULL)
		{
			return 0;
		}

		MUTEX_GUARD(lock, m_stLock);

		int32_t nDataSize = ((m_nDataSize < nWantSize) ? m_nDataSize : nWantSize);
		if(m_nReadIndex < m_nWriteIndex)
		{
			memcpy(pBuf, &m_pBuf[m_nReadIndex], nDataSize);
		}
		else if(m_nReadIndex == m_nWriteIndex)
		{
			return 0;
		}
		else
		{
			int32_t nLeftDataSize = m_nBufSize - m_nReadIndex;
			if(nLeftDataSize < nDataSize)
			{
				memcpy(pBuf, &m_pBuf[m_nReadIndex], nLeftDataSize);
				memcpy(pBuf + nLeftDataSize, &m_pBuf[0], nDataSize - nLeftDataSize);
			}
			else
			{
				memcpy(pBuf, &m_pBuf[m_nReadIndex], nDataSize);
			}
		}

		return nDataSize;
	}

	void Reset()
	{
		MUTEX_GUARD(lock, m_stLock);
		m_nReadIndex = 0;
		m_nWriteIndex = 0;
		m_nDataSize = 0;
		//memset(m_pBuf, 0, m_nBufSize);
	}

	int32_t Size()
	{
		MUTEX_GUARD(lock, m_stLock);
		return m_nDataSize;
	}

	int32_t Capatity()
	{
		MUTEX_GUARD(lock, m_stLock);
		return m_nBufSize;
	}

	int32_t Space()
	{
		MUTEX_GUARD(lock, m_stLock);
		return m_nBufSize - m_nDataSize;
	}

protected:
	bool					m_bQueue;
	CriticalSection			m_stLock;
	volatile int32_t		m_nReadIndex;
	volatile int32_t		m_nWriteIndex;
	volatile int32_t		m_nDataSize;
//	uint8_t					m_arrBuf[SIZE];
	volatile int32_t		m_nBufSize;
	uint8_t					*m_pBuf;
};

#endif /* COMMON_CYCLEBUFFER_H_ */
