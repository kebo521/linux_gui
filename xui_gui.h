

#ifndef _UI_DISPLAY_
#define _UI_DISPLAY_

//#define DISPLAY_HORIZONTAL_SCREEN		//ºáÆÁ¶¨Òå



#define		OPER_OK				0
	
#define	 	OPER_RET			-1
#define	 	OPER_QUIT			-2
#define	 	OPER_TIMEOUT		-3
#define    	OPER_RDMagERR     	-4 		//¶Á¿¨´í
#define    	OPER_CRCERR     	-5 		//Ğ£ÑéÊ§°Ü
#define		OPER_ERR			-6		//²Ù×÷Ê§°Ü
#define 	OPER_SYSTEM_ERROR	-7		//²Ù×÷Ê§°Ü
#define 	OPER_TRANS_OVER		-8	    //·µ»ØÊı¾İÕıÈ·£¬ÏÔÊ¾Ê§°Ümsg
#define		OPER_OFFLINE		-11		//ÏÂ¼¶
#define		OPER_LOW_VOLTAGE	-15		//µçÑ¹¹ıµÍ
#define		OPER_HARD_Err		-16		//Ó²¼ş´íÎó
#define		OPER_SEND_ERR		-1000	//·¢ËÍÊı¾İÊ§°Ü
#define		OPER_RECV_ERR		-2000	//·¢ËÍÊı¾İÊ§°Ü


#define		OPER_NO_CHECK		15		//ÎŞÇ©ÃûÊı¾İ
#define		OPER_NoEND 			6		//³É¹¦£¬µ«Î´½áÊø	
#define		OPER_NoHEAD  		5		//Î´ÕÒµ½Í·²¿
#define		OPER_HARD			4		//ÊÖ¹¤ÊäÈë
#define		OPER_NEW			3		//½øÈëĞÂÁ÷³Ì
#define		OPER_PAY			1		//È·ÈÏÖ§¸¶

#define		SOCKET_UN_APN			-101		//Î´´ò¿ªAPN
#define		SOCKET_UN_IP			-102		//Î´Á¬½ÓIP
#define		SOCKET_UN				-200		//Î´Á¬½ÓIP

/**
 * ÎÄ±¾ÊäÈëÀàĞÍ¶¨Òå
 */
typedef enum 
{
	IME_NONE			=0x00000000,		//!< NONE
	IME_NUM			=0x00000001,		//!< Êı×Ö
	IME_abc			=0x00000002,		//!< Ğ¡Ğ´×ÖÄ¸
	IME_ABC			=0x00000004,		//!< ´óĞ¡×ÖÄ¸
	IME_Abc			=0x00000008,		//!< ´óĞ¡Ğ´»ìºÏ×ÖÄ¸
	IME_SYM			=0x00000010,		//!< ·ûºÅ
	IME_SUM			=0x00000020,		//!< ½ğ¶î
	IME_PY				=0x00000040,		//!< Æ´Òô Ö§³ÖGB2312±àÂë
	IME_PYGBK			=0x00000080,		//!< È«Æ´ Ö§³ÖGBK±àÂë
	IME_FMT			=0x00000100,		//!< ¸ñÊ½»¯Ä£Ê½Êı×ÖÊäÈë£¬ÈçIP,ÕËºÅµÈ.
	IME_SCAN			=0x00000200,		//!< É¨ÂëÊäÈë
	IME_MASK			=0x000003FF,		//!< ÑÚÂë
	IME_PIN_MODE		=0x10000000,		//!< ÃÜÂëÄ£Ê½	
	IME_SYM_MODE		=0x20000000,		//!< ·ûºÅÄ£Ê½	
	IME_ENG			=IME_ABC|IME_abc|IME_Abc, //!< Ó¢ÎÄÊäÈë
	IME_END	
}IME_TYPE;			//V1.2	



#define FONT_SIZE    			(24)    
#define HZ_INDEX(hz)    		((hz[0] - 0xa1) * 94 + (hz[1] - 0xa1))
#define DOTS_BYTES    			((FONT_SIZE * FONT_SIZE / 8))
#define	RGB_CURR(r,g,b)		((r*0x10000)|(g*0x100)|b)	//(u32)(rgba_t)(b,g,r,0xff))

#define RGB565_WITHE		RGB_CURR(0xff,0xff,0xff)	//0x0000FFFF
#define RGB565_BLACK		RGB_CURR(0x00,0x00,0x00)	//0x00000000
#define RGB565_PARENT		0xFF000000

#define RGB565_RED			RGB_CURR(0xff,0x00,0x00)
#define RGB565_GREEN		RGB_CURR(0x00,0xff,0x00)	//0x07E0
#define RGB565_BLUE		RGB_CURR(0x00,0x00,0xff)	//0x001F

#ifdef DISPLAY_HORIZONTAL_SCREEN			//ºáÆÁ¶¨Òå
//---------È«ÆÁ²ÎÊı----------------------
#define SCREEN_WIDTH		(320) 
#define SCREEN_HEIGT		(240) 
#define EFFECTIVE_X			(0) 
#define EFFECTIVE_Y			(24)
//---------Ó¦ÓÃÊ¹ÓÃµÄÆÁÇøÓò-------------------
#define SCREEN_APP_X		(EFFECTIVE_X) 
#define SCREEN_APP_Y		(EFFECTIVE_Y)
#define SCREEN_APP_W		(SCREEN_WIDTH) 
#define SCREEN_APP_H		(SCREEN_HEIGT-EFFECTIVE_Y) 
//-------Í¼Æ¬±êÌâÓë²Ù×÷ÄÚÈİÏÔÊ¾-------------------------------
#define UI_MAP_TITLE_X		SCREEN_APP_X
#define UI_MAP_TITLE_Y		SCREEN_APP_Y
#define UI_MAP_TITLE_W		SCREEN_APP_W 
#define UI_MAP_TITLE_H		40 

#define UI_MAP_CONT_X		SCREEN_APP_X
#define UI_MAP_CONT_Y		(SCREEN_APP_Y+UI_MAP_TITLE_H)
#define UI_MAP_CONT_W		SCREEN_APP_W 
#define UI_MAP_CONT_H		(SCREEN_APP_H-UI_MAP_TITLE_H)
//----------ÏµÍ³²Ù×÷½çÃæÅäÖÃ---------------------------------
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
//----------¶¨ÖÆ²Ëµ¥²ÎÊı----------------------
#define SCREEN_MENU_X		(SCREEN_APP_X) 
#define SCREEN_MENU_Y		(SCREEN_APP_Y)
#define SCREEN_MENU_W		(SCREEN_APP_W) 
#define SCREEN_MENU_H		(SCREEN_APP_H) 
#define MENU_TITLE_H		36

#define MENU_TIEM_ALL_Y		(SCREEN_MENU_Y+MENU_TITLE_H)
#define MENU_TIEM_ALL_H		(SCREEN_APP_H-MENU_TITLE_H)
#define MENU_TIEM_H			30
#define MENU_TIEM_MAX		((SCREEN_APP_H-MENU_TITLE_H)/MENU_TIEM_H)			//×î¶à²Ëµ¥Ïî
//------------²Ëµ¥É«²Ê¶¨Òå-----------------
#define RGB565_TITLE_ICON		RGB_CURR(0,127,255)   //0x03ff
#define RGB565_TITLE_FONT		RGB565_WITHE
#define RGB565_TIEM_ICON		UI_CONT_COLOUR
#define RGB565_TIEM_FONT		RGB_CURR(48,48,48)	//0x3186
#define RGB565_SELE_ICON		RGB_CURR(224,224,224)	//0xe71c
#define RGB565_SELE_FONT		RGB_CURR(0,127,255)	//0x03ff

#else		//===============================================================================

//---------È«ÆÁ²ÎÊı----------------------
#define SCREEN_WIDTH		(240) 
#define SCREEN_HEIGT		(320) 
#define EFFECTIVE_X			(0) 
#define EFFECTIVE_Y			(24)
//---------Ó¦ÓÃÊ¹ÓÃµÄÆÁÇøÓò-------------------
#define SCREEN_APP_X		(EFFECTIVE_X) 
#define SCREEN_APP_Y		(EFFECTIVE_Y)
#define SCREEN_APP_W		(SCREEN_WIDTH) 
#define SCREEN_APP_H		(SCREEN_HEIGT-EFFECTIVE_Y) 
//-------Í¼Æ¬±êÌâÓë²Ù×÷ÄÚÈİÏÔÊ¾-------------------------------
#define UI_MAP_TITLE_X		SCREEN_APP_X
#define UI_MAP_TITLE_Y		SCREEN_APP_Y
#define UI_MAP_TITLE_W		SCREEN_APP_W 
#define UI_MAP_TITLE_H		40 

#define UI_MAP_CONT_X		SCREEN_APP_X
#define UI_MAP_CONT_Y		(SCREEN_APP_Y+UI_MAP_TITLE_H)
#define UI_MAP_CONT_W		SCREEN_APP_W 
#define UI_MAP_CONT_H		(SCREEN_APP_H-UI_MAP_TITLE_H)
//----------ÏµÍ³²Ù×÷½çÃæÅäÖÃ---------------------------------
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
//----------¶¨ÖÆ²Ëµ¥²ÎÊı----------------------
#define SCREEN_MENU_X		(SCREEN_APP_X) 
#define SCREEN_MENU_Y		(SCREEN_APP_Y)
#define SCREEN_MENU_W		(SCREEN_APP_W) 
#define SCREEN_MENU_H		(SCREEN_APP_H) 
#define MENU_TITLE_H		40

#define MENU_TIEM_ALL_Y		(SCREEN_MENU_Y+MENU_TITLE_H)
#define MENU_TIEM_ALL_H		(SCREEN_APP_H-MENU_TITLE_H)
#define MENU_TIEM_H			32
#define MENU_TIEM_MAX		((SCREEN_APP_H-MENU_TITLE_H)/MENU_TIEM_H)			//×î¶à²Ëµ¥Ïî
//------------²Ëµ¥É«²Ê¶¨Òå-----------------
#define RGB565_TITLE_ICON		RGB_CURR(0,127,255)   //0x03ff
#define RGB565_TITLE_FONT		RGB565_WITHE
#define RGB565_TIEM_ICON		UI_CONT_COLOUR
#define RGB565_TIEM_FONT		RGB_CURR(48,48,48)	//0x3186
#define RGB565_SELE_ICON		RGB_CURR(224,224,224)	//0xe71c
#define RGB565_SELE_FONT		RGB_CURR(0,127,255)	//0x03ff

#endif


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


typedef struct
{
	RECTL 	Rect;
	u16	LcdBuff[2];		//!< µ¥É«Í¼Æ¬³¤¶ÈÎª:num * ((w+8-1)/8) * h <br/> ²ÊÉ«Í¼Æ¬³¤¶ÈÎª:num * w * h * (DISP_DEPTH/8)
}DisplayBackup;		

typedef enum
{
	MENU_ICON_NUM		=0x0001,			//Í¼±êÎªĞòºÅ,	ÓëMENU_LIST×éºÏÊ¹ÓÃ
	MENU_ICOM_IMG		=0x0002,			//Í¼±êÎªÍ¼Ïñ,	ÓëMENU_LIST×éºÏÊ¹ÓÃ
	MENU_LIST			=0x0100,			//ÁĞ±í²Ëµ¥
	MENU_RADIO			=0x0200,			//µ¥ÏîÑ¡Ôñ²Ëµ¥
	MENU_CHECK			=0x0400,			//¶àÏîÑ¡Ôñ²Ëµ¥
	MENU_MUILT_LINE		=0x1000,			//¶àĞĞÎÄ±¾,		ÓëÆäËüÀàĞÍ×éºÏÊ¹ÓÃ
	MENU_MAX			=0xFFFF
}UI_MENU_TYPE;

/**
 * ÎÄ±¾ÏÔÊ¾ÀàĞÍ¶¨Òå
 */
typedef enum
{
	TEXT_3X5				=4,				//!< 3x5×ÖÌå´óĞ¡
	TEXT_6X8				=6,				//!< 6x8×ÖÌå´óĞ¡
	TEXT_12					=12,			//!< 12x12×ÖÌå´óĞ¡
	TEXT_16					=16,			//!< 16x16×ÖÌå´óĞ¡
	TEXT_20					=20,			//!< 20x20×ÖÌå´óĞ¡
	TEXT_24					=24,			//!< 24x24×ÖÌå´óĞ¡
	TEXT_32					=32,			//!< 32x32×ÖÌå´óĞ¡
	TEXT_SIZE				=0x000000FF,	//!< ×ÖÌå´óĞ¡ÑÚÂë
	                    	
	TEXT_NONE				=0x00000000,	//!< ÆÕÍ¨ÎÄ±¾
	TEXT_NORMAL				=0x00000000,	//!< ÆÕÍ¨ÎÄ±¾
	TEXT_SHADOW				=0x00000100,	//!< ÒõÓ°×Ö(²ÊÉ«ÓĞĞ§)
	TEXT_BORDER				=0x00000200,	//!< ¿ÕĞÄ×Ö
	TEXT_BOLD				=0x00000400,	//!< ¼Ó´Ö
	TEXT_BACK				=0x00000800,	//!< ´ø±³¾°É«
	
	TEXT_ALIGN_LEFT			=0x10000000,	//!< Ë®Æ½×ó¶ÔÆë
	TEXT_ALIGN_CENTER		=0x20000000,	//!< Ë®Æ½¾ÓÖĞ¶ÔÆë
	TEXT_ALIGN_RIGHT		=0x40000000,	//!< Ë®Æ½ÓÒ¶ÔÆë
	TEXT_ALIGN_MASK			=0x70000000,	//!< Ë®Æ½¶ÔÆëÑÚÂë
	                    	
	TEXT_VALIGN_TOP			=0x01000000,	//!< ´¹Ö±ÉÏ¶ÔÆë
	TEXT_VALIGN_CENTER		=0x02000000,	//!< ´¹Ö±¾ÓÖĞ¶ÔÆë
	TEXT_VALIGN_BOTTOM		=0x04000000,	//!< ´¹Ö±ÏÂ¶ÔÆë
	TEXT_VALIGN_MASK		=0x07000000,	//!< ´¹Ö±¶ÔÆëÑÚÂë

	TEXT_EXSTYLE_UNDERLINE  =0x00100000,	//¼ÓÏÂ»®Ïß
	TEXT_EXSTYLE_OVERLINE   =0x00200000,	//¼ÓÉÏ»®Ïß
	TEXT_EXSTYLE_BORDER     =0x00400000,	//¼Ó¿ò
	TEXT_LINE_NUM_MASK		=0x000F0000,	//Ö¸¶¨ĞĞºÅ(0~N)
}TEXT_TYPE;	//V1.1


#define TOK							"OK"
#define TCANCEL					"CANCEL"


typedef struct {
	unsigned short left,top;
	unsigned short width;	//Window å®½åº¦
	unsigned short height;	//Window é«˜åº¦
	unsigned short key;		//å…³è”æŒ‰é”®å€¼
	unsigned short type;	//Window çª—å£ç±»å‹ï¼Œè¯¦è§XuiWindowType
	union 
	{
		unsigned char* widget;	//Window å…³è”ç”»å¸ƒæŒ‡é’ˆ
		pixel_rgb565_t *rgb565buff;
		pixel_bgra8888_t *bgra8888buff;
	};
	screen_buffer *fb;
} XuiWindow;
extern XuiWindow UI_screen;

extern int XuiRootCanvas(screen_buffer* fb);
extern void XuiPush(void);
extern void XuiClose(void);

//=================================================================================
extern int UI_ShowPictureFile(RECTL *prclTrg,const char *pfilePath);

extern void UI_DisplayBitMapGet(u16 x,u16 y,u16 w,u16 h);
extern void UI_DisplayBitMapSet(void);
extern void UI_DisplayBitMapEND(void);

extern void UI_ShowBottomProgress(u8 ratio);
extern void UI_ShowParMiddleSlide(u8 par,int ratio);
//==================ÏÔÊ¾Ó¢ÎÄ£¬²»ĞèÒª×Ö¿âÖ§³Ö===================================
extern void APP_ShowLineEn(u8 Line,char *pMsgEn,int timeoutms);

//==================================================================================
extern void UI_ShowColorRect(RECTL *pRect,u16 Width,u32 Color);
extern void API_GUI_InputEdit(char* pStrDef,int tMaxLen,u32 Way,Fun_ShowNum pShow);



//===============Çå³ıÏÔÊ¾ÄÚÈİ(Ö¸¶¨ÇøÓò)=================================================
extern void API_GUI_ClearScreen(RECTL* pRect);
/**
 * ´´½¨Ò»¸ö´°ÌåÓÃÓÚGUI½çÃæ½»»¥
 *
 * @param pTitle 		½çÃæ±êÌâ
 * @param pOk 			°´Å¥À¸ÖĞ´æÔÚOK¼üµÄÌáÊ¾, ÎªNULL²»´æÔÚ¸Ã°´Å¥
 * @param pCancel 		°´Å¥À¸ÖĞ´æÔÚCancel¼üµÄÌáÊ¾, ÎªNULL²»´æÔÚ¸Ã°´Å¥
 * @param tGuiType 		´°ÌåÏÔÊ¾ÀàĞÍ {@link WIN_TYPE} ÀıÈç WIN_FULL|WIN_NOBTN »òÕß¡¡WIN_POP|WIN_NOBTN
 * @return {@link RET_OK} 						³É¹¦
 * @return {@link RET_API_PARAMETER_ERROR}		º¯Êı²ÎÊı´íÎó
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		¹¦ÄÜ²»Ö§³Ö
 * @return {@link RET_SYSTEM_ERROR}     		ÏµÍ³´íÎó
 */
extern int   API_GUI_CreateWindow(const char* pTitle,const char* pOk,const char* pCancel,u32 tGuiType);

/**
 * GUIÌáÊ¾ĞÅÏ¢ÏÔÊ¾
 *
 * IMG + ÌáÊ¾ÎÄ±¾
 * @param pImg 		{@link IMAGE}Ö¸Õë»òIMAGE´æ´¢½á¹¹µÄÂ·¾¶
 * @param tTextType {@link TEXT_TYPE}×éºÏÀàĞÍ
 * @param pTextBuf 	ÎÄ±¾ÌáÊ¾ĞÅÏ¢
 * @return {@link RET_OK} 						³É¹¦
 * @return {@link RET_API_PARAMETER_ERROR}		º¯Êı²ÎÊı´íÎó
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		¹¦ÄÜ²»Ö§³Ö
 * @return {@link RET_SYSTEM_ERROR}     		ÏµÍ³´íÎó
 */
extern int  API_GUI_Info(const IMAGE* pImg,u32 tTextType,const char* pTextBuf);
extern void API_GUI_Edit_Prompt(u32 tFrontTextType,char* pFrontTextBuf,u32 tAfterTextType,char* pAfterTextBuf);
extern void API_GUI_Edit(u32 tTextType,char* pTextDef,int tMaxLen,u32 tImeType,u32 tImeLimit);


extern int API_GUI_Edit_GetText(char* pAscText,int tMaxLen);


/**
 * ¹¹½¨Ò»¸öGUI²Ëµ¥
 *
 * ¹¹½¨Ò»¸öGUI²Ëµ¥, {@link MENU_CHECK}¶àÏîÑ¡Ôñ²Ëµ¥²»Ö§³Ö.
 * @param pMsg 			²Ëµ¥ÏÔÊ¾ÌáÊ¾ĞÅÏ¢£¬Ä¬ÈÏÎªNULL
 * @param tNum 			²Ëµ¥ÏîÊı
 * @param tCurInx 		µ±Ç°Ä¬ÈÏÑ¡ÔñÏî, 0~tNum-1
 * @param tHead 		Ê×ÏîÏÔÊ¾Ë÷Òı
 * @param pMenuText 	²Ëµ¥ÎÄ±¾Êı×é
 * @return {@link RET_OK} 						³É¹¦
 * @return {@link RET_API_PARAMETER_ERROR}		º¯Êı²ÎÊı´íÎó
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		¹¦ÄÜ²»Ö§³Ö
 * @return {@link RET_SYSTEM_ERROR}     		ÏµÍ³´íÎó
 */
extern int API_GUI_Menu(void* pMenu,void (*pShowItem)(void *,int,int,char*),int tNum,int tCurInx,int tHead,char* pAfterText,void* KeyFunEn);
extern int API_GUI_Menu_GetInx(void);
extern int API_GUI_Menu_GetInxAndHear(int* tHead);


extern int   APP_GUI_Menu(char* pTitle,int stratIndex,int tNum,int tCurInx,char** pMenuText);

extern void APP_ShowChangeInfo(char *pOriginal,int Originalsize,const char* format,...);

//extern void API_GUI_Draw565QRcode(RECTL* prclTrg,char *pInMsg,u32 fgColor, u32 bgColor);

/**
 * ÏÔÊ¾GUI½çÃæ, ¿ªÊ¼½»»¥
 *
 * @return {@link RET_OK} 						³É¹¦
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		¹¦ÄÜ²»Ö§³Ö
 * @return {@link RET_SYSTEM_ERROR}     		ÏµÍ³´íÎó
 */
extern int API_GUI_Show(void);

extern void APP_ShowSta(char *pTitle,char *pMsg);
//===========¾çÖĞÏÔÊ¾ÄÚÈİ=======
extern int APP_ShowMsg(char *pTitle,char *pMsg,int timeOutMs);
//===========×óÉÏ½ÇÏÔÊ¾ÄÚÈİ=======
extern int APP_ShowInfo(char *pTitle,char *pInfo,int timeOutMs);

//extern int APP_GUI_Edit(char *pTitle,const char* pFrontTextBuf,const char* pAfterTextBuf,char* pOutMsg,int MinLen,int MaxLen);


extern void APP_HitMsg(const char* pMsg,int tTimeOutMS);

extern void API_UI_ShowQrCode(RECTL* pRect,const char* pInfo,u32 Color);

/**
 * ¹¹½¨Ò»¸öGUI²Ëµ¥ĞĞÁĞ±í 
 * @param pTitle 		½çÃæ±êÌâ
 * @param pLineText 	ĞĞÄÚÈİ
 * @param LineMax 		ĞĞÊıÁ¿(Bit15 ĞĞºÅ±ê¼Ç)
 * @param pAfterText 	Ä©ĞĞÏÔÊ¾ÄÚÈİ(ÉÏ»®ÏßÓÒ¿¿)
 */
extern void APP_GUI_LineMenu(char* pTitle,char **pLineText,u16 LineMax,char* pAfterText);

//====»Øµ÷º¯ÊıÖ¸Õë£¬ÓÃÓÚGUIÑ¡ÏîÁĞ±íÏÔÊ¾Ïî==========
/**
 * ¶¨Òå¹¹½¨Ò»¸öGUIÑ¡ÏîÁĞ±íÏÔÊ¾Ïîº¯ÊıÖ¸Õë
 * @param char* 		·µ»ØÏÔÊ¾Êı¾İÖ¸Õë
 * @param int 			ÁĞ±íÏî0~n
 * @param return 		0~nÊ£ÓàÃ»ÏÔÊ¾µÄÏîÊı£¬<0 ²»ÄÜÏÔÊ¾µÄÄÚÈİ
 */
typedef int (*fOption_ShowItem)(char*,int);
/**
 * ¹¹½¨Ò»¸öGUIÑ¡ÏîÁĞ±í
 *
 * ¹¹½¨Ò»¸öGUIÑ¡ÏîÁĞ±í, 
 * @param pTitle 		½çÃæ±êÌâ
 * @param Start 		ÆğÊ¼Ë÷Òı
 * @param pFunShowItem 	ÏÔÊ¾ÄÚÈİÉú³Éº¯Êı
 * @param return 		ÏÔÊ¾²ÎÊıÅÅÁĞÏîÊ£ÓàÏî
 */
extern void APP_GUI_Option(char* pTitle,char **pMenuText,int ItemMax,u8 UpDownBit);


//====»Øµ÷º¯ÊıÖ¸Õë£¬ÓÃÓÚGUIÑ¡Ïî´¦Àí¹¦ÄÜ==========
/**
 * ¶¨Òå¹¹½¨Ò»¸öGUIÑ¡ÏîÁĞ±íÏÔÊ¾Ïîº¯ÊıÖ¸Õë
 * @param char* 		ÏÔÊ¾±êÌâ
 * @param int 			ÁĞ±íÏî0~n
 * @return {@link TRUE,FALSE} TRUE(¼ÌĞøÏÔÊ¾²Ëµ¥)£¬FALSE(ÍË³ö²Ëµ¥ÏÔÊ¾)
 */
typedef int (*fOption_RunItem)(char*,int);

/**
 * ¹¹½¨Ò»¸öGUIÑ¡ÏîÁĞ±í´¦Àíº¯Êı
 *
 * ¹¹½¨Ò»¸öGUIÑ¡ÏîÁĞ±í,²¢¶ÔÆä½øĞĞ´¦Àí, 
 * @param pTitle 		½çÃæ±êÌâ
 * @param pFunShowItem 	ÏÔÊ¾ÄÚÈİÉú³Éº¯Êı
 * @param pFunRun 		´¦Àí¶ÔÓ¦Ïî²Ù×÷
 * @param pFunKey 		¹¦ÄÜ¼ü²Ù×÷ÄÚÈİ
 * @param return 		ÏÔÊ¾²ÎÊıÅÅÁĞÏî
 */
int APP_GUI_OpProcess(char *pTitle,fOption_ShowItem pFunShowItem,fOption_RunItem pFunRunItem,APP_HANDLE pFunKey);





#endif

