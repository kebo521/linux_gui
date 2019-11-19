

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



typedef struct _XuiWindow{
	struct _XuiWindow *pChild;	//��һ������
	struct _XuiWindow *pParent;	//�Ӵ���
	struct _XuiWindow *pNext;		//�Ӵ���....
	
	unsigned short left,top,width,height;
	unsigned int wLen;		//ÿһ�е�bety ��
	
	unsigned short key;		//关联按键值
	unsigned short type;	//Window 窗口类型，详见XuiWindowType
	
	
	unsigned int* wBack;	//Window ����ɫ���� �ɽ��ø����ڻ�ԭ
	unsigned int* widget;	//�ýṹ�����Ŀռ䲻��Ҫ�ͷţ�Window 关联画布指针
} XuiWindow;
//extern XuiWindow UI_screen;

//extern int UI_RootCanvas(screen_buffer* fb);
//extern void XuiClose(void);
typedef struct {
	int SetFlag;	//�Ƿ��ʼ�����
	int iRotate;	//��ת���� 0��90 ��180 ��270 
	int iStatusbar;	///*״̬���߶ȣ�0-64��Ĭ��ֵ0�����ò�֧�ֵ�ֵʱ��ʹ��Ĭ��ֵ��*/
	int keys_fd;
	int TsDev_fd;
	int Screen_fd;
	XuiWindow tHardWindow;
	u16 left,top;
}gUi_def;
extern gUi_def gUiDataAll;



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

	void (*FillRectSingLe)(XuiWindow *,RECTL*,u32);	//(xywh,RGB_CURR(r,g,b))
	void (*SetRectBuff)(XuiWindow *,RECTL*,gUIrgba*);	//(xywh,gUIrgba...)
	void (*GetRectBuff)(XuiWindow *,RECTL*,gUIrgba*); //(xywh,gUIrgba...)

	void (*ShowQrCode)(XuiWindow *,RECTL* ,const char*,u32);	//(xywh,"Text",RGB_CURR(r,g,b))
	int (*ShowPictureFile)(XuiWindow *,RECTL *,const char *);
	void (*ShowBottomProgress)(XuiWindow *,int);	//ratio (0~100)
	void (*ShowParMiddleSlide)(XuiWindow *,int); //ratio (0~100)
}API_UI_Def;


extern const API_UI_Def ApiUI;

#endif

