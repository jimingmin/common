/*
 * common_msgmap.h
 *
 *  Created on: 2013年12月20日
 *      Author: jimm
 */

#ifndef COMMON_MSGMAP_H_
#define COMMON_MSGMAP_H_

#include "common_object.h"
#include "common_typedef.h"
#include <map>
using namespace std;

class IMsg : public CObject
{
public:
	virtual ~IMsg(){};

	virtual int32_t Encode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset) = 0;

	virtual int32_t Decode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset) = 0;
};

typedef IMsg	IMsgHead;
typedef IMsg	IMsgBody;

typedef int32_t (CObject::*MsgHandleProc)(CObject *, IMsgHead *, IMsgBody *);

class MsgEntry
{
public:
	MsgEntry(CObject *pObj, IMsgBody *pMsgBody, MsgHandleProc Proc)
	{
		m_pObject = pObj;
		m_pMsgBody = pMsgBody;
		m_pMsgHandleProc = Proc;
	}
	CObject			*m_pObject;
	IMsgBody		*m_pMsgBody;
	MsgHandleProc	m_pMsgHandleProc;
};

class CMsgMapDecl
{
public:
	CMsgMapDecl()
	{

	}
	~CMsgMapDecl(){}

	MsgEntry *GetMsgEntry(uint32_t nMsgID)
	{
		map<uint32_t, MsgEntry *>::iterator it = m_stMsgMap.find(nMsgID);
		if(it != m_stMsgMap.end())
		{
			return it->second;
		}

		return 0;
	}

	void RegistMsgEntry(uint32_t nMsgID, CObject *pObj, IMsgBody *pMsgBody, MsgHandleProc Proc)
	{
		MsgEntry *pMsgEntry = new MsgEntry(pObj, pMsgBody, Proc);
		m_stMsgMap[nMsgID] = pMsgEntry;
	}

	void Clear()
	{
		m_stMsgMap.clear();
	}
protected:
	map<uint32_t, MsgEntry *>		m_stMsgMap;
};

#define g_MsgMapDecl				CSingleton<CMsgMapDecl>::GetInstance()

#define MSGMAP_BEGIN(entity)	\
class CMsgMapDecl_##entity	\
{	\
public:	\
	CMsgMapDecl_##entity()	\
	{	\
		DeclMsgMap();	\
	}	\
	~CMsgMapDecl_##entity()	\
	{	\
	}	\
	void DeclMsgMap()	\
	{

#define ON_MSGEVENT(id, obj, msg, msgproc)	\
		g_MsgMapDecl.RegistMsgEntry(id, obj, msg, msgproc);

#define MSGMAP_END(entity)	\
	}	\
};	\
static CMsgMapDecl_##entity l_##entity;

#endif /* COMMON_MSGMAP_H_ */
