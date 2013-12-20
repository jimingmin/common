/*
 * common_complextype.h
 *
 *  Created on: 2011-11-21
 *      Author: jimm
 */

#ifndef COMMON_COMPLEXTYPE_H_
#define COMMON_COMPLEXTYPE_H_

#include "common_typedef.h"
#include "common_errordef.h"
#include "common_object.h"


#define CCOMPLEXTYPE_SPECIFICALLY(type)	\
	template<>	\
	class CComplexType<type>	\
	{	\
	public:	\
		inline int32_t Init()	\
		{	\
			return m_object.Init();	\
		}	\
		inline int32_t Uninit()	\
		{	\
			return m_object.Uninit();	\
		}	\
		inline int32_t GetSize()	\
		{	\
			return m_object.GetSize();	\
		}	\
		inline type& GetValue()	\
		{	\
			return m_object;	\
		}	\
		inline void SetValue(type& object)	\
		{	\
			m_object = object;	\
		}	\
	protected:	\
		type		m_object;	\
	};


template<typename T>
class CComplexType
{
public:
	inline int32_t Init()
	{
		return S_OK;
	}
	inline int32_t Uninit()
	{
		return S_OK;
	}
	inline int32_t GetSize()
	{
		return 0;
	}
	inline T& GetValue()
	{
		return m_object;
	}
	inline void SetValue(T& object)
	{
		m_object = object;
	}

protected:
	T			m_object;
};


//CCOMPLEXTYPE_SPECIFICALLY(int8_t)
//CCOMPLEXTYPE_SPECIFICALLY(int16_t)
//CCOMPLEXTYPE_SPECIFICALLY(int32_t)
//CCOMPLEXTYPE_SPECIFICALLY(int64_t)
//CCOMPLEXTYPE_SPECIFICALLY(uint8_t)
//CCOMPLEXTYPE_SPECIFICALLY(uint16_t)
//CCOMPLEXTYPE_SPECIFICALLY(uint32_t)
//CCOMPLEXTYPE_SPECIFICALLY(uint64_t)
//CCOMPLEXTYPE_SPECIFICALLY(float)
//CCOMPLEXTYPE_SPECIFICALLY(double)

#endif /* COMMON_COMPLEXTYPE_H_ */
