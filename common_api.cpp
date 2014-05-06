/*
 * common_api.cpp
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"Ws2_32")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/time.h>
#endif

#include <string.h>

#include "common_api.h"
#include "common_datetime.h"
#include <time.h>

#define SERVER_NAME		"Common Library"

int ErrorNo()
{
#ifdef WIN32
	return GetLastError();
#else
	return errno;
#endif
}

//CPU休眠函数
void Delay(uint32_t usec)
{
//#ifdef win32
//	socket soc;
//	soc = socket(af_inet, sock_dgram, 0);
//	fd_set(soc, &fds);
//#endif
#ifdef WIN32
	Sleep(usec / 1000);
#else
	fd_set fds;
	FD_ZERO(&fds);
	struct timeval timeout;
	timeout.tv_sec = usec / 1000000;
	timeout.tv_usec = usec % 1000000;
	int ret = select(0, NULL, NULL, &fds, &timeout);
	if(0 > ret)
	{
		perror("select");
	}
#endif
//#ifdef WIN32
//	closesocket(soc);
//#endif
}

//字符串拷贝函数
void StringCopy(char* szDest, const char* szSource, uint32_t nLength)
{
	strncpy(szDest, szSource, nLength - 1);
	szDest[nLength - 1] = '\0';
}

//剔除字符串包含的空字符
void StringTrim(char *szString)
{
	if (NULL == szString)
	{
		return;
	}

	char szDest[1024 * 1024] = { 0 };
	int8_t c = 0;
	uint32_t len = (uint32_t)strlen(szString);

	uint32_t i = 0;
	uint32_t j = 0;

	for (i = 0; i < len; ++i)
	{
		c = szString[i];

		//先处理全角转半角
		if (((uint8_t)c == 0xe3) && (i + 2 < len) && ((uint8_t)szString[i + 1] == 0x80) &&  ((uint8_t)szString[i + 2] == 0x80))	//处理全角的空格符 0xe3 0x80 0x80
		{
			//连续跳过两个字符
			i += 2;
			continue;
		}
		else if (((uint8_t)c == 0xef) && (i + 2 < len))	//处理其它全角字符
		{
			uint16_t v = (uint8_t)szString[i + 1];
			v = (v << 8) + (uint8_t)szString[i + 2];

			//全角字符对应的范围
			if (v >= 0xbc81 && v <= 0xbd9e)
			{
				//变换为半角字符
				c = 0x20 + ((v & 0x0100) >> 2) + (v & 0x3f);
				i += 2;
			}
		}

		//过滤空字符
		if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
		{
			continue;
		}

		szDest[j++] = c;
	}

	strcpy(szString, szDest);

	return;
}

//将字符串转换为小写
void StringToLower(char* szString)
{
	if (NULL == szString)
	{
		return;
	}

	char* p = szString;
	while ('\0' != *p)
	{
		if (('A' <= *p) && ('Z' >= *p))
		{
			*p++ += 0x20;
		}
		else
		{
			++p;
		}
	}
}

int64_t	atoi64(const char* num)
{

	int64_t I = 0;
	int64_t signal = 1;
	if ('-' == *num)
	{
		signal = -1;
		++num;
	}

	while(NULL != num)
	{
		if ('0' <= *num && '9' >= *num)
		{
			I *= 10;
			I += (*num - '0');
			++num;
		}
		else
		{
			break;
		}
	}
	return I * signal;

}

//获取一个随机数
int32_t Random(int32_t nRange)
{
	if (0 == nRange)
	{
		return 0;
	}
	return ((int32_t)(((double)rand()) / RAND_MAX * nRange)) % nRange;
}

//获取一个随机数
int32_t Random2(int32_t nRange)
{
	if (0 == nRange)
	{
		return 0;
	}

	int32_t rand1 = Random(256);
	int32_t rand2 = Random(256);
	int32_t rand3 = Random(256);
	int32_t rand4 = Random(128);
	int32_t number = (rand4 << 24) | (rand3 << 16) | (rand2 << 8) | rand1;

	return ((int32_t)(((double)number) / 0x7FFFFFFF * nRange)) % nRange;
}

//通过概率随机
int32_t RandomByProbability(uint32_t nCount, int32_t arrID[], int32_t arrProbability[], int32_t nRange)
{
	if (0 == nCount)
	{
		return -1;
	}
	if (enmMaxProbabilityCount < nCount)
	{
		nCount = enmMaxProbabilityCount;
	}
	int32_t value = Random2(nRange);
	int32_t arrProbabilityEnd[enmMaxProbabilityCount] = { 0 };
	for (uint32_t i = 0; i < nCount; ++i)
	{
		arrProbabilityEnd[i] = (0 == i) ? arrProbability[i] : (arrProbabilityEnd[i - 1] + arrProbability[i]);
		if (value < arrProbabilityEnd[i])
		{
			return i;
		}
	}
	return 0;
}

uint32_t ELFHash(const char *str)
{
	uint32_t hash = 0 ;
	uint32_t x = 0 ;

	char *p = (char*)str;
	while  (*p)
	{
		hash  =  (hash << 4 )  +  ( *p++ );
		if  ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24 );
			hash &= ~ x;
		}
	}

	return  (hash & 0x7FFFFFFF);
}


int32_t HexToInt32(const char* hex_num)
{
	int32_t res = 0;

	size_t hex_num_len = strlen(hex_num);
	if (hex_num_len > 10 || hex_num_len < 3)
	{
		return -1;
	}

	if ('0' != hex_num[0])
	{
		return -1;
	}

	if ('x' != hex_num[1])
	{
		return -1;
	}

	size_t real_begin_index = 2;
	for (; real_begin_index < hex_num_len; ++real_begin_index)
	{
		if ('0' != hex_num[real_begin_index])
		{
			break;
		}
	}

	if (real_begin_index == hex_num_len)
	{
		return 0;
	}

	int32_t b_ = 1;
	for (size_t index = hex_num_len - 1; index >= real_begin_index; --index)
	{
		if (hex_num[index] >= '0' && hex_num[index] <= '9')
		{
			res += (hex_num[index] - '0') * b_;
		}
		else if(hex_num[index] >= 'A' && hex_num[index] <= 'F')
		{
			res += (hex_num[index] - 'A' + 10) * b_;
		}
		else if (hex_num[index] >= 'a' && hex_num[index] <= 'f')
		{
			res += (hex_num[index] - 'a' + 10) * b_;
		}
		else
		{
			return res;
		}
		b_ *= 16;
	}

	return res;
}

/**
注意：这两个函数不支持UCS-4
*/

int32_t UnicodeToUTF8(int32_t unicode_count, const uint16_t unicode_array[],
					  uint32_t& utf8_count, uint8_t utf8_array[], uint32_t array_size)
{
	utf8_count = 0;
	for (int32_t i = 0; i < unicode_count; ++i)
	{
		uint16_t temp_unicode = unicode_array[i];

		if (temp_unicode >= 0x0800)
		{
			if ((uint32_t)utf8_count >= array_size - 3)
				return -1;
			utf8_array[utf8_count + 0] = 0xE0 | (temp_unicode >> (12));
			utf8_array[utf8_count + 1] = 0x80 | ((temp_unicode & 0x0fff) >> 6);
			utf8_array[utf8_count + 2] = 0x80 | ((temp_unicode & 0x3f));
			utf8_count += 3;
		}
		else if(temp_unicode <=0x7FF && temp_unicode >= 0x0080)
		{
			if ((uint32_t)utf8_count >= array_size - 2)
				return -1;
			utf8_array[utf8_count + 1] = 0x80 | (temp_unicode & 0x3f);
			utf8_array[utf8_count + 0] = 0xC0 | (temp_unicode >> 6);

			utf8_count += 2;
		}
		else if(temp_unicode <=0x7F)
		{
			if ((uint32_t)utf8_count >= array_size - 1)
				return -1;
			utf8_array[utf8_count + 0] = (uint8_t)temp_unicode;
			utf8_count += 1;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}


int32_t UTF8ToUnicode( int32_t utf8_count, const uint8_t utf8_array[],
					 uint32_t& unicode_count, uint16_t unicode_array[], uint32_t array_size)
{
	unicode_count = 0;
	for (int32_t i = 0; i < utf8_count;)
	{
		if ((uint32_t)unicode_count > array_size)
		{
			return -1;
		}

		unicode_array[unicode_count] = 0;
		if((utf8_array[i] & 0xf0) == 0xe0) /**1110xxxx 10xxxxxx 10xxxxxx*/
		{
			if(i + 3 > utf8_count)
				return -1;
			unicode_array[unicode_count] |= (utf8_array[i + 2] & 0x3f);
			unicode_array[unicode_count] |= (utf8_array[i + 1] & 0x3f) << 6;
			unicode_array[unicode_count] |= (0x0f & utf8_array[i]) << 12;
			i += 3;
		}
		else if((utf8_array[i] & 0xe0) == 0xc0)/**110xxxxx 10xxxxxx*/
		{
			if(i + 2 > utf8_count)
				return -1;
			unicode_array[unicode_count] |= (utf8_array[i + 1] & 0x3f);
			unicode_array[unicode_count] |= (0x1f & utf8_array[i]) << 6;
			i += 2;
		}
		else if (utf8_array[i] <= 0x7f)
		{
			unicode_array[unicode_count] = utf8_array[i];
			i += 1;
		}
		else
		{
			return -1;
		}
		unicode_count += 1;
	}
	return 0;
}

/**
得到给定的UTF-8字符串对应的Unicode字符串的长度
*/
int32_t GetUnicodeLengthFromUTF8Buffer( int32_t utf8_count, const uint8_t utf8_array[])
{
	int32_t unicode_count = 0;
	for (int32_t i = 0; i < utf8_count;)
	{
		if((utf8_array[i] & 0xf0) == 0xe0) /**1110xxxx 10xxxxxx 10xxxxxx*/
		{
			if(i + 3 > utf8_count)
				return -1;
			i += 3;
		}
		else if((utf8_array[i] & 0xe0) == 0xc0)/**110xxxxx 10xxxxxx*/
		{
			if(i + 2 > utf8_count)
				return -1;
			i += 2;
		}
		else if (utf8_array[i] <= 0x7f)
		{
			i += 1;
		}
		else
		{
			return -1;
		}
		unicode_count += 1;
	}
	return unicode_count;
}

/**
about the base64
*/
static uint8_t ms_Encoding[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int32_t base64_encode(char* dest, const uint8_t* src, const uint32_t dest_size, const uint32_t srclen)
{
	int32_t len = 0;
	const uint8_t* pt = src;
	int32_t count = srclen;

	//判断目标缓冲区的大小是否足够
	if ((srclen + 3) / 3 * 4 >= dest_size)
	{
		return 1;
	}

	while (0 < count)
	{
		*dest++ = ms_Encoding[ ( pt[0] >> 2 ) & 0x3f];
		if( count > 2 )
		{
			*dest++ = ms_Encoding[((pt[0] & 0x03) << 4) | (pt[1] >> 4)];
			*dest++ = ms_Encoding[((pt[1] & 0xF) << 2) | (pt[2] >> 6)];
			*dest++ = ms_Encoding[ pt[2] & 0x3F];
		}
		else
		{
			switch( count )
			{
			case 1:
				*dest++ = ms_Encoding[(pt[0] & 0x03) << 4];
				*dest++ = '=';
				*dest++ = '=';
				break;
			case 2:
				*dest++ = ms_Encoding[((pt[0] & 0x03) << 4) | (pt[1] >> 4)];
				*dest++ = ms_Encoding[((pt[1] & 0x0F) << 2)];
				*dest++ = '=';
			}
		}
		pt += 3;
		count -= 3;
		len += 4;
	}
	*dest = 0;

	return 0;
}

int32_t base64_decode(uint8_t* dest, const char* src, const uint32_t dest_size, uint32_t& destlen)
{
	uint8_t	ucs[4];
	const char* pt = src;
	int32_t i = 0;
	int32_t flag = 0;

	uint32_t count = (uint32_t)strlen(src);
	if(0 != (count % 4))
	{
		return 1;
	}

	if (count / 4 * 3 > dest_size)
	{
		return 1;
	}

	while( 0  < count)
	{
		flag = 0;
		for( i = 0; i < 4 ; ++i )
		{
			if (*pt >= 'A' && *pt <= 'Z')
			{
				ucs[i] = *pt - 'A';
			}
			else if (*pt >= 'a' && *pt <= 'z')
			{
				ucs[i] = *pt - 'a' + 26;
			}
			else if (*pt >= '0' && *pt <= '9')
			{
				ucs[i] = *pt - '0' + 52;
			}
			else
			{
				switch (*pt)
				{
				case '+':
					ucs[i] = 62;
					break;
				case '/':
					ucs[i] = 63;
					break;
				case '=': /* base64 padding */
					++flag;
					ucs[i] = 0;
					break;
				case '\0': /*遇到文尾,直接返回上次字符 buf 的长度*/
					*dest = 0;
					return destlen;
					break;
				default:  /* 遇到其它非法字符,直接返回 */
					*dest = 0;
					return -1;
				}
			}
			pt++;
		}
		*dest++ = (ucs[0] << 2) | (ucs[1] >> 4);
		*dest++ = (ucs[1] << 4) | (ucs[2] >> 2);
		*dest++ = (ucs[2] << 6) | (ucs[3]);
		count -= 4;
		destlen += (3 - flag);
	}
	*dest = 0;

	return 0;
}

int32_t AnsiCodeLengthFromUnicodeBuffer(uint16_t* buf, int32_t len)
{
	int32_t count = 0;
	for (int32_t i = 0; i < len; ++i)
	{
		if (buf[i] < 0x7f)
		{
			count += 1;
		}
		else
		{
			count += 2;
		}
	}
	return count;
}
int32_t AnsiCodeLengthFromUTF8Buffer(uint8_t* buf, int32_t len)
{
	int32_t count = 0;
	for (int32_t i = 0; i < len;)
	{
		if (buf[i] < 0x7f)
		{
			count += 1;
			i += 1;
		}
		else if ((buf[i] & 0xe0) == 0xc0)
		{
			count += 2;
			i += 2;
		}
		else if ((buf[i] & 0xf0) == 0xe0)
		{
			count += 2;
			i += 3;
		}
		else
		{
			return -1;
		}
	}
	return count;
}

//字符串转换成端口号
void StringToPorts(const char* szPorts, char separator, uint16_t& nPortCount, uint16_t arrPorts[])
{
	nPortCount = 0;

	char str[64];
	strcpy(str, szPorts);
	char *p = str;
	char *p2 = NULL;

	p2 = strchr(p, separator);
	while (NULL != p2)
	{
		*p2 = '\0';
		arrPorts[nPortCount++] = atoi(p);

		p = p2 + 1;
		p2 = strchr(p, separator);
	}
	arrPorts[nPortCount++] = atoi(p);
}

uint8_t GetBitNum(uint8_t u)
{
	uint8_t i = 0;
	uint8_t ret = 0;
	for(i = 0 ; i < 8; ++i)
	{
		ret += (u>>i)&1;
	}
	return ret;
}

int32_t GetDateBySecs(int32_t secs)
{
	struct tm sttmLocalTime = {0};
#ifndef WIN32
	localtime_r((time_t *)&secs, &sttmLocalTime);
#endif
	return ((int32_t)((sttmLocalTime.tm_year + 1900) * 10000 + (sttmLocalTime.tm_mon + 1) * 100 + sttmLocalTime.tm_mday) );
}

int32_t GetSecsFromMiddleNight(int32_t secs)
{
	struct tm sttmLocalTime = {0};
#ifndef WIN32
	localtime_r((time_t *)&secs, &sttmLocalTime);
#endif
	return ((int32_t)((sttmLocalTime.tm_hour * SECOND_PER_HOUR) + (sttmLocalTime.tm_min*SECOND_PER_MINUTE) + sttmLocalTime.tm_sec) );
}


void RandomSerial(const int32_t nCount, const int32_t arrIn[], int32_t arrOut[])
{
	memcpy(arrOut, arrIn, sizeof(int32_t) * nCount);
	srand((uint32_t)time(NULL));
	for (int32_t i = 0; i < nCount; i++)
	{
		const int32_t nRandomIndex = Random(nCount);
		const int32_t nTmpValue = arrOut[nRandomIndex];
		arrOut[nRandomIndex] = arrOut[i];
		arrOut[i] = nTmpValue;
	}
}

//是否同一天
bool IsSameDay(const int32_t nTime1, const int32_t nTime2)
{
	tm tm1 = { 0 };
	tm tm2 = { 0 };
#ifndef WIN32
	time_t time1 = (time_t)nTime1;
	time_t time2 = (time_t)nTime2;
	localtime_r((time_t*)&time1, &tm1);
	localtime_r((time_t*)&time2, &tm2);
#endif

	return (tm1.tm_yday == tm2.tm_yday) && (tm1.tm_year == tm2.tm_year);
}

int32_t GetMiddleNightTime(const int32_t now)
{
	//struct tm sttmLocalTime = *localtime(&tmCurrentTime);
	struct tm sttmLocalTime = {0};
#ifndef WIN32
	localtime_r((time_t*)&now, &sttmLocalTime);
#endif
	sttmLocalTime.tm_hour = sttmLocalTime.tm_min = sttmLocalTime.tm_sec = 0;
	time_t tmMiddleNight = mktime(&sttmLocalTime);

	return (int32_t)tmMiddleNight;
}

//nDate:20110506
int32_t GetSecsByDate(int32_t nDate)
{
	char buf[10] = {0};
#ifndef WIN32
	snprintf(buf,10, "%d", nDate);
#else
	sprintf_s(buf, 10, "%d", nDate);
#endif
	if (strlen(buf) != 8)
	{
		return 1;
	}

	struct tm st = {0};
	char tmp[5] = {0};

	tmp[0] = buf[0];
	tmp[1] = buf[1];
	tmp[2] = buf[2];
	tmp[3] = buf[3];
	tmp[4] = '\0';
	st.tm_year = atoi(tmp) - 1900;

	tmp[0] = buf[4];
	tmp[1] = buf[5];
	tmp[2] = '\0';
	st.tm_mon = atoi(tmp) - 1;

	tmp[0] = buf[6];
	tmp[1] = buf[7];
	tmp[2] = '\0';

	st.tm_mday = atoi(tmp);

	time_t t = mktime(&st);

	struct tm st2 = {0};

#ifndef WIN32
	localtime_r(&t, &st2);
#else
	localtime_s(&st2, &t);
#endif

	time_t t2 = mktime(&st2);

	return (int32_t)t2;
}

//取指定日期的下一天
int32_t GetNextDate(int32_t nDate)
{
	int32_t t = GetSecsByDate(nDate);
	if (t < 0)
	{
		return 1;
	}
	return GetDateBySecs(t + SECOND_PER_DAY);
}

//安全的字符串拷贝 (强制'\0'结尾)
char *stringcopy(char *dest, const char *src, size_t n)
{
	if (NULL == dest || NULL == src || 0 == n)
	{
		return NULL;
	}

	char *ret = snprintf(dest, n, "%s", src) > 0 ? dest : NULL;
	return ret;
}

void WriteBill(const char* szFileName, const char* szFormat, ...)
{
	if ((NULL == szFileName) || (NULL == szFormat))
	{
		return;
	}


	CDateTime dt = CDateTime::CurrentDateTime();


	FILE *pf = fopen(szFileName, "a+");
	if (NULL == pf)
	{
		return;
	}

	fprintf(pf, "%04d-%02d-%02d %02d:%02d:%02d,",
		dt.Year(), dt.Month(), dt.Day(), dt.Hour(), dt.Minute(), dt.Second());

	va_list ap;
	va_start(ap, szFormat);
	vfprintf(pf, szFormat, ap);
	va_end(ap);

	fclose(pf);
}

ThreadID gettid()
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
//	return syscall(SYS_gettid);
return 0;
#endif
}

int32_t GetTimeString(char arrTimeString[])
{
	struct timeval tv;
 	gettimeofday(&tv, NULL);
	struct tm now = *localtime((time_t *)&tv.tv_sec);

	int32_t nRetLen = sprintf(arrTimeString, "%04d/%02d/%02d %02d:%02d:%02d.%03ld",
			now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour,
			now.tm_min, now.tm_sec, tv.tv_usec / 1000);

	arrTimeString[nRetLen] = '\0';

	return nRetLen;
}



