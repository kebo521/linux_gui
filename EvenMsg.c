//#include "communal.h"

//#include<stdarg.h>
#include <signal.h>
#include <sys/time.h>
#include<pthread.h>

#include "types_def.h"
#include "EvenMsg.h"
#include "xui_show.h"



typedef struct _CMessageTable
{
	unsigned int		ReadID;
	unsigned int		WriteID;
	unsigned short		MessageID[4];
	unsigned short		MessagePar[4];
	//MERCURY_MESSAGE_S	tEvenMsg[4];
	pthread_t			threadID;
	fPushTaskMsg		pFunMessageTask;
	//------------------------------------
	struct _CMessageTable	*pPrevious;
}CMessageTable;
static CMessageTable *pMessageTable=NULL;

//==================================================================

void FIFO_OperatSetMsg(u16 MessageID,u16 Message)
{
	
}
//阻塞式接收消息接口
int  FIFO_OperatGetMsg(u16 *pMessageID,u16 *pMessage)
{
	return 0;
}

//=====非阻塞式接收消息接口=========================
int  FIFO_OperatPeekGetMsg(u16 *pMessageID,u16 *pMessage)
{
	return 0;
}



//======================================================================================
void  *APP_OperationKillThread()
{
	CMessageTable* pMsgTable;
	if(pMessageTable)
	{
		//if(threadID == NULL)
		{
			pMsgTable=pMessageTable;
			pMessageTable=pMsgTable->pPrevious;
			free(pMsgTable);
			TRACE("APP OperationKillThread pMessageTable[%X],threadID[%X]\r\n",pMessageTable,pMessageTable->threadID);
			//return NULL;
		}
		pMsgTable=pMessageTable;
		pMessageTable=pMsgTable->pPrevious;
		free(pMsgTable);
		//------通知上层应用继续执行-------
		FIFO_OperatSetMsg(EVEN_ID_FUNTION_OUT,0);
		//------结束当前线程----------
		//ExitThread(threadID);
	}
	return NULL;
}


void APP_PushMessageTask(fPushTaskMsg pFun,u16 par)
{
	if(pMessageTable)
	{
		pMessageTable->pFunMessageTask=pFun;
		FIFO_OperatSetMsg(EVEN_ID_MSG_TASK,par);
	}
}

void APP_OperationCreateThread(int pFunThread,u32 cbStack)
{
	CMessageTable* pMsgTable;
	pMsgTable = (CMessageTable*)malloc(sizeof(CMessageTable));
	API_memset(pMsgTable,0x00,sizeof(CMessageTable));
	//pMsgTable->threadID  = CreateThread(NULL,cbStack, pFunThread,0,APP_OperationKillThread,STACK_SIZE_RESERVATION,NULL);
	/*
	if(pthread_create(&pMsgTable->threadID, NULL,APP_OperationKillThread,NULL))
	{
		TRACE("pthread create error!\r\n");
		return;
	}
	*/
	pMsgTable->pFunMessageTask = NULL;
	pMsgTable->pPrevious = pMessageTable;
	pMessageTable=pMsgTable;
	TRACE("APP OperationCreateThread pMessageTable[%X],threadID[%X]\r\n",pMessageTable,pMessageTable->threadID);
}

/*
void APP_OperationLoadThread(u32 threadID)
{
	CMessageTable* pMsgTable;
	pMsgTable = (CMessageTable*)malloc(sizeof(CMessageTable));
	API_memset(pMsgTable,0x00,sizeof(CMessageTable));
	pMsgTable->threadID = threadID;
	pMsgTable->pFunMessageTask = NULL;
	pMsgTable->pPrevious = pMessageTable;
	pMessageTable=pMsgTable;
	TRACE("APP OperationLoadThread pMessageTable[%X],threadID[%X]\r\n",pMessageTable,pMessageTable->threadID);
}*/
//=======================================================================================
//==================500ms定时器模块======================================================
static int tTimeOutMsEvent=0,externalLoadTimeMs=0;
//static sigaction TimedRefresh500MsID = {NULL,NULL,0,0,0};

void  TimedRefresh500Ms()
{
	if(tWaitEventMsg.EventControl&EVENT_TIMEOUT)
	{
		if(externalLoadTimeMs)
		{
			tTimeOutMsEvent=externalLoadTimeMs;
			externalLoadTimeMs=0;
		}
		tTimeOutMsEvent -= 500;
		if(tTimeOutMsEvent <= 0)
		{
			FIFO_OperatSetMsg(EVEN_ID_TIME_OUT,500);
		}
	}
}

/*
void handler(int sig)
{
    printf("Handle the signal %d\n", sig);
}
 
int main(int argc, char **argv)
{
    sigset_t sigset;    // 用于记录屏蔽字
    sigset_t ign;       // 用于记录被阻塞(屏蔽)的信号集
    struct sigaction act;
 
    // 清空信号集
    sigemptyset(&sigset);
    
 
    // 向信号集中添加 SIGINT
    sigaddset(&sigset, SIGINT);
 
    // 设置处理函数 和 信号集
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
 
    printf("Wait the signal SIGNAL...\n");
    pause();
 
    // 设置进程屏蔽字, 在本例中为屏蔽 SIGINT
    sigprocmask(SIG_SETMASK, &sigset, 0);
    printf("Please press Ctrl + C in 10 seconds...\n");
    sleep(10);
 
    // 测试 SIGINT 是否被屏蔽
    sigemptyset(&ign);
    sigpending(&ign);
    if (sigismember(&ign, SIGINT))
    {
        printf("The SIGINT signal has ignored\n");
    }
 
    // 从信号集中删除信号 SIGINT
    sigdelset(&sigset, SIGINT);
    printf("Wait the signal SIGINT...\n");
 
    // 将进程的屏蔽字重新设置, 即取消对 SIGINT 的屏蔽
    // 并挂起进程
    sigsuspend(&sigset);
 
    printf("The app will exit in 5 secondes!\n");
    sleep(5);
 
    return 0;
}



void StartTimed500ms(void)
{
	if(0)//if(TimedRefresh500MsID.sa_handler)
	{
		struct itimerval val;
		val.it_value.tv_sec = 0; //1秒后启用定时器
		val.it_value.tv_usec = 500*1000;
		val.it_interval = val.it_value; //定时器间隔为1s
		setitimer(ITIMER_PROF, &val, NULL);
	}
	else
	{
		struct itimerval val;
		struct sigaction act;
		act.sa_handler = TimedRefresh500Ms; //设置处理信号的函数
		act.sa_flags  = 0;
		sigemptyset(&act.sa_mask);
		sigaction(SIGPROF,&act, NULL);//时间到发送SIGROF信号
	         
	    val.it_value.tv_sec = 0; //1秒后启用定时器
	    val.it_value.tv_usec = 500*1000;
	    val.it_interval = val.it_value; //定时器间隔为1s
	    setitimer(ITIMER_PROF, &val, NULL);
		TimedRefresh500MsID = 0xff;
	}
}

void StopTimed500ms(void)
{
	if(TimedRefresh500MsID)
	{
		struct itimerval val;
		val.it_value.tv_sec = 0; //1秒后启用定时器
		val.it_value.tv_usec = 0;
		val.it_interval = val.it_value; //定时器间隔为1s
		setitimer(ITIMER_PROF, &val, NULL);
	}
}
*/

void Set_WaitEvent(int tTimeOutMs,u32 EventControl)
{
	tWaitEventMsg.EventControl=EventControl;
	if(tTimeOutMs > 0)
	{
		externalLoadTimeMs = tTimeOutMs;
		tWaitEventMsg.EventControl |= EVENT_TIMEOUT;
	}
}

void Get_EventMsg(int *pTimeOutMs,u32 *pEventControl)
{
	if(pEventControl)
		*pEventControl=tWaitEventMsg.EventControl;
	if(pTimeOutMs)
		*pTimeOutMs=externalLoadTimeMs;
}


void Rewrite_WaitTime(int tTimeOutMs)
{
	if(externalLoadTimeMs^tTimeOutMs)
		externalLoadTimeMs = tTimeOutMs;
}



//========================事件处理模块===================================================
WAIT_EVENT_MSG tWaitEventMsg={0};


//===========================================================================

u32  API_WaitEvent(int tTimeOutMs,...)
{
	u16 MessageID,Message;
	u32 Event,EventControl=0x00000000;
	va_list arg;
	va_start( arg, tTimeOutMs );
	while(EVENT_NONE != (Event=va_arg(arg, u32)))
	{
		EventControl |= Event;
	}	
	va_end( arg );

	tWaitEventMsg.EventControl=EventControl;
	if(tTimeOutMs >= 0)
	{
		tWaitEventMsg.EventControl |= EVENT_TIMEOUT;
		externalLoadTimeMs = tTimeOutMs;
	}
	//----------------------------------------------------
	Event=EVENT_NONE;
	do
	{//---------------阻塞处理-----------------------------------
		if(FIFO_OperatGetMsg(&MessageID,&Message))
		{
			if(MessageID==EVEN_ID_KEY_DOWN)
			{
				if(tWaitEventMsg.EventControl&EVENT_UI)
				{
					if(Message==1)
					{
						Event=EVENT_OK;
						break;
					}
					if(Message==2)
					{
						Event=EVENT_CANCEL;
						break;
					}
				}
				if(tWaitEventMsg.EventControl&EVENT_MISC)
				{
					if(Message==3)
					{
						Event=EVENT_QUIT;
						break;
					}
				}
				if(tWaitEventMsg.pFunEvenKey)
				{
					Event=(*tWaitEventMsg.pFunEvenKey)(Message);
				}
				else if(tWaitEventMsg.EventControl&EVENT_KEY)
				{
					Event=EVENT_KEY|Message;
				}
				if(Event==EVENT_NONE)
				{
					externalLoadTimeMs = tTimeOutMs;
				}
			}
			else if(MessageID==EVEN_ID_UART_RECV)
			{
				Event=EVENT_UART;
			}
			else if(MessageID==EVEN_ID_TIME_OUT)
			{
				Event=EVENT_TIMEOUT;
			}
			else if(MessageID==EVEN_ID_SCAN)
			{
				Event=EVENT_SCAN;
			}
			else if(MessageID==EVEN_ID_ICC_MSG)
			{
				Event=EVENT_ICC|Message;
			}
			else if(MessageID==EVEN_ID_SHOW_UI)
			{
				if(tWaitEventMsg.pFunEvenUI)
				{
					fEven_Process	pOldEvenKey;
					pOldEvenKey=tWaitEventMsg.pFunEvenKey;
					Event=(*tWaitEventMsg.pFunEvenUI)(Message);
					
					tWaitEventMsg.EventControl=EventControl;
					if(tTimeOutMs>0)
					{
						tWaitEventMsg.EventControl |= EVENT_TIMEOUT;
						externalLoadTimeMs = tTimeOutMs;
					}
					tWaitEventMsg.pFunEvenKey=pOldEvenKey;
				}
			}
		}
	}while(Event == EVENT_NONE);
	tWaitEventMsg.EventControl=0x00000000;
	tWaitEventMsg.pFunEvenKey=NULL;
	return Event;
}






