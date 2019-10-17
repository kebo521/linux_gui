#ifndef _UI_MENU_
#define _UI_MENU_
//=======����˵��ṹ==========
typedef struct 
{//��ʾ+����
	char			*pText;		//��ʾ����
	APP_HANDLE		pFunMenu;	//������
}CMenuItemStru;


typedef struct _CMenuListPar
{//������֯�˵���
	struct _CMenuListPar *pNext;
	APP_HANDLE		pFunMenu;	//������
	int				TextLen;	//��ʾ���ݳ���
	char			Text[4];	//��ʾ����,������֧�ֵĻ� Text[0]
}CMenuListPar;

typedef int (*APP_IndexH)(char*,int); 


extern int APP_CreateNewMenuByStr(char *pTitle,int tNum,char* *pMenuText,const APP_IndexH pUnifiedFunc,int TimeOutMs);
extern int APP_CreateNewMenuByStruct(char *pTitle,int tNum,CMenuItemStru *pMenuAll,int TimeOutMs);
extern int APP_CreateNewMenuByList(char* pTitle,CMenuListPar* pMenulist,int TimeOutMs);
extern void APP_AddCurrentMenuFun(APP_HANDLE pKeyFunc,APP_HANDLE pOutFunc,char *pAfterText);
typedef enum 
{
	MENU_KEY_FUN,		//���ܼ�ִ�й���
	MENU_OUT_FUN,		//�˳�ʱ��ִ�й���
	MENU_SHOW_AFT,		//���һ����ʾ
}UI_MENU_ADD_TYPE;
extern void APP_AddCurrentMenuOtherFun(UI_MENU_ADD_TYPE type,APP_HANDLE pFunc,const char *pFunTitle);

extern int APP_ShowProsseMenu(void);


#define GREATE_MENU_BY_STRUCT(pTitle,pMenuItem,TimeOutMs)	APP_CreateNewMenuByStruct(pTitle,sizeof(pMenuItem)/sizeof(pMenuItem[0]),pMenuItem,TimeOutMs)



#endif
