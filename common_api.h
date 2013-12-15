/*
 * common_api.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_API_H_
#define COMMON_API_H_

#include <stdio.h>
#include "common_typedef.h"
#include <stdarg.h>
#include <sys/types.h>
#include <sys/syscall.h>

enum
{
	enmMaxProbabilityCount		= 1024,
};

//最大的时间格式化字符串长度
#define MAX_FORMAT_TIMESTRING_LEN		128

#ifdef WIN32
#define snprintf _snprintf
//int snprintf(char *buffer, int count, const char *format, ...);	//the type of count according to MSDN _snprintf is size_t(unsignde), while "man snprintf" gives int. We keep the func consistent with the behavior on Linux

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	//打印common库版本号
	void print_version_common_lib();


	//CPU休眠函数
	void Delay(uint32_t usec);

	//字符串拷贝函数
	void StringCopy(char* szDest, const char* szSource, uint32_t nLength);

	//剔除字符串包含的空字符
	void StringTrim(char* szString);
	//将字符串转换为小写
	void StringToLower(char* szString);

	//
	int64_t atoi64(const char* num);

	//获取一个随机数
	int32_t Random(int32_t nRange);
	//获取一个大随机数
	int32_t Random2(int32_t nRange);
	//通过概率随机
	int32_t RandomByProbability(uint32_t nCount, int32_t arrID[], int32_t arrProbability[], int32_t nRange);

	//哈希函数(ELF)
	uint32_t ELFHash(const char *str);

	//是否未成年人
	bool	IsPupil(const char* szIDCard);

	int32_t HexToInt32(const char* hex_num);

	int32_t UnicodeToUTF8(int32_t unicode_count, const uint16_t unicode_array[],
		uint32_t& utf8_count, uint8_t utf8_array[], uint32_t array_size);

	int32_t UTF8ToUnicode( int32_t utf8_count, const uint8_t utf8_array[],
		uint32_t& unicode_count, uint16_t unicode_array[], uint32_t array_size);

	int32_t GetUnicodeLengthFromUTF8Buffer( int32_t utf8_count,
									   const uint8_t utf8_array[]);

	int32_t base64_encode(char* dest, const uint8_t* src, const uint32_t dest_size, const uint32_t srclen);
	int32_t base64_decode(uint8_t* dest, const char* src, const uint32_t dest_size, uint32_t& destlen);

	/**
	给定一个Unicode字符串，返回该字符串对应的ANSI字符串的字节数
	*/
	int32_t AnsiCodeLengthFromUnicodeBuffer(uint16_t* buf, int32_t len);
	/**
	给定一个UTF8字符串，返回该字符串对应的ANSI字符串的字节数
	*/
	int32_t AnsiCodeLengthFromUTF8Buffer(uint8_t* buf, int32_t len);

	//字符串转换成端口号
	void StringToPorts(const char* szPorts, char separator, uint16_t& nPortCount, uint16_t arrPorts[]);

	int CommonGetLastError();

	uint8_t GetBitNum(uint8_t u);
	int32_t GetDateBySecs(int32_t secs);

	//nDate:20110506
	int32_t GetSecsByDate(int32_t nDate);

	//取指定日期的下一天
	int32_t GetNextDate(int32_t nDate);

	//获取今天过了多少秒了
	int32_t GetSecsFromMiddleNight(int32_t secs);

	void RandomSerial(const int32_t nCount, const int32_t arrIn[], int32_t arrOut[]);

	//是否同一天
	bool IsSameDay(const int32_t nTime1, const int32_t nTime2);

	int32_t GetMiddleNightTime(const int32_t now);

	//安全的字符串拷贝 (强制'\0'结尾)
	char *stringcopy(char *dest, const char *src, size_t n);

	void WriteBill(const char* szFileName, const char* szFormat, ...);

	pid_t gettid();

	//获取当前时间的格式化字符串
	int32_t GetTimeString(char arrTimeString[]);

#if defined(DEBUG) || defined(_DEBUG) || defined(_DEBUG_)
#define 	DEBUG_CODE(debug_code)	{debug_code}
#else
#define		DEBUG_CODE(debug_code)
#endif

#ifdef __cplusplus
}
#endif


#endif /* COMMON_API_H_ */
