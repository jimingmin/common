/*
 * common_api.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_API_H_
#define COMMON_API_H_

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include "common_typedef.h"
#include "common_export.h"

enum
{
	enmMaxProbabilityCount		= 1024,
};

//最大的时间格式化字符串长度
#define MAX_FORMAT_TIMESTRING_LEN		128

#ifdef WIN32
//#include <WinSock2.h>
//#include <Windows.h>
#define snprintf _snprintf
//int snprintf(char *buffer, int count, const char *format, ...);	//the type of count according to MSDN _snprintf is size_t(unsignde), while "man snprintf" gives int. We keep the func consistent with the behavior on Linux
#else
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT int ErrorNo();

	//打印common库版本号
	EXPORT void print_version_common_lib();


	//CPU休眠函数
	EXPORT void Delay(uint32_t usec);

	//字符串拷贝函数
	EXPORT void StringCopy(char* szDest, const char* szSource, uint32_t nLength);

	//剔除字符串包含的空字符
	EXPORT void StringTrim(char* szString);
	//将字符串转换为小写
	EXPORT void StringToLower(char* szString);

	//
	EXPORT int64_t atoi64(const char* num);

	//获取一个随机数
	EXPORT int32_t Random(int32_t nRange);
	//获取一个大随机数
	EXPORT int32_t Random2(int32_t nRange);
	//通过概率随机
	EXPORT int32_t RandomByProbability(uint32_t nCount, int32_t arrID[], int32_t arrProbability[], int32_t nRange);

	//哈希函数(ELF)
	EXPORT uint32_t ELFHash(const char *str);

	//是否未成年人
	EXPORT bool	IsPupil(const char* szIDCard);

	EXPORT int32_t HexToInt32(const char* hex_num);

	EXPORT int32_t UnicodeToUTF8(int32_t unicode_count, const uint16_t unicode_array[],
		uint32_t& utf8_count, uint8_t utf8_array[], uint32_t array_size);

	EXPORT int32_t UTF8ToUnicode( int32_t utf8_count, const uint8_t utf8_array[],
		uint32_t& unicode_count, uint16_t unicode_array[], uint32_t array_size);

	EXPORT int32_t GetUnicodeLengthFromUTF8Buffer( int32_t utf8_count,
									   const uint8_t utf8_array[]);

	EXPORT int32_t base64_encode(char* dest, const uint8_t* src, const uint32_t dest_size, const uint32_t srclen);
	EXPORT int32_t base64_decode(uint8_t* dest, const char* src, const uint32_t dest_size, uint32_t& destlen);

	/**
	给定一个Unicode字符串，返回该字符串对应的ANSI字符串的字节数
	*/
	EXPORT int32_t AnsiCodeLengthFromUnicodeBuffer(uint16_t* buf, int32_t len);
	/**
	给定一个UTF8字符串，返回该字符串对应的ANSI字符串的字节数
	*/
	EXPORT int32_t AnsiCodeLengthFromUTF8Buffer(uint8_t* buf, int32_t len);

	//字符串转换成端口号
	EXPORT void StringToPorts(const char* szPorts, char separator, uint16_t& nPortCount, uint16_t arrPorts[]);

	EXPORT int CommonGetLastError();

	EXPORT uint8_t GetBitNum(uint8_t u);
	EXPORT int32_t GetDateBySecs(int32_t secs);

	//nDate:20110506
	EXPORT int32_t GetSecsByDate(int32_t nDate);

	//取指定日期的下一天
	EXPORT int32_t GetNextDate(int32_t nDate);

	//获取今天过了多少秒了
	EXPORT int32_t GetSecsFromMiddleNight(int32_t secs);

	EXPORT void RandomSerial(const int32_t nCount, const int32_t arrIn[], int32_t arrOut[]);

	//是否同一天
	EXPORT bool IsSameDay(const int32_t nTime1, const int32_t nTime2);

	EXPORT int32_t GetMiddleNightTime(const int32_t now);

	//安全的字符串拷贝 (强制'\0'结尾)
	EXPORT char *stringcopy(char *dest, const char *src, size_t n);

	EXPORT void WriteBill(const char* szFileName, const char* szFormat, ...);

	EXPORT ThreadID gettid();

	//获取当前时间的格式化字符串
	EXPORT int32_t GetTimeString(char arrTimeString[]);

#if defined(DEBUG) || defined(_DEBUG) || defined(_DEBUG_)
#define 	DEBUG_CODE(debug_code)	{debug_code}
#define		ASSERT(x)		assert(x)
#else
#define		DEBUG_CODE(debug_code)
#define		ASSERT(x)
#endif

#ifdef __cplusplus
}
#endif


#endif /* COMMON_API_H_ */
