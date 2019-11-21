

#ifndef _GUI_DISPLAY_
#define _GUI_DISPLAY_




#define		OPER_OK				0
	
#define	 	OPER_RET			-1
#define	 	OPER_QUIT			-2
#define	 	OPER_TIMEOUT		-3
#define    	OPER_RDMagERR     	-4 		//������
#define    	OPER_CRCERR     	-5 		//У��ʧ��
#define		OPER_ERR			-6		//����ʧ��
#define 	OPER_SYSTEM_ERROR	-7		//����ʧ��
#define 	OPER_TRANS_OVER		-8	    //����������ȷ����ʾʧ��msg
#define		OPER_OFFLINE		-11		//�¼�
#define		OPER_LOW_VOLTAGE	-15		//��ѹ����
#define		OPER_HARD_Err		-16		//Ӳ������
#define		OPER_SEND_ERR		-1000	//��������ʧ��
#define		OPER_RECV_ERR		-2000	//��������ʧ��


#define		OPER_NO_CHECK		15		//��ǩ������
#define		OPER_NoEND 			6		//�ɹ�����δ����	
#define		OPER_NoHEAD  		5		//δ�ҵ�ͷ��
#define		OPER_HARD			4		//�ֹ�����
#define		OPER_NEW			3		//����������
#define		OPER_PAY			1		//ȷ��֧��

#define		SOCKET_UN_APN			-101		//δ��APN
#define		SOCKET_UN_IP			-102		//δ����IP
#define		SOCKET_UN				-200		//δ����IP


typedef enum 
{
	GUI_SHOW_MSG	= 0x11	,		//��ͨ��ʾ����
	GUI_EDIT_MSG	= 0x21	,		//�༭����
	GUI_MENU_LINE	= 0x51	,		//һά�˵� 
	GUI_MENU_TWO	= 0x52	,		//��ά�˵�
}GUI_TYPE;			//V1.2	

/**
 * �ı��������Ͷ���
 */
typedef enum 
{
	IME_NONE			=0x00000000,		//!< NONE
	IME_NUM			=0x00000001,		//!< ����
	IME_abc			=0x00000002,		//!< Сд��ĸ
	IME_ABC			=0x00000004,		//!< ��С��ĸ
	IME_Abc			=0x00000008,		//!< ��Сд�����ĸ
	IME_SYM			=0x00000010,		//!< ����
	IME_SUM			=0x00000020,		//!< ���
	IME_PY				=0x00000040,		//!< ƴ�� ֧��GB2312����
	IME_PYGBK			=0x00000080,		//!< ȫƴ ֧��GBK����
	IME_FMT			=0x00000100,		//!< ��ʽ��ģʽ�������룬��IP,�˺ŵ�.
	IME_SCAN			=0x00000200,		//!< ɨ������
	IME_MASK			=0x000003FF,		//!< ����
	IME_PIN_MODE		=0x10000000,		//!< ����ģʽ	
	IME_SYM_MODE		=0x20000000,		//!< ����ģʽ	
	IME_ENG			=IME_ABC|IME_abc|IME_Abc, //!< Ӣ������
	IME_END	
}IME_TYPE;			//V1.2	


#define FONT_SIZE    			(24)    
#define HZ_INDEX(hz)    		((hz[0] - 0xa1) * 94 + (hz[1] - 0xa1))

#define RGB565_WITHE		RGB_CURR(0xff,0xff,0xff)	//0x0000FFFF
#define RGB565_BLACK		RGB_CURR(0x00,0x00,0x00)	//0x00000000
#define RGB565_PARENT		0xFF000000

#define RGB565_RED			RGB_CURR(0xff,0x00,0x00)
#define RGB565_GREEN		RGB_CURR(0x00,0xff,0x00)	//0x07E0
#define RGB565_BLUE		RGB_CURR(0x00,0x00,0xff)	//0x001F

#ifdef DISPLAY_HORIZONTAL_SCREEN			//��������
//---------ȫ������----------------------
#define SCREEN_WIDTH		(320) 
#define SCREEN_HEIGT		(240) 
#define EFFECTIVE_X			(0) 
#define EFFECTIVE_Y			(24)
//---------Ӧ��ʹ�õ�������-------------------
#define SCREEN_APP_X		(EFFECTIVE_X) 
#define SCREEN_APP_Y		(EFFECTIVE_Y)
#define SCREEN_APP_W		(SCREEN_WIDTH) 
#define SCREEN_APP_H		(SCREEN_HEIGT-EFFECTIVE_Y) 
//-------ͼƬ���������������ʾ-------------------------------
#define UI_MAP_TITLE_X		SCREEN_APP_X
#define UI_MAP_TITLE_Y		SCREEN_APP_Y
#define UI_MAP_TITLE_W		SCREEN_APP_W 
#define UI_MAP_TITLE_H		40 

#define UI_MAP_CONT_X		SCREEN_APP_X
#define UI_MAP_CONT_Y		(SCREEN_APP_Y+UI_MAP_TITLE_H)
#define UI_MAP_CONT_W		SCREEN_APP_W 
#define UI_MAP_CONT_H		(SCREEN_APP_H-UI_MAP_TITLE_H)
//----------ϵͳ������������---------------------------------
#define UI_TITLE_X			SCREEN_APP_X
#define UI_TITLE_Y			SCREEN_APP_Y
#define UI_TITLE_W			SCREEN_APP_W 
#define UI_TITLE_H			36 
#define UI_TITLE_COLOUR		RGB_CURR(24,120,200) 
#define FONT_TITLE_COLOUR	RGB565_WITHE 

#define UI_CONT_X			SCREEN_APP_X
#define UI_CONT_Y			(SCREEN_APP_Y+UI_TITLE_H)
#define UI_CONT_W			SCREEN_APP_W 
#define UI_CONT_H			(SCREEN_APP_H-UI_TITLE_H)
#define UI_CONT_COLOUR		RGB_CURR(240,244,248)
#define FONT_CONT_COLOUR	RGB565_BLACK 
//----------���Ʋ˵�����----------------------
#define SCREEN_MENU_X		(SCREEN_APP_X) 
#define SCREEN_MENU_Y		(SCREEN_APP_Y)
#define SCREEN_MENU_W		(SCREEN_APP_W) 
#define SCREEN_MENU_H		(SCREEN_APP_H) 
#define MENU_TITLE_H		36

#define MENU_TIEM_ALL_Y		(SCREEN_MENU_Y+MENU_TITLE_H)
#define MENU_TIEM_ALL_H		(SCREEN_APP_H-MENU_TITLE_H)
#define MENU_TIEM_H			30
#define MENU_TIEM_MAX		((SCREEN_APP_H-MENU_TITLE_H)/MENU_TIEM_H)			//���˵���
//------------�˵�ɫ�ʶ���-----------------
#define RGB565_TITLE_ICON		RGB_CURR(0,127,255)   //0x03ff
#define RGB565_TITLE_FONT		RGB565_WITHE
#define RGB565_TIEM_ICON		UI_CONT_COLOUR
#define RGB565_TIEM_FONT		RGB_CURR(48,48,48)	//0x3186
#define RGB565_SELE_ICON		RGB_CURR(224,224,224)	//0xe71c
#define RGB565_SELE_FONT		RGB_CURR(0,127,255)	//0x03ff

#else		//===============================================================================

//---------ȫ������----------------------
#define SCREEN_WIDTH		(240) 
#define SCREEN_HEIGT		(320) 
#define EFFECTIVE_X			(0) 
#define EFFECTIVE_Y			(24)
//---------Ӧ��ʹ�õ�������-------------------
#define SCREEN_APP_X		(EFFECTIVE_X) 
#define SCREEN_APP_Y		(EFFECTIVE_Y)
#define SCREEN_APP_W		(SCREEN_WIDTH) 
#define SCREEN_APP_H		(SCREEN_HEIGT-EFFECTIVE_Y) 
//-------ͼƬ���������������ʾ-------------------------------
#define UI_MAP_TITLE_X		SCREEN_APP_X
#define UI_MAP_TITLE_Y		SCREEN_APP_Y
#define UI_MAP_TITLE_W		SCREEN_APP_W 
#define UI_MAP_TITLE_H		40 

#define UI_MAP_CONT_X		SCREEN_APP_X
#define UI_MAP_CONT_Y		(SCREEN_APP_Y+UI_MAP_TITLE_H)
#define UI_MAP_CONT_W		SCREEN_APP_W 
#define UI_MAP_CONT_H		(SCREEN_APP_H-UI_MAP_TITLE_H)
//----------ϵͳ������������---------------------------------
#define UI_TITLE_X			SCREEN_APP_X
#define UI_TITLE_Y			SCREEN_APP_Y
#define UI_TITLE_W			SCREEN_APP_W 
#define UI_TITLE_H			40 
#define UI_TITLE_COLOUR		RGB_CURR(24,120,200) 
#define FONT_TITLE_COLOUR	RGB565_WITHE 

#define UI_CONT_X			SCREEN_APP_X
#define UI_CONT_Y			(SCREEN_APP_Y+UI_TITLE_H)
#define UI_CONT_W			SCREEN_APP_W 
#define UI_CONT_H			(SCREEN_APP_H-UI_TITLE_H)
#define UI_CONT_COLOUR		RGB_CURR(240,244,248)
#define FONT_CONT_COLOUR	RGB565_BLACK 
//----------���Ʋ˵�����----------------------
#define SCREEN_MENU_X		(SCREEN_APP_X) 
#define SCREEN_MENU_Y		(SCREEN_APP_Y)
#define SCREEN_MENU_W		(SCREEN_APP_W) 
#define SCREEN_MENU_H		(SCREEN_APP_H) 
#define MENU_TITLE_H		40

#define MENU_TIEM_ALL_Y		(SCREEN_MENU_Y+MENU_TITLE_H)
#define MENU_TIEM_ALL_H		(SCREEN_APP_H-MENU_TITLE_H)
#define MENU_TIEM_H			32
#define MENU_TIEM_MAX		((SCREEN_APP_H-MENU_TITLE_H)/MENU_TIEM_H)			//���˵���
//------------�˵�ɫ�ʶ���-----------------
#define RGB565_TITLE_ICON		RGB_CURR(0,127,255)   //0x03ff
#define RGB565_TITLE_FONT		RGB565_WITHE
#define RGB565_TIEM_ICON		UI_CONT_COLOUR
#define RGB565_TIEM_FONT		RGB_CURR(48,48,48)	//0x3186
#define RGB565_SELE_ICON		RGB_CURR(224,224,224)	//0xe71c
#define RGB565_SELE_FONT		RGB_CURR(0,127,255)	//0x03ff

#endif



typedef enum
{
	MENU_ICON_NUM		=0x0001,			//ͼ��Ϊ���,	��MENU_LIST���ʹ��
	MENU_ICOM_IMG		=0x0002,			//ͼ��Ϊͼ��,	��MENU_LIST���ʹ��
	MENU_LIST			=0x0100,			//�б�˵�
	MENU_RADIO			=0x0200,			//����ѡ��˵�
	MENU_CHECK			=0x0400,			//����ѡ��˵�
	MENU_MUILT_LINE		=0x1000,			//�����ı�,		�������������ʹ��
	MENU_MAX			=0xFFFF
}UI_MENU_TYPE;
/**
 * �ı���ʾ���Ͷ���
 */
typedef enum
{
	TEXT_3X5				=4,				//!< 3x5�����С
	TEXT_6X8				=6,				//!< 6x8�����С
	TEXT_12					=12,			//!< 12x12�����С
	TEXT_16					=16,			//!< 16x16�����С
	TEXT_20					=20,			//!< 20x20�����С
	TEXT_24					=24,			//!< 24x24�����С
	TEXT_32					=32,			//!< 32x32�����С
	TEXT_SIZE				=0x000000FF,	//!< �����С����
	                    	
	TEXT_NONE				=0x00000000,	//!< ��ͨ�ı�
	TEXT_NORMAL				=0x00000000,	//!< ��ͨ�ı�
	TEXT_SHADOW				=0x00000100,	//!< ��Ӱ��(��ɫ��Ч)
	TEXT_BORDER				=0x00000200,	//!< ������
	TEXT_BOLD				=0x00000400,	//!< �Ӵ�
	TEXT_BACK				=0x00000800,	//!< ������ɫ
	
	TEXT_ALIGN_LEFT			=0x10000000,	//!< ˮƽ�����
	TEXT_ALIGN_CENTER		=0x20000000,	//!< ˮƽ���ж���
	TEXT_ALIGN_RIGHT		=0x40000000,	//!< ˮƽ�Ҷ���
	TEXT_ALIGN_MASK			=0x70000000,	//!< ˮƽ��������
	                    	
	TEXT_VALIGN_TOP			=0x01000000,	//!< ��ֱ�϶���
	TEXT_VALIGN_CENTER		=0x02000000,	//!< ��ֱ���ж���
	TEXT_VALIGN_BOTTOM		=0x04000000,	//!< ��ֱ�¶���
	TEXT_VALIGN_MASK		=0x07000000,	//!< ��ֱ��������

	TEXT_EXSTYLE_UNDERLINE  =0x00100000,	//���»���
	TEXT_EXSTYLE_OVERLINE   =0x00200000,	//���ϻ���
	TEXT_EXSTYLE_BORDER     =0x00400000,	//�ӿ�
	TEXT_LINE_NUM_MASK		=0x000F0000,	//ָ���к�(0~N)
}TEXT_TYPE;	//V1.1


#define TOK							"OK"
#define TCANCEL					"CANCEL"




extern void API_GUI_LoadWindow(XuiWindow *pWindow);
extern XuiWindow *API_GUI_GetCurrWindow(void);

extern void API_FillMenuBack(A_RGB* pBack,int w,int h);
extern void API_FillShowBack(A_RGB* pBack,int w,int h);

//extern int API_Set_Background(FunFillColour pFillColour);

extern int APP_WaitUiEvent(int tTimeOutMS);
//=================================================================================

//extern void UI_DisplayBitMapGet(u16 x,u16 y,u16 w,u16 h);
//extern void UI_DisplayBitMapSet(void);
//extern void UI_DisplayBitMapEND(void);

//==================================================================================
extern void API_GUI_ShowColorRect(RECTL *pRect,u16 Width,A_RGB Color);
extern void API_GUI_InputEdit(char* pStrDef,int tMaxLen,u32 Way,Fun_ShowNum pShow);


extern void API_GUI_Show(void);

//===============�����ʾ����(ָ������)=================================================
extern void API_GUI_ClearScreen(RECTL* pRect);
/**
 * ����һ����������GUI���潻��
 *
 * @param pTitle 		�������
 * @param pOk 			��ť���д���OK������ʾ, ΪNULL�����ڸð�ť
 * @param pCancel 		��ť���д���Cancel������ʾ, ΪNULL�����ڸð�ť
 * @param tGuiType 		������ʾ���� {@link WIN_TYPE} ���� WIN_FULL|WIN_NOBTN ���ߡ�WIN_POP|WIN_NOBTN
 * @return {@link RET_OK} 						�ɹ�
 * @return {@link RET_API_PARAMETER_ERROR}		������������
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		���ܲ�֧��
 * @return {@link RET_SYSTEM_ERROR}     		ϵͳ����
 */
extern int   API_GUI_CreateWindow(const char* pTitle,const char* pOk,const char* pCancel,FunFillColour fBackColour);

/**
 * GUI��ʾ��Ϣ��ʾ
 *
 * IMG + ��ʾ�ı�
 * @param pImg 		{@link IMAGE}ָ���IMAGE�洢�ṹ��·��
 * @param tTextType {@link TEXT_TYPE}�������
 * @param pTextBuf 	�ı���ʾ��Ϣ
 * @return {@link RET_OK} 						�ɹ�
 * @return {@link RET_API_PARAMETER_ERROR}		������������
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		���ܲ�֧��
 * @return {@link RET_SYSTEM_ERROR}     		ϵͳ����
 */
extern int  API_GUI_Info(const IMAGE* pImg,u32 tTextType,const char* pTextBuf);
extern void API_GUI_Edit_Prompt(u32 tFrontTextType,char* pFrontTextBuf,u32 tAfterTextType,char* pAfterTextBuf);
extern void API_GUI_Edit(u32 tTextType,char* pTextDef,int tMaxLen,u32 tImeType,u32 tImeLimit);


extern int API_GUI_Edit_GetText(char* pAscText,int tMaxLen);


/**
 * ����һ��GUI�˵�
 *
 * ����һ��GUI�˵�, {@link MENU_CHECK}����ѡ��˵���֧��.
 * @param pMsg 			�˵���ʾ��ʾ��Ϣ��Ĭ��ΪNULL
 * @param tNum 			�˵�����
 * @param tCurInx 		��ǰĬ��ѡ����, 0~tNum-1
 * @param tHead 		������ʾ����
 * @param pMenuText 	�˵��ı�����
 * @return {@link RET_OK} 						�ɹ�
 * @return {@link RET_API_PARAMETER_ERROR}		������������
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		���ܲ�֧��
 * @return {@link RET_SYSTEM_ERROR}     		ϵͳ����
 */
extern int API_GUI_Menu(void* pMenu,void (*pShowItem)(void *,int,int,char*),int tNum,int tCurInx,int tHead,char* pAfterText,void* KeyFunEn);
extern int API_GUI_Menu_GetInx(void);
extern int API_GUI_Menu_GetInxAndHear(int* tHead);


extern int   APP_GUI_Menu(char* pTitle,int stratIndex,int tNum,int tCurInx,char** pMenuText);

extern void APP_ShowChangeInfo(char *pOriginal,int Originalsize,const char* format,...);


/**
 * ��ʾGUI����, ��ʼ����
 *
 * @return {@link RET_OK} 						�ɹ�
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		���ܲ�֧��
 * @return {@link RET_SYSTEM_ERROR}     		ϵͳ����
 */

extern void APP_ShowSta(char *pTitle,char *pMsg);
//===========������ʾ����=======
extern int APP_ShowMsg(char *pTitle,char *pMsg,int timeOutMs);
//===========���Ͻ���ʾ����=======
extern int APP_ShowInfo(char *pTitle,char *pInfo,int timeOutMs);

extern int APP_ShowQrCode(char *pTitle,const char* pInfo,A_RGB Color);

//extern int APP_GUI_Edit(char *pTitle,const char* pFrontTextBuf,const char* pAfterTextBuf,char* pOutMsg,int MinLen,int MaxLen);


//====�ص�����ָ�룬����GUIѡ���б���ʾ��==========
/**
 * ���幹��һ��GUIѡ���б���ʾ���ָ��
 * @param char* 		������ʾ����ָ��
 * @param int 			�б���0~n
 * @param return 		0~nʣ��û��ʾ��������<0 ������ʾ������
 */
typedef int (*fOption_ShowItem)(char*,int);
/**
 * ����һ��GUIѡ���б�
 *
 * ����һ��GUIѡ���б�, 
 * @param pTitle 		�������
 * @param Start 		��ʼ����
 * @param pFunShowItem 	��ʾ�������ɺ���
 * @param return 		��ʾ����������ʣ����
 */
extern void APP_GUI_Option(char* pTitle,char **pMenuText,int ItemMax,u8 UpDownBit);


//====�ص�����ָ�룬����GUIѡ�����==========
/**
 * ���幹��һ��GUIѡ���б���ʾ���ָ��
 * @param char* 		��ʾ����
 * @param int 			�б���0~n
 * @return {@link TRUE,FALSE} TRUE(������ʾ�˵�)��FALSE(�˳��˵���ʾ)
 */
typedef int (*fOption_RunItem)(char*,int);

/**
 * ����һ��GUIѡ���б�����
 *
 * ����һ��GUIѡ���б�,��������д���, 
 * @param pTitle 		�������
 * @param pFunShowItem 	��ʾ�������ɺ���
 * @param pFunRun 		�����Ӧ�����
 * @param pFunKey 		���ܼ���������
 * @param return 		��ʾ����������
 */
//int APP_GUI_OpProcess(XuiWindow *pWindow,char *pTitle,fOption_ShowItem pFunShowItem,fOption_RunItem pFunRunItem,APP_HANDLE pFunKey);


typedef struct	
{
	char Mask[4]; 	// "UI"
	
}API_GUI_Def;


extern const API_GUI_Def ApiGUI;

#endif

