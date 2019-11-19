/*****************************�˵�����******************************
//����-------  ���ڴ���˵�������ִ�С���ʾ������
//����-------  �˹���
//����ʱ��--20150604
******************************************************************************/
#include "types_def.h"

#include "EvenMsg.h"

#include "xui_ui.h"
#include "xui_fb.h"
#include "xui_font.h"
#include "xui_gui.h"

#include "ui_menu.h"

//#include "EvenMsg.h"


//============�˵���ʾ�����õ��Ĳ���======================
typedef enum
{
	_GUI_MENU_SHOW,
	_GUI_MENU_PROCESS,
	_GUI_MENU_RUN,
	_GUI_MENU_RET,
	_GUI_MENU_QUT,
	_GUI_MENU_EXT,
} GUI_Menu_msg;
//============�˵�����ṹ==================================
typedef struct _CMenuUITable
{
	struct _CMenuUITable *pPrevious;	//����ָ��
	u8		ShowState;		//-�˵�����������״̬(GUI_Menu_msg)
	u8		TeamTatla;		//-�˵�������
	u8		TeamCurr;		//-��ǰ��ѡ��Ĳ˵���
	u8		ShowHead;		//-����һ����ʾ�Ĳ˵���
	//----�����ֽ���������4��������----------------
	char	*pAfterText;		//���һ����ʾ����
	fMenuFun pKeyFunc;		//���ܼ���������
	char	*pKeyFuncTitle;		//���ܼ�������
	fMenuFun pOutFunc;		//�˳�����
	char	*pOutFuncTitle;		//�˳����ܱ���
	APP_IndexH pUnifiedFunc;	//ͳһ����˵��ӿ�

	int				TimeOutMs;		//�˵���ʾʱ��
	char  			*pTitle;		//�˵�����
	CMenuItemStru	pItem[1];		//�˵�����,�˵�����
}CMenuUITable;
static CMenuUITable *pMenuUiTable=NULL;


//====================================================================
//����:   �����µĹ̶����ز˵�(�˹�����Ҫ�����ڴ�ռ�)
//��������:pTitle ���⣬pMenuText�˵���ʾ�tNum�˵���������pMenuFunc�˵��Ĺ���
//�������:��
//����ʱ��:  	20161128
//---------------------------------------------------------------
int APP_CreateNewMenuByStr(char *pTitle,int tNum,char* *pMenuText,const APP_IndexH pUnifiedFunc,int TimeOutMs)
{
	CMenuUITable *pNeTable;
	char *pText;
	u16 mLen,i,slen,len;
	/*
	if(pMenuUiTable!=NULL && pMenuUiTable->pMenuFunc == pMenuFunc)
	{//----ͬһ���ڷǵ�һ�δ�����ֱ����ʾ-------
		pMenuUiTable->ShowState=_GUI_MENU_SHOW;
		return;
	}*/
	mLen=sizeof(CMenuUITable) + sizeof(CMenuItemStru)*tNum;
	slen=0;
	for(i=0;i<tNum;i++)
	{
		len =	API_strlen(pMenuText[i])+1;
		slen += DfGetBeiSu(len,4);	//��Ӧ����ṹ
	}
	pNeTable=(CMenuUITable*)malloc(mLen+slen);
	if(pNeTable == NULL)
	{//�ڴ�����ʧ�ܣ��˻�֮ǰ��Ui
		TRACE("CreateNewMenuByStr malloc[%d] fail!\n",mLen+slen);
		return RET_ERR;
	}
	API_memset(pNeTable,0x00,mLen);
	//------------------------------------------------------------
	pText = (char*)&pNeTable->pItem[tNum];
	for(i=0;i<tNum;i++)
	{
		len =	API_strlen(pMenuText[i])+1;
		API_memcpy(pText,pMenuText[i],len);		
		pNeTable->pItem[i].pText=pText;
		pNeTable->pItem[i].pFunMenu=NULL;
		pText += DfGetBeiSu(len,4);	//��Ӧ����ṹ
	}
	//----------------------------------------------------------
	pNeTable->TimeOutMs=TimeOutMs;
	pNeTable->pTitle=pTitle;
	pNeTable->pUnifiedFunc=pUnifiedFunc;
	pNeTable->TeamTatla=tNum;
	pNeTable->ShowState=_GUI_MENU_SHOW;
	pNeTable->pPrevious =pMenuUiTable;
	pMenuUiTable=pNeTable;
	return RET_OK;
}

//====================================================================
//����:   �����µ��ڴ�˵�(���в˵���ʹ���ⲿ�ռ䣬�˹�����Ҫ����ϴ��ڴ�ռ�)
//��������:pTitle ���⣬tNum�˵���������pMenuAll�˵���ʾ�빦��
//�������:RET_OK�����ɹ���RET_ERR ����ʧ��
//����ʱ��:  	20151230
//---------------------------------------------------------------
int APP_CreateNewMenuByStruct(char *pTitle,int tNum,CMenuItemStru *pMenuAll,int TimeOutMs)
{
	CMenuUITable *pNeTable;
	int 		i;
	//----------------------------------------------------------
	pNeTable=(CMenuUITable*)malloc(sizeof(CMenuUITable)+sizeof(CMenuItemStru)*tNum);
	if(pNeTable == NULL)
	{	//�ڴ�����ʧ�ܣ��˻�֮ǰ��Ui
	//	TRACE("pMenuUiTable Null!\n");
		//APP_ShowMsg(pTitle,"pMenuUiTable Null",2000);
		return RET_ERR;
	}
	API_memset(pNeTable,0x00,sizeof(CMenuUITable));
	//--------------�޸���ʱ����------------------
	for(i=0;i<tNum;i++)
	{
		pNeTable->pItem[i].pFunMenu=pMenuAll[i].pFunMenu;
		pNeTable->pItem[i].pText=pMenuAll[i].pText;
	}	
	//-------------д�������Ϣ-------
	pNeTable->TimeOutMs=TimeOutMs;
	pNeTable->pTitle = pTitle;
	pNeTable->TeamTatla=tNum;
	pNeTable->TeamCurr=0;	//Ĭ��ѡ����
	pNeTable->ShowState=_GUI_MENU_SHOW;
	pNeTable->pPrevious =pMenuUiTable;
	pMenuUiTable=pNeTable;
	return RET_OK;
}


//====================================================================
//����:   ������˵�ת�ɽṹ�˵�(�˹�����Ҫ�����ڴ�ռ䣬�˵��Ĵ�С�������ڴ����)
//����:   ��pStrlist�еĲ˵����ݿ����ṹ�˵��У����ͷ�pStrlist(��ShowFlowMenu������)����Ӧ�����ڿռ�
//��������:pTitle �˵�����,pStrlist �˵�������(��ʾ���ݣ�����ʽ)
//�������:RET_OK�����ɹ���RET_ERR ����ʧ��
//����ʱ��:  	20170207
//---------------------------------------------------------------
int APP_CreateNewMenuByList(char* pTitle,CMenuListPar* pMenulist,int TimeOutMs)
{
	u8 i,MenuCount=0;
	u8 slen,TatleLen=0;
	char *pShowMsg;
	CMenuUITable *pNeTable;
	CMenuListPar *pNewList;
	if(pMenulist==NULL) return RET_ERR;
	pNewList=pMenulist;
	while(pNewList != NULL)
	{
		MenuCount++;
		slen=pNewList->TextLen+1;
		TatleLen += DfGetBeiSu(slen,4);	//��Ӧ����ṹ
		pNewList=pNewList->pNext;
	}
	//----------------------------------------------------------
	pNeTable=(CMenuUITable*)malloc(sizeof(CMenuUITable)+sizeof(CMenuItemStru)*MenuCount +TatleLen);
	API_memset(pNeTable,0x00,sizeof(CMenuUITable));
	//--------------�޸���ʱ����------------------
	pShowMsg=(char*)&pNeTable->pItem[MenuCount];
	for(i=0;i<MenuCount;i++)
	{
		API_memcpy(pShowMsg,pMenulist->Text,pMenulist->TextLen);
		pShowMsg[pMenulist->TextLen]='\0';

		pNeTable->pItem[i].pText = pShowMsg;
		pNeTable->pItem[i].pFunMenu=pMenulist->pFunMenu;

		slen=pNewList->TextLen+1;
		pShowMsg += DfGetBeiSu(slen,4);	//��Ӧ����ṹ
		
		pNewList=pMenulist;
		pMenulist=pMenulist->pNext;
		free(pNewList);
	}
	//-------------------------------------------------------
	pNeTable->TimeOutMs=TimeOutMs;
	pNeTable->pTitle =pTitle;
	pNeTable->TeamTatla=MenuCount;
	pNeTable->ShowState= _GUI_MENU_SHOW;
	pNeTable->pPrevious =pMenuUiTable;
	pMenuUiTable=pNeTable;
	return RET_OK;
}


//====================================================================
//����:  ������һ��(����һ��)�˵�
//����:   ����CreateNewRamMenu��������ڴ�ռ�
//��������:series ���Ϸ��ؼ�����series=0ʱ���˳����в˵�,pEndMenuΪ�����Ϸ��ؼ�����pEndMenu��Ϊ�գ����ص�pEndMenu����һ�����˳���ʾ����
//�������:GUI_Menu_msg
//����ʱ��:  	20170317
//---------------------------------------------------------------
GUI_Menu_msg ReturnToPreviousMenu(XuiWindow *pWindow,u8 series,CMenuUITable *pEndMenu)  //RevocateMenuList
{
	CMenuUITable* pOldeMenu;
	while(pMenuUiTable!=NULL)
	{
		if(pMenuUiTable->pOutFunc)
			pMenuUiTable->pOutFunc(pWindow,pMenuUiTable->pOutFuncTitle);
		if(pEndMenu == pMenuUiTable)
		{
			pMenuUiTable=pMenuUiTable->pPrevious;
			free(pEndMenu);
			return _GUI_MENU_EXT;
		}
		pOldeMenu=pMenuUiTable;
		pMenuUiTable=pMenuUiTable->pPrevious;
		free(pOldeMenu);
		if(series)
		{
			if((--series)==0)
			{
				if(pMenuUiTable)
					return _GUI_MENU_SHOW;
				return _GUI_MENU_EXT;
			}
		}
	}
	return _GUI_MENU_EXT;
}


//====================================================================
//����:  ��ǰ�˵�����������ܼ�ִ�й���
//����:  ���ڲ˵���Ҫ���Ӷ��⹦��
//��������:type ��������,pFunc����ִ�к�����pFunTitle ���ܱ���(��֧��ջ�ռ�)��
//�������:��
//---------------------------------------------------------------
void APP_AddCurrentMenuOtherFun(UI_MENU_ADD_TYPE type,fMenuFun pFunc,const char *pFunTitle)
{
	if(pMenuUiTable)
	{
		if(type==MENU_KEY_FUN)
		{
			pMenuUiTable->pKeyFunc	= pFunc;
			pMenuUiTable->pKeyFuncTitle = (char*)pFunTitle;
		}
		else if(type==MENU_OUT_FUN)
		{
			pMenuUiTable->pOutFunc	= pFunc;
			pMenuUiTable->pOutFuncTitle = (char*)pFunTitle;
		}
		else if(type==MENU_SHOW_AFT)
		{
			pMenuUiTable->pAfterText = (char*)pFunTitle;
		}
	}
}

//====================================================================
//����:  ����˵���ʾ��
//��������:pMenu �˵��ṹ��index�����line�к�(1~n)
//�������:pOutShow ��ʾ���
//---------------------------------------------------------------
void ShowMenuItem(void *pMenu,int index,int line,char *pOutShow)
{
	CMenuItemStru *pItem=(CMenuItemStru *)pMenu;
	API_sprintf(pOutShow,"%d.%s",line,pItem[index].pText);
}
//====================================================================
//����:  ��׼SDK��ʾ����˵���
//����:  ��ʾ������������pMenuUiTable����Ĳ˵�
//��������:��
//�������:�˵�ִ�н��
//ע:�ı��� pWindow ��Ҫ����show,û�ı䲻��
//---------------------------------------------------------------
int APP_ShowProsseMenu(XuiWindow *pWindow)
{
	CMenuUITable *pStartMenuAdd=pMenuUiTable;
	u32 event,ret=EVENT_NONE;
	while(pMenuUiTable)
	{
		//----------��ʾ�˵�---------------------
		API_GUI_CreateWindow(pWindow,pMenuUiTable->pTitle,TOK,TCANCEL,1);
		API_GUI_Menu(pWindow,pMenuUiTable->pItem,&ShowMenuItem,pMenuUiTable->TeamTatla,pMenuUiTable->TeamCurr,pMenuUiTable->ShowHead,pMenuUiTable->pAfterText,pMenuUiTable->pKeyFunc);
		API_GUI_Show(pWindow);
	//	pMenuUiTable->ShowState=_GUI_MENU_PROCESS;
		//----------����˵�----------------------
		event=API_WaitEvent(pMenuUiTable->TimeOutMs,EVENT_UI,EVENT_NONE);	
		if(event==EVENT_INDEX)
		{
			int index,head;
			index=API_GUI_Menu_GetInxAndHear(&head);
			if(index<pMenuUiTable->TeamTatla)
			{
				pMenuUiTable->TeamCurr=index;
				pMenuUiTable->ShowHead=head;
	//			pMenuUiTable->ShowState=_GUI_MENU_SHOW; //ִ������ʾ�˵�
				if(pMenuUiTable->pUnifiedFunc)
				{//-----ͳһ�˵�����----------
					ret=(*pMenuUiTable->pUnifiedFunc)(pWindow,pMenuUiTable->pItem[index].pText,index);
					if((ret&EVENT_MASK) == EVENT_QUIT)
					{
						if(_GUI_MENU_EXT == ReturnToPreviousMenu(pWindow,ret&EVENT_INDEX,pStartMenuAdd))
							break;
					}
					continue;
				}
				if(pMenuUiTable->pItem[index].pFunMenu)
				{//-----�����˵�����------
					ret=(*pMenuUiTable->pItem[index].pFunMenu)(pWindow,pMenuUiTable->pItem[index].pText);
					if((ret&EVENT_MASK) == EVENT_QUIT)
					{
						if(_GUI_MENU_EXT == ReturnToPreviousMenu(pWindow,ret&EVENT_INDEX,pStartMenuAdd))
							break;
					}
				}
			}
		}
		else if(event==EVENT_CANCEL)
		{
			if(_GUI_MENU_EXT == ReturnToPreviousMenu(pWindow,1,pStartMenuAdd))
				break;
	//		else
	//			pMenuUiTable->ShowState=_GUI_MENU_SHOW;
		}
		else if(event==EVENT_TIMEOUT)
		{
			if(_GUI_MENU_EXT == ReturnToPreviousMenu(pWindow,1,pStartMenuAdd))
				break;
	//		else
	//			pMenuUiTable->ShowState=_GUI_MENU_SHOW;
		}
		else if(event==(EVENT_KEY|3))//K_FUNC
		{
			if(pMenuUiTable->pKeyFunc==NULL)
				continue;
	//		pMenuUiTable->ShowState=_GUI_MENU_SHOW; //ִ������ʾ�˵�
			if(pMenuUiTable->pKeyFuncTitle)
				ret=pMenuUiTable->pKeyFunc(pWindow,pMenuUiTable->pKeyFuncTitle);
			else
				ret=pMenuUiTable->pKeyFunc(pWindow,pMenuUiTable->pTitle);
			if(EVENT_QUIT&ret)
			{//-----�˳����в˵�----------
				ReturnToPreviousMenu(pWindow,ret&EVENT_INDEX,pStartMenuAdd);
				break;
			}
		}
	}
	return ret;
}
//====================================================================
//����:  ��ʾ����˵���
//����:  ��ʾ������������pMenuUiTable����Ĳ˵�
//��������:��
//�������:��
//����ʱ��:  	20140726
//---------------------------------------------------------------
/*
int APP_ShowMenuProsse(void)
{
	CMenuUITable *pStartMenuAdd=pMenuUiTable;
	u16 MessageID,Message;
	while(pMenuUiTable)
	{
		if(pMenuUiTable->ShowState==_GUI_MENU_SHOW)
		{//----------��ʾ�˵�--------
			APP_GUI_Menu(pMenuUiTable->pShowTatle,(int)pMenuUiTable->ShowHead,(int)pMenuUiTable->TeamTatla,(int)pMenuUiTable->TeamCurr,pMenuUiTable->pMenuText);
			pMenuUiTable->ShowState=_GUI_MENU_PROCESS;
		}
		Set_WaitEvent(pMenuUiTable->TimeOutMs,EVENT_KEY);
		//---------------����ʽ------------------------------------------
		if(FIFO_OperatGetMsg(&MessageID,&Message))
		{
			if(MessageID==EVEN_ID_KEY_DOWN)
			{
				Rewrite_WaitTime(pMenuUiTable->TimeOutMs);
				if((Message==K_OK)||(Message>=K_1&&Message<=K_9))
				{
					if(Message!=K_OK)
					{
						Message -= K_1;
						if(Message >= pMenuUiTable->TeamTatla)
							continue;
						pMenuUiTable->TeamCurr=Message;
					}
					if(pMenuUiTable->pMenuFunc[pMenuUiTable->TeamCurr])
					{
						pMenuUiTable->ShowState=_GUI_MENU_SHOW;
						if(EVENT_QUIT == (*pMenuUiTable->pMenuFunc[pMenuUiTable->TeamCurr])(pMenuUiTable->pMenuText[pMenuUiTable->TeamCurr]))
						{//-----�˳����в˵�----------
							ReturnToPreviousMenu(0,pStartMenuAdd);
							Set_WaitEvent(TIME_INFINE,EVENT_NONE);
							return EVENT_QUIT;
						}
						Set_WaitEvent(pMenuUiTable->TimeOutMs,EVENT_KEY);
					}
				}
				else if(Message==K_CANCEL)
				{
					if(_GUI_MENU_EXT == ReturnToPreviousMenu(1,pStartMenuAdd))
						break;
					else
						pMenuUiTable->ShowState=_GUI_MENU_SHOW;
				}
				else if(Message==K_UP)
				{
					if(pMenuUiTable->TeamCurr > 0)
					{
						pMenuUiTable->TeamCurr--;
						if(pMenuUiTable->TeamCurr < pMenuUiTable->ShowHead)
							pMenuUiTable->ShowHead--;
						pMenuUiTable->ShowState=_GUI_MENU_SHOW;
					}
					else
					{
						pMenuUiTable->TeamCurr=pMenuUiTable->TeamTatla-1;
						if(pMenuUiTable->TeamCurr >= MENU_TIEM_MAX)
							pMenuUiTable->ShowHead=pMenuUiTable->TeamCurr-MENU_TIEM_MAX;
						else
							pMenuUiTable->ShowHead=0;
						pMenuUiTable->ShowState=_GUI_MENU_SHOW;
					}
				}
				else if(Message==K_DOWN)
				{
					if((pMenuUiTable->TeamCurr+1) < pMenuUiTable->TeamTatla)
					{
						pMenuUiTable->TeamCurr++;
						if((pMenuUiTable->TeamCurr-pMenuUiTable->ShowHead) >= MENU_TIEM_MAX)
							pMenuUiTable->ShowHead++;
						pMenuUiTable->ShowState=_GUI_MENU_SHOW;
					}
					else
					{
						pMenuUiTable->TeamCurr=0;
						pMenuUiTable->ShowHead=0;
						pMenuUiTable->ShowState=_GUI_MENU_SHOW;
					}
				}
			}
			else if(MessageID==EVEN_ID_TIME_OUT)
			{
				if(_GUI_MENU_EXT == ReturnToPreviousMenu(1,pStartMenuAdd))
					break;
				else
					pMenuUiTable->ShowState=_GUI_MENU_SHOW;
			}
		}
	}
	Set_WaitEvent(TIME_INFINE,EVENT_NONE);
	return EVENT_NONE;
}
*/


