/*
 * common_string.h
 *
 *  Created on: 2013年12月14日
 *      Author: jimm
 */

#ifndef COMMON_CODEENGINE_H_
#define COMMON_CODEENGINE_H_

//#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "common_typedef.h"
#include "common_export.h"
#include <string>
using namespace std;

class EXPORT CEndian
{
public:
	CEndian()
	{
		union
		{
			uint16_t	s1;
			uint8_t		c2[2];
		} un;
		un.s1 = 0x0102;
		m_bBigEndian = (un.c2[0] == 0x01);
	}

	inline bool IsBigEndian()
	{
		return m_bBigEndian;
	}

	bool			m_bBigEndian;
};


class EXPORT CCodeEngine
{
public:
	CCodeEngine();
	virtual ~CCodeEngine();

public:

	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const bool data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int8_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int16_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int32_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int64_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint8_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint16_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint32_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint64_t data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint8_t *data, const uint32_t length, const uint32_t maxLength);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const char *data, const uint16_t maxLength);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const double data);
	static int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const string &data);

public:

	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, bool& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int8_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int16_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int32_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int64_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint8_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint16_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint32_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint64_t& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint8_t* data, const uint32_t length, const uint32_t maxLength);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, char* data, const uint16_t maxLength);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, double& data);
	static int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, string &data);

protected:
	static CEndian	ms_endian;
};

extern "C"
{
EXPORT int32_t encode_bool(uint8_t* buf, const uint32_t size, uint32_t* offset, const bool data);
EXPORT int32_t encode_int8(uint8_t* buf, const uint32_t size, uint32_t* offset, const int8_t data);
EXPORT int32_t encode_int16(uint8_t* buf, const uint32_t size, uint32_t* offset, const int16_t data);
EXPORT int32_t encode_int32(uint8_t* buf, const uint32_t size, uint32_t* offset, const int32_t data);
EXPORT int32_t encode_int64(uint8_t* buf, const uint32_t size, uint32_t* offset, const int64_t data);
EXPORT int32_t encode_uint8(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint8_t data);
EXPORT int32_t encode_uint16(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint16_t data);
EXPORT int32_t encode_uint32(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint32_t data);
EXPORT int32_t encode_uint64(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint64_t data);
EXPORT int32_t encode_uint8_mem(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint8_t *data, const uint32_t length, const uint32_t maxLength);
EXPORT int32_t encode_char_mem(uint8_t* buf, const uint32_t size, uint32_t* offset, const char *data, const uint16_t maxLength);
EXPORT int32_t encode_double(uint8_t* buf, const uint32_t size, uint32_t* offset, const double data);

EXPORT int32_t decode_bool(const uint8_t *buf, const uint32_t size, uint32_t* offset, bool* data);
EXPORT int32_t decode_int8(const uint8_t *buf, const uint32_t size, uint32_t* offset, int8_t* data);
EXPORT int32_t decode_int16(const uint8_t *buf, const uint32_t size, uint32_t* offset, int16_t* data);
EXPORT int32_t decode_int32(const uint8_t *buf, const uint32_t size, uint32_t* offset, int32_t* data);
EXPORT int32_t decode_int64(const uint8_t *buf, const uint32_t size, uint32_t* offset, int64_t* data);
EXPORT int32_t decode_uint8(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint8_t* data);
EXPORT int32_t decode_uint16(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint16_t* data);
EXPORT int32_t decode_uint32(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint32_t* data);
EXPORT int32_t decode_uint64(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint64_t* data);
EXPORT int32_t decode_uint8_mem(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint8_t* data, const uint32_t length, const uint32_t maxLength);
EXPORT int32_t decode_char_mem(const uint8_t *buf, const uint32_t size, uint32_t* offset, char* data, const uint16_t maxLength);
EXPORT int32_t decode_double(const uint8_t *buf, const uint32_t size, uint32_t* offset, double* data);

}

#endif /* COMMON_CODEENGINE_H_ */
