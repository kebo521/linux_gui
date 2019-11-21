#ifndef _UI_MENU_
#define _UI_MENU_
//=======����˵��ṹ==========

typedef int (*fMenuFun)(char*); 
typedef int (*APP_IndexH)(char*,int); 

typedef struct 
{//��ʾ+����
	char			*pText;		//��ʾ����
	fMenuFun		pFunMenu;	//������
}CMenuItemStru;


typedef struct _CMenuListPar
{//������֯�˵���
	struct _CMenuListPar *pNext;
	fMenuFun		pFunMenu;	//������
	int				TextLen;	//��ʾ���ݳ���
	char			Text[4];	//��ʾ����,������֧�ֵĻ� Text[0]
}CMenuListPar;

extern int APP_CreateNewMenuByStr(char *pTitle,int tNum,char* *pMenuText,const APP_IndexH pUnifiedFunc,int TimeOutMs);
extern int APP_CreateNewMenuByStruct(char *pTitle,int tNum,CMenuItemStru *pMenuAll,int TimeOutMs);
extern int APP_CreateNewMenuByList(char* pTitle,CMenuListPar* pMenulist,int TimeOutMs);
typedef enum 
{
	MENU_KEY_FUN,		//���ܼ�ִ�й���
	MENU_OUT_FUN,		//�˳�ʱ��ִ�й���
	MENU_SHOW_AFT,	//���һ����ʾ
	MENU_BACK_MAP,	//����ͼƬ����
}UI_MENU_ADD_TYPE;
extern void APP_AddCurrentMenuOtherFun(UI_MENU_ADD_TYPE type,void* pFunc,const char *pData);

extern int APP_ShowProsseMenu();


#define GREATE_MENU_BY_STRUCT(pTitle,pMenuItem,TimeOutMs)	APP_CreateNewMenuByStruct(pTitle,sizeof(pMenuItem)/sizeof(pMenuItem[0]),pMenuItem,TimeOutMs)



#endif
