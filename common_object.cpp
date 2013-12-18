/*
 * common_object.cpp
 *
 *  Created on: 2013年12月14日
 *      Author: jimm
 */

#include "common_object.h"
#include "common_errordef.h"


CObject::CObject()
{

}

CObject::~CObject()
{

}

int32_t CObject::Initialize()
{
	return S_OK;
}

int32_t CObject::Uninitialize()
{
	return S_OK;
}

int32_t CObject::GetSize()
{
	return sizeof(*this);
}


