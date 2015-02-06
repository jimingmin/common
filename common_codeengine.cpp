/*
 * common_string.h
 *
 *  Created on: 2013年12月14日
 *      Author: jimm
 */

#include "common_codeengine.h"
#include "common_errordef.h"
#include "common_memmgt.h"


#define ENCODE(buf, size, offset, data, type)\
	if ((NULL == buf) || (0 == size))\
	{\
		return E_INVALIDARGUMENT;\
	}\
	if (offset + sizeof(type) > size)\
	{\
		return E_ENCODEBUFLENGTH;\
	}\
	uint8_t *p = (uint8_t*)&data;\
	if (ms_endian.IsBigEndian())\
	{\
		for(uint32_t i = 0; i < sizeof(type); ++i)\
		{\
			buf[offset++] = p[i];\
		}\
	}\
	else\
	{\
		for(uint32_t i = 0; i < sizeof(type); ++i)\
		{\
			buf[offset++] = p[sizeof(type)-i-1];\
		}\
	}\
	return S_OK;


#define DECODE(buf, size, offset, data, type)\
	if ((NULL == buf) || (0 == size))\
	{\
		return E_INVALIDARGUMENT;\
	}\
	if (offset + sizeof(type) > size)\
	{\
		return E_DECODEBUFLENGTH;\
	}\
	uint8_t *p = (uint8_t*)&data;\
	if (ms_endian.IsBigEndian())\
	{\
		for(uint32_t i = 0; i < sizeof(type); ++i)\
		{\
			 p[i] = buf[offset++];\
		}\
	} \
	else\
	{\
		for (uint32_t i = 0; i < sizeof(type); ++i)\
		{\
			p[sizeof(type)-i-1] = buf[offset++];\
		}\
	}\
	return S_OK;


CEndian	CCodeEngine::ms_endian;

CCodeEngine::CCodeEngine()
{

}

CCodeEngine::~CCodeEngine()
{

}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const bool data)
{
	uint8_t v = (data ? 1 : 0);
	return Encode(buf, size, offset, v);
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int8_t data)
{
	ENCODE(buf, size, offset, data, int8_t)
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int16_t data)
{
	ENCODE(buf, size, offset, data, int16_t)
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int32_t data)
{
	ENCODE(buf, size, offset, data, int32_t)
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const int64_t data)
{
	ENCODE(buf, size, offset, data, int64_t)
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint8_t data)
{
	ENCODE(buf, size, offset, data, uint8_t)
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint16_t data)
{
	ENCODE(buf, size, offset, data, uint16_t)
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint32_t data)
{
	ENCODE(buf, size, offset, data, uint32_t)
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint64_t data)
{
	ENCODE(buf, size, offset, data, uint64_t)
}
int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const double data)
{
	ENCODE(buf, size, offset, data, double)
}
int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const uint8_t *data, const uint32_t length, const uint32_t maxLength)
{
	if ((NULL == buf) || (0 == size))
	{
		return E_INVALIDARGUMENT;
	}
	if ((NULL == data) || (0 == length))
	{
		return S_OK;
	}
	if (offset + length > size)
	{
		return E_ENCODEBUFLENGTH;
	}
	if (length > maxLength)
	{
		return E_MAXLENGTH;
	}

	memcpy(buf + offset, data, length);
	offset += length;

	return S_OK;
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const char *data, const uint16_t maxLength)
{
	if ((NULL == buf) || (0 == size))
	{
		return E_INVALIDARGUMENT;
	}
	if (NULL == data)
	{
		return E_INVALIDARGUMENT;
	}
	if (0 == maxLength)
	{
		return E_MAXLENGTH;
	}

	int32_t ret = S_OK;

	uint16_t len = (uint16_t)strlen(data) + 1;
	ret = Encode(buf, size, offset, len);
	if (0 > ret)
	{
		return ret;
	}

	ret = Encode(buf, size, offset, (uint8_t*)data, len, (uint32_t)maxLength);
	if (0 > ret)
	{
		return ret;
	}

	return S_OK;
}

int32_t CCodeEngine::Encode(uint8_t* buf, const uint32_t size, uint32_t& offset, const string &data)
{
	return Encode(buf, size, offset, data.c_str(), data.length());
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, bool& data)
{
	int32_t ret = S_OK;

	uint8_t v = 0;
	ret = Decode(buf, size, offset, v);
	if (0 > ret)
	{
		return ret;
	}

	data = (0 != v);

	return S_OK;
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int8_t& data)
{
	DECODE(buf, size, offset, data, int8_t)
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int16_t& data)
{
	DECODE(buf, size, offset, data, int16_t)
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int32_t& data)
{
	DECODE(buf, size, offset, data, int32_t)
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, int64_t& data)
{
	DECODE(buf, size, offset, data, int64_t)
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint8_t& data)
{
	DECODE(buf, size, offset, data, uint8_t)
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint16_t& data)
{
	DECODE(buf, size, offset, data, uint16_t)
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint32_t& data)
{
	DECODE(buf, size, offset, data, uint32_t)
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint64_t& data)
{
	DECODE(buf, size, offset, data, uint64_t)
}
int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, double& data)
{
	DECODE(buf, size, offset, data, double)
}
int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, uint8_t* data, const uint32_t length, const uint32_t maxLength)
{
	if ((NULL == buf) || (0 == size))
	{
		return E_INVALIDARGUMENT;
	}
	if ((NULL == data) || (0 == length))
	{
		return S_OK;
	}
	if (offset + length > size)
	{
		return E_DECODEBUFLENGTH;
	}
	if (length > maxLength)
	{
		return E_MAXLENGTH;
	}

	memcpy(data, buf + offset, length);
	offset += length;

	return S_OK;
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, char* data, const uint16_t maxLength)
{
	if (0 == maxLength)
	{
		return E_MAXLENGTH;
	}

	int32_t ret = S_OK;

	uint16_t len = 0;
	ret = Decode(buf, size, offset, len);
	if (0 > ret)
	{
		return ret;
	}
	if (0 == len)
	{
		data[0] = '\0';
		return S_OK;
	}
	ret = Decode(buf, size, offset, (uint8_t*)data, (uint32_t)len, (uint32_t)maxLength);
	if (0 > ret)
	{
		return ret;
	}

	return S_OK;
}

int32_t CCodeEngine::Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, string &data)
{
	int32_t ret = S_OK;

	uint16_t len = 0;
	ret = Decode(buf, size, offset, len);
	if (0 > ret)
	{
		return ret;
	}

	if (0 == len)
	{
		data = "";
		return S_OK;
	}

#ifdef WIN32
	char *arrBuf = (char *)MALLOC(len + 1);
#else
	char arrBuf[len + 1];
#endif
	ret = Decode(buf, size, offset, (uint8_t*)arrBuf, (uint32_t)len, (uint32_t)len);
	if (0 > ret)
	{
#ifdef WIN32
		FREE(arrBuf);
#endif
		return ret;
	}

	arrBuf[len] = '\0';
	data = arrBuf;

#ifdef WIN32
	FREE(arrBuf);
#endif

	return S_OK;
}

int32_t encode_bool(uint8_t* buf, const uint32_t size, uint32_t* offset, const bool data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_int8(uint8_t* buf, const uint32_t size, uint32_t* offset, const int8_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_int16(uint8_t* buf, const uint32_t size, uint32_t* offset, const int16_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_int32(uint8_t* buf, const uint32_t size, uint32_t* offset, const int32_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_int64(uint8_t* buf, const uint32_t size, uint32_t* offset, const int64_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_uint8(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint8_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_uint16(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint16_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_uint32(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint32_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_uint64(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint64_t data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

int32_t encode_uint8_mem(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint8_t *data, const uint32_t length, const uint32_t maxLength)
{
	return CCodeEngine::Encode(buf, size, *offset, data, length, maxLength);
}

int32_t encode_char_mem(uint8_t* buf, const uint32_t size, uint32_t* offset, const char *data, const uint16_t maxLength)
{
	return CCodeEngine::Encode(buf, size, *offset, data, maxLength);
}

int32_t encode_double(uint8_t* buf, const uint32_t size, uint32_t* offset, const double data)
{
	return CCodeEngine::Encode(buf, size, *offset, data);
}

//int32_t Encode(uint8_t* buf, const uint32_t size, uint32_t* offset, const string &data)
//{
//	CCodeEngine::Encode(buf, size, *offset, data);
//}

int32_t decode_bool(const uint8_t *buf, const uint32_t size, uint32_t* offset, bool* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_int8(const uint8_t *buf, const uint32_t size, uint32_t* offset, int8_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_int16(const uint8_t *buf, const uint32_t size, uint32_t* offset, int16_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_int32(const uint8_t *buf, const uint32_t size, uint32_t* offset, int32_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_int64(const uint8_t *buf, const uint32_t size, uint32_t* offset, int64_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_uint8(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint8_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_uint16(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint16_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_uint32(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint32_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_uint64(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint64_t* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

int32_t decode_uint8_mem(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint8_t* data, const uint32_t length, const uint32_t maxLength)
{
	return CCodeEngine::Decode(buf, size, *offset, data, length, maxLength);
}

int32_t decode_char_mem(const uint8_t *buf, const uint32_t size, uint32_t* offset, char* data, const uint16_t maxLength)
{
	return CCodeEngine::Decode(buf, size, *offset, data, maxLength);
}

int32_t decode_double(const uint8_t *buf, const uint32_t size, uint32_t* offset, double* data)
{
	return CCodeEngine::Decode(buf, size, *offset, *data);
}

//int32_t Decode(const uint8_t *buf, const uint32_t size, uint32_t& offset, string &data)
//{
//	return CCodeEngine::Decode(buf, size, *offset, *data);
//}
