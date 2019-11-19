

#ifndef _GUI_DISPLAY_
#define _GUI_DISPLAY_




#define		OPER_OK				0
	
#define	 	OPER_RET			-1
#define	 	OPER_QUIT			-2
#define	 	OPER_TIMEOUT		-3
#define    	OPER_RDMagERR     	-4 		//读卡错
#define    	OPER_CRCERR     	-5 		//校验失败
#define		OPER_ERR			-6		//操作失败
#define 	OPER_SYSTEM_ERROR	-7		//操作失败
#define 	OPER_TRANS_OVER		-8	    //返回数据正确，显示失败msg
#define		OPER_OFFLINE		-11		//下级
#define		OPER_LOW_VOLTAGE	-15		//电压过低
#define		OPER_HARD_Err		-16		//硬件错误
#define		OPER_SEND_ERR		-1000	//发送数据失败
#define		OPER_RECV_ERR		-2000	//发送数据失败


#define		OPER_NO_CHECK		15		//无签名数据
#define		OPER_NoEND 			6		//成功，但未结束	
#define		OPER_NoHEAD  		5		//未找到头部
#define		OPER_HARD			4		//手工输入
#define		OPER_NEW			3		//进入新流程
#define		OPER_PAY			1		//确认支付

#define		SOCKET_UN_APN			-101		//未打开APN
#define		SOCKET_UN_IP			-102		//未连接IP
#define		SOCKET_UN				-200		//未连接IP

/**
 * 文本输入类型定义
 */
typedef enum 
{
	IME_NONE			=0x00000000,		//!< NONE
	IME_NUM			=0x00000001,		//!< 数字
	IME_abc			=0x00000002,		//!< 小写字母
	IME_ABC			=0x00000004,		//!< 大小字母
	IME_Abc			=0x00000008,		//!< 大小写混合字母
	IME_SYM			=0x00000010,		//!< 符号
	IME_SUM			=0x00000020,		//!< 金额
	IME_PY				=0x00000040,		//!< 拼音 支持GB2312编码
	IME_PYGBK			=0x00000080,		//!< 全拼 支持GBK编码
	IME_FMT			=0x00000100,		//!< 格式化模式数字输入，如IP,账号等.
	IME_SCAN			=0x00000200,		//!< 扫码输入
	IME_MASK			=0x000003FF,		//!< 掩码
	IME_PIN_MODE		=0x10000000,		//!< 密码模式	
	IME_SYM_MODE		=0x20000000,		//!< 符号模式	
	IME_ENG			=IME_ABC|IME_abc|IME_Abc, //!< 英文输入
	IME_END	
}IME_TYPE;			//V1.2	


#define FONT_SIZE    			(24)    
#define HZ_INDEX(hz)    		((hz[0] - 0xa1) * 94 + (hz[1] - 0xa1))
#define DOTS_BYTES    			((FONT_SIZE * FONT_SIZE / 8))

#define RGB565_WITHE		RGB_CURR(0xff,0xff,0xff)	//0x0000FFFF
#define RGB565_BLACK		RGB_CURR(0x00,0x00,0x00)	//0x00000000
#define RGB565_PARENT		0xFF000000

#define RGB565_RED			RGB_CURR(0xff,0x00,0x00)
#define RGB565_GREEN		RGB_CURR(0x00,0xff,0x00)	//0x07E0
#define RGB565_BLUE		RGB_CURR(0x00,0x00,0xff)	//0x001F

#ifdef DISPLAY_HORIZONTAL_SCREEN			//横屏定义
//---------全屏参数----------------------
#define SCREEN_WIDTH		(320) 
#define SCREEN_HEIGT		(240) 
#define EFFECTIVE_X			(0) 
#define EFFECTIVE_Y			(24)
//---------应用使用的屏区域-------------------
#define SCREEN_APP_X		(EFFECTIVE_X) 
#define SCREEN_APP_Y		(EFFECTIVE_Y)
#define SCREEN_APP_W		(SCREEN_WIDTH) 
#define SCREEN_APP_H		(SCREEN_HEIGT-EFFECTIVE_Y) 
//-------图片标题与操作内容显示-------------------------------
#define UI_MAP_TITLE_X		SCREEN_APP_X
#define UI_MAP_TITLE_Y		SCREEN_APP_Y
#define UI_MAP_TITLE_W		SCREEN_APP_W 
#define UI_MAP_TITLE_H		40 

#define UI_MAP_CONT_X		SCREEN_APP_X
#define UI_MAP_CONT_Y		(SCREEN_APP_Y+UI_MAP_TITLE_H)
#define UI_MAP_CONT_W		SCREEN_APP_W 
#define UI_MAP_CONT_H		(SCREEN_APP_H-UI_MAP_TITLE_H)
//----------系统操作界面配置---------------------------------
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
//----------定制菜单参数----------------------
#define SCREEN_MENU_X		(SCREEN_APP_X) 
#define SCREEN_MENU_Y		(SCREEN_APP_Y)
#define SCREEN_MENU_W		(SCREEN_APP_W) 
#define SCREEN_MENU_H		(SCREEN_APP_H) 
#define MENU_TITLE_H		36

#define MENU_TIEM_ALL_Y		(SCREEN_MENU_Y+MENU_TITLE_H)
#define MENU_TIEM_ALL_H		(SCREEN_APP_H-MENU_TITLE_H)
#define MENU_TIEM_H			30
#define MENU_TIEM_MAX		((SCREEN_APP_H-MENU_TITLE_H)/MENU_TIEM_H)			//最多菜单项
//------------菜单色彩定义-----------------
#define RGB565_TITLE_ICON		RGB_CURR(0,127,255)   //0x03ff
#define RGB565_TITLE_FONT		RGB565_WITHE
#define RGB565_TIEM_ICON		UI_CONT_COLOUR
#define RGB565_TIEM_FONT		RGB_CURR(48,48,48)	//0x3186
#define RGB565_SELE_ICON		RGB_CURR(224,224,224)	//0xe71c
#define RGB565_SELE_FONT		RGB_CURR(0,127,255)	//0x03ff

#else		//===============================================================================

//---------全屏参数----------------------
#define SCREEN_WIDTH		(240) 
#define SCREEN_HEIGT		(320) 
#define EFFECTIVE_X			(0) 
#define EFFECTIVE_Y			(24)
//---------应用使用的屏区域-------------------
#define SCREEN_APP_X		(EFFECTIVE_X) 
#define SCREEN_APP_Y		(EFFECTIVE_Y)
#define SCREEN_APP_W		(SCREEN_WIDTH) 
#define SCREEN_APP_H		(SCREEN_HEIGT-EFFECTIVE_Y) 
//-------图片标题与操作内容显示-------------------------------
#define UI_MAP_TITLE_X		SCREEN_APP_X
#define UI_MAP_TITLE_Y		SCREEN_APP_Y
#define UI_MAP_TITLE_W		SCREEN_APP_W 
#define UI_MAP_TITLE_H		40 

#define UI_MAP_CONT_X		SCREEN_APP_X
#define UI_MAP_CONT_Y		(SCREEN_APP_Y+UI_MAP_TITLE_H)
#define UI_MAP_CONT_W		SCREEN_APP_W 
#define UI_MAP_CONT_H		(SCREEN_APP_H-UI_MAP_TITLE_H)
//----------系统操作界面配置---------------------------------
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
//----------定制菜单参数----------------------
#define SCREEN_MENU_X		(SCREEN_APP_X) 
#define SCREEN_MENU_Y		(SCREEN_APP_Y)
#define SCREEN_MENU_W		(SCREEN_APP_W) 
#define SCREEN_MENU_H		(SCREEN_APP_H) 
#define MENU_TITLE_H		40

#define MENU_TIEM_ALL_Y		(SCREEN_MENU_Y+MENU_TITLE_H)
#define MENU_TIEM_ALL_H		(SCREEN_APP_H-MENU_TITLE_H)
#define MENU_TIEM_H			32
#define MENU_TIEM_MAX		((SCREEN_APP_H-MENU_TITLE_H)/MENU_TIEM_H)			//最多菜单项
//------------菜单色彩定义-----------------
#define RGB565_TITLE_ICON		RGB_CURR(0,127,255)   //0x03ff
#define RGB565_TITLE_FONT		RGB565_WITHE
#define RGB565_TIEM_ICON		UI_CONT_COLOUR
#define RGB565_TIEM_FONT		RGB_CURR(48,48,48)	//0x3186
#define RGB565_SELE_ICON		RGB_CURR(224,224,224)	//0xe71c
#define RGB565_SELE_FONT		RGB_CURR(0,127,255)	//0x03ff

#endif



typedef enum
{
	MENU_ICON_NUM		=0x0001,			//图标为序号,	与MENU_LIST组合使用
	MENU_ICOM_IMG		=0x0002,			//图标为图像,	与MENU_LIST组合使用
	MENU_LIST			=0x0100,			//列表菜单
	MENU_RADIO			=0x0200,			//单项选择菜单
	MENU_CHECK			=0x0400,			//多项选择菜单
	MENU_MUILT_LINE		=0x1000,			//多行文本,		与其它类型组合使用
	MENU_MAX			=0xFFFF
}UI_MENU_TYPE;
/**
 * 文本显示类型定义
 */
typedef enum
{
	TEXT_3X5				=4,				//!< 3x5字体大小
	TEXT_6X8				=6,				//!< 6x8字体大小
	TEXT_12					=12,			//!< 12x12字体大小
	TEXT_16					=16,			//!< 16x16字体大小
	TEXT_20					=20,			//!< 20x20字体大小
	TEXT_24					=24,			//!< 24x24字体大小
	TEXT_32					=32,			//!< 32x32字体大小
	TEXT_SIZE				=0x000000FF,	//!< 字体大小掩码
	                    	
	TEXT_NONE				=0x00000000,	//!< 普通文本
	TEXT_NORMAL				=0x00000000,	//!< 普通文本
	TEXT_SHADOW				=0x00000100,	//!< 阴影字(彩色有效)
	TEXT_BORDER				=0x00000200,	//!< 空心字
	TEXT_BOLD				=0x00000400,	//!< 加粗
	TEXT_BACK				=0x00000800,	//!< 带背景色
	
	TEXT_ALIGN_LEFT			=0x10000000,	//!< 水平左对齐
	TEXT_ALIGN_CENTER		=0x20000000,	//!< 水平居中对齐
	TEXT_ALIGN_RIGHT		=0x40000000,	//!< 水平右对齐
	TEXT_ALIGN_MASK			=0x70000000,	//!< 水平对齐掩码
	                    	
	TEXT_VALIGN_TOP			=0x01000000,	//!< 垂直上对齐
	TEXT_VALIGN_CENTER		=0x02000000,	//!< 垂直居中对齐
	TEXT_VALIGN_BOTTOM		=0x04000000,	//!< 垂直下对齐
	TEXT_VALIGN_MASK		=0x07000000,	//!< 垂直对齐掩码

	TEXT_EXSTYLE_UNDERLINE  =0x00100000,	//加下划线
	TEXT_EXSTYLE_OVERLINE   =0x00200000,	//加上划线
	TEXT_EXSTYLE_BORDER     =0x00400000,	//加框
	TEXT_LINE_NUM_MASK		=0x000F0000,	//指定行号(0~N)
}TEXT_TYPE;	//V1.1


#define TOK							"OK"
#define TCANCEL					"CANCEL"




extern int APP_WaitUiEvent(int tTimeOutMS);
//=================================================================================

//extern void UI_DisplayBitMapGet(u16 x,u16 y,u16 w,u16 h);
//extern void UI_DisplayBitMapSet(void);
//extern void UI_DisplayBitMapEND(void);

//==================================================================================
extern void UI_ShowColorRect(XuiWindow *pWindow,RECTL *pRect,u16 Width,u32 Color);
extern void API_GUI_InputEdit(XuiWindow *pWindow,char* pStrDef,int tMaxLen,u32 Way,Fun_ShowNum pShow);



//===============清除显示内容(指定区域)=================================================
extern void API_GUI_ClearScreen(XuiWindow *pWindow,RECTL* pRect);
/**
 * 创建一个窗体用于GUI界面交互
 *
 * @param pTitle 		界面标题
 * @param pOk 			按钮栏中存在OK键的提示, 为NULL不存在该按钮
 * @param pCancel 		按钮栏中存在Cancel键的提示, 为NULL不存在该按钮
 * @param tGuiType 		窗体显示类型 {@link WIN_TYPE} 例如 WIN_FULL|WIN_NOBTN 或者　WIN_POP|WIN_NOBTN
 * @return {@link RET_OK} 						成功
 * @return {@link RET_API_PARAMETER_ERROR}		函数参数错误
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		功能不支持
 * @return {@link RET_SYSTEM_ERROR}     		系统错误
 */
extern int   API_GUI_CreateWindow(XuiWindow *pWindow,const char* pTitle,const char* pOk,const char* pCancel,u32 tGuiType);

/**
 * GUI提示信息显示
 *
 * IMG + 提示文本
 * @param pImg 		{@link IMAGE}指针或IMAGE存储结构的路径
 * @param tTextType {@link TEXT_TYPE}组合类型
 * @param pTextBuf 	文本提示信息
 * @return {@link RET_OK} 						成功
 * @return {@link RET_API_PARAMETER_ERROR}		函数参数错误
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		功能不支持
 * @return {@link RET_SYSTEM_ERROR}     		系统错误
 */
extern int  API_GUI_Info(XuiWindow *pWindow,const IMAGE* pImg,u32 tTextType,const char* pTextBuf);
extern void API_GUI_Edit_Prompt(XuiWindow *pWindow,u32 tFrontTextType,char* pFrontTextBuf,u32 tAfterTextType,char* pAfterTextBuf);
extern void API_GUI_Edit(XuiWindow *pWindow,u32 tTextType,char* pTextDef,int tMaxLen,u32 tImeType,u32 tImeLimit);


extern int API_GUI_Edit_GetText(char* pAscText,int tMaxLen);


/**
 * 构建一个GUI菜单
 *
 * 构建一个GUI菜单, {@link MENU_CHECK}多项选择菜单不支持.
 * @param pMsg 			菜单显示提示信息，默认为NULL
 * @param tNum 			菜单项数
 * @param tCurInx 		当前默认选择项, 0~tNum-1
 * @param tHead 		首项显示索引
 * @param pMenuText 	菜单文本数组
 * @return {@link RET_OK} 						成功
 * @return {@link RET_API_PARAMETER_ERROR}		函数参数错误
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		功能不支持
 * @return {@link RET_SYSTEM_ERROR}     		系统错误
 */
extern int API_GUI_Menu(XuiWindow *pWindow,void* pMenu,void (*pShowItem)(void *,int,int,char*),int tNum,int tCurInx,int tHead,char* pAfterText,void* KeyFunEn);
extern int API_GUI_Menu_GetInx(void);
extern int API_GUI_Menu_GetInxAndHear(int* tHead);


extern int   APP_GUI_Menu(XuiWindow *pWindow,char* pTitle,int stratIndex,int tNum,int tCurInx,char** pMenuText);

extern void APP_ShowChangeInfo(XuiWindow *pWindow,char *pOriginal,int Originalsize,const char* format,...);

//extern void API_GUI_Draw565QRcode(RECTL* prclTrg,char *pInMsg,u32 fgColor, u32 bgColor);

/**
 * 显示GUI界面, 开始交互
 *
 * @return {@link RET_OK} 						成功
 * @return {@link RET_FUNCTION_NOT_SUPPORT}		功能不支持
 * @return {@link RET_SYSTEM_ERROR}     		系统错误
 */
extern int API_GUI_Show(XuiWindow *pWindow);

extern void APP_ShowSta(XuiWindow *pWindow,char *pTitle,char *pMsg);
//===========剧中显示内容=======
extern int APP_ShowMsg(XuiWindow *pWindow,char *pTitle,char *pMsg,int timeOutMs);
//===========左上角显示内容=======
extern int APP_ShowInfo(XuiWindow *pWindow,char *pTitle,char *pInfo,int timeOutMs);

//extern int APP_GUI_Edit(char *pTitle,const char* pFrontTextBuf,const char* pAfterTextBuf,char* pOutMsg,int MinLen,int MaxLen);

/**
 * 构建一个GUI菜单行列表 
 * @param pTitle 		界面标题
 * @param pLineText 	行内容
 * @param LineMax 		行数量(Bit15 行号标记)
 * @param pAfterText 	末行显示内容(上划线右靠)
 */
extern void APP_GUI_LineMenu(XuiWindow *pWindow,char* pTitle,char **pLineText,u16 LineMax,char* pAfterText);

//====回调函数指针，用于GUI选项列表显示项==========
/**
 * 定义构建一个GUI选项列表显示项函数指针
 * @param char* 		返回显示数据指针
 * @param int 			列表项0~n
 * @param return 		0~n剩余没显示的项数，<0 不能显示的内容
 */
typedef int (*fOption_ShowItem)(char*,int);
/**
 * 构建一个GUI选项列表
 *
 * 构建一个GUI选项列表, 
 * @param pTitle 		界面标题
 * @param Start 		起始索引
 * @param pFunShowItem 	显示内容生成函数
 * @param return 		显示参数排列项剩余项
 */
extern void APP_GUI_Option(XuiWindow *pWindow,char* pTitle,char **pMenuText,int ItemMax,u8 UpDownBit);


//====回调函数指针，用于GUI选项处理功能==========
/**
 * 定义构建一个GUI选项列表显示项函数指针
 * @param char* 		显示标题
 * @param int 			列表项0~n
 * @return {@link TRUE,FALSE} TRUE(继续显示菜单)，FALSE(退出菜单显示)
 */
typedef int (*fOption_RunItem)(char*,int);

/**
 * 构建一个GUI选项列表处理函数
 *
 * 构建一个GUI选项列表,并对其进行处理, 
 * @param pTitle 		界面标题
 * @param pFunShowItem 	显示内容生成函数
 * @param pFunRun 		处理对应项操作
 * @param pFunKey 		功能键操作内容
 * @param return 		显示参数排列项
 */
//int APP_GUI_OpProcess(XuiWindow *pWindow,char *pTitle,fOption_ShowItem pFunShowItem,fOption_RunItem pFunRunItem,APP_HANDLE pFunKey);


typedef struct	
{
	char Mask[4]; 	// "UI"
	
}API_GUI_Def;


extern const API_GUI_Def ApiGUI;

#endif

