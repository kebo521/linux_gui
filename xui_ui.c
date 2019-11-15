//============================================================
//---��ʾ����---
#include <unistd.h>
//#include <stdarg.h>
#include <fcntl.h>
#include <sys/time.h>
#include <linux/input.h>

#include "types_def.h"

#include "xui_fb.h"
#include "xui_show.h"
//#include "xui_font.h"
#include "xui_ui.h"
#include "xui_gui.h"
//#include "EvenMsg.h"
#include "QR_Encode.h"
#include "key_hard.h"



//=========================��������==========================
//static const char* KeyMsgABC[10]={"*#:/?%@-+=","&$!,;'`^~\"_|","ABC","DEF","GHI","JKL","MNO","PQRS","TUV","WXYZ"};

XuiWindow UI_screen = {0};
gUi_def gUiDataAll={0};

void u32_memcpy(u32 *u1,u32 *u2,u32 len)
{
	while(len--)
		*u1++ = *u2++;
}

void u32_memset(u32 *u1,u32 nun,u32 len)
{
	while(len--)
		*u1++ = nun;
}


//========��src1�в���src2�ֶ�=����ƥ��src1�е�ĩ�˵�ַ=======
char *API_eStrstr(char* src1, const char* src2)
{
	unsigned char *pS1,*pS2;
	//if(src1==NULL || src2==NULL) return NULL;
	pS1=(unsigned char*)src1;
	pS2=(unsigned char*)src2;
	while(*pS1)
	{
		if(*pS1 != *pS2)
		{
			if(pS2 != (unsigned char*)src2)
			{
				pS2=(unsigned char*)src2;
				pS1=(unsigned char*)src1;
			}
			else pS1++;
		}
		else
		{
			pS1++; 
			if(pS2 == (unsigned char*)src2) //��¼��1���(src1+1)
				src1=(char*)pS1;
			pS2++;
			if(*pS2 == '\0')
				return (char*)pS1;
		}
	}
	return NULL;
}

//------------------��ʾ��� ת ���׽��-------------------------------
void Conv_DmoneyToTmoney(char* pOutStr,char* pInsMoney)
{
	u8 i,j,k;
	u8 Inlen=API_strlen(pInsMoney);
	for(i=0;i<Inlen;i++)
		if(pInsMoney[i]=='.') break;
	//----��������------
	for(j=0;j<i;j++)
		pOutStr[j]=pInsMoney[j];
	//---С�����־�ȷ��λ----
	if(i<Inlen)	//��С����
	{
		for(i=0; i<2; i++)
		{
			if((j+1) < Inlen)
				pOutStr[j]=pInsMoney[j+1];
			else
				pOutStr[j]='0';
			j++;
		}
	}
	else	//��С����,��λ0
	{
		pOutStr[j++]='0';
		pOutStr[j++]='0';
	}
	//---������Чλ------
	for(i=0; i<j; i++)
		if(pOutStr[i]!='0')
			break;
	if(i)
	{
		k=j-i;
		for(j=0;j<k;j++)
			pOutStr[j]=pOutStr[j+i];
	}
	pOutStr[j]='\0';
}

//------------------���׽�� ת ��ʾ���-------------------------------
void Conv_TmoneyToDmoney(char* pOutdMoney,char* pIntMoney)
{
	u16 i=0,Inlen=API_strlen(pIntMoney);
	if(Inlen == 0)
	{
		pOutdMoney[0]='\0';
		return;
	}
	if(Inlen <= 2)
	{
		pOutdMoney[i++]='0';
		pOutdMoney[i++]='.';
		if(Inlen < 2)
			pOutdMoney[i++]='0';
		else
			pOutdMoney[i++]=*pIntMoney++;
		pOutdMoney[i++]=*pIntMoney++;
	}
	else
	{
		Inlen -= 2;
		while(i<Inlen)
		{
			pOutdMoney[i++]=*pIntMoney++;
		}
		pOutdMoney[i++]='.';
		pOutdMoney[i++]=*pIntMoney++;
		pOutdMoney[i++]=*pIntMoney++;
	}

	while(i--)	//ȥ����Ч����
	{
		if(pOutdMoney[i] == '.')
		{
			i--;
			break;
		}
		else if(pOutdMoney[i] != '0') 
			break;
	}
	pOutdMoney[++i]='\0';
}



//argv ֧�ֵĸ�ʽΪ
//FB=xxxxx /*framebuffer �豸�ڵ㣨Ĭ��ֵ"/dev/graphics/fb0"��*/
//INPUT=xxxx /*input �豸�ڵ㣬���Զ����Ĭ��ֵ/dev/keypad ��/dev/tp����ֻ����Ӧ�ò����øò���ʱ������Ĭ��ֵ���������豸����Ӧ�������˸ò���ʱ����ֻ���������õ��豸�ڵ㣬��������������������ΪINPUT=/dev/tp����ôXUI ��ʼ��ʱ��ֻ����Ĭ�ϴ��������룬��������������̣���͵���������������������*/
//ROTATE=xxx /*��Ļ��ת(ȡֵ��Ϊ0��90��180��Ĭ��ֵ0�����ò�֧�ֵ�ֵʱ��ʹ��Ĭ��ֵ��*/
//TSDEV=xxxx/*�������豸�ڵ㣬��Ĭ��ֵ/dev/input/event2��*/
//STATUSBAR=xxx /*״̬���߶ȣ�0-64��Ĭ��ֵ0�����ò�֧�ֵ�ֵʱ��ʹ��Ĭ��ֵ��*/
//ʾ����char *xui_argv[] = {"ROTATE=90","STATUSBAR=18"};
//XuiOpen(sizeof(xui_argv)/sizeof(xui_argv[0]), xui_argv);




int XuiOpen(int argc,char **argv)
{
	int i;
	char *pTag;
	for(i=0;i<argc;i++)
	{
		pTag=API_eStrstr(argv[i],"FB=");
		if(pTag)
		{
			gUiDataAll.pFbMsg=open_screen(pTag);
			if(gUiDataAll.pFbMsg == NULL) 
			{
				TRACE("->main open_screen ret NULL \r\n");
				return 2;
			}
			continue;
		}
		pTag=API_eStrstr(argv[i],"INPUT=");
		if(pTag)
		{
			TRACE("Open Input[%d]\r\n",pTag);
			gUiDataAll.keys_fd = open(pTag,O_RDWR);  // O_RDONLY
			if(gUiDataAll.keys_fd > 0)
			{
				Start_Key_thread();
			}
			continue;
		}
		pTag=API_eStrstr(argv[i],"ROTATE=");
		if(pTag)
		{
			gUiDataAll.iRotate=atoi(pTag);
			if(gUiDataAll.iRotate>270 ||(gUiDataAll.iRotate % 90))
			{
				gUiDataAll.iRotate=0;
			}
			continue;
		}
		pTag=API_eStrstr(argv[i],"TSDEV=");
		if(pTag)
		{
			TRACE("Open TSDEV[%d]\r\n",pTag);
			gUiDataAll.TsDev_fd = open(pTag,O_RDWR);  // O_RDONLY
			continue;
		}
		pTag=API_eStrstr(argv[i],"STATUSBAR=");
		if(pTag)
		{
			gUiDataAll.iStatusbar=atoi(pTag);
			if(gUiDataAll.iStatusbar > 64)
			{
				gUiDataAll.iStatusbar = 0;
			}
			continue;
		}
	}
	return 0;
}

//�ر�XUI
void XuiClose(void)
{
	if(gUiDataAll.keys_fd)
	{
		Stop_Key_thread();
		close(gUiDataAll.keys_fd);
	}
	if(gUiDataAll.TsDev_fd)
	{
		close(gUiDataAll.TsDev_fd);
	}
	if(gUiDataAll.pFbMsg)
	{
		close_screen((screen_buffer *)gUiDataAll.pFbMsg);
		free(UI_screen.widget);
	}
	memset(&gUiDataAll,0x00,sizeof(gUiDataAll));
}


//�ж�XUI �Ƿ��������С�
int XuiIsRunning(void)
{

	return 0;
}


//��ͣXUI ���С�
int XuiSuspend(void)
{

	return 0;
}

//�ָ�XUI ����
int XuiResume(void)
{

	return 0;
}

//XuiWindow *XuiRootCanvas(void);
#define FB_WIDTH	260
#define FB_HEIGHT	340

//��ȡ������
int UI_RootCanvas(void)
{
	screen_buffer *screenfb;
	if(gUiDataAll.pFbMsg==NULL)
	{
		TRACE("InitRootCanvas .pFbMsg=NULL\r\n");
		return -1;
	}
	screenfb = (screen_buffer *)gUiDataAll.pFbMsg;
	//-------------------------------------------
	{
		rgba_t rgba;
		s16 staX,staY;
		memset(&rgba,0x7f,sizeof(rgba));
		staX=screenfb->width-FB_WIDTH;
		staY=0;
		xui_fb_fill_rect(screenfb,staX,staY,FB_WIDTH,FB_HEIGHT,&rgba);
	}
	
	if(screenfb->width < SCREEN_WIDTH || screenfb->height < SCREEN_HEIGT)
	{
		TRACE("InitRootCanvas");
		return -1;
	}
		
	#ifdef DISPLAY_HORIZONTAL_SCREEN
	UI_screen.left=screenfb->width-SCREEN_HEIGT-10;
	#else
	UI_screen.left=screenfb->width-SCREEN_WIDTH-10;
	#endif
	UI_screen.top=10;
	UI_screen.width=SCREEN_WIDTH;
	UI_screen.height=SCREEN_HEIGT;
	UI_screen.widget=(unsigned int*)malloc(SCREEN_WIDTH*SCREEN_HEIGT*4);
	//memset(UI_screen.widget,0x00,SCREEN_WIDTH*SCREEN_HEIGT*4);
	u32_memset(UI_screen.widget,0,SCREEN_WIDTH*SCREEN_HEIGT);
	UI_screen.fb=gUiDataAll.pFbMsg;
	return 0;
}





//��ȡ״̬������
//XuiWindow *XuiStatusbarCanvas(void)

int UI_StatusbarCanvas(void)
{
	return 0;
}




/*
//��������
XuiFont *XuiCreateFont(char *fontfile,int index,XuiFontSet fontset);
{
	return NULL;
}

int XuiCanvasDrawText(XuiWindow *window,unsigned int x,unsigned int y,unsigned int height,XuiFont *font,XuiTextStyle textstyle,XuiColor fg,char *text)
{
	return 0;
}

*/
void UI_Push(RECTL *pRect)
{
	xui_fb_rect_push(UI_screen.fb,UI_screen.left,UI_screen.top,UI_screen.width,UI_screen.height,(rgba_t*)UI_screen.widget);
}


void UI_FillRectSingLe(RECTL *pRect,u32 rtg)
{
	u16 i,j,sx,sy,w,h;
	u32 *destin;
	sx = pRect->left;
	sy = pRect->top;
	w = sx+pRect->width;
	h = sy+pRect->height;
	if(w > UI_screen.width) w=UI_screen.width;
	if(h > UI_screen.height) h=UI_screen.height;
	for (j = sy; j < h; j++) 
	{
		destin=&UI_screen.widget[j*UI_screen.width + sx];
		for(i=sx; i<w; i++)
		{
			*destin++ = rtg;
		}
	}
}

void UI_SetRectBuff(RECTL *pRect,gUIrgba *pRGB)
{
	u16 i,j,sx,sy,w,h;
	u32 *destin,*source;
	sx = pRect->left;
	sy = pRect->top;
	w = sx+pRect->width;
	h = sy+pRect->height;
	if(w > UI_screen.width) w=UI_screen.width;
	if(h > UI_screen.height) h=UI_screen.height;
	source = (u32*)pRGB;
	for (j = sy; j < h; j++) 
	{
		destin=&UI_screen.widget[j*UI_screen.width + sx];
		for(i=sx; i<w; i++)
		{
			*destin++ = *source++;
		}
	}
}

void UI_GetRectBuff(RECTL *pRect,gUIrgba *pRGB)
{
	u16 i,j,sx,sy,w,h;
	u32 *destin,*source;
	sx = pRect->left;
	sy = pRect->top;
	w = sx+pRect->width;
	h = sy+pRect->height;
	if(w > UI_screen.width) w=UI_screen.width;
	if(h > UI_screen.height) h=UI_screen.height;
	source = (u32*)pRGB;
	for (j = sy; j < h; j++) 
	{
		destin=&UI_screen.widget[j*UI_screen.width + sx];
		for(i=sx; i<w; i++)
		{
			*source++ = *destin++;
		}
	}
}


//static u8 SPF_runLock=FALSE;
int UI_ShowPictureFile(RECTL *prclTrg,const char *pfilePath)
{
	
	return 0;
}


//=============�ײ���ʾ������==ratio(0~100)===============================
void UI_ShowBottomProgress(int ratio)
{
	RECTL tRect;
	tRect.width=SCREEN_WIDTH;
	tRect.left = 0; 
	tRect.height=16;
	tRect.top = SCREEN_HEIGT-tRect.height;
	UI_FillRectSingLe(&tRect,RGB_CURR(199,200,244));
	if(ratio)
	{
		tRect.top	+=	3;
		tRect.left	+=	3;
		tRect.width -=	6;
		tRect.height -=	6;
		if(ratio < 100)
			tRect.width = tRect.width*ratio/100;
		UI_FillRectSingLe(&tRect,RGB_CURR(0,255,255));
	}
}

//=============��ʾ����==ratio(0~100)===============================
void UI_ShowParMiddleSlide(int ratio)
{
	u16 Width1,Width2;
	RECTL tRect;
	tRect.width = SCREEN_APP_W-8;
	tRect.height = 32;
	Width1 = (tRect.width-tRect.height)*ratio/100;
	Width2 = (tRect.width-tRect.height)-Width1;
	tRect.top = SCREEN_APP_Y+(SCREEN_APP_H-32)/2;
	tRect.left = SCREEN_APP_X+4;
	UI_FillRectSingLe(&tRect,RGB_CURR(255,255,255));

	
	tRect.top = SCREEN_APP_Y+(SCREEN_APP_H-16)/2;
	if(Width1)
	{
		tRect.left = SCREEN_APP_X+4;
		tRect.width = Width1;
		tRect.height = 16;
		UI_FillRectSingLe(&tRect,RGB_CURR(210,99,25));
	}
	if(Width2)
	{
		tRect.left = SCREEN_APP_X+4+Width1+tRect.height;
		tRect.width = Width2;
		tRect.height = 16;
		UI_FillRectSingLe(&tRect,RGB_CURR(56,56,56));
	}
	tRect.left = SCREEN_APP_X+4+Width1;
	tRect.top = SCREEN_APP_Y+(SCREEN_APP_H-32)/2;
	tRect.height = 32;
	UI_FillRectSingLe(&tRect,RGB_CURR(199,206,218));
/*
	ret=API_sprintf(sbuff,"%d",par);
	tRect.left = SCREEN_APP_X+(SCREEN_APP_W-ret*FONT_SIZE/2)/2;
	tRect.top = SCREEN_APP_Y+ (SCREEN_APP_H-32)/2  +32;
//	ApiFont.SetFontColor(RGB_CURR(56,56,56),RGB_CURR(255,255,255));
//	ApiFont.DisplayFont(&tRect,(u8*)sbuff);
	DisplayPaintEnd();
	*/
}


void UI_ShowQrCode(RECTL* pRect,const char* pInfo,u32 Color)
{
	IMAGE image;
	if(0==Lib_QrCodeImg(&image,pInfo,FALSE))
	{
		RECTL oImage;
		u8 *pByte;
		u32 *pbgra;
		u16 x,y,ix,mx,iy,my;
		//RECTL tRect;
		//----���ֳ���ȣ�����С��ȷŴ�--------------
		mx = MIN(pRect->width,pRect->height)/image.w;
		my = mx;
		//------�����ֳ���ȣ���ָ������Ŵ�---------
		//mx =pRect->width/image.w;
		//my = pRect->height/image.h;
		oImage.width = image.w * mx;
		oImage.height = image.h * my;
		oImage.left = pRect->left;
		oImage.top = pRect->top;
		if(pRect->width > oImage.width)
			oImage.left +=(pRect->width - oImage.width)/2;
		if(pRect->height > oImage.height)
			oImage.top +=(pRect->height - oImage.height)/2;		

		//tRect.height=my;
		//tRect.width =mx;
		for(x=0 ;x<image.w ; x++)
		{
			pByte = &image.pByte[x*image.mline];
			for(y = 0; y < image.h; y++) 
			{
				if(pByte[y])
				{
					for(iy=0;iy<my;iy++)	
					{
						pbgra=&UI_screen.widget[(oImage.top + y*my+iy)*UI_screen.width + oImage.left + x*mx];
						for(ix=0;ix<mx;ix++)
						{
							*pbgra++ = Color;
						}
					}
					//tRect.left = oImage.left++ x*mx;
					//tRect.top = oImage.top+y*my;
					//UI_FillRectSingLe(&tRect,Color);
				}
			}
		}
	}
}


const API_UI_Def ApiUI={
	{'U','I','\0',26},
	XuiOpen,
	XuiClose,

	UI_RootCanvas,
	UI_StatusbarCanvas,

	UI_Push,
	UI_FillRectSingLe,
	UI_SetRectBuff,
	UI_GetRectBuff,
	
	UI_ShowQrCode,
	UI_ShowPictureFile,
	UI_ShowBottomProgress,
	UI_ShowParMiddleSlide,
};

