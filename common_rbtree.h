/*
 * common_rbtree.h
 *
 *  Created on: 2011-11-21
 *      Author: jimm
 */

#ifndef COMMON_RBTREE_H_
#define COMMON_RBTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_typedef.h"
#include "common_errordef.h"
#include "common_object.h"


template<typename TKEY, typename T, uint32_t CAPACITY>
class CRBTree : public CObject
{
public:
	enum
	{
		enmIndexFlag_Free				= 0,
		enmIndexFlag_Used				= 1,
		enmInvalidObjectID				= -1,
	};

	enum
	{
		enmNodeColor_Red				= 0,
		enmNodeColor_Black				= 1,
	};


public:

	class CIndex : public CObject
	{
	public:
		CIndex()
		{
		}
		virtual ~CIndex()
		{
		}

	public:

		virtual int32_t Initialize()
		{
			m_pContainer = NULL;
			m_nFlag = enmIndexFlag_Free;
			m_nIndex = enmInvalidObjectID;
			//m_pNext = NULL;
			//m_pPrev = NULL;
			m_nNext = enmInvalidObjectID;
			m_nPrev = enmInvalidObjectID;

			m_nColor = enmNodeColor_Red;
			//m_pParent = NULL;
			//m_pLeft = NULL;
			//m_pRight = NULL;
			m_nParent = enmInvalidObjectID;
			m_nLeft = enmInvalidObjectID;
			m_nRight = enmInvalidObjectID;

			memset(&m_key, 0, sizeof(m_key));

			//m_object.Initialize();

			return S_OK;
		}

		virtual int32_t Uninitialize()
		{
			m_nFlag = enmIndexFlag_Free;
			m_nIndex = enmInvalidObjectID;
			//m_pNext = NULL;
			//m_pPrev = NULL;
			m_nNext = enmInvalidObjectID;
			m_nPrev = enmInvalidObjectID;

			m_nColor = enmNodeColor_Red;
			//m_pParent = NULL;
			//m_pLeft = NULL;
			//m_pRight = NULL;
			m_nParent = enmInvalidObjectID;
			m_nLeft = enmInvalidObjectID;
			m_nRight = enmInvalidObjectID;

			memset(&m_key, 0, sizeof(m_key));

			//m_object.Uninitialize();

			return S_OK;
		}

		virtual int32_t GetSize()
		{
			return sizeof(*this);
		}
	public:

		CIndex* Prev()
		{
			if (enmInvalidObjectID != m_nLeft)
			{
				return Left()->MaxNode();
			}

			CIndex* pNode = this;
			CIndex* pParent = Parent();
			while (NULL != pParent)
			{
				if (pNode != pParent->Left())
				{
					return pParent;
				}
				pNode = pParent;
				pParent = pParent->Parent();
			}

			return NULL;
		}

		CIndex* Next()
		{
			if (enmInvalidObjectID != m_nRight)
			{
				return Right()->MinNode();
			}

			CIndex* pNode = this;
			CIndex* pParent = Parent();
			while (NULL != pParent)
			{
				if (pNode != pParent->Right())
				{
					return pParent;
				}
				pNode = pParent;
				pParent = pParent->Parent();
			}

			return NULL;
		}

		inline CIndex* NextNode()
		{
			if (enmInvalidObjectID == m_nNext)
			{
				return NULL;
			}
			return &m_pContainer->m_arrIndex[m_nNext];
		}

		inline CIndex* PrevNode()
		{
			if (enmInvalidObjectID == m_nPrev)
			{
				return NULL;
			}
			return &m_pContainer->m_arrIndex[m_nPrev];
		}

		inline const TKEY& GetKey() const
		{
			return m_key;
		}

		inline T& GetObject()
		{
			return m_object.GetValue();
		}
		inline T& Object()
		{
			return m_object.GetValue();
		}

		inline void SetObject(T& object)
		{
			m_object.SetValue(object);
		}

		inline int32_t Index()
		{
			return m_nIndex;
		}

	protected:

		inline void SetContainer(CRBTree* pContainer)
		{
			m_pContainer = pContainer;
		}

		inline void SetUsed()
		{
			m_nFlag = enmIndexFlag_Used;
		}

		inline void SetFree()
		{
			m_nFlag = enmIndexFlag_Free;
		}

		inline bool IsUsed() const
		{
			return (enmIndexFlag_Used == m_nFlag);
		}

		inline uint32_t GetColor()
		{
			return m_nColor;
		}

		inline void SetColor(uint32_t nColor)
		{
			m_nColor = nColor;
		}


		inline int32_t GetNext() const
		{
			return m_nNext;
		}

		inline int32_t GetPrev() const
		{
			return m_nPrev;
		}

		inline void SetNext(int32_t nNext)
		{
			m_nNext = nNext;
		}

		inline void SetPrev(int32_t nPrev)
		{
			m_nPrev = nPrev;
		}

		inline int32_t GetParent() const
		{
			return m_nParent;
		}

		inline int32_t GetLeft() const
		{
			return m_nLeft;
		}

		inline int32_t GetRight() const
		{
			return m_nRight;
		}

		inline void SetParent(int32_t nParent)
		{
			m_nParent = nParent;
		}

		inline void SetLeft(int32_t nLeft)
		{
			m_nLeft = nLeft;
		}

		inline void SetRight(int32_t nRight)
		{
			m_nRight = nRight;
		}

		inline CIndex* Parent()
		{
			if (enmInvalidObjectID == m_nParent)
			{
				return NULL;
			}
			return &m_pContainer->m_arrIndex[m_nParent];
		}

		inline CIndex* Left()
		{
			if (enmInvalidObjectID == m_nLeft)
			{
				return NULL;
			}
			return &m_pContainer->m_arrIndex[m_nLeft];
		}

		inline CIndex* Right()
		{
			if (enmInvalidObjectID == m_nRight)
			{
				return NULL;
			}
			return &m_pContainer->m_arrIndex[m_nRight];
		}

		inline void SetIndex(int32_t nIndex)
		{
			m_nIndex = nIndex;
		}

		inline void SetKey(TKEY key)
		{
			m_key = key;
		}

		inline void ResetRBNode()
		{
			m_nParent = enmInvalidObjectID;
			m_nLeft = enmInvalidObjectID;
			m_nRight = enmInvalidObjectID;
			memset(&m_key, 0, sizeof(m_key));
		}

		CIndex* MinNode()
		{
			CIndex* pNode = this;

			while (NULL != pNode->Left())
			{
				pNode = pNode->Left();
			}

			return pNode;
		}

		CIndex* MaxNode()
		{
			CIndex* pNode = this;

			while (NULL != pNode->Right())
			{
				pNode = pNode->Right();
			}

			return pNode;
		}

	protected:
		CRBTree*		m_pContainer;

		uint32_t		m_nFlag;
		int32_t			m_nIndex;

		int32_t			m_nNext;
		int32_t			m_nPrev;

		uint32_t		m_nColor;

		int32_t			m_nParent;
		int32_t			m_nLeft;
		int32_t			m_nRight;

		TKEY			m_key;
		CComplexType<T>	m_object;

		friend class CRBTree;
	};


public:
	CRBTree()
	{
	}
	virtual ~CRBTree()
	{
	}

public:

	virtual int32_t Initialize()
	{
		m_nObjectCount = 0;
		m_nFreeCount = CAPACITY;
		//m_pFreeHead = &m_arrIndex[0];
		//m_pFreeTail = &m_arrIndex[CAPACITY - 1];
		//m_pUsedHead = NULL;
		//m_pUsedTail = NULL;
		m_nFreeHead = 0;
		m_nFreeTail = CAPACITY - 1;
		m_nUsedHead = enmInvalidObjectID;
		m_nUsedTail = enmInvalidObjectID;

		//m_pRoot = NULL;
		m_nRoot = enmInvalidObjectID;

		for (uint32_t i = 0; i < CAPACITY; ++i)
		{
			m_arrIndex[i].Initialize();
		}

		BuildIndexList();

		return S_OK;
	}

	virtual int32_t Uninitialize()
	{
		m_nObjectCount = 0;
		m_nFreeCount = CAPACITY;
		//m_pFreeHead = &m_arrIndex[0];
		//m_pFreeTail = &m_arrIndex[CAPACITY - 1];
		//m_pUsedHead = NULL;
		//m_pUsedTail = NULL;
		m_nFreeHead = 0;
		m_nFreeTail = CAPACITY - 1;
		m_nUsedHead = enmInvalidObjectID;
		m_nUsedTail = enmInvalidObjectID;

		//m_pRoot = NULL;
		m_nRoot = enmInvalidObjectID;

		for (uint32_t i = 0; i < CAPACITY; ++i)
		{
			m_arrIndex[i].Uninitialize();
		}

		BuildIndexList();

		return S_OK;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}
public:

	CIndex* Insert(TKEY key, T object)
	{
		CIndex* pParent = NULL;
		CIndex* pNode = NULL;

		pParent = FindInsertPostion(key);


		pNode = CreateIndex();
		if (NULL == pNode)
		{
			return NULL;
		}

		pNode->SetParent((NULL == pParent) ? enmInvalidObjectID : pParent->Index());
		pNode->SetLeft(enmInvalidObjectID);
		pNode->SetRight(enmInvalidObjectID);
		pNode->SetColor(enmNodeColor_Red);

		pNode->SetKey(key);
		pNode->SetObject(object);

		if (NULL != pParent)
		{
			if (pParent->GetKey() > key)
			{
				pParent->SetLeft(pNode->Index());
			}
			else
			{
				pParent->SetRight(pNode->Index());
			}
		}
		else
		{
			m_nRoot = pNode->Index();
		}

		InsertRebalance(pNode);

		return pNode;
	}

	uint32_t Erase(TKEY key)
	{
		CIndex* pNode = NULL;

		pNode = FindNode(key);
		while (NULL != pNode)
		{
			Erase(pNode);
			pNode = FindNode(key);
		}

		return S_OK;
	}

	uint32_t Erase(CIndex* pNode)
	{
		CIndex* pChild = NULL;
		CIndex* pParent = NULL;
		CIndex* pOld = NULL;
		//CIndex* pLeft = NULL;
		uint32_t nColor = enmNodeColor_Red;

		pOld = pNode;
		if ((NULL != pNode->Left()) && (NULL != pNode->Right()))
		{
			pNode = pNode->Right();
			while (NULL != pNode->Left())
			{
				pNode = pNode->Left();
			}
			pChild = pNode->Right();
			pParent = pNode->Parent();
			nColor = pNode->GetColor();

			if (NULL != pChild)
			{
				pChild->SetParent((NULL == pParent) ? enmInvalidObjectID : pParent->Index());
			}
			if (NULL != pParent)
			{
				if (pParent->Left() == pNode)
				{
					pParent->SetLeft((NULL == pChild) ? enmInvalidObjectID : pChild->Index());
				}
				else
				{
					pParent->SetRight((NULL == pChild) ? enmInvalidObjectID : pChild->Index());
				}
			}
			else
			{
				m_nRoot = (NULL == pChild) ? enmInvalidObjectID : pChild->Index();
			}

			if (pNode->Parent() == pOld)
			{
				pParent = pNode;
			}

			pNode->SetParent(pOld->GetParent());
			pNode->SetColor(pOld->GetColor());
			pNode->SetRight(pOld->GetRight());
			pNode->SetLeft(pOld->GetLeft());

			if (NULL != pOld->Parent())
			{
				if (pOld->Parent()->Left() == pOld)
				{
					pOld->Parent()->SetLeft(pNode->Index());
				}
				else
				{
					pOld->Parent()->SetRight(pNode->Index());
				}
			}
			else
			{
				m_nRoot = pNode->Index();
			}

			pOld->Left()->SetParent(pNode->Index());
			if (NULL != pOld->Right())
			{
				pOld->Right()->SetParent(pNode->Index());
			}
		}
		else
		{
			if (NULL == pNode->Left())
			{
				pChild = pNode->Right();
			}
			else if (NULL == pNode->Right())
			{
				pChild = pNode->Left();
			}
			pParent = pNode->Parent();
			nColor = pNode->GetColor();

			if (NULL != pChild)
			{
				pChild->SetParent((NULL == pParent) ? enmInvalidObjectID : pParent->Index());
			}
			if (NULL != pParent)
			{
				if (pParent->Left() == pNode)
				{
					pParent->SetLeft((NULL == pChild) ? enmInvalidObjectID : pChild->Index());
				}
				else
				{
					pParent->SetRight((NULL == pChild) ? enmInvalidObjectID : pChild->Index());
				}
			}
			else
			{
				m_nRoot = (NULL == pChild) ? enmInvalidObjectID : pChild->Index();
			}
		}

		DestroyIndex(pOld);
		pOld->ResetRBNode();

		if (enmNodeColor_Black == nColor)
		{
			EraseRebalance(pChild, pParent);
		}

		return S_OK;
	}

	void Clear()
	{
		Uninitialize();
	}

	CIndex* Find(TKEY key)
	{
		return FindNode(key);
	}

	inline CIndex* First()
	{
		if (enmInvalidObjectID == m_nRoot)
		{
			return NULL;
		}

		return m_arrIndex[m_nRoot].MinNode();
	}

	inline CIndex* Last()
	{
		if (enmInvalidObjectID == m_nRoot)
		{
			return NULL;
		}

		return m_arrIndex[m_nRoot].MaxNode();
	}

	inline CIndex* FirstNode()
	{
		return (enmInvalidObjectID == m_nUsedHead) ? NULL : &m_arrIndex[m_nUsedHead];
	}

	inline CIndex* LastNode()
	{
		return (enmInvalidObjectID == m_nUsedTail) ? NULL : &m_arrIndex[m_nUsedTail];
	}

	inline uint32_t Capacity() const
	{
		return CAPACITY;
	}

	inline uint32_t ObjectCount() const
	{
		return m_nObjectCount;
	}

	inline bool IsEmpty() const
	{
		return (0 == m_nObjectCount);
	}

	inline bool IsFull() const
	{
		return (m_nObjectCount == CAPACITY);
	}

	inline CIndex* GetIndex(const int32_t nIndex)
	{
		int32_t ret = CheckIndex(nIndex);
		if (0 > ret)
		{
			return NULL;
		}
		return &m_arrIndex[nIndex];
	}

	inline int32_t GetIndex(const CIndex* pIndex)
	{
		int32_t ret = CheckIndex(pIndex);
		if (0 > ret)
		{
			return enmInvalidObjectID;
		}
		return ((uint8_t*)pIndex - (uint8_t*)m_arrIndex) / sizeof(CIndex);
	}

protected:

	int32_t BuildIndexList()
	{
		//���õ�0������
		m_arrIndex[0].SetContainer(this);
		m_arrIndex[0].SetIndex(0);
		m_arrIndex[0].SetPrev(enmInvalidObjectID);

		for (uint32_t i = 1; i < CAPACITY; ++i)
		{
			m_arrIndex[i].SetContainer(this);
			m_arrIndex[i].SetIndex(i);
			m_arrIndex[i].SetPrev(i - 1);
			m_arrIndex[i - 1].SetNext(i);
		}

		//�������һ������
		m_arrIndex[CAPACITY - 1].SetNext(enmInvalidObjectID);

		return S_OK;
	}

	CIndex* CreateIndex()
	{
		//���������
		if (IsFull())
		{
			return NULL;
		}

		//�ӿ�������ͷ��ȡ��һ��������
		if (enmInvalidObjectID == m_nFreeHead)
		{
			return NULL;
		}
		CIndex* pIndex = &m_arrIndex[m_nFreeHead];

		//����������������ͷ����β��ָ��
		if (m_nFreeTail == pIndex->Index())
		{
			m_nFreeTail = pIndex->GetNext();
		}
		m_nFreeHead = pIndex->GetNext();
		if (enmInvalidObjectID != m_nFreeHead)
		{
			m_arrIndex[m_nFreeHead].SetPrev(enmInvalidObjectID);
		}

		//����������ӵ���ʹ����������β��
		pIndex->SetPrev(m_nUsedTail);
		pIndex->SetNext(enmInvalidObjectID);
		if (enmInvalidObjectID != m_nUsedTail)
		{
			m_arrIndex[m_nUsedTail].SetNext(pIndex->Index());
		}
		m_nUsedTail = pIndex->Index();
		if (enmInvalidObjectID == m_nUsedHead)
		{
			m_nUsedHead = m_nUsedTail;
		}

		pIndex->SetUsed();

		++m_nObjectCount;
		--m_nFreeCount;

		pIndex->m_object.Initialize();

		return pIndex;
	}

	int32_t DestroyIndex(CIndex* pIndex)
	{
		int32_t ret = CheckIndex(pIndex);
		if (ret < 0)
		{
			return ret;
		}

		//������ʹ�����������ͷ����β��ָ��
		if (m_nUsedHead == pIndex->Index())
		{
			m_nUsedHead = pIndex->GetNext();
		}
		if (m_nUsedTail == pIndex->Index())
		{
			m_nUsedTail = pIndex->GetPrev();
		}

		//���ڵ��������ɾ��
		if (enmInvalidObjectID != pIndex->GetPrev())
		{
			pIndex->PrevNode()->SetNext(pIndex->GetNext());
		}
		if (enmInvalidObjectID != pIndex->GetNext())
		{
			pIndex->NextNode()->SetPrev(pIndex->GetPrev());
		}

		//��������ӵ�������������β��
		pIndex->SetPrev(m_nFreeTail);
		pIndex->SetNext(enmInvalidObjectID);
		if (enmInvalidObjectID != m_nFreeTail)
		{
			m_arrIndex[m_nFreeTail].SetNext(pIndex->Index());
		}
		m_nFreeTail = pIndex->Index();
		if (enmInvalidObjectID == m_nFreeHead)
		{
			m_nFreeHead = m_nFreeTail;
		}

		pIndex->SetFree();

		--m_nObjectCount;
		++m_nFreeCount;

		pIndex->m_object.Uninitialize();

		return S_OK;
	}

	inline int32_t CheckIndex(CIndex* pIndex)
	{
		if ((pIndex < m_arrIndex) || (pIndex >= m_arrIndex + CAPACITY))
		{
			return E_INVALIDOBJECTINDEX;
		}
		if (((uint8_t*)pIndex - (uint8_t*)m_arrIndex) % sizeof(CIndex) != 0)
		{
			return E_INVALIDOBJECTINDEX;
		}
		if (!pIndex->IsUsed())
		{
			return E_OBJECTINDEXNOTUSED;
		}
		return S_OK;
	}

	inline int32_t CheckIndex(int32_t nIndex)
	{
		if ((0 > nIndex) || ((int32_t)CAPACITY < nIndex))
		{
			return E_INVALIDOBJECTINDEX;
		}
		if (!m_arrIndex[nIndex].IsUsed())
		{
			return E_OBJECTINDEXNOTUSED;
		}
		return S_OK;
	}

	inline CIndex* Root()
	{
		if (enmInvalidObjectID == m_nRoot)
		{
			return NULL;
		}
		return &m_arrIndex[m_nRoot];
	}

protected:

	/*-----------------------------------------------------------
	|   node           right
	|   / \    ==>     / \
	|   a  right     node  y
	|       / \           / \
	|       b  y         a   b
	-----------------------------------------------------------*/
	void RotateLeft(CIndex* pNode)
	{
		CIndex* pRight = pNode->Right();

		pNode->SetRight(pRight->GetLeft());
		if (NULL != pNode->Right())
		{
			pRight->Left()->SetParent(pNode->Index());
		}
		pRight->SetLeft(pNode->Index());

		pRight->SetParent(pNode->GetParent());
		if (NULL != pRight->Parent())
		{
			if (pNode == pNode->Parent()->Right())
			{
				pNode->Parent()->SetRight(pRight->Index());
			}
			else
			{
				pNode->Parent()->SetLeft(pRight->Index());
			}
		}
		else
		{
			m_nRoot = pRight->Index();
		}
		pNode->SetParent(pRight->Index());
	}
	/*-----------------------------------------------------------
	|       node           left
	|       / \            / \
	|    left  y   ==>    a   node
	|   / \               / \
	|  a   b             b   y
	-----------------------------------------------------------*/
	void RotateRight(CIndex* pNode)
	{
		CIndex* pLeft = pNode->Left();

		pNode->SetLeft(pLeft->GetRight());
		if (NULL != pNode->Left())
		{
			pLeft->Right()->SetParent(pNode->Index());
		}
		pLeft->SetRight(pNode->Index());

		pLeft->SetParent(pNode->GetParent());
		if (NULL != pLeft->Parent())
		{
			if (pNode == pNode->Parent()->Right())
			{
				pNode->Parent()->SetRight(pLeft->Index());
			}
			else
			{
				pNode->Parent()->SetLeft(pLeft->Index());
			}
		}
		else
		{
			m_nRoot = pLeft->Index();
		}
		pNode->SetParent(pLeft->Index());
	}

	void InsertRebalance(CIndex* pNode)
	{
		CIndex* pParent = NULL;
		CIndex* pGParent = NULL;
		CIndex* pUncle = NULL;
		CIndex* p = NULL;

		pParent = pNode->Parent();
		while ((NULL != pParent) && (enmNodeColor_Red == pParent->GetColor()))
		{
			pGParent = pParent->Parent();

			if (pParent == pGParent->Left())
			{
				pUncle = pGParent->Right();
				if ((NULL != pUncle) && (enmNodeColor_Red == pUncle->GetColor()))
				{
					pUncle->SetColor(enmNodeColor_Black);
					pParent->SetColor(enmNodeColor_Black);
					pGParent->SetColor(enmNodeColor_Red);
					pNode = pGParent;
				}
				else
				{
					if (pParent->Right() == pNode)
					{
						RotateLeft(pParent);
						p = pParent;
						pParent = pNode;
						pNode = p;
					}

					pParent->SetColor(enmNodeColor_Black);
					pGParent->SetColor(enmNodeColor_Red);
					RotateRight(pGParent);
				}
			}
			else
			{
				pUncle = pGParent->Left();
				if ((NULL != pUncle) && (enmNodeColor_Red == pUncle->GetColor()))
				{
					pUncle->SetColor(enmNodeColor_Black);
					pParent->SetColor(enmNodeColor_Black);
					pGParent->SetColor(enmNodeColor_Red);
					pNode = pGParent;
				}
				else
				{
					if (pParent->Left() == pNode)
					{
						RotateRight(pParent);
						p = pParent;
						pParent = pNode;
						pNode = p;
					}

					pParent->SetColor(enmNodeColor_Black);
					pGParent->SetColor(enmNodeColor_Red);
					RotateLeft(pGParent);
				}
			}

			pParent = pNode->Parent();
		}

		Root()->SetColor(enmNodeColor_Black);
	}

	void EraseRebalance(CIndex* pNode, CIndex* pParent)
	{
		CIndex* pOther = NULL;
		CIndex* pOtherLeft = NULL;
		CIndex* pOtherRight = NULL;

		while (((NULL == pNode) || (enmNodeColor_Black == pNode->GetColor())) && (pNode != Root()))
		{
			if (pParent->Left() == pNode)
			{
				pOther = pParent->Right();
				if (enmNodeColor_Red == pOther->GetColor())
				{
					pOther->SetColor(enmNodeColor_Black);
					pParent->SetColor(enmNodeColor_Red);
					RotateLeft(pParent);
					pOther = pParent->Right();
				}
				if (((NULL == pOther->Left()) || (enmNodeColor_Black == pOther->Left()->GetColor())) &&
					((NULL == pOther->Right()) || (enmNodeColor_Black == pOther->Right()->GetColor())))
				{
					pOther->SetColor(enmNodeColor_Red);
					pNode = pParent;
					pParent = pNode->Parent();
				}
				else
				{
					if ((NULL == pOther->Right()) || (enmNodeColor_Black == pOther->Right()->GetColor()))
					{
						pOtherLeft = pOther->Left();
						if (NULL != pOtherLeft)
						{
							pOtherLeft->SetColor(enmNodeColor_Black);
						}
						pOther->SetColor(enmNodeColor_Red);
						RotateRight(pOther);
						pOther = pParent->Right();
					}
					pOther->SetColor(pParent->GetColor());
					pParent->SetColor(enmNodeColor_Black);
					if (NULL != pOther->Right())
					{
						pOther->Right()->SetColor(enmNodeColor_Black);
					}
					RotateLeft(pParent);
					pNode = Root();
					break;
				}
			}
			else
			{
				pOther = pParent->Left();
				if (enmNodeColor_Red == pOther->GetColor())
				{
					pOther->SetColor(enmNodeColor_Black);
					pParent->SetColor(enmNodeColor_Red);
					RotateRight(pParent);
					pOther = pParent->Left();
				}
				if (((NULL == pOther->Left()) || (enmNodeColor_Black == pOther->Left()->GetColor())) &&
					((NULL == pOther->Right()) || (enmNodeColor_Black == pOther->Right()->GetColor())))
				{
					pOther->SetColor(enmNodeColor_Red);
					pNode = pParent;
					pParent = pNode->Parent();
				}
				else
				{
					if ((NULL == pOther->Left()) || (enmNodeColor_Black == pOther->Left()->GetColor()))
					{
						pOtherRight = pOther->Right();
						if (NULL != pOtherRight)
						{
							pOtherRight->SetColor(enmNodeColor_Black);
						}
						pOther->SetColor(enmNodeColor_Red);
						RotateLeft(pOther);
						pOther = pParent->Left();
					}
					pOther->SetColor(pParent->GetColor());
					pParent->SetColor(enmNodeColor_Black);
					if (NULL != pOther->Left())
					{
						pOther->Left()->SetColor(enmNodeColor_Black);
					}
					RotateRight(pParent);
					pNode = Root();
					break;
				}
			}
		}

		if (NULL != pNode)
		{
			pNode->SetColor(enmNodeColor_Black);
		}
	}

	CIndex* FindNode(TKEY key)
	{
		CIndex* pNode = Root();
		CIndex* pParent = NULL;

		while (NULL != pNode)
		{
			pParent = pNode;
			if (pNode->GetKey() > key)
			{
				pNode = pNode->Left();
			}
			else if (pNode->GetKey() < key)
			{
				pNode = pNode->Right();
			}
			else
			{
				return pNode;
			}
		}

		return NULL;
	}

	CIndex* FindInsertPostion(TKEY key)
	{
		CIndex* pNode = Root();
		CIndex* pParent = NULL;

		while (NULL != pNode)
		{
			pParent = pNode;
			if (pNode->GetKey() > key)
			{
				pNode = pNode->Left();
			}
			else
			{
				pNode = pNode->Right();
			}
		}

		return pParent;
	}


protected:
	uint32_t		m_nObjectCount;
	uint32_t		m_nFreeCount;
	int32_t			m_nFreeHead;
	int32_t			m_nFreeTail;
	int32_t			m_nUsedHead;
	int32_t			m_nUsedTail;

	int32_t			m_nRoot;

	CIndex			m_arrIndex[CAPACITY];
};

#endif /* COMMON_RBTREE_H_ */
