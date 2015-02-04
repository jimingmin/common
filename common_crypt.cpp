/*
 * common_crypt.cpp
 *
 *  Created on: 2012-12-10
 *      Author: jimm
 */

#include <stdlib.h>

#include "common_crypt.h"
#include <memory.h>

void xtea(int32_t* v, // 64bit of data [in/out]
		  int32_t* o, // 64bits of data [out]
		  int32_t* k, // 128bit key [in]
		  int32_t N) // Routine rounds [in]
{
	uint32_t y = v[0];
	uint32_t z = v[1];
	uint32_t DELTA = 0x9e3779b9; // 0x00000000 - 0x61C88647 == 0x9e3779b9

	if(N>0)
	{
		// Encoding
		uint32_t limit = DELTA*N;
		uint32_t sum = 0;
		while(sum != limit)
		{
			y += (z<<4 ^ z>>5) + z ^ sum + k[sum&3];
			sum += DELTA;
			z += (y<<4 ^ y>>5) + y ^ sum + k[sum>>11&3];
		}
	}
	else
	{
		// Decoding
		uint32_t sum = DELTA*(-N);
		while(sum)
		{
			z -= (y<<4 ^ y>>5) + y ^ sum + k[sum>>11&3];
			sum -= DELTA;
			y -= (z<<4 ^ z>>5) + z ^ sum + k[sum&3];
		}
	}

	o[0] = y;
	o[1] = z;
}

/*****************************************************************************
//	加密后的Buffer结构
//  ┌──────┬───────────┬────┬────┐
//  │ PadLength  │  Padded Random BYTEs │  Data  │ Zero s │
//  ├──────┼───────────┼────┼────┤
//  │    1Byte   │    PadLength Bytes   │ N Bytes│ 7Bytes │
//  └──────┴───────────┴────┴────┘
// Pad部分用于将整个Buffer补齐到8字节对齐
******************************************************************************/

#define ZERO_LENGTH 7
#define ENCRYPT_ROUNDS (32)
#define DECRYPT_ROUNDS (-32)

int32_t CXTEA::Encrypt( char* pbyInBuffer, int32_t nInBufferLength, char* pbyOutBuffer, int32_t nOutBufferLength, char arrbyKey[16] )
{
	if(pbyInBuffer == NULL || nInBufferLength <= 0)
	{
		return 0;
	}

	// 计算需要的输出Buffer大小
	int32_t nPadDataZero = 1/*Pad Length*/ + nInBufferLength + ZERO_LENGTH;
	int32_t nPadLength = nPadDataZero % ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	if(nPadLength != 0)
	{
		nPadLength = ENCRYPT_BLOCK_LENGTH_IN_BYTE - nPadLength;
	}

	int32_t nTotalLength = nPadDataZero + nPadLength;

	if(nTotalLength > nOutBufferLength || pbyOutBuffer == NULL)
	{
		return 0;
	}

	char* pbyInCursor = pbyInBuffer;
	char* pbyOutCurosr = pbyOutBuffer;

	memset(pbyOutBuffer, 0, nOutBufferLength);

	char arrbyFirst8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	// Pad length, 只使用最低三位，高5位用随机数填充
	arrbyFirst8Bytes[0] = (((char)rand()) & 0xf8) | ((char)nPadLength);
        //arrbyFirst8Bytes[0] = ((char)nPadLength);

	// 用随机数填充Pad区
	for(int32_t i =1; i<=nPadLength; ++i)
	{
		arrbyFirst8Bytes[i] = (char)rand();
                //arrbyFirst8Bytes[i] = (char)0;
	}

	// 用待加密数据补满第一块明文
	for(int32_t i = 1+nPadLength; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbyFirst8Bytes[i] = *pbyInCursor++;
	}

	// 上一个加密块的明文与密文，用于后面的异或操作
	char* pbyLast8BytesCryptedData = pbyOutBuffer;
	char* pbyLast8BytesPlainData = arrbyFirst8Bytes;

	// 第一段Buffer，不需要异或操作
	xtea((int32_t*)arrbyFirst8Bytes, (int32_t*)pbyOutCurosr, (int32_t*)arrbyKey, ENCRYPT_ROUNDS);
//	xtea((Int32*)pbyOutCurosr, (Int32*)arrbyFirst8Bytes, (Int32*)arrbyKey, DECRYPT_ROUNDS);
	pbyOutCurosr += ENCRYPT_BLOCK_LENGTH_IN_BYTE;

	// 下面这段是是用于不更改InBuffer的加密过程
	char arrbySrcBuffer[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	while((pbyInCursor - pbyInBuffer) < (nInBufferLength - 1))
	{
		memcpy(arrbySrcBuffer, pbyInCursor, ENCRYPT_BLOCK_LENGTH_IN_BYTE);
		// 和上一块密文异或
		for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			arrbySrcBuffer[i] ^= pbyLast8BytesCryptedData[i];
		}
		xtea((int32_t*)arrbySrcBuffer, (int32_t*)pbyOutCurosr, (int32_t*)arrbyKey, ENCRYPT_ROUNDS);
		// 和上一块明文异或
		for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyOutCurosr[i] ^= pbyLast8BytesPlainData[i];
		}

		pbyLast8BytesCryptedData = pbyOutCurosr;
		pbyLast8BytesPlainData = pbyInCursor;

		pbyOutCurosr += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
		pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	}

	// 这一段和上一段被注释掉的功能相同，只不过会修改InBuffer的内容
/*	while((pbyInCursor - pbyInBuffer) < (nInBufferLength - 1))
	{
		// 和上一块密文异或
		for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyInCursor[i] ^= pbyLast8BytesCryptedData[i];
		}
		xtea((Int32*)pbyInCursor, (Int32*)pbyOutCurosr, (Int32*)arrbyKey, ENCRYPT_ROUNDS);
		// 和上一块明文异或
		for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyOutCurosr[i] ^= pbyLast8BytesPlainData[i];
		}

		pbyLast8BytesCryptedData = pbyOutCurosr;
		pbyLast8BytesPlainData = pbyInCursor;

		pbyOutCurosr += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
		pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	}
*/
	// 结尾的 1Byte数据 + 7Byte 校验
	char arrbyLast8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	arrbyLast8Bytes[0] = *pbyInCursor;

	// 和上一块密文异或
	for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbyLast8Bytes[i] ^= pbyLast8BytesCryptedData[i];
	}
	xtea((int32_t*)arrbyLast8Bytes, (int32_t*)pbyOutCurosr, (int32_t*)arrbyKey, ENCRYPT_ROUNDS);
	// 和上一块明文异或
	for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		pbyOutCurosr[i] ^= pbyLast8BytesPlainData[i];
	}

	return nTotalLength;
}
	// TODO: OutBufferLength判断
int32_t CXTEA::Decrypt( char* pbyInBuffer, int32_t nInBufferLength, char* pbyOutBuffer, int32_t nOutBufferLength, char arrbyKey[16] )
{
	if(pbyInBuffer == NULL || nInBufferLength <= 0)
	{
		return false;
	}

	// Buffer长度应该是能被 ENCRYPT_BLOCK_LENGTH_IN_BYTE 整除的
	if(nInBufferLength%ENCRYPT_BLOCK_LENGTH_IN_BYTE || nInBufferLength <= 8)
	{
		return 0;
	}

	char* pbyInCursor = pbyInBuffer;
	char* pbyOutCursor = pbyOutBuffer;

	// 先解出最前面包含Pad的ENCRYPT_BLOCK_LENGTH_IN_BYTE个Byte
	char arrbyFirst8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	xtea((int32_t*)pbyInCursor, (int32_t*)arrbyFirst8Bytes, (int32_t*)arrbyKey, DECRYPT_ROUNDS);
	pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	// Pad长度只是用了第一个字节的最低3bit，高5bit是随机数
	int32_t nPadLength = arrbyFirst8Bytes[0] & 0x07;

	// 计算原始数据的长度
	int32_t nPlainDataLength = nInBufferLength - 1/*PadLength Length*/ - nPadLength - ZERO_LENGTH;
	if(nPlainDataLength <= 0 || pbyOutBuffer == NULL)
	{
		return 0;
	}

	// OutBuffer不够
	if(nPlainDataLength > nOutBufferLength)
	{
		return 0;
	}

	// 前一块的明文和密文，用于后面的异或操作
	char* pbyLast8BytesCryptedData = pbyInBuffer;
	char* pbyLast8BytesPlainData = arrbyFirst8Bytes;

	// 将第一块里Pad信息之后的数据移到输出Buffer
	for(int32_t i=0; i < 7/*ENCRYPT_BLOCK_LENGTH_IN_BYTE - 1*/ - nPadLength; ++i)
	{
		*pbyOutCursor++ = arrbyFirst8Bytes[1+nPadLength + i];
	}

	// 解密除了最后一块以外的所有块
	// 同加密过程，下面注释掉的，是不改动InBuffer的

	char arrbySrcBuffer[8] = {0};
	//for(int32_t i=8; i<nInBufferLength-8; i+=8)
	while(pbyInCursor - pbyInBuffer < nInBufferLength - 8)
	{
		memcpy(arrbySrcBuffer, pbyInCursor, 8);
		// 和上一个8char明文异或
		for(int32_t i=0; i<8; ++i)
		{
			arrbySrcBuffer[i] ^= pbyLast8BytesPlainData[i];
		}
		xtea((int32_t*)arrbySrcBuffer, (int32_t*)pbyOutCursor, (int32_t*)arrbyKey, DECRYPT_ROUNDS);
		// 和上一个8char密文异或
		for(int32_t i=0; i<8; ++i)
		{
			pbyOutCursor[i] ^= pbyLast8BytesCryptedData[i];
		}

		pbyLast8BytesCryptedData = pbyInCursor;
		pbyLast8BytesPlainData = pbyOutCursor;

		pbyInCursor += 8;
		pbyOutCursor += 8;
	}

	// 直接改InBuffer的版本
/*	while(pbyInCursor - pbyInBuffer < nInBufferLength - ENCRYPT_BLOCK_LENGTH_IN_BYTE)
	{
		// 和上一个8Byte明文异或
		for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyInCursor[i] ^= pbyLast8BytesPlainData[i];
		}
		xtea((Int32*)pbyInCursor, (Int32*)pbyOutCursor, (Int32*)arrbyKey, DECRYPT_ROUNDS);
		// 和上一个8Byte密文异或
		for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyOutCursor[i] ^= pbyLast8BytesCryptedData[i];
		}

		pbyLast8BytesCryptedData = pbyInCursor;
		pbyLast8BytesPlainData = pbyOutCursor;

		pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
		pbyOutCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	}
*/
	// 最后8Byte， 最后有7Byte的校验
	char arrbyLast8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	// 和上一个8Byte明文异或
	memcpy(arrbySrcBuffer, pbyInCursor, 8);
	for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbySrcBuffer[i] ^= pbyLast8BytesPlainData[i];
	}
	xtea((int32_t*)arrbySrcBuffer, (int32_t*)arrbyLast8Bytes, (int32_t*)arrbyKey, DECRYPT_ROUNDS);
	// 和上一个8Byte密文异或
	for(int32_t i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbyLast8Bytes[i] ^= pbyLast8BytesCryptedData[i];
	}

	// 校验最后的0
	for(int32_t i=1; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		if(arrbyLast8Bytes[i] != 0)
		{
			return 0;
		}
	}
	*pbyOutCursor = arrbyLast8Bytes[0];

	return nPlainDataLength;
}

int32_t func_encrypt(char* pbyInBuffer, int32_t nInBufferLength, char* pbyOutBuffer, int32_t nOutBufferLength, char arrbyKey[16])
{
	return CXTEA::Encrypt(pbyInBuffer, nInBufferLength, pbyOutBuffer, nOutBufferLength, arrbyKey);
}

int32_t func_decrypt(char* pbyInBuffer, int32_t nInBufferLength, char* pbyOutBuffer, int32_t nOutBufferLength, char arrbyKey[16])
{
	return CXTEA::Decrypt(pbyInBuffer, nInBufferLength, pbyOutBuffer, nOutBufferLength, arrbyKey);
}

