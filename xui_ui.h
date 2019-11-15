

#ifndef _UI_DISPLAY_
#define _UI_DISPLAY_

//==========================================================
typedef struct{
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
}gUIrgba;
#define	RGB_CURR(r,g,b)		((r*0x10000)|(g*0x100)|b)	//(u32)(gUIrgba)(b,g,r,0xff))


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



typedef struct {
	unsigned short left,top;
	unsigned short width;	//Window å®½åº¦
	unsigned short height;	//Window é«˜åº¦
	unsigned short key;		//å…³è”æŒ‰é”®å€¼
	unsigned short type;	//Window çª—å£ç±»åž‹ï¼Œè¯¦è§XuiWindowType
	unsigned int* widget;	//Window å…³è”ç”»å¸ƒæŒ‡é’ˆ
	void *fb;
} XuiWindow;
extern XuiWindow UI_screen;

//extern int UI_RootCanvas(screen_buffer* fb);
//extern void XuiClose(void);
typedef struct {
	int SetFlag;	//ÊÇ·ñ³õÊ¼»¯±ê¼Ç
	int iRotate;	//Ðý×ª·½Ïò 0£¬90 £¬180 £¬270 
	int iStatusbar;	///*×´Ì¬À¸¸ß¶È£¨0-64£¬Ä¬ÈÏÖµ0£¬ÉèÖÃ²»Ö§³ÖµÄÖµÊ±¾ùÊ¹ÓÃÄ¬ÈÏÖµ£©*/
	int keys_fd;
	int TsDev_fd;
	void* pFbMsg;
}gUi_def;
extern gUi_def gUiDataAll;



typedef struct	
{
	char Mask[4]; 	// "UI"
	int (*open)(int,char **);	//(int argc,char **argv)
	void (*close)(void);

	int (*RootCanvas)(void);
	int(*StatusbarCanvas)(void);

	void (*Push)(RECTL*);	//Cache area is pushed to video memory,(RECTL==NULL,Show full area)
	void (*FillRectSingLe)(RECTL*,u32);	//(xywh,RGB_CURR(r,g,b))
	void (*SetRectBuff)(RECTL*,gUIrgba*);	//(xywh,gUIrgba...)
	void (*GetRectBuff)(RECTL*,gUIrgba*); //(xywh,gUIrgba...)

	void (*ShowQrCode)(RECTL* ,const char*,u32);	//(xywh,"Text",RGB_CURR(r,g,b))
	int (*ShowPictureFile)(RECTL *,const char *);
	void (*ShowBottomProgress)(int);	//ratio (0~100)
	void (*ShowParMiddleSlide)(int); //ratio (0~100)
}API_UI_Def;


extern const API_UI_Def ApiUI;

#endif

