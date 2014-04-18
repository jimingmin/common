/*
 * common_object.h
 *
 *  Created on: 2013年12月14日
 *      Author: jimm
 */

#ifndef COMMON_OBJECT_H_
#define COMMON_OBJECT_H_

#include "common_typedef.h"
#include "common_export.h"

class EXPORT CObject
{
public:
	CObject();
	virtual ~CObject();
public:
	virtual int32_t Init();
	virtual int32_t Uninit();
	virtual int32_t GetSize();
};


#define ENTITYTYPE_BASE			= 0,		//基础实体类型
#define ENTITYTYPE_OBJECT		= 1,		//对象实体类型

#endif /* COMMON_OBJECT_H_ */
