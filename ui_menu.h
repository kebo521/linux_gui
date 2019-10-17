#ifndef _UI_MENU_
#define _UI_MENU_
//=======单项菜单结构==========
typedef struct 
{//显示+功能
	char			*pText;		//显示内容
	APP_HANDLE		pFunMenu;	//功能项
}CMenuItemStru;


typedef struct _CMenuListPar
{//用于组织菜单的
	struct _CMenuListPar *pNext;
	APP_HANDLE		pFunMenu;	//功能项
	int				TextLen;	//显示内容长度
	char			Text[4];	//显示内容,编译器支持的话 Text[0]
}CMenuListPar;

typedef int (*APP_IndexH)(char*,int); 


extern int APP_CreateNewMenuByStr(char *pTitle,int tNum,char* *pMenuText,const APP_IndexH pUnifiedFunc,int TimeOutMs);
extern int APP_CreateNewMenuByStruct(char *pTitle,int tNum,CMenuItemStru *pMenuAll,int TimeOutMs);
extern int APP_CreateNewMenuByList(char* pTitle,CMenuListPar* pMenulist,int TimeOutMs);
extern void APP_AddCurrentMenuFun(APP_HANDLE pKeyFunc,APP_HANDLE pOutFunc,char *pAfterText);
typedef enum 
{
	MENU_KEY_FUN,		//功能键执行功能
	MENU_OUT_FUN,		//退出时键执行功能
	MENU_SHOW_AFT,		//最后一行显示
}UI_MENU_ADD_TYPE;
extern void APP_AddCurrentMenuOtherFun(UI_MENU_ADD_TYPE type,APP_HANDLE pFunc,const char *pFunTitle);

extern int APP_ShowProsseMenu(void);


#define GREATE_MENU_BY_STRUCT(pTitle,pMenuItem,TimeOutMs)	APP_CreateNewMenuByStruct(pTitle,sizeof(pMenuItem)/sizeof(pMenuItem[0]),pMenuItem,TimeOutMs)



#endif
