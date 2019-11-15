
#ifndef _MIAI_
#define _MIAI_

typedef struct {
	unsigned short b;	//Window 瀹藉害
	unsigned short a;	//Window 楂樺害
} XuiFont;


/*
extern void API_Trace(char* pMsg,...);
extern void APP_Trace_Hex(char* msg,void* pBuff,int Len);

#define TRACE							API_Trace		//MercuryTrace		//
#define TRACE_HEX(msg,pBuff,Len)		APP_Trace_Hex(msg,pBuff,Len)
*/

typedef enum{
	LOG_DEBUG, 	/*显示调试信息*/
	LOG_INFO, 		/*显示提示信息*/
	LOG_WARN, 		/*显示警告信息*/
	LOG_ERROR, 	/*显示错误信息*/
}LOG_T; 
#define CURR_LOG_T						LOG_ERROR			
extern void OsLogSetTag(const char *Tag);
extern int OsLog(LOG_T Prio,const char *fmt,...);
#define LogSetTag						OsLogSetTag		
#define Log								OsLog


#endif
