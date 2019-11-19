#ifndef _UI_MENU_
#define _UI_MENU_
//=======单项菜单结构==========

typedef int (*fMenuFun)(XuiWindow*,char*); 
typedef int (*APP_IndexH)(XuiWindow*,char*,int); 

typedef struct 
{//显示+功能
	char			*pText;		//显示内容
	fMenuFun		pFunMenu;	//功能项
}CMenuItemStru;


typedef struct _CMenuListPar
{//用于组织菜单的
	struct _CMenuListPar *pNext;
	fMenuFun		pFunMenu;	//功能项
	int				TextLen;	//显示内容长度
	char			Text[4];	//显示内容,编译器支持的话 Text[0]
}CMenuListPar;

extern int APP_CreateNewMenuByStr(char *pTitle,int tNum,char* *pMenuText,const APP_IndexH pUnifiedFunc,int TimeOutMs);
extern int APP_CreateNewMenuByStruct(char *pTitle,int tNum,CMenuItemStru *pMenuAll,int TimeOutMs);
extern int APP_CreateNewMenuByList(char* pTitle,CMenuListPar* pMenulist,int TimeOutMs);
typedef enum 
{
	MENU_KEY_FUN,		//功能键执行功能
	MENU_OUT_FUN,		//退出时键执行功能
	MENU_SHOW_AFT,		//最后一行显示
}UI_MENU_ADD_TYPE;
extern void APP_AddCurrentMenuOtherFun(UI_MENU_ADD_TYPE type,fMenuFun pFunc,const char *pFunTitle);

extern int APP_ShowProsseMenu(XuiWindow *pWindow);


#define GREATE_MENU_BY_STRUCT(pTitle,pMenuItem,TimeOutMs)	APP_CreateNewMenuByStruct(pTitle,sizeof(pMenuItem)/sizeof(pMenuItem[0]),pMenuItem,TimeOutMs)



#endif
