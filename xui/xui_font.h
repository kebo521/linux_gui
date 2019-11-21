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

#ifndef XUI_FONT_H
#define XUI_FONT_H


extern void UI_DisplaySysEn(XuiWindow *pWindow,int x,int y,int type,char*pMsgEn);
extern void API_ShowLineEn(u8 Line,char *pMsgEn,int timeoutms);



#define FONT_SIZE    			(24)    

//=============================================ks�ļ�����========================================================
typedef struct 										// APP��װ���ļ�ͷ
{
    u8 	        Mask[4];				// FKSP
    u8	 		HeadLen[4];				// ͷ������
    u8	 		DataLen[4];				// ���ݳ���
    u8	 		Ver;					// 1
    u8	 		Type;					// �ļ�����(KSP_TYPE)
    u8	 		Format;					// �ļ���ʽ(tar.gz,�Զ���)
    u8	 		Flag;					// ��� 
    u8			filenum;                // �ļ�����
    u8          bl[3];
    u8			signcode[256];			//
}KSP_FILE_HEAD;

typedef struct 										// APP��װ���ļ�ͷ
{
    u8	 		DataLen[4];				// �ļ�������
    u8			nouese;
	u8			fileName[16];
    u8	 		fileLen[4];				// ���ݳ���
}KSP_FILE_ITEM;

typedef  struct 					   // APP��Ϣ
{
   char 	   tag[15+1];			   // Ӧ�ñ�ʶ
   char 	   name_en[30+2];		   // Ӧ������(Ӣ��)
   char 	   name_cn[30+2];		   // Ӧ������(����)
   char 	   sup_dev[15+1];		   // Ӧ��֧�ֵ��豸   (��ʱ��ʹ��)
   char 	   sdk_ver[7+1];		   // ֧�ֵ�SDK�汾    [123.567]
   char 	   app_ver[11+1];		   // Ӧ������汾	   [123.567.901]
   char 	   app_time[16+4];		   // Ӧ���������ʱ�� [2013-10-08]
}KSP_SIGN_APP;							   // ��136�ֽ�
	   

typedef struct					   // ǩ���������
{	 
   u8 signFlag[8];		   // 0x01SZZTSIG
   u8 hashResult[32];	   // SHA256���
   u8 signType[1];		   // ��������, 0BOOT 1ϵͳ 2KEY 3Ӧ��(����ϵͳ��Ӧ��ͳһ��KBOOT��֤�̼�,������֤ͳһ)
   u8 signData[7];		   // ʱ������
   u8 codeLen[4];		   // �������ݳ���,������Ȳ�������ֹ���账��
   u8 codeAddress[4];	   // ǩ�����ݵĿ�ʼ����
   u8 signer[16];		   // ��������	   ���Ϲ�68�ֽ� 	   
   KSP_SIGN_APP    app; 			   // Ӧ����Ϣ	   
}KSP_SIGN_CONTEXT;  

//================================�̶�����4K�ռ�=======================================================================================
typedef struct
{
	u8      auth[256];       //��Դ����ǩ������(��ѹ�����:KSP_SIGN_CONTEXT)

	u8      mask[4];		    //"RESA"
	u8      ver;           
	u8      count;           //���255��RES_ITEM
	u8      itemlen;         //sizeof��RES_ITEM��
	u8      va[1+8];         //����
}RES_Head;
typedef struct
{
	char	name[16];        	//��Դ�ļ���
	u8		datalen[4];      	//��Դ���ݳ���
	u8		offset[4];      	//��Դ����ƫ�Ƶ�ַ���磺��1����齫����4096
} RES_ITEM;	

//========================��ʾ��Դ(�ֿ�)==================================================
typedef struct
{
	u32		ParLen;      			//��Դ���ݳ���
	u32		Offset;      		//��Դ����ƫ�Ƶ�ַ
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
//	int		fd;		//�ļ����
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
//==================��ʾ���ģ���Ҫ�ֿ�ks.res֧��===================================
extern void UI_SetFontColor(A_RGB fgColor,A_RGB bgColor);
extern int UI_DisplayFont(XuiWindow *pWindow,POINT* prclTrg, u8* hzData);
extern int UI_DrawLineString(XuiWindow *pWindow,POINT* prclTrg,const char *src);
extern int UI_DrawRectString(XuiWindow *pWindow,RECTL* pRect,const char *src);


typedef struct	
{
	char Mask[4]; 	// "Fnt"
	void (*DisplaySysString)(XuiWindow*,int,int,int,char*);	//(Window��x,y,TEXT_TYPE,pMsg) TEXT_TYPE=TEXT_12/TEXT_16/TEXT_24
	
	int (*InitFontLib)(char*);		//(char *pfile)
	void (*DeInitFontLib)(void);	//
	void (*SetFontColor)(A_RGB,A_RGB);	//(u32 fgColor,u32 bgColor) RGB_CURR
	int (*DisplayFont)(XuiWindow*,POINT*,u8*);	//(pWindow,POINT* prclTrg, u8* hzData)
	int (*DrawLineString)(XuiWindow*,POINT*,const char*);//Display single line of text, no automatic line breaks
	int (*DrawRectString)(XuiWindow*,RECTL*,const char*);//The specified area displays text and wraps automatically
}API_FONT_Def;


extern const API_FONT_Def ApiFont;




#endif /*LINUX_FB_H*/
