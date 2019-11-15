/**
 * File:   types_def.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  basic types definitions.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TYPES_DEF_H
#define TYPES_DEF_H

#include <math.h>
#include <ctype.h>
#include <wchar.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#define DISPLAY_HORIZONTAL_SCREEN		//��������


typedef enum _ret_t { RET_OK = 0, RET_OOM, RET_ERR,RET_FAIL, RET_BAD_PARAMS } ret_t;

#define		BYTE2_TO_INT(buf)	(buf[0]*0x100+buf[1])
#define		BYTE4_TO_INT(buf)	(buf[0]*0x1000000+buf[1]*0x10000+buf[2]*0x100+buf[3])

#define		DfGetStr2Len(nL,sL)	{nL=sL[0]*0x100+sL[1];}
#define		DfSetStr2Len(sL,nL)	{sL[0]=(u8)(nL/0x100); sL[1]=(u8)nL;}
#define 	DfGetStr4Len(nL,sL) {nL=sL[0]*0x1000000+sL[1]*0x10000+sL[2]*0x100+sL[3];}
#define 	DfSetStr4Len(sL,nL) {sL[0]=nL/0x1000000; sL[1]=(u8)(nL/0x10000);sL[2]=(u8)(nL/0x100);sL[3]=(u8)nL;}
#define		DfGetBeiSu(nL,n)	((nL+(n-1))&(~(n-1)))	//�������������2/4/8/16/32...
#ifndef offsetof
#define 	offsetof(bak,item)	(int)(&((bak *)0)->item)
#endif
#ifndef offsize
#define 	offsize(bak,item)	(sizeof(((bak *)0)->item))
#endif
#define 	_BIT(__n)       	(1<<(__n))
#define		MIN(a,b)			(((a) < (b)) ? (a) : (b))
#define		MAX(a,b)			(((a) > (b)) ? (a) : (b))

typedef signed char			s8;     //!< Define s8 ����������charΪ��unsigned��������ʹ��signed����
typedef unsigned char   		u8;		//!< Define u8

typedef signed short			s16;	//!< Define s16
typedef unsigned short  		u16;	//!< Define u16

typedef signed int			s32;	//!< Define s32
typedef unsigned int			u32;	//!< Define u32




typedef struct{
	int Year; /*��1970�C 2037*/
	int Month; /*��1 �C12*/
	int Day; /*��1 �C31*/
	int Hour; /*ʱ0 �C 23*/
	int Minute; /*��0 �C59*/
	int Second; /*��0 �C59*/
	int DayOfWeek; /*��1�C��7������ȡʱ����Ч��*/
} ST_TIME;

//#define		TRACE			printf

#define log_debug(format, args...) printf(format, ##args)
#define log_info(format, args...) printf(format, ##args)
#define log_warn(format, args...) printf(format, ##args)
#define log_error(format, args...) printf(format, ##args)

typedef void (*Fun_ShowNum)(u32,char*); 
typedef int (*APP_HANDLE)(char*); 

#define API_strlen		strlen
#define API_strcpy		strcpy
#define API_memcpy		memcpy
#define API_memset		memset
#define API_memmove	memmove
#define API_sprintf	sprintf

extern void API_Trace(char* pMsg,...);
extern void APP_Trace_Hex(char* msg,void* pBuff,int Len);

#define TRACE							API_Trace		//MercuryTrace		//
#define TRACE_HEX(msg,pBuff,Len)		APP_Trace_Hex(msg,pBuff,Len)

//===================================================================================


typedef struct{
  u16 left;
  u16 top;
} POINT, *PPOINT;

typedef struct{
  u16 left;
  u16 top;
  u16 width;
  u16 height;
}RECTL,*LPRECTL;

//===================================================================================

#define ENSURE(p) assert(p)
#define goto_error_if_fail(p)                           \
  if (!(p)) {                                           \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    goto error;                                         \
  }

#define break_if_fail(p)                                \
  if (!(p)) {                                           \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    break;                                              \
  }

#define return_if_fail(p)                               \
  if (!(p)) {                                           \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    return;                                             \
  }

#define return_value_if_fail(p, value)                  \
  if (!(p)) {                                           \
    assert(!"" #p);                                     \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    return (value);                                     \
  }

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#ifndef TRUE
#define TRUE 1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE 0
#endif /*FALSE*/

#ifndef bool_t
#define bool_t uint8_t
#endif /*bool_t*/

typedef struct _rgba_t {/*
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;*/

  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
} rgba_t;

#endif /*TYPES_DEF_H*/
