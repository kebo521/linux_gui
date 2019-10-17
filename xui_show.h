
#ifndef _XUI_SHOW_
#define _XUI_SHOW_

typedef struct {
	unsigned short b;	//Window 宽度
	unsigned short a;	//Window 高度
} XuiFont;



extern void API_Trace(char* pMsg,...);
extern void APP_Trace_Hex(char* msg,void* pBuff,int Len);

#define TRACE							API_Trace		//MercuryTrace		//
#define TRACE_HEX(msg,pBuff,Len)		APP_Trace_Hex(msg,pBuff,Len)





#endif
