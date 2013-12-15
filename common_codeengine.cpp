/*
 * common_string.h
 *
 *  Created on: 2013年12月14日
 *      Author: jimm
 */

#include "common_codeengine.h"
#include "common_errordef.h"


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
	data[len - 1] = '\0';

	return S_OK;
}


