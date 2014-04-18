/*
 * common_datetime.h
 *
 *  Created on: 2013年12月13日
 *      Author: jimm
 */


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>

#include "common_datetime.h"

#ifdef WIN32
#pragma warning(disable:4309)
#endif

#ifdef WIN32

int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year     = wtm.wYear - 1900;
	tm.tm_mon     = wtm.wMonth - 1;
	tm.tm_mday     = wtm.wDay;
	tm.tm_hour     = wtm.wHour;
	tm.tm_min     = wtm.wMinute;
	tm.tm_sec     = wtm.wSecond;
	tm. tm_isdst    = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;

	return (0);
}

struct tm* localtime_r(const time_t *timep, struct tm *result)
{
	if(localtime_s(result, timep) != 0)
	{
		return NULL;
	}
	return result;
}

char* asctime_r(const struct tm *tm, size_t buf_size, char *buf)
{
	if(asctime_s(buf, buf_size, tm) != 0)
	{
		return NULL;
	}
	return buf;
}

char* ctime_r(const time_t *timep, size_t buf_size, char *buf)
{
	if(ctime_s(buf, buf_size, timep) != 0)
	{
		return NULL;
	}
	return buf;
}
struct tm* gmtime_r(const time_t *timep, struct tm *result)
{
	return NULL;
}
#endif


CDateTimeSpan::CDateTimeSpan()
{
	m_nDateTimeSpan = 0;
}

CDateTimeSpan::CDateTimeSpan(const int64_t nDateTimeSpan)
{
	m_nDateTimeSpan = nDateTimeSpan;
}

CDateTimeSpan::CDateTimeSpan(const int32_t nDays, const int32_t nHours, const int32_t nMinutes, const int32_t nSeconds)
{
	m_nDateTimeSpan = ((((int64_t)nDays * HOUR_PER_DAY + nHours) * MINUTE_PER_HOUR + nMinutes) * SECOND_PER_MINUTE + nSeconds) * US_PER_SECOND;
}

CDateTimeSpan::CDateTimeSpan(const int32_t nDays, const int32_t nHours, const int32_t nMinutes, const int32_t nSeconds, const int32_t nMilliseconds)
{
	m_nDateTimeSpan = (((((int64_t)nDays * HOUR_PER_DAY + nHours) * MINUTE_PER_HOUR + nMinutes) * SECOND_PER_MINUTE + nSeconds) * MS_PER_SECOND + nMilliseconds) * US_PER_MS;
}

CDateTimeSpan::CDateTimeSpan(const CDateTimeSpan& dts)
{
	m_nDateTimeSpan = dts.m_nDateTimeSpan;
}

CDateTimeSpan::~CDateTimeSpan()
{

}

CDateTimeSpan CDateTimeSpan::operator + (const CDateTimeSpan& dts) const
{
	CDateTimeSpan sum;
	sum.m_nDateTimeSpan = m_nDateTimeSpan + dts.m_nDateTimeSpan;
	return sum;
}

CDateTimeSpan CDateTimeSpan::operator - (const CDateTimeSpan& dts) const
{
	CDateTimeSpan diff;
	diff.m_nDateTimeSpan = m_nDateTimeSpan - dts.m_nDateTimeSpan;
	return diff;
}

bool CDateTimeSpan::operator < (const CDateTimeSpan& dts) const
{
	return (m_nDateTimeSpan < dts.m_nDateTimeSpan);
}

bool CDateTimeSpan::operator > (const CDateTimeSpan& dts) const
{
	return (m_nDateTimeSpan > dts.m_nDateTimeSpan);
}

bool CDateTimeSpan::operator <= (const CDateTimeSpan& dts) const
{
	return (m_nDateTimeSpan <= dts.m_nDateTimeSpan);
}

bool CDateTimeSpan::operator >= (const CDateTimeSpan& dts) const
{
	return (m_nDateTimeSpan >= dts.m_nDateTimeSpan);
}

double CDateTimeSpan::TotalDays() const
{
	return (double)m_nDateTimeSpan / US_PER_DAY;
}

double CDateTimeSpan::TotalHours() const
{
	return (double)m_nDateTimeSpan / US_PER_HOUR;
}

double CDateTimeSpan::TotalMinutes() const
{
	return (double)m_nDateTimeSpan / US_PER_MINUTE;
}

double CDateTimeSpan::TotalSeconds() const
{
	return (double)m_nDateTimeSpan / US_PER_SECOND;
}

double CDateTimeSpan::TotalMilliseconds() const
{
	return (double)m_nDateTimeSpan / US_PER_MS;
}

int64_t CDateTimeSpan::GetDateTimeSpan() const
{
	return m_nDateTimeSpan;
}


CDateTime CDateTime::CurrentDateTime()
{
	time_t now;
	time(&now);
	return CDateTime((int64_t)now * US_PER_SECOND);
}

CDateTime::CDateTime()
{
	m_nDateTime = 0;
}

CDateTime::CDateTime(const int64_t nDateTime)
{
	m_nDateTime = nDateTime;
}

CDateTime::CDateTime(const char * szDateTimeString)
{
	struct tm st = { 0 };
	char szDateTime[enmDateTimeStringSize] = { 0 };

	strncpy(szDateTime, szDateTimeString, sizeof(szDateTime) - 1);
	if (strlen(szDateTime) < enmDateTimeStringLength)
	{
		m_nDateTime = 0;
		return;
	}

	char* p = szDateTime;

	p[4] = '\0';
	st.tm_year = atoi(p) - 1900;
	p += 5;

	p[2] = '\0';
	st.tm_mon = atoi(p) - 1;
	p += 3;

	p[2] = '\0';
	st.tm_mday = atoi(p);
	p += 3;

	p[2] = '\0';
	st.tm_hour = atoi(p);
	p += 3;

	p[2] = '\0';
	st.tm_min = atoi(p);
	p += 3;

	p[2] = '\0';
	st.tm_sec = atoi(p);
	p += 3;	

	time_t t = mktime(&st);

	struct tm st2 = { 0 };
	localtime_r(&t, &st2);
	time_t t2 = mktime(&st2);

	m_nDateTime = (int64_t)t2 * US_PER_SECOND;
}

CDateTime::CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay)
{
	struct tm st = { 0 };

	st.tm_year = (int)nYear - 1900;
	st.tm_mon = nMonth - 1;
	st.tm_mday = nDay;

	time_t t = mktime(&st);

	struct tm st2 = { 0 };
	localtime_r(&t, &st2);
	time_t t2 = mktime(&st2);

	m_nDateTime = (int64_t)t2 * US_PER_SECOND;
}

CDateTime::CDateTime(const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond)
{
	m_nDateTime = ((((int64_t)nDay * HOUR_PER_DAY + nHour) * MINUTE_PER_HOUR + nMinute) * SECOND_PER_MINUTE + nSecond) * US_PER_SECOND;
}

CDateTime::CDateTime(const uint16_t nYear, const uint16_t nMonth, const uint16_t nDay, const uint16_t nHour, const uint16_t nMinute, const uint16_t nSecond, const uint16_t nMilliseconds)
{
	struct tm st = { 0 };

	st.tm_year = (int)nYear - 1900;
	st.tm_mon = nMonth - 1;
	st.tm_mday = nDay;
	st.tm_hour = nHour;
	st.tm_min = nMinute;
	st.tm_sec = nSecond;

	time_t t = mktime(&st);

	struct tm st2 = { 0 };
	localtime_r(&t, &st2);
	time_t t2 = mktime(&st2);

	m_nDateTime = (int64_t)t2 * US_PER_SECOND + nMilliseconds * MS_PER_SECOND;
}

CDateTime::CDateTime(const CDateTime& dt)
{
	m_nDateTime = dt.m_nDateTime;
}

CDateTime::~CDateTime()
{

}

CDateTime CDateTime::operator + (const int64_t usec) const
{
	CDateTime dt;
	dt.m_nDateTime = m_nDateTime + usec;
	return dt;
}

CDateTime CDateTime::operator + (const CDateTimeSpan& dts) const
{
	CDateTime dt;
	dt.m_nDateTime = m_nDateTime + dts.GetDateTimeSpan();
	return dt;
}

CDateTime CDateTime::operator - (const CDateTimeSpan& dts) const
{
	CDateTime dt;
	dt.m_nDateTime = m_nDateTime - dts.GetDateTimeSpan();
	return dt;
}

CDateTimeSpan CDateTime::operator - (const CDateTime& dt) const
{
	int64_t ts = m_nDateTime - dt.m_nDateTime;

	CDateTimeSpan dts(ts);
	return dts;
}

CDateTime& CDateTime::Add(const CDateTimeSpan& dts)
{
	m_nDateTime += dts.GetDateTimeSpan();
	return *this;
}

CDateTime& CDateTime::AddDays(const int16_t nDays)
{
	CDateTimeSpan dts(nDays, 0, 0, 0);
	return Add(dts);
}

CDateTime& CDateTime::AddHours(const int16_t nHours)
{
	CDateTimeSpan dts(0, nHours, 0, 0);
	return Add(dts);
}

CDateTime& CDateTime::AddMinutes(const int16_t nMinutes)
{
	CDateTimeSpan dts(0, 0, nMinutes, 0);
	return Add(dts);
}

CDateTime& CDateTime::AddSeconds(const int16_t nSeconds)
{
	CDateTimeSpan dts(0, 0, 0, nSeconds);
	return Add(dts);
}

CDateTime& CDateTime::AddMilliseconds(const int16_t nMilliseconds)
{
	CDateTimeSpan dts(0, 0, 0, 0, nMilliseconds);
	return Add(dts);
}

bool CDateTime::operator == (const CDateTime& dt) const
{
	return (m_nDateTime == dt.m_nDateTime);
}

bool CDateTime::operator != (const CDateTime& dt) const
{
	return (m_nDateTime != dt.m_nDateTime);
}

bool CDateTime::operator < (const CDateTime& dt) const
{
	return (m_nDateTime < dt.m_nDateTime);
}

bool CDateTime::operator > (const CDateTime& dt) const
{
	return (m_nDateTime > dt.m_nDateTime);
}

bool CDateTime::operator <= (const CDateTime& dt) const
{
	return (m_nDateTime <= dt.m_nDateTime);
}

bool CDateTime::operator >= (const CDateTime& dt) const
{
	return (m_nDateTime >= dt.m_nDateTime);
}

uint16_t CDateTime::Year() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_year + 1900;
}

uint16_t CDateTime::Month() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_mon + 1;
}

uint16_t CDateTime::Day() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_mday;
}

uint16_t CDateTime::DayOfWeek() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_wday;
}

uint16_t CDateTime::DayOfYear() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_yday;
}

uint16_t CDateTime::Hour() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_hour;
}

uint16_t CDateTime::Minute() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_min;
}

uint16_t CDateTime::Second() const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);

	return (uint16_t)st.tm_sec;
}

uint16_t CDateTime::Millisecond() const
{
	return (uint16_t)((m_nDateTime % US_PER_SECOND) / US_PER_MS);
}

int64_t CDateTime::Seconds() const
{
	return m_nDateTime / US_PER_SECOND;
}

int64_t CDateTime::GetValue() const
{
	return m_nDateTime;
}

int32_t CDateTime::Date() const
{
	return (Year()*10000 + Month()*100 + Day());
}

void CDateTime::ToDateTimeString(char *szDest) const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);
	sprintf(szDest, "%04d-%02d-%02d %02d:%02d:%02d", st.tm_year + 1900, st.tm_mon + 1, st.tm_mday, st.tm_hour, st.tm_min, st.tm_sec);
}

void CDateTime::ToDateString(char *szDest, int32_t nFormat/* = 0*/) const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);
	switch (nFormat)
	{
	case 1:
		{
			char strMonth[] = {0xE6, 0x9C, 0x88, 0};
			char strDay[] = {0xE6, 0x97, 0xA5, 0};
			sprintf(szDest, "%d%s%d%s", st.tm_mon + 1, strMonth, st.tm_mday, strDay);
		}
		break;
	default:
		sprintf(szDest, "%04d-%02d-%02d", st.tm_year + 1900, st.tm_mon + 1, st.tm_mday);
		break;
	}
}

void CDateTime::ToTimeString(char *szDest, int32_t nFormat/* = 0*/) const
{
	struct tm st = { 0 };
	time_t t = (time_t)(m_nDateTime / US_PER_SECOND);

	localtime_r(&t, &st);
	switch (nFormat)
	{
	case 1:
		{
			sprintf(szDest, "%02d:%02d", st.tm_hour, st.tm_min);
		}
		break;
	default:
		sprintf(szDest, "%02d:%02d:%02d", st.tm_hour, st.tm_min, st.tm_sec);
		break;
	}
}


CTimeValue CTimeValue::CurrentTime()
{
	struct timeval stTimeVal;

	gettimeofday(&stTimeVal, NULL);
	CTimeValue tv(stTimeVal);

	return tv;
}

CTimeValue::CTimeValue()
{
	m_nTimeVal = 0;
}

CTimeValue::CTimeValue(int32_t ms)
{
	m_nTimeVal = (int64_t)ms * US_PER_MS;
}

CTimeValue::CTimeValue(struct timeval stTimeVal)
{
	m_nTimeVal = (int64_t)stTimeVal.tv_sec * US_PER_SECOND + (int64_t)stTimeVal.tv_usec;
}

CTimeValue::~CTimeValue()
{

}

CTimeValue CTimeValue::operator + (const struct timeval stTimeVal) const
{
	CTimeValue tv;
	tv.m_nTimeVal = m_nTimeVal + (int64_t)stTimeVal.tv_sec * US_PER_SECOND + (int64_t)stTimeVal.tv_usec;
	return tv;
}

CTimeValue CTimeValue::operator - (const struct timeval stTimeVal) const
{
	CTimeValue tv;
	tv.m_nTimeVal = m_nTimeVal - (int64_t)stTimeVal.tv_sec * US_PER_SECOND - (int64_t)stTimeVal.tv_usec;
	return tv;
}

CTimeValue& CTimeValue::Add(const struct timeval stTimeVal)
{
	m_nTimeVal += (int64_t)stTimeVal.tv_sec * US_PER_SECOND + (int64_t)stTimeVal.tv_usec;
	return *this;
}

CTimeValue& CTimeValue::Submit(const struct timeval stTimeVal)
{
	m_nTimeVal -= (int64_t)stTimeVal.tv_sec * US_PER_SECOND + (int64_t)stTimeVal.tv_usec;
	return *this;
}

CTimeValue& CTimeValue::AddSeconds(int32_t nSeconds)
{
	m_nTimeVal += (int64_t)nSeconds * US_PER_SECOND;
	return *this;
}

CTimeValue& CTimeValue::AddMilliseconds(int32_t nMilliseconds)
{
	m_nTimeVal += (int64_t)nMilliseconds * US_PER_MS;
	return *this;
}

CTimeValue& CTimeValue::AddMicroseconds(int32_t nMicroseconds)
{
	m_nTimeVal += (int64_t)nMicroseconds;
	return *this;
}

int64_t CTimeValue::Seconds()
{
	return m_nTimeVal / US_PER_SECOND;
}

int64_t CTimeValue::Milliseconds()
{
	return m_nTimeVal / US_PER_MS;
}

int64_t CTimeValue::Microseconds()
{
	return m_nTimeVal;
}

void CTimeValue::ToTimeString(char *szDest) const
{
	int16_t hour = (int16_t)((m_nTimeVal % US_PER_DAY) / US_PER_HOUR);
	int16_t min = (int16_t)((m_nTimeVal % US_PER_HOUR) / US_PER_MINUTE);
	int16_t sec = (int16_t)((m_nTimeVal % US_PER_MINUTE) / US_PER_SECOND);
	int16_t ms = (int16_t)((m_nTimeVal % US_PER_SECOND) / US_PER_MS);

	sprintf(szDest, "%02d:%02d:%02d %03d", hour, min, sec, ms);
}

void CTimeValue::ToDayString(char *szDest) const
{
	int16_t day = (int16_t)(m_nTimeVal / US_PER_DAY);
	int16_t hour = (int16_t)((m_nTimeVal % US_PER_DAY) / US_PER_HOUR);
	int16_t min = (int16_t)((m_nTimeVal % US_PER_HOUR) / US_PER_MINUTE);
	int16_t sec = (int16_t)((m_nTimeVal % US_PER_MINUTE) / US_PER_SECOND);

	sprintf(szDest, "%04d %02d:%02d:%02d", day, hour, min, sec);
}


