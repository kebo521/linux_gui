

#ifndef _UI_DISPLAY_
#define _UI_DISPLAY_

//==========================================================
typedef struct {/*
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;*/

  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
} rgba_t;
typedef unsigned int  			A_RGB;		// <= RGB_CURR
typedef union {
	A_RGB uPix;
	rgba_t tPix;
}Pixel_Color;


#define	RGB_CURR(r,g,b)		((r*0x10000)|(g*0x100)|b)	//(u32)(rgba_t)(b,g,r,0xff))





typedef void (*FunFillColour)(A_RGB*,int,int);

/**
 * GUIÖ§³ÖµÄÍ¼Ïñ¸ñÊ½¶¨Òå
 */
typedef struct
{
	u16					w;				//!< Í¼Ïñ¿í
	u16					h;				//!< ¸ß
	u16					mline,len;				//!< ¸ß
	u8*					pByte;			
}IMAGE;									



typedef struct _XuiWindow{
	struct _XuiWindow *pChild;	//¸¸Ò»¸ö´°¿Ú
	struct _XuiWindow *pParent;	//×Ó´°¿Ú
	struct _XuiWindow *pNext;		//×Ó´°¿Ú....
	
	unsigned short left,top,width,height;
	
//	unsigned short key;		//å…³è”æŒ‰é”®å€¼
//	unsigned short type;	//Window çª—å£ç±»åž‹ï¼Œè¯¦è§XuiWindowType
	
	A_RGB* 		wBack;	//Window ±³¾°É«£¬ÎÞ ¿É½èÓÃ¸´´°¿Ú»¹Ô­
	A_RGB* 		widget;	//ÓÃ½á¹¹ÌåºóÃæµÄ¿Õ¼ä²»ÐèÒªÊÍ·Å£¬Window å…³è”ç”»å¸ƒæŒ‡é’ˆ
} XuiWindow;
//extern XuiWindow UI_screen;

//extern int UI_RootCanvas(screen_buffer* fb);
typedef struct {
	int SetFlag;	//ÊÇ·ñ³õÊ¼»¯±ê¼Ç
	int iRotate;	//Ðý×ª·½Ïò 0£¬90 £¬180 £¬270 
	int iStatusbar;	///*×´Ì¬À¸¸ß¶È£¨0-64£¬Ä¬ÈÏÖµ0£¬ÉèÖÃ²»Ö§³ÖµÄÖµÊ±¾ùÊ¹ÓÃÄ¬ÈÏÖµ£©*/
	int keys_fd;
	int TsDev_fd;
	int Screen_fd;
	XuiWindow tHardWindow;
//	u16 left,top;
}gUi_def;
extern gUi_def gUiDataAll;
extern int XuiOpen(int argc,char **argv);
extern void XuiClose(void);

extern XuiWindow *XuiRootCanvas(void);
extern XuiWindow *XuiStatusbarCanvas(void);
extern XuiWindow *XuiCreateCanvas(XuiWindow *parent, unsigned int x, unsigned int y,unsigned int width, unsigned int height);
extern void UI_Push(XuiWindow *pWindow,RECTL *pRect);
extern void XuiCanvasSetBackground(XuiWindow *pWindow,int bgstyle,void *img,A_RGB bg);
extern void XuiDestroyWindow(XuiWindow *window);
extern int XuiClearArea(XuiWindow *window, unsigned int x,unsigned int y, unsigned int width, unsigned int height);
extern void XuiShowWindow(XuiWindow *window,int show, int flag);

extern void UI_SetBackground(XuiWindow *pWindow,FunFillColour pFillColour);	//(u32* pOut,int width,int height)
extern void UI_vline(XuiWindow *pWindow,POINT *pRect,int width,A_RGB Color);
extern void UI_hline(XuiWindow *pWindow,POINT *pRect,int height,A_RGB Color);
extern void UI_FillRectSingLe(XuiWindow *pWindow,RECTL *pRect,A_RGB Color);


typedef struct _API_UI	
{
	char Mask[4]; 	// "UI"
	int (*open)(int,char **);	//(int argc,char **argv)
	void (*close)(void);

	XuiWindow* (*RootCanvas)(void);
	XuiWindow* (*StatusbarCanvas)(void);
	void (*ShowWindow)(XuiWindow *,int,int);	//XuiWindow *,int show, int flag
	void (*DestroyWindow)(XuiWindow *);

	void (*Push)(XuiWindow *,RECTL*);	//Cache area is pushed to video memory,(RECTL==NULL,Show full area)

	void (*FillRectSingLe)(XuiWindow *,RECTL*,A_RGB);	//(xywh,RGB_CURR(r,g,b))

	void (*ShowQrCode)(XuiWindow *,RECTL* ,const char*,A_RGB);	//(xywh,"Text",RGB_CURR(r,g,b))
	int (*ShowPictureFile)(XuiWindow *,RECTL *,const char *);
	void (*ShowBottomProgress)(XuiWindow *,int);	//ratio (0~100)
	void (*ShowParMiddleSlide)(XuiWindow *,int); //ratio (0~100)
}API_UI_Def;


extern const API_UI_Def ApiUI;

#endif

