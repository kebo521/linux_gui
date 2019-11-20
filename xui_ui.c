//============================================================
//---显示功能---
#include <unistd.h>
//#include <stdarg.h>
#include <fcntl.h>
#include <sys/time.h>
#include <linux/input.h>

#include "types_def.h"

#include "xui_ui.h"
#include "xui_fb.h"
//#include "xui_font.h"
#include "xui_gui.h"
//#include "EvenMsg.h"
#include "QR_Encode.h"
#include "key_hard.h"



//=========================按键定义==========================
//static const char* KeyMsgABC[10]={"*#:/?%@-+=","&$!,;'`^~\"_|","ABC","DEF","GHI","JKL","MNO","PQRS","TUV","WXYZ"};

//XuiWindow UI_screen = {0};
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


//========从src1中查找src2字段=返回匹配src1中的末端地址=======
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
			if(pS2 == (unsigned char*)src2) //记录串1起点(src1+1)
				src1=(char*)pS1;
			pS2++;
			if(*pS2 == '\0')
				return (char*)pS1;
		}
	}
	return NULL;
}




//argv 支持的格式为
//FB=xxxxx /*framebuffer 设备节点（默认值"/dev/graphics/fb0"）*/
//INPUT=xxxx /*input 设备节点，可以多个（默认值/dev/keypad 和/dev/tp）。只有在应用不设置该参数时，按照默认值加载输入设备，当应用设置了该参数时，则只加载所设置的设备节点，比如如果仅设置这个参数为INPUT=/dev/tp，那么XUI 初始化时则只加载默认触摸屏输入，而不加载物理键盘，这就等于是屏蔽了物理按键输入*/
//ROTATE=xxx /*屏幕旋转(取值可为0，90，180，默认值0，设置不支持的值时均使用默认值）*/
//TSDEV=xxxx/*触摸屏设备节点，（默认值/dev/input/event2）*/
//STATUSBAR=xxx /*状态栏高度（0-64，默认值0，设置不支持的值时均使用默认值）*/
//示例：char *xui_argv[] = {"ROTATE=90","STATUSBAR=18"};
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
			gUiDataAll.Screen_fd=open_screen(pTag,&gUiDataAll.tHardWindow);
			if(gUiDataAll.Screen_fd < 0) 
			{
				TRACE("->main open screen ret NULL \r\n");
				return 2;
			}
			continue;
		}
		pTag=API_eStrstr(argv[i],"INPUT=");
		if(pTag)
		{
			//TRACE("Open Input[%d]\r\n",pTag);
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
			//TRACE("gUiDataAll iRotate[%d]\r\n",gUiDataAll.iRotate);
			continue;
		}
		pTag=API_eStrstr(argv[i],"TSDEV=");
		if(pTag)
		{
			//TRACE("Open TSDEV[%d]\r\n",pTag);
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
			//TRACE("Open iStatusbar[%d]\r\n",gUiDataAll.iStatusbar);
			continue;
		}
	}
	return 0;
}

//关闭XUI
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
	if(gUiDataAll.Screen_fd < 0)
	{
		close_screen();
		free(gUiDataAll.tHardWindow.wBack);
	}
	memset(&gUiDataAll,0x00,sizeof(gUiDataAll));
}


//判断XUI 是否正在运行。
int XuiIsRunning(void)
{

	return 0;
}


//暂停XUI 运行。
int XuiSuspend(void)
{

	return 0;
}

//恢复XUI 运行
int XuiResume(void)
{

	return 0;
}

//XuiWindow *XuiRootCanvas(void);
#define FB_WIDTH	260
#define FB_HEIGHT	340

//获取根画布
XuiWindow *XuiRootCanvas(void)
{
	if(gUiDataAll.Screen_fd < 0)
	{
		TRACE("Xui Root Canvas Screen_fd=%d\r\n",gUiDataAll.Screen_fd);
		return NULL;
	}
	//-------------------------------------------
	#ifdef DISPLAY_HORIZONTAL_SCREEN
	gUiDataAll.tHardWindow.left = gUiDataAll.tHardWindow.width-SCREEN_HEIGT-10;
	#else
	gUiDataAll.tHardWindow.left = gUiDataAll.tHardWindow.width-SCREEN_WIDTH-10;
	#endif
	gUiDataAll.tHardWindow.top = 10;
	{
		s16 staX,staY;
		staX=gUiDataAll.tHardWindow.left-10;
		staY=0;
		xui_fb_fill_rect(staX,staY,FB_WIDTH,FB_HEIGHT,RGB_CURR(0x7f,0x7f,0x7f));
	}
	
	if(gUiDataAll.tHardWindow.width < SCREEN_WIDTH || gUiDataAll.tHardWindow.height < SCREEN_HEIGT)
	{
		TRACE("InitRootCanvas Err too small\r\n");
		return NULL;
	}

	//-------------------------------------------
	XuiWindow *Window;
	u16 width,height;
	int nWindLen;
	width=SCREEN_WIDTH;
	height= SCREEN_HEIGT-gUiDataAll.iStatusbar;
	nWindLen =sizeof(XuiWindow) +  (height*width)*sizeof(A_RGB);
	Window = (XuiWindow *)malloc(nWindLen);
	if(Window==NULL)
	{
		TRACE("InitRootCanvas malloc(%d) is NULL\r\n",nWindLen);
		return NULL;
	}
	u32_memset((u32*)Window,0x0000000,nWindLen/4);
	Window->widget = (A_RGB*)((u8*)Window + sizeof(XuiWindow));	
	Window->left=gUiDataAll.tHardWindow.left;
	Window->top=gUiDataAll.tHardWindow.top + gUiDataAll.iStatusbar;
	Window->width = width;
	Window->height = height;
	//TRACE("InitRootCanvas [%X][%X]\r\n",(u32)Window,(u32)Window->widget);
	return Window;
}

//获取状态栏画布
XuiWindow *XuiStatusbarCanvas(void)
{
	XuiWindow *Window;
	u16 width,height;
	int nWindLen;
	if(gUiDataAll.Screen_fd < 0)
	{
		TRACE("Xui Status barCanvas .Screen_fd=%d\r\n",gUiDataAll.Screen_fd);
		return NULL;
	}
	width=SCREEN_WIDTH;
	height= gUiDataAll.iStatusbar;
	nWindLen =sizeof(XuiWindow) +  (height*width)*sizeof(A_RGB);
	Window = (XuiWindow *)malloc(nWindLen);
	u32_memset((u32*)Window,0x0000000,nWindLen/4);
	Window->widget = (A_RGB*)((u8*)Window + sizeof(XuiWindow));	
	Window->left=gUiDataAll.tHardWindow.left;
	Window->top=gUiDataAll.tHardWindow.top;
	Window->width = width;
	Window->height = height;
	return Window;
}


XuiWindow *XuiCreateCanvas(XuiWindow *parent, unsigned int x, unsigned int y,unsigned int width, unsigned int height)
{
	XuiWindow *Window;
	int nWindLen;
	if((x+width)> parent->width || (y+height)> parent->height)
		return NULL;
	
	nWindLen =sizeof(XuiWindow) + (height*width)*sizeof(A_RGB);

	Window = (XuiWindow *)malloc(nWindLen);
	u32_memset((u32*)Window,0x0000000,nWindLen/4);
	Window->widget = (A_RGB*)((u8*)Window + sizeof(XuiWindow));	
	Window->left=parent->left+ x;
	Window->top =parent->top + y;
	Window->width = width;
	Window->height = height;
	Window->pChild = parent;

	if(parent->pParent==NULL)
	{
		parent->pParent = Window;
	}
	else
	{
		XuiWindow *pParentNext;
		pParentNext = parent->pParent;
		while(pParentNext->pNext)
		{
			pParentNext=pParentNext->pNext;
		}
		pParentNext->pNext = Window;
	}
	return Window;
}







/*
//创建字体
XuiFont *XuiCreateFont(char *fontfile,int index,XuiFontSet fontset);
{
	return NULL;
}

int XuiCanvasDrawText(XuiWindow *window,unsigned int x,unsigned int y,unsigned int height,XuiFont *font,XuiTextStyle textstyle,XuiColor fg,char *text)
{
	return 0;
}

*/
void UI_Push(XuiWindow *pWindow,RECTL *pRect)
{
	xui_fb_push(pWindow,pRect,pWindow->widget);
}


void XuiCanvasSetBackground(XuiWindow *pWindow,int bgstyle,void *img,A_RGB bg)
{
	u16 y,x;
	u16 width,height;
	A_RGB* pWidget;
	width	= pWindow->width;
	height = pWindow->height;
	free(pWindow->wBack);
	pWidget = (A_RGB*)malloc((height * width)*sizeof(A_RGB));
	if(img)
	{


	}
	else for (y = 0; y < height; y++) 
	{
		for(x=0; x<width; x++)
		{
			*pWidget++ = bg;
		}
	}
	xui_fb_push(pWindow,NULL,pWidget);
	pWindow->wBack = pWidget;
}

//================================================================================================================
void FreeParentWindow(XuiWindow *window)
{
	XuiWindow *pNext;
	if(window->pParent)
	{
		FreeParentWindow(window->pParent);
		window->pParent = NULL;
	}
	while(window)
	{	
		pNext = window->pNext;
		free(window->wBack);
		free(window);
		window= pNext;
	}
}

void XuiDestroyWindow(XuiWindow *window)
{
	XuiWindow *pParent;
	pParent= window->pParent;
	if(window->pParent)
	{	
		FreeParentWindow(window->pParent);
	}
	if(window->pChild)
	{//-----拆链表------
		pParent=window->pChild;
		if(pParent->pParent != window)
		{
			pParent = pParent->pParent;
			while(pParent->pNext != window)
				pParent= pParent->pNext;
		}
		pParent->pNext=window->pNext;
	}
	
	if(window->pChild)
	{
		RECTL tRect;
		pParent=window->pChild;
		tRect.left = window->left - pParent->left;
		tRect.top= 	window->top - pParent->top;
		tRect.width= window->width;
		tRect.height= window->height;
		xui_fb_push(pParent,&tRect,pParent->widget);
	}
	else if(window->wBack)
	{//-----根画布------
		xui_fb_push(window,NULL,window->wBack);
	}
	free(window->wBack);
	free(window);
}

int XuiClearArea(XuiWindow *window, unsigned int x,unsigned int y, unsigned int width, unsigned int height)
{
	u16 i,j,w,h;
	u16 mWidth,mChildw;
	A_RGB *pBack,*pWidget;
	A_RGB *destin,*source;
	
	w = x + width;
	h = y + height;
	mWidth = window->width;
	pWidget = window->widget;
	if(window->wBack)
	{
		pBack = window->wBack;
		for (j = y; j < h; j++) 
		{
			source=&pBack[j*mWidth + x];
			destin=&pWidget[j*mWidth + x];
			for(i=x; i<w; i++)
			{
				*destin++ = *source++;
			}
		}
	}
	else if(window->pChild)
	{
		u16 xOff,yOff;
		xOff = window->left - window->pChild->left;
		yOff = window->top  - window->pChild->top;;
		pBack = window->pChild->widget;
		mChildw = window->pChild->width;
		for (j = y; j < h; j++) 
		{
			source=&pBack[(yOff+j)*mChildw + xOff+x];
			destin=&pWidget[j*mWidth + x];
			for(i=x; i<w; i++)
			{
				*destin++ = *source++;
			}
		}
	}
	else return -1;

	{
		RECTL tRect;
		tRect.left = window->left + x;
		tRect.top=	window->top + y;
		tRect.width= width;
		tRect.height= height;
		xui_fb_push(window,&tRect,window->widget);
	}
	return 0;
}


void XuiShowWindow(XuiWindow *window,int show, int flag)
{
	RECTL tRect;
	tRect.left = window->left;
	tRect.top=	window->top;
	tRect.width= window->width;
	tRect.height= window->height;
	if(show == 1)
	{
		xui_fb_push(window,&tRect,window->widget);
	}
	else
	{
		if(window->wBack)
		{
			xui_fb_push(window,&tRect,window->wBack);
		}
		else if(window->pChild)
		{
			xui_fb_push(window->pChild,&tRect,window->pChild->widget);
		}
	}
}


void UI_SetBackground(XuiWindow *pWindow,FunFillColour pFillColour)	//(u32* pOut,int width,int height)
{
	if(pWindow->wBack == NULL)
		pWindow->wBack = (A_RGB*)malloc((pWindow->height * pWindow->width)*sizeof(A_RGB));
	pFillColour(pWindow->wBack,pWindow->width,pWindow->height);
}


void UI_vline(XuiWindow *pWindow,POINT *pRect,int width,A_RGB Color)
{
	u16 sX,eX;
	A_RGB *destinE,*destinS;
	sX = pRect->left;
	if(pRect->top>=pWindow->height || sX >= pWindow->width)
		return;

	eX = sX+width;
	if(eX > pWindow->width) eX=pWindow->width;
	
	destinS=&pWindow->widget[pRect->top*pWindow->width + sX];
	destinE=destinS+(eX-sX);
	while(destinS < destinE)
	{
		*destinS++ = Color;
	}
}

void UI_hline(XuiWindow *pWindow,POINT *pRect,int height,A_RGB Color)
{
	u16 sH,eH,width;
	A_RGB *destinE,*destinS;
	sH = pRect->top;
	width = pWindow->width;
	if(pRect->left >= width || sH>=pWindow->height)
		return;
	eH = sH + height;
	if(eH > pWindow->height) eH=pWindow->height;
	
	destinS=&pWindow->widget[sH*width + pRect->left];
	destinE=destinS + (eH-sH)*width;
	while(destinS < destinE)
	{
		*destinS = Color;
		destinS += width;
	}
}

void UI_FillRectSingLe(XuiWindow *pWindow,RECTL *pRect,A_RGB Color)
{
	u16 sX,sY,eX,eY,i,mWidth;
	A_RGB* pWidget;
	A_RGB *destin;
	sX = pRect->left;
	sY = pRect->top;
	if(sY >= pWindow->height || sX >= pWindow->width)
		return;
	mWidth	= pWindow->width;
	eX = sX+pRect->width;
	if(eX > mWidth) eX=mWidth;

	eY = sY+pRect->height;
	if(eY > pWindow->height) 
		eY=pWindow->height;
	pWidget = pWindow->widget;
	while(sY < eY)
	{
		destin=pWidget+(sY*mWidth + sX);
		for(i=sX; i<eX; i++)
		{
			*destin++ = Color;
		}
		sY++;
	}
}

/*
void UI_SetRectBuff(XuiWindow *pWindow,RECTL *pRect,A_RGB *pRGB)
{
	u16 i,j,sx,sy,w,h;
	int mWidth;
	A_RGB *destin;
	sx = pRect->left;
	sy = pRect->top;
	
	mWidth = pWindow->width;
	w = sx+mWidth;
	if(w > mWidth) w=mWidth;
	h = sy+pRect->height;
	if(h > pWindow->height) h=pWindow->height;
	
	for (j = sy; j < h; j++) 
	{
		destin=&pWindow->widget[j*mWidth + sx];
		for(i=sx; i<w; i++)
		{
			*destin++ = *pRGB++;
		}
	}
}

void UI_GetRectBuff(XuiWindow *pWindow,RECTL *pRect,A_RGB *pRGB)
{
	u16 i,j,sx,sy,w,h;
	int mWidth;
	A_RGB *destin;
	sx = pRect->left;
	sy = pRect->top;
	mWidth = pWindow->width;
	w = sx+mWidth;
	if(w > mWidth) w=mWidth;
	h = sy+pRect->height;
	if(h > pWindow->height) h=pWindow->height;
	for (j = sy; j < h; j++) 
	{
		destin=&pWindow->widget[j*mWidth + sx];
		for(i=sx; i<w; i++)
		{
			*pRGB++ = *destin++;
		}
	}
}
*/

//static u8 SPF_runLock=FALSE;
int UI_ShowPictureFile(XuiWindow *pWindow,RECTL *prclTrg,const char *pfilePath)
{
	
	return 0;
}


//=============底部显示进度条==ratio(0~100)===============================
void UI_ShowBottomProgress(XuiWindow *pWindow,int ratio)
{
	RECTL tRect;
	tRect.width=SCREEN_WIDTH;
	tRect.left = 0; 
	tRect.height=16;
	tRect.top = SCREEN_HEIGT-tRect.height;
	UI_FillRectSingLe(pWindow,&tRect,RGB_CURR(199,200,244));
	if(ratio)
	{
		tRect.top	+=	3;
		tRect.left	+=	3;
		tRect.width -=	6;
		tRect.height -=	6;
		if(ratio < 100)
			tRect.width = tRect.width*ratio/100;
		UI_FillRectSingLe(pWindow,&tRect,RGB_CURR(0,255,255));
	}
}

//=============显示滑轨==ratio(0~100)===============================
void UI_ShowParMiddleSlide(XuiWindow *pWindow,int ratio)
{
	u16 Width1,Width2;
	RECTL tRect;
	tRect.width = SCREEN_APP_W-8;
	tRect.height = 32;
	Width1 = (tRect.width-tRect.height)*ratio/100;
	Width2 = (tRect.width-tRect.height)-Width1;
	tRect.top = SCREEN_APP_Y+(SCREEN_APP_H-32)/2;
	tRect.left = SCREEN_APP_X+4;
	UI_FillRectSingLe(pWindow,&tRect,RGB_CURR(255,255,255));

	
	tRect.top = SCREEN_APP_Y+(SCREEN_APP_H-16)/2;
	if(Width1)
	{
		tRect.left = SCREEN_APP_X+4;
		tRect.width = Width1;
		tRect.height = 16;
		UI_FillRectSingLe(pWindow,&tRect,RGB_CURR(210,99,25));
	}
	if(Width2)
	{
		tRect.left = SCREEN_APP_X+4+Width1+tRect.height;
		tRect.width = Width2;
		tRect.height = 16;
		UI_FillRectSingLe(pWindow,&tRect,RGB_CURR(56,56,56));
	}
	tRect.left = SCREEN_APP_X+4+Width1;
	tRect.top = SCREEN_APP_Y+(SCREEN_APP_H-32)/2;
	tRect.height = 32;
	UI_FillRectSingLe(pWindow,&tRect,RGB_CURR(199,206,218));
/*
	ret=API_sprintf(sbuff,"%d",par);
	tRect.left = SCREEN_APP_X+(SCREEN_APP_W-ret*FONT_SIZE/2)/2;
	tRect.top = SCREEN_APP_Y+ (SCREEN_APP_H-32)/2  +32;
//	ApiFont.SetFontColor(RGB_CURR(56,56,56),RGB_CURR(255,255,255));
//	ApiFont.DisplayFont(&tRect,(u8*)sbuff);
	DisplayPaintEnd();
	*/
}


void UI_ShowQrCode(XuiWindow *pWindow,RECTL* pRect,const char* pInfo,A_RGB Color)
{
	IMAGE image;
	if(0==Lib_QrCodeImg(&image,pInfo,FALSE))
	{
		RECTL oImage;
		u8 *pByte;
		A_RGB *pbgra;
		u16 x,y,ix,mx,iy,my;
		u16 width;//height,width,
		A_RGB *pWidget;
		//RECTL tRect;
		//----保持长宽比，按最小宽度放大--------------
		mx = MIN(pRect->width,pRect->height)/image.w;
		my = mx;
		//------不保持长宽比，按指定区域放大---------
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

	//	width	= pWindow->width;
	//	height	= pWindow->height;
		width = pWindow->width;
		pWidget = pWindow->widget;
		for(x=0 ;x<image.w ; x++)
		{
			pByte = &image.pByte[x*image.mline];
			for(y = 0; y < image.h; y++) 
			{
				if(pByte[y])
				{
					for(iy=0;iy<my;iy++)	
					{
						pbgra=&pWidget[(oImage.top + y*my+iy)*width + oImage.left + x*mx];
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

	XuiRootCanvas,
	XuiStatusbarCanvas,
	XuiShowWindow,
	XuiDestroyWindow,

	UI_Push,
	UI_FillRectSingLe,
	
	UI_ShowQrCode,
	UI_ShowPictureFile,
	UI_ShowBottomProgress,
	UI_ShowParMiddleSlide,
};

