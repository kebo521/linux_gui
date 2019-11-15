#ifndef _EVENMSG_
#define _EVENMSG_

//typedef int (*Fun_KillThread)(void*); 

typedef void (*fPushTaskMsg)(u16);

typedef enum
{
	EVEN_ID_KEY_DOWN, 
	EVEN_ID_SCAN,
	EVEN_ID_NOTIFY_NSG,
	EVEN_ID_UART_RECV,
	EVEN_ID_WIFI_MSG,
	EVEN_ID_TIME_OUT,
	EVEN_ID_SHOW_UI,
	EVEN_ID_FUNTION_OUT, 
	EVEN_ID_ICC_MSG,
	EVEN_ID_MSG_TASK,	//pFunMessageTask
} EVEN_MSG_STATE;

extern void FIFO_Init(int pshared, unsigned int value);
extern void FIFO_OperatSetMsg(u16 MessageID,u16 Message);
extern int FIFO_OperatGetMsg(u16 *pMessageID,u16 *pMessage);
extern int FIFO_OperatPeekGetMsg(u16 *pMessageID,u16 *pMessage);


extern void APP_PushMessageTask(fPushTaskMsg pFun,u16 par);

//extern void APP_OperationKillThread(void* threadID);
//extern void APP_OperationCreateThread(void *(*pFunThread)(void*));
//extern void APP_OperationLoadThread(void* threadID);



//◎通用定义
	#define TIME_ONCE		(0)			//!< 马上返回
	#define TIME_INFINE		(-1)		//!< 一直等
	#define TIME_NOWAIT		(-2)		//!< 不等待
	
	/**
	 * WaitEvent事件类型定义
	 */
	enum EVENT_TYPE
	{
		EVENT_NONE		=0x00000000,	//!< 无效事件		
		EVENT_INDEX		=0x000000FF,	//!< 设备索引		
		EVENT_ICC		=0x00000100,	//!< 接触式IC卡/非接触式IC卡/PSAM卡
		EVENT_MAG		=0x00000200,	//!< 磁卡刷卡
		EVENT_NET		=0x00000400,	//!< 网络通信接口			API_NetEvent
		EVENT_SCAN		=0x00000800,	//!< 摄像头扫码						
		EVENT_KEY		=0x00001000,	//!< 按键
		EVENT_PEN		=0x00002000,	//!< 触摸屏
		EVENT_UART		=0x00004000,	//!< UART串口
		EVENT_AUDIO		=0x00008000,	//!< 音频通信接口		
		EVENT_UI		=0x00010000,	//!< 图形用户界面接口
		EVENT_SYS		=0x00020000,	//!< 系统接口
		EVENT_MISC		=0x00040000,	//!< 其他杂项接口
		EVENT_PRINT		=0x00080000,	//!< 打印完成
		EVENT_PHONE		=0x00100000,	//!< 电话功能				API_Phone_GetEvent
		EVENT_ERROR		=0x01000000,	//!< 错误
		EVENT_TIMEOUT	=0x02000000,	//!< 超时
		EVENT_OK  		=0x04000000,	//!< 确认	   确认		确认	是
		EVENT_CANCEL	=0x08000000,	//!< 取消	   返回		退出	否
		EVENT_QUIT		=0x10000000,	//!< 退出
		EVENT_MASK		=0x7FFFFF00,		//!< EVENT掩码
	};	

	//-------定时器控制---------------------
	extern void StartTimed500ms(void);
	extern void StopTimed500ms(void);
//	extern void Set_WaitEvent(int tTimeOutMs,u32 EventControl);
//	extern void Get_EventMsg(int *pTimeOutMs,u32 *pEventControl);
	extern void Rewrite_WaitTime(int tTimeOutMs);

	typedef u32 (*fEven_Process)(u16);
	typedef struct 
	{
		u32 				EventControl;
		fEven_Process		pFunEvenKey;
		fEven_Process		pFunEvenUI;
	}WAIT_EVENT_MSG;
	extern WAIT_EVENT_MSG tWaitEventMsg;

	/**
	 * 等待相关事件列表触发
	 * 
	 * @param tTimeOutMs 等待超时时间，单位:毫秒
	 * @param ... 事件类型(EVENT_TYPE|EVENT_INDEX),如：(EVENT_UART|UART_1)表示串口1
	 * @return 参考{@link EVENT_TYPE}
	 *	任何事件 返回EVENT_ERROR|EVENT_TIMEOUT
	 * 	EVENT_UI事件 返回EVENT_OK|EVENT_CANCEL|EVENT_QUIT
	 *  其它EVENT_TYPE 返回EVENT_TYPE|EVENT_INDEX
	 */
	extern u32  API_WaitEvent(int tTimeOutMs,...);
	


	typedef struct	
	{
		char Mask[4]; 	// "Even"
		void (*Init)(int,unsigned int);	//int pshared, unsigned int value
		void (*DeInit)(void);
		void (*SetMsg)(u16,u16);
		int (*GetMsg)(u16*,u16*);
		int (*GetMsgPeek)(u16*,u16*);
		void (*KillThread)(void*);
		void (*CreateThread)(void *(*pFunThread)(void*));
		void (*LoadThread)(void*); 
		void (*SetEvent)(int,u32);
		void (*GetEvent)(int *,u32 *);
		void (*SetEventTime)(int );
		u32  (*WaitEvent)(int,...);
	}API_Even_Def;

	extern const API_Even_Def ApiEven;
#endif
