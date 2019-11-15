

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
 * GUI支持的图像格式定义
 */
typedef struct
{
	u16					w;				//!< 图像宽
	u16					h;				//!< 高
	u16					mline,len;				//!< 高
	u8*					pByte;			
}IMAGE;									



typedef struct {
	unsigned short left,top;
	unsigned short width;	//Window 瀹藉害
	unsigned short height;	//Window 楂樺害
	unsigned short key;		//鍏宠仈鎸夐敭鍊�
	unsigned short type;	//Window 绐楀彛绫诲瀷锛岃瑙乆uiWindowType
	unsigned int* widget;	//Window 鍏宠仈鐢诲竷鎸囬拡
	void *fb;
} XuiWindow;
extern XuiWindow UI_screen;

//extern int UI_RootCanvas(screen_buffer* fb);
//extern void XuiClose(void);
typedef struct {
	int SetFlag;	//是否初始化标记
	int iRotate;	//旋转方向 0，90 ，180 ，270 
	int iStatusbar;	///*状态栏高度（0-64，默认值0，设置不支持的值时均使用默认值）*/
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

