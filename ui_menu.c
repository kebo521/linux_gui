/*****************************菜单服务******************************
//功能-------  用于处理菜单创建、执行、显示、处理
//作者-------  邓国祖
//创作时间--20150604
******************************************************************************/
#include "types_def.h"

#include "EvenMsg.h"

#include "xui_ui.h"
#include "xui_fb.h"
#include "xui_font.h"
#include "xui_gui.h"

#include "ui_menu.h"

//#include "EvenMsg.h"


//============菜单显示处理用到的参数======================
typedef enum
{
	_GUI_MENU_SHOW,
	_GUI_MENU_PROCESS,
	_GUI_MENU_RUN,
	_GUI_MENU_RET,
	_GUI_MENU_QUT,
	_GUI_MENU_EXT,
} GUI_Menu_msg;
//============菜单处理结构==================================
typedef struct _CMenuUITable
{
	struct _CMenuUITable *pPrevious;	//回退指针
	u8		ShowState;		//-菜单处理所处的状态(GUI_Menu_msg)
	u8		TeamTatla;		//-菜单总条数
	u8		TeamCurr;		//-当前所选择的菜单项
	u8		ShowHead;		//-屏第一行显示的菜单项
	//----上面字节数必须是4的整数倍----------------
	char	*pAfterText;		//最后一行显示内容
	fMenuFun pKeyFunc;		//功能键处理内容
	char	*pKeyFuncTitle;		//功能键处标题
	fMenuFun pOutFunc;		//退出功能
	char	*pOutFuncTitle;		//退出功能标题
	APP_IndexH pUnifiedFunc;	//统一处理菜单接口

	int				TimeOutMs;		//菜单显示时间
	char  			*pTitle;		//菜单标题
	CMenuItemStru	pItem[1];		//菜单内容,菜单功能
}CMenuUITable;
static CMenuUITable *pMenuUiTable=NULL;


//====================================================================
//功能:   创建新的固定返回菜单(此过程需要申请内存空间)
//输入数据:pTitle 标题，pMenuText菜单显示项，tNum菜单总项数，pMenuFunc菜单的功能
//输出数据:无
//创作时间:  	20161128
//---------------------------------------------------------------
int APP_CreateNewMenuByStr(char *pTitle,int tNum,char* *pMenuText,const APP_IndexH pUnifiedFunc,int TimeOutMs)
{
	CMenuUITable *pNeTable;
	char *pText;
	u16 mLen,i,slen,len;
	/*
	if(pMenuUiTable!=NULL && pMenuUiTable->pMenuFunc == pMenuFunc)
	{//----同一个第非第一次创建，直接显示-------
		pMenuUiTable->ShowState=_GUI_MENU_SHOW;
		return;
	}*/
	mLen=sizeof(CMenuUITable) + sizeof(CMenuItemStru)*tNum;
	slen=0;
	for(i=0;i<tNum;i++)
	{
		len =	API_strlen(pMenuText[i])+1;
		slen += DfGetBeiSu(len,4);	//适应任意结构
	}
	pNeTable=(CMenuUITable*)malloc(mLen+slen);
	if(pNeTable == NULL)
	{//内存申请失败，退回之前的Ui
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
		pText += DfGetBeiSu(len,4);	//适应任意结构
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
//功能:   创建新的内存菜单(运行菜单不使用外部空间，此过程需要申请较大内存空间)
//输入数据:pTitle 标题，tNum菜单总项数，pMenuAll菜单显示与功能
//输出数据:RET_OK创建成功，RET_ERR 创建失败
//创作时间:  	20151230
//---------------------------------------------------------------
int APP_CreateNewMenuByStruct(char *pTitle,int tNum,CMenuItemStru *pMenuAll,int TimeOutMs)
{
	CMenuUITable *pNeTable;
	int 		i;
	//----------------------------------------------------------
	pNeTable=(CMenuUITable*)malloc(sizeof(CMenuUITable)+sizeof(CMenuItemStru)*tNum);
	if(pNeTable == NULL)
	{	//内存申请失败，退回之前的Ui
	//	TRACE("pMenuUiTable Null!\n");
		//APP_ShowMsg(pTitle,"pMenuUiTable Null",2000);
		return RET_ERR;
	}
	API_memset(pNeTable,0x00,sizeof(CMenuUITable));
	//--------------修改临时属性------------------
	for(i=0;i<tNum;i++)
	{
		pNeTable->pItem[i].pFunMenu=pMenuAll[i].pFunMenu;
		pNeTable->pItem[i].pText=pMenuAll[i].pText;
	}	
	//-------------写入标题信息-------
	pNeTable->TimeOutMs=TimeOutMs;
	pNeTable->pTitle = pTitle;
	pNeTable->TeamTatla=tNum;
	pNeTable->TeamCurr=0;	//默认选择项
	pNeTable->ShowState=_GUI_MENU_SHOW;
	pNeTable->pPrevious =pMenuUiTable;
	pMenuUiTable=pNeTable;
	return RET_OK;
}


//====================================================================
//功能:   将链表菜单转成结构菜单(此过程需要申请内存空间，菜单的大小级数视内存而定)
//作用:   将pStrlist中的菜单内容考到结构菜单中，并释放pStrlist(由ShowFlowMenu所申请)所对应的内在空间
//输入数据:pTitle 菜单标题,pStrlist 菜单数据链(显示内容，处理方式)
//输出数据:RET_OK创建成功，RET_ERR 创建失败
//创作时间:  	20170207
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
		TatleLen += DfGetBeiSu(slen,4);	//适应任意结构
		pNewList=pNewList->pNext;
	}
	//----------------------------------------------------------
	pNeTable=(CMenuUITable*)malloc(sizeof(CMenuUITable)+sizeof(CMenuItemStru)*MenuCount +TatleLen);
	API_memset(pNeTable,0x00,sizeof(CMenuUITable));
	//--------------修改临时属性------------------
	pShowMsg=(char*)&pNeTable->pItem[MenuCount];
	for(i=0;i<MenuCount;i++)
	{
		API_memcpy(pShowMsg,pMenulist->Text,pMenulist->TextLen);
		pShowMsg[pMenulist->TextLen]='\0';

		pNeTable->pItem[i].pText = pShowMsg;
		pNeTable->pItem[i].pFunMenu=pMenulist->pFunMenu;

		slen=pNewList->TextLen+1;
		pShowMsg += DfGetBeiSu(slen,4);	//适应任意结构
		
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
//功能:  返回上一级(或上一段)菜单
//作用:   撤销CreateNewRamMenu所申请的内存空间
//输入数据:series 向上返回级数，series=0时，退出所有菜单,pEndMenu为空向上返回级数，pEndMenu不为空，返回到pEndMenu的上一级并退出显示处理
//输出数据:GUI_Menu_msg
//创作时间:  	20170317
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
//功能:  向当前菜单添加其它功能键执行功能
//作用:  用于菜单需要增加额外功能
//输入数据:type 功能类型,pFunc功能执行函数，pFunTitle 功能标题(不支持栈空间)。
//输出数据:无
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
//功能:  单项菜单显示。
//输入数据:pMenu 菜单结构，index索引项，line行号(1~n)
//输出数据:pOutShow 显示结果
//---------------------------------------------------------------
void ShowMenuItem(void *pMenu,int index,int line,char *pOutShow)
{
	CMenuItemStru *pItem=(CMenuItemStru *)pMenu;
	API_sprintf(pOutShow,"%d.%s",line,pItem[index].pText);
}
//====================================================================
//功能:  标准SDK显示处理菜单，
//作用:  显示，并处理链表pMenuUiTable里面的菜单
//输入数据:无
//输出数据:菜单执行结果
//注:改变了 pWindow 需要重新show,没改变不用
//---------------------------------------------------------------
int APP_ShowProsseMenu(XuiWindow *pWindow)
{
	CMenuUITable *pStartMenuAdd=pMenuUiTable;
	u32 event,ret=EVENT_NONE;
	while(pMenuUiTable)
	{
		//----------显示菜单---------------------
		API_GUI_CreateWindow(pWindow,pMenuUiTable->pTitle,TOK,TCANCEL,1);
		API_GUI_Menu(pWindow,pMenuUiTable->pItem,&ShowMenuItem,pMenuUiTable->TeamTatla,pMenuUiTable->TeamCurr,pMenuUiTable->ShowHead,pMenuUiTable->pAfterText,pMenuUiTable->pKeyFunc);
		API_GUI_Show(pWindow);
	//	pMenuUiTable->ShowState=_GUI_MENU_PROCESS;
		//----------处理菜单----------------------
		event=API_WaitEvent(pMenuUiTable->TimeOutMs,EVENT_UI,EVENT_NONE);	
		if(event==EVENT_INDEX)
		{
			int index,head;
			index=API_GUI_Menu_GetInxAndHear(&head);
			if(index<pMenuUiTable->TeamTatla)
			{
				pMenuUiTable->TeamCurr=index;
				pMenuUiTable->ShowHead=head;
	//			pMenuUiTable->ShowState=_GUI_MENU_SHOW; //执行完显示菜单
				if(pMenuUiTable->pUnifiedFunc)
				{//-----统一菜单处理----------
					ret=(*pMenuUiTable->pUnifiedFunc)(pWindow,pMenuUiTable->pItem[index].pText,index);
					if((ret&EVENT_MASK) == EVENT_QUIT)
					{
						if(_GUI_MENU_EXT == ReturnToPreviousMenu(pWindow,ret&EVENT_INDEX,pStartMenuAdd))
							break;
					}
					continue;
				}
				if(pMenuUiTable->pItem[index].pFunMenu)
				{//-----独立菜单处理------
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
	//		pMenuUiTable->ShowState=_GUI_MENU_SHOW; //执行完显示菜单
			if(pMenuUiTable->pKeyFuncTitle)
				ret=pMenuUiTable->pKeyFunc(pWindow,pMenuUiTable->pKeyFuncTitle);
			else
				ret=pMenuUiTable->pKeyFunc(pWindow,pMenuUiTable->pTitle);
			if(EVENT_QUIT&ret)
			{//-----退出所有菜单----------
				ReturnToPreviousMenu(pWindow,ret&EVENT_INDEX,pStartMenuAdd);
				break;
			}
		}
	}
	return ret;
}
//====================================================================
//功能:  显示处理菜单，
//作用:  显示，并处理链表pMenuUiTable里面的菜单
//输入数据:无
//输出数据:无
//创作时间:  	20140726
//---------------------------------------------------------------
/*
int APP_ShowMenuProsse(void)
{
	CMenuUITable *pStartMenuAdd=pMenuUiTable;
	u16 MessageID,Message;
	while(pMenuUiTable)
	{
		if(pMenuUiTable->ShowState==_GUI_MENU_SHOW)
		{//----------显示菜单--------
			APP_GUI_Menu(pMenuUiTable->pShowTatle,(int)pMenuUiTable->ShowHead,(int)pMenuUiTable->TeamTatla,(int)pMenuUiTable->TeamCurr,pMenuUiTable->pMenuText);
			pMenuUiTable->ShowState=_GUI_MENU_PROCESS;
		}
		Set_WaitEvent(pMenuUiTable->TimeOutMs,EVENT_KEY);
		//---------------阻塞式------------------------------------------
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
						{//-----退出所有菜单----------
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


