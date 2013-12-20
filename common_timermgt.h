/*
 * common_timermgt.h
 *
 *  Created on: 2013年12月18日
 *      Author: jimm
 */

#ifndef COMMON_TIMERMGT_H_
#define COMMON_TIMERMGT_H_

#include "common_object.h"
#include "common_pool.h"
#include "common_rbtree.h"
#include "common_singleton.h"
#include "common_mutex.h"
#include "common_timer.h"

#define MAX_TIMER_COUNT			100000		//定时器个数
typedef CPool<CTimer, MAX_TIMER_COUNT>	TimerPool;
typedef CRBTree<int64_t, TimerIndex, MAX_TIMER_COUNT>	TimerMap;

class CTimerMgt : public CObject
{
protected:
	enum
	{
		enmAdditionalIndex_RBTreeIndex			= 0,
	};
public:
	CTimerMgt();
	virtual ~CTimerMgt();

public:
	virtual int32_t Init();
	virtual int32_t Uninit();
	virtual int32_t GetSize();

public:
	bool Process();

	int32_t CreateTimer(TimerProc Proc, CObject *pTimer, CObject *pTimerData, int64_t nCycleTime, bool bLoop, TimerIndex& timerIndex);

	int32_t RemoveTimer(TimerIndex timerIndex);

	int32_t Clear();

	int32_t TimerFired(TimerIndex timerIndex, uint32_t timerSeq);

	int32_t GetFirstTimer(CTimer*& pTimer, TimerIndex& timerIndex);

	int32_t GetTimer(TimerIndex timerIndex, CTimer*& pTimer);

	int32_t RemoveTimer(TimerPool::CIndex* pIndex);

	int32_t TimerFired(TimerIndex timerIndex);

	int32_t GetFirstTimer(TimerPool::CIndex*& pIndex);

protected:
	uint32_t				m_nTimerSeq;
	CriticalSection			m_stTimerLock;
//	CriticalSection			m_stTimerPoolLock;
	TimerPool				m_timerPool;
//	CriticalSection			m_stTimerMapLock;
	TimerMap				m_timerMap;

};

#endif /* COMMON_TIMERMGT_H_ */
