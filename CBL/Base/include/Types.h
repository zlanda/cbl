/*******************************************************************************
*   文  件 名：Types.h
*   功     能：数据类型定义
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_TYPES_H_
#define  _CBL_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#ifndef INT8
typedef signed char                 INT8;
#endif

#ifndef UINT8
typedef unsigned char               UINT8;
#endif

#ifndef CHAR
typedef char                 		CHAR;
#endif

#ifndef UCHAR
typedef unsigned char               UCHAR;
#endif

#ifndef INT16
typedef signed short                INT16;
#endif

#ifndef UINT16
typedef unsigned short              UINT16;
#endif

#ifndef INT32
typedef signed int                  INT32;
#endif

#ifndef UINT32
typedef unsigned int                UINT32;
#endif

#ifndef INT64
typedef signed long long            INT64;
#endif

#ifndef UINT64
typedef unsigned long long          UINT64;
#endif

#ifndef LONG
typedef signed long                 LONG;
#endif

#ifndef ULONG
typedef unsigned long               ULONG;
#endif

#ifndef VOID
typedef void                        VOID;
#endif

#ifndef BOOL
typedef bool                        BOOL;
#endif

#ifndef CBL_TRUE
#define CBL_TRUE                    true
#endif

#ifndef CBL_FALSE
#define CBL_FALSE                   false
#endif

#ifndef CBL_NULL
#define CBL_NULL                    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif


