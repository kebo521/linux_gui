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



//��ͨ�ö���
	#define TIME_ONCE		(0)			//!< ���Ϸ���
	#define TIME_INFINE		(-1)		//!< һֱ��
	#define TIME_NOWAIT		(-2)		//!< ���ȴ�
	
	/**
	 * WaitEvent�¼����Ͷ���
	 */
	enum EVENT_TYPE
	{
		EVENT_NONE		=0x00000000,	//!< ��Ч�¼�		
		EVENT_INDEX		=0x000000FF,	//!< �豸����		
		EVENT_ICC		=0x00000100,	//!< �Ӵ�ʽIC��/�ǽӴ�ʽIC��/PSAM��
		EVENT_MAG		=0x00000200,	//!< �ſ�ˢ��
		EVENT_NET		=0x00000400,	//!< ����ͨ�Žӿ�			API_NetEvent
		EVENT_SCAN		=0x00000800,	//!< ����ͷɨ��						
		EVENT_KEY		=0x00001000,	//!< ����
		EVENT_PEN		=0x00002000,	//!< ������
		EVENT_UART		=0x00004000,	//!< UART����
		EVENT_AUDIO		=0x00008000,	//!< ��Ƶͨ�Žӿ�		
		EVENT_UI		=0x00010000,	//!< ͼ���û�����ӿ�
		EVENT_SYS		=0x00020000,	//!< ϵͳ�ӿ�
		EVENT_MISC		=0x00040000,	//!< ��������ӿ�
		EVENT_PRINT		=0x00080000,	//!< ��ӡ���
		EVENT_PHONE		=0x00100000,	//!< �绰����				API_Phone_GetEvent
		EVENT_ERROR		=0x01000000,	//!< ����
		EVENT_TIMEOUT	=0x02000000,	//!< ��ʱ
		EVENT_OK  		=0x04000000,	//!< ȷ��	   ȷ��		ȷ��	��
		EVENT_CANCEL	=0x08000000,	//!< ȡ��	   ����		�˳�	��
		EVENT_QUIT		=0x10000000,	//!< �˳�
		EVENT_MASK		=0x7FFFFF00,		//!< EVENT����
	};	

	//-------��ʱ������---------------------
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
	 * �ȴ�����¼��б���
	 * 
	 * @param tTimeOutMs �ȴ���ʱʱ�䣬��λ:����
	 * @param ... �¼�����(EVENT_TYPE|EVENT_INDEX),�磺(EVENT_UART|UART_1)��ʾ����1
	 * @return �ο�{@link EVENT_TYPE}
	 *	�κ��¼� ����EVENT_ERROR|EVENT_TIMEOUT
	 * 	EVENT_UI�¼� ����EVENT_OK|EVENT_CANCEL|EVENT_QUIT
	 *  ����EVENT_TYPE ����EVENT_TYPE|EVENT_INDEX
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
