/**
 * File:   linux_fb.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  linux_framebuffer
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

#include "pixel.h"
#include "bitmap.h"

#ifndef XUI_FONT_H
#define XUI_FONT_H


#define FONT_SIZE    			(24)    
#define DOTS_BYTES    			((FONT_SIZE * FONT_SIZE / 8))

//=============================================ks文件定义========================================================
typedef struct 										// APP安装包文件头
{
    u8 	        Mask[4];				// FKSP
    u8	 		HeadLen[4];				// 头部长度
    u8	 		DataLen[4];				// 数据长度
    u8	 		Ver;					// 1
    u8	 		Type;					// 文件类型(KSP_TYPE)
    u8	 		Format;					// 文件格式(tar.gz,自定义)
    u8	 		Flag;					// 标记 
    u8			filenum;                // 文件数量
    u8          bl[3];
    u8			signcode[256];			//
}KSP_FILE_HEAD;

typedef struct 										// APP安装包文件头
{
    u8	 		DataLen[4];				// 文件名长度
    u8			nouese;
	u8			fileName[16];
    u8	 		fileLen[4];				// 数据长度
}KSP_FILE_ITEM;

typedef  struct 					   // APP信息
{
   char 	   tag[15+1];			   // 应用标识
   char 	   name_en[30+2];		   // 应用名称(英文)
   char 	   name_cn[30+2];		   // 应用名称(中文)
   char 	   sup_dev[15+1];		   // 应用支持的设备   (暂时不使用)
   char 	   sdk_ver[7+1];		   // 支持的SDK版本    [123.567]
   char 	   app_ver[11+1];		   // 应用软件版本	   [123.567.901]
   char 	   app_time[16+4];		   // 应用软件日期时间 [2013-10-08]
}KSP_SIGN_APP;							   // 共136字节
	   

typedef struct					   // 签名结果定义
{	 
   u8 signFlag[8];		   // 0x01SZZTSIG
   u8 hashResult[32];	   // SHA256结果
   u8 signType[1];		   // 数据内容, 0BOOT 1系统 2KEY 3应用(对于系统和应用统一由KBOOT验证固件,下载验证统一)
   u8 signData[7];		   // 时间日期
   u8 codeLen[4];		   // 数据内容长度,如果长度不符合终止后需处理
   u8 codeAddress[4];	   // 签名数据的开始长度
   u8 signer[16];		   // 渠道名称	   以上共68字节 	   
   KSP_SIGN_APP    app; 			   // 应用信息	   
}KSP_SIGN_CONTEXT;  

//================================固定定义4K空间=======================================================================================
typedef struct
{
	u8      auth[256];       //资源区的签名数据(解压后就是:KSP_SIGN_CONTEXT)

	u8      mask[4];		    //"RESA"
	u8      ver;           
	u8      count;           //最大255个RES_ITEM
	u8      itemlen;         //sizeof（RES_ITEM）
	u8      va[1+8];         //保留
}RES_Head;
typedef struct
{
	char	name[16];        	//资源文件名
	u8		datalen[4];      	//资源数据长度
	u8		offset[4];      	//资源数据偏移地址，如：第1项按建议将等于4096
} RES_ITEM;	

//========================显示资源(字库)==================================================
typedef struct
{
	u32		ParLen;      			//资源数据长度
	u32		Offset;      		//资源数据偏移地址
}DisRES_ITEM;	

typedef struct
{
	u8 reqLen_en;
	u8 reqLen_ch;
	u8 outLen_en;
	u8 outLen_ch;
}DisFont_Len;	

typedef struct
{
//	int		fd;		//文件句柄
	u32			lenFont;
	u8			*pbFont;
	DisRES_ITEM gbk;
	DisRES_ITEM ucode;
	DisFont_Len resFd;
	DisRES_ITEM fn[6];	//en+hz1+hz2+hz3+hz4+hz5
}DisRES_table;
extern DisRES_table resDisTable;

extern int InitExtResLib(char *pfile);
extern void DeInitExtResLib(void);
extern void API_ShowLineEn(u8 Line,char *pMsgEn,int timeoutms);

//==================显示中文，需要字库ks.res支持===================================
extern void UI_SetFontColor(u32 fgColor,u32 bgColor);
extern int UI_DisplayFont(XuiWindow *pWindow,POINT* prclTrg, u8* hzData);
extern int UI_DrawLineString(XuiWindow *pWindow,POINT* prclTrg,const char *src);
extern int UI_DrawRectString(XuiWindow *pWindow,RECTL* pRect,const char *src);


typedef struct	
{
	char Mask[4]; 	// "Fnt"
	void (*DisplaySysString)(XuiWindow*,int,int,int,char*);	//(Window，x,y,TEXT_TYPE,pMsg) TEXT_TYPE=TEXT_12/TEXT_16/TEXT_24
	
	int (*InitFontLib)(char*);		//(char *pfile)
	void (*DeInitFontLib)(void);	//
	void (*SetFontColor)(u32,u32);	//(u32 fgColor,u32 bgColor) RGB_CURR
	int (*DisplayFont)(XuiWindow*,POINT*,u8*);	//(pWindow,POINT* prclTrg, u8* hzData)
	int (*DrawLineString)(XuiWindow*,POINT*,const char*);//Display single line of text, no automatic line breaks
	int (*DrawRectString)(XuiWindow*,RECTL*,const char*);//The specified area displays text and wraps automatically
}API_FONT_Def;


extern const API_FONT_Def ApiFont;




#endif /*LINUX_FB_H*/
