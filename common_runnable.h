/*
 * common_runnable.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef COMMON_RUNNABLE_H_
#define COMMON_RUNNABLE_H_

#include "common_export.h"
#include "common_typedef.h"

class EXPORT IRunnable
{
public:
	IRunnable(){};
	virtual ~IRunnable(){};

	virtual int32_t Run() = 0;
};

#endif /* COMMON_RUNNABLE_H_ */
