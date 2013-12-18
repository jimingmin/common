/*
 * common_timermgt.cpp
 *
 *  Created on: 2013年12月18日
 *      Author: jimm
 */

#include "common_datetime.h"
#include "common_errordef.h"
#include "common_timermgt.h"
#include "common_memmgt.h"


CTimerMgt::CTimerMgt()
{
	m_nTimerSeq = 0;
}

CTimerMgt::~CTimerMgt()
{

}

//初始化数据中心
int32_t CTimerMgt::Initialize()
{
	MUTEX_GUARD(TimerLock, m_stTimerLock);
	m_timerPool.Initialize();
	m_timerMap.Initialize();
	return S_OK;
}

//注销定时器管理器
int32_t CTimerMgt::Uninitialize()
{
	MUTEX_GUARD(TimerLock, m_stTimerLock);
	m_timerPool.Uninitialize();
	m_timerMap.Uninitialize();
	return S_OK;
}

//管理器大小
int32_t CTimerMgt::GetSize()
{
	return sizeof(*this);
}

int32_t CTimerMgt::CreateTimer(TimerProc Proc, CObject *pTimer, ITimerData *pTimerData, int64_t nCycleTime, bool bLoop, TimerIndex& timerIndex)
{
	uint8_t *pObj = NULL;
	if(pTimerData != NULL)
	{
		int32_t nTimerDataSize = pTimerData->GetSize();
		pObj = MALLOC(nTimerDataSize);//g_FrameMemMgt.AllocBlock(nTimerDataSize);
		if(pObj == NULL)
		{
			return E_UNKNOWN;
		}

		memcpy(pObj, pTimerData, nTimerDataSize);
	}

	//初始化定时器参数
	Timer timer;
	timer.nTimerID = 0;
	timer.pData = pObj;
	timer.nStartTime = CTimeValue::CurrentTime().Microseconds();
	timer.nCycleTime = nCycleTime;
	timer.nEndTime = timer.nStartTime + timer.nCycleTime;
	timer.bLoop = bLoop;
	timer.pTimerHandler = pTimer;
	timer.pTimerProc = Proc;
	timer.nFiredCount = 0;

	MUTEX_GUARD(TimerLock, m_stTimerLock);
	//设置Timer生成时的Seq
	timer.nTimerSeq = m_nTimerSeq;
	//递增TimerSeq;
	++m_nTimerSeq;

	//创建定时器对象
	TimerPool::CIndex* pIndex = m_timerPool.CreateObject();
	if (NULL == pIndex)
	{
		FREE(pObj);
		return E_UNKNOWN;
	}

	//建立定时器索引
	TimerMap::CIndex* pMapIndex = m_timerMap.Insert(timer.nEndTime, pIndex->Index());
	if (NULL == pMapIndex)
	{
		FREE(pObj);
		m_timerPool.DestroyObject(pIndex);
		pIndex = NULL;
		return E_UNKNOWN;
	}

	//将索引保存到附加数据表中
	int32_t ret = pIndex->SetAdditionalData(enmAdditionalIndex_RBTreeIndex, (uint64_t)pMapIndex);
	if (0 > ret)
	{
		FREE(pObj);
		m_timerMap.Erase(pMapIndex);
		m_timerPool.DestroyObject(pIndex);
		pIndex = NULL;
		return ret;
	}

	//设置定时器参数
	pIndex->ObjectPtr()->SetTimer(timer);
	timerIndex = pIndex->Index();

	return S_OK;
}

//删除定时器
int32_t CTimerMgt::RemoveTimer(TimerIndex timerIndex)
{
	MUTEX_GUARD(TimerLock, m_stTimerLock);
	TimerPool::CIndex* pIndex = m_timerPool.GetIndex(timerIndex);
	if (NULL == pIndex)
	{
		return E_UNKNOWN;
	}

	return RemoveTimer(pIndex);
}

//清空定时器管理器
int32_t CTimerMgt::Clear()
{
	MUTEX_GUARD(TimerLock, m_stTimerLock);
	m_timerPool.Clear();
	m_timerMap.Clear();
	return S_OK;
}

//定时器已触发
int32_t CTimerMgt::TimerFired(TimerIndex timerIndex, uint32_t timerSeq)
{
	CTimer *pTimer = NULL;
	int32_t nRet = GetTimer(timerIndex,pTimer);
	if( nRet == S_OK && pTimer->GetTimerSeq() == timerSeq)
	{
		return TimerFired(timerIndex);
	}

	return S_FALSE;
}


//获取定时器
int32_t CTimerMgt::GetTimer(TimerIndex timerIndex, CTimer*& pTimer)
{
	MUTEX_GUARD(TimerLock, m_stTimerLock);
	TimerPool::CIndex* pIndex = m_timerPool.GetIndex(timerIndex);
	if (NULL == pIndex)
	{
		return E_UNKNOWN;
	}

	pTimer = pIndex->ObjectPtr();

	return S_OK;
}

//删除定时器
int32_t CTimerMgt::RemoveTimer(TimerPool::CIndex* pIndex)
{
	if (NULL == pIndex)
	{
		return E_UNKNOWN;
	}

	CTimer* pTimer = pIndex->ObjectPtr();
	if(pTimer == NULL)
	{
		return E_NULLPOINTER;
	}
	//回收定时器所申请的内存资源
	Timer timer;
	pTimer->GetTimer(timer);
	if(timer.pData != NULL)
	{
		FREE((uint8_t *)timer.pData);//g_FrameMemMgt.RecycleBlock((uint8_t *)timer.pData);
	}

	uint64_t nAddtionalValue = 0;
	pIndex->GetAdditionalData(enmAdditionalIndex_RBTreeIndex, nAddtionalValue);
	TimerMap::CIndex* pMapIndex = (TimerMap::CIndex*)nAddtionalValue;

	//将定时器从对象池及索引表中删除
//	MUTEX_GUARD(TimerLock, m_stTimerLock);
	m_timerMap.Erase(pMapIndex);
	m_timerPool.DestroyObject(pIndex);

	return S_OK;
}

//定时器已触发
int32_t CTimerMgt::TimerFired(TimerIndex timerIndex)
{
	MUTEX_GUARD(TimerLock, m_stTimerLock);
	TimerPool::CIndex* pIndex = m_timerPool.GetIndex(timerIndex);
	if (NULL == pIndex)
	{
		return E_UNKNOWN;
	}

	CTimer* pTimer = pIndex->ObjectPtr();
	if(pTimer == NULL)
	{
		return E_NULLPOINTER;
	}

	if (!pTimer->IsLoop())
	{
		//若不是循环触发则删除定时器
		return RemoveTimer(pIndex);
	}

	Timer timer;
	pTimer->GetTimer(timer);

	//更新定时器参数
	timer.nEndTime = CTimeValue::CurrentTime().Microseconds() + timer.nCycleTime;
	++timer.nFiredCount;
	pTimer->SetTimer(timer);

	//调整定时器索引
	uint64_t nAddtionalValue = 0;
	pIndex->GetAdditionalData(enmAdditionalIndex_RBTreeIndex, nAddtionalValue);
	TimerMap::CIndex* pMapIndex = (TimerMap::CIndex*)nAddtionalValue;

	m_timerMap.Erase(pMapIndex);
	pMapIndex = m_timerMap.Insert(timer.nEndTime, pIndex->Index());
	if (NULL == pMapIndex)
	{
		m_timerPool.DestroyObject(pIndex);
		return E_UNKNOWN;
	}

	//将索引保存到附加数据表中
	int32_t ret = pIndex->SetAdditionalData(enmAdditionalIndex_RBTreeIndex, (uint64_t)pMapIndex);
	if (0 > ret)
	{
		m_timerMap.Erase(pMapIndex);
		m_timerPool.DestroyObject(pIndex);
		return ret;
	}

	return S_OK;
}

//获取结束时间最小的定时器
int32_t CTimerMgt::GetFirstTimer(CTimer*& pTimer, TimerIndex& timerIndex)
{
	MUTEX_GUARD(TimerLock, m_stTimerLock);
	TimerMap::CIndex* pMapIndex = m_timerMap.First();
	if (NULL == pMapIndex)
	{
		return E_UNKNOWN;
	}

	TimerPool::CIndex* pIndex = m_timerPool.GetIndex( pMapIndex->Object() );
	if (NULL == pIndex)
	{
		//索引列表中存在无效索引则删除
		m_timerMap.Erase(pMapIndex);
		return E_UNKNOWN;
	}

	pTimer = pIndex->ObjectPtr();
	timerIndex = pIndex->Index();

	return S_OK;
}

//处理定时器事件
bool CTimerMgt::Process()
{
	CTimer *pTimer = NULL;
	TimerIndex timerIndex = -1;

	//获取定时器列表中的结束时间最早的定时器
	int32_t ret = GetFirstTimer(pTimer, timerIndex);
	if (0 > ret)
	{
		return false;
	}

	//定时器结束时间小于当前时间
	if (pTimer->GetEndTime() > CTimeValue::CurrentTime().Microseconds())
	{
		return false;
	}

	//保留TimerSeq 用于保护
	uint32_t nTimerSeq = pTimer->GetTimerSeq();

	//外部Timer直接回调接口
	CObject *pHandler = pTimer->GetTimerHandler();
	if( NULL != pHandler)
	{
		TimerProc proc = pTimer->GetEventProc();
		if(proc != NULL)
		{
			(pHandler->*proc)(pTimer);
		}
	}

	//定时器事件已触发
	TimerFired(timerIndex,nTimerSeq);

	return true;
}
