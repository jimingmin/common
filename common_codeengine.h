/*
 * common_string.h
 *
 *  Created on: 2013年12月14日
 *      Author: jimm
 */

#ifndef COMMON_CODEENGINE_H_
#define COMMON_CODEENGINE_H_

#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "common_typedef.h"


class CEndian
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


class CCodeEngine
{
public:
	CCodeEngine();
	virtual ~CCodeEngine();

public:
	/* ��ݴ�������ŵ���������
	*  buf      - Ŀ�Ļ�������ռ���ڴ��ں����ڲ����䣬���ɵ������ͷ�
	*  size     - Ŀ�Ļ������
	*  offset   - ��������ݷ��뵽���������ʼƫ���������Ϊ��ƫ������
	*             �´�����ݳ���֮��
	*  data     - ��������
	*  return   - ������
	*/
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

public:
	/* ��ݽ��Ի������е���ݽ��н���
	*  buf      - ��ݻ���������������
	*  size     - ��ݻ������
	*  offset   - ���뻺��������ݽ�����ʼƫ���������Ϊ��ƫ�������½��
	*             ����ݳ���֮��
	*  data     - ���ؽ��Ľ����ռ�õ��ڴ��ں����ڲ����䣬�ɵ������ͷ�
	*  return   - ������
	*/
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

protected:
	static CEndian	ms_endian;
};


#endif /* COMMON_CODEENGINE_H_ */
