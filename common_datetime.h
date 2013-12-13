/*
 * common_datetime.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */

#ifndef COMMON_DATETIME_H_
#define COMMON_DATETIME_H_

#include "common_typedef.h"


enum
{
	enmDateTimeStringSize				= 20,
	enmDateTimeStringLength				= 19
};


//时间间隔类
class CDateTimeSpan
{
public:
	CDateTimeSpan();
	CDateTimeSpan(const int64_t nDateTimeSpan);
	CDateTimeSpan(const int32_t nDays, const int32_t nHours, const int32_t nMinutes, const int32_t nSeconds);
	CDateTimeSpan(const int32_t nDays, const int32_t nHours, const int32_t nMinutes, const int32_t nSeconds, const int32_t nMilliseconds);
	CDateTimeSpan(const CDateTimeSpan& dts);
	virtual ~CDateTimeSpan();

public:
	CDateTimeSpan operator + (const CDateTimeSpan& dts) const;
	CDateTimeSpan operator - (const CDateTimeSpan& dts) const;

public:
	bool operator < (const CDateTimeSpan& dts) const;
	bool operator > (const CDateTimeSpan& dts) const;
	bool operator <= (const CDateTimeSpan& dts) const;
	bool operator >= (const CDateTimeSpan& dts) const;

public:
	double TotalDays() const;
	double TotalHours() const;
	double TotalMinutes() const;
	double TotalSeconds() const;
	double TotalMilliseconds() const;

	int64_t GetDateTimeSpan() const;

protected:
	int64_t			m_nDateTimeSpan;	//微秒数

};


//日期时间类
class CDateTime
{
public:
	static CDateTime CurrentDateTime();

public:
	CDateTime();
	CDateTime(const int64_t nDateTime);
	CDateTime(const char * szDateTimeString);
	CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay);
	CDateTime(const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond);
	CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond, const uint16_t nMilliseconds);
	CDateTime(const CDateTime& dt);
	virtual ~CDateTime();

public:
	CDateTime operator + (const int64_t usec) const;
	CDateTime operator + (const CDateTimeSpan& dts) const;
	CDateTime operator - (const CDateTimeSpan& dts) const;
	CDateTimeSpan operator - (const CDateTime& dt) const;

	CDateTime& Add(const CDateTimeSpan& dts);
	CDateTime& AddDays(const int16_t nDays);
	CDateTime& AddHours(const int16_t nHours);
	CDateTime& AddMinutes(const int16_t nMinutes);
	CDateTime& AddSeconds(const int16_t nSeconds);
	CDateTime& AddMilliseconds(const int16_t nMilliseconds);

	bool operator == (const CDateTime& dt) const;
	bool operator != (const CDateTime& dt) const;
	bool operator < (const CDateTime& dt) const;
	bool operator > (const CDateTime& dt) const;
	bool operator <= (const CDateTime& dt) const;
	bool operator >= (const CDateTime& dt) const;

public:
	uint16_t Year() const;
	uint16_t Month() const;
	uint16_t Day() const;
	uint16_t DayOfWeek() const;
	uint16_t DayOfYear() const;
	uint16_t Hour() const;
	uint16_t Minute() const;
	uint16_t Second() const;
	uint16_t Millisecond() const;

	int64_t Seconds() const;

	int64_t GetValue() const;

	int32_t Date() const;

public:
	void ToDateTimeString(char *szDest) const;
	void ToDateString(char *szDest, int32_t nFormat = 0) const;
	void ToTimeString(char *szDest, int32_t nFormat = 0) const;

protected:
	int64_t			m_nDateTime;	//格林威治标准时间1970-01-01到所表示时间的间隔微秒数
};

class CTimeValue
{
public:
	static CTimeValue CurrentTime();

public:
	CTimeValue();
	CTimeValue(int32_t ms);
	CTimeValue(struct timeval stTimeVal);
	virtual ~CTimeValue();

public:
	CTimeValue operator + (const struct timeval stTimeVal) const;
	CTimeValue operator - (const struct timeval stTimeVal) const;

	CTimeValue& Add(const struct timeval stTimeVal);
	CTimeValue& Submit(const struct timeval stTimeVal);
	CTimeValue& AddSeconds(int32_t nSeconds);
	CTimeValue& AddMilliseconds(int32_t nMilliseconds);
	CTimeValue& AddMicroseconds(int32_t nMicroseconds);

public:
	int64_t Seconds();
	int64_t Milliseconds();
	int64_t Microseconds();

public:
	void ToTimeString(char *szDest) const;
	void ToDayString(char *szDest) const;

	int64_t GetValue() const
	{
		return m_nTimeVal;
	}

protected:
	int64_t			m_nTimeVal;
};


#endif /* COMMON_DATETIME_H_ */
