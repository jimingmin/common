/*
 * common_typedef.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_TYPEDEF_H_
#define COMMON_TYPEDEF_H_

#ifdef __GNUC__
#include <stdint.h>
#include <inttypes.h>
#endif

#if defined(WIN32) || defined(_WIN32)
typedef char						int8_t;
typedef unsigned char				uint8_t;
typedef short						int16_t;
typedef unsigned short				uint16_t;
typedef int							int32_t;
typedef unsigned int				uint32_t;
typedef long long					int64_t;
typedef unsigned long long			uint64_t;
#elif defined(_WIN64)
typedef char						int8_t;
typedef unsigned char				uint8_t;
typedef short						int16_t;
typedef unsigned short				uint16_t;
typedef int							int32_t;
typedef unsigned int				uint32_t;
typedef long 						int64_t;
typedef unsigned long				uint64_t;
#endif

#if defined(WIN32) || defined(_WIN32)
#pragma warning(disable:4996)
#pragma warning(disable:4005)
#endif

#if defined(WIN32) || defined(_WIN32)
#define OS_WIN32 1;
#else
#define OS_LINUX 1;
#endif

/*********************************************************************
* 时间日期相关常量定义
*********************************************************************/
#define US_PER_MS				((int64_t)1000)
#define US_PER_SECOND			((int64_t)(1000 * US_PER_MS))
#define US_PER_MINUTE			((int64_t)(60 * US_PER_SECOND))
#define US_PER_HOUR				((int64_t)(60 * US_PER_MINUTE))
#define US_PER_DAY				((int64_t)(24 * US_PER_HOUR))

#define MS_PER_SECOND			1000
#define SECOND_PER_MINUTE		60
#define MINUTE_PER_HOUR			60
#define HOUR_PER_DAY			24
#define DAY_PER_YEAR			365

#define SECOND_PER_HOUR			(MINUTE_PER_HOUR * SECOND_PER_MINUTE)
#define SECOND_PER_DAY			(HOUR_PER_DAY * SECOND_PER_HOUR)
#define SECOND_PER_WEEK			(SECOND_PER_DAY * 7)
#define SECOND_PER_YEAR			(DAY_PER_YEAR * SECOND_PER_DAY)


#endif /* COMMON_TYPEDEF_H_ */
