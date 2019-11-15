
#ifndef _MIAI_
#define _MIAI_

typedef struct {
	unsigned short b;	//Window 宽度
	unsigned short a;	//Window 高度
} XuiFont;


/*
extern void API_Trace(char* pMsg,...);
extern void APP_Trace_Hex(char* msg,void* pBuff,int Len);

#define TRACE							API_Trace		//MercuryTrace		//
#define TRACE_HEX(msg,pBuff,Len)		APP_Trace_Hex(msg,pBuff,Len)
*/

typedef enum{
	LOG_DEBUG, 	/*��ʾ������Ϣ*/
	LOG_INFO, 		/*��ʾ��ʾ��Ϣ*/
	LOG_WARN, 		/*��ʾ������Ϣ*/
	LOG_ERROR, 	/*��ʾ������Ϣ*/
}LOG_T; 
#define CURR_LOG_T						LOG_ERROR			
extern void OsLogSetTag(const char *Tag);
extern int OsLog(LOG_T Prio,const char *fmt,...);
#define LogSetTag						OsLogSetTag		
#define Log								OsLog


#endif
