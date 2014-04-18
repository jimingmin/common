/*
 * common_export.h
 *
 *  Created on: 2014年4月12日
 *      Author: jimm
 */

#ifndef COMMON_EXPORT_H_
#define COMMON_EXPORT_H_

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT
#define IMPORT
#endif


#endif /* COMMON_EXPORT_H_ */
