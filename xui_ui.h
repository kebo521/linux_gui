

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
 * GUI֧�ֵ�ͼ���ʽ����
 */
typedef struct
{
	u16					w;				//!< ͼ���
	u16					h;				//!< ��
	u16					mline,len;				//!< ��
	u8*					pByte;			
}IMAGE;									



typedef struct {
	unsigned short left,top;
	unsigned short width;	//Window 宽度
	unsigned short height;	//Window 高度
	unsigned short key;		//关联按键值
	unsigned short type;	//Window 窗口类型，详见XuiWindowType
	unsigned int* widget;	//Window 关联画布指针
	void *fb;
} XuiWindow;
extern XuiWindow UI_screen;

//extern int UI_RootCanvas(screen_buffer* fb);
//extern void XuiClose(void);
typedef struct {
	int SetFlag;	//�Ƿ��ʼ�����
	int iRotate;	//��ת���� 0��90 ��180 ��270 
	int iStatusbar;	///*״̬���߶ȣ�0-64��Ĭ��ֵ0�����ò�֧�ֵ�ֵʱ��ʹ��Ĭ��ֵ��*/
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

