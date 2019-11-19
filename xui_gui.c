//============================================================
//---显示功能---
#include <unistd.h>
//#include <stdarg.h>
#include <sys/time.h>

#include "types_def.h"

#include "EvenMsg.h"
#include "xui_ui.h"
#include "xui_fb.h"
#include "xui_font.h"
#include "xui_gui.h"

#include "key_hard.h"


//#include "QR_Encode.h"


//------------------显示金额 转 交易金额-------------------------------
void Conv_DmoneyToTmoney(char* pOutStr,char* pInsMoney)
{
	u8 i,j,k;
	u8 Inlen=API_strlen(pInsMoney);
	for(i=0;i<Inlen;i++)
		if(pInsMoney[i]=='.') break;
	//----整数部分------
	for(j=0;j<i;j++)
		pOutStr[j]=pInsMoney[j];
	//---小数部分精确两位----
	if(i<Inlen)	//有小数点
	{
		for(i=0; i<2; i++)
		{
			if((j+1) < Inlen)
				pOutStr[j]=pInsMoney[j+1];
			else
				pOutStr[j]='0';
			j++;
		}
	}
	else	//无小数点,两位0
	{
		pOutStr[j++]='0';
		pOutStr[j++]='0';
	}
	//---过滤无效位------
	for(i=0; i<j; i++)
		if(pOutStr[i]!='0')
			break;
	if(i)
	{
		k=j-i;
		for(j=0;j<k;j++)
			pOutStr[j]=pOutStr[j+i];
	}
	pOutStr[j]='\0';
}

//------------------交易金额 转 显示金额-------------------------------
void Conv_TmoneyToDmoney(char* pOutdMoney,char* pIntMoney)
{
	u16 i=0,Inlen=API_strlen(pIntMoney);
	if(Inlen == 0)
	{
		pOutdMoney[0]='\0';
		return;
	}
	if(Inlen <= 2)
	{
		pOutdMoney[i++]='0';
		pOutdMoney[i++]='.';
		if(Inlen < 2)
			pOutdMoney[i++]='0';
		else
			pOutdMoney[i++]=*pIntMoney++;
		pOutdMoney[i++]=*pIntMoney++;
	}
	else
	{
		Inlen -= 2;
		while(i<Inlen)
		{
			pOutdMoney[i++]=*pIntMoney++;
		}
		pOutdMoney[i++]='.';
		pOutdMoney[i++]=*pIntMoney++;
		pOutdMoney[i++]=*pIntMoney++;
	}

	while(i--)	//去掉无效参数
	{
		if(pOutdMoney[i] == '.')
		{
			i--;
			break;
		}
		else if(pOutdMoney[i] != '0') 
			break;
	}
	pOutdMoney[++i]='\0';
}

//=========================按键定义==========================
//static const char* KeyMsgABC[10]={"*#:/?%@-+=","&$!,;'`^~\"_|","ABC","DEF","GHI","JKL","MNO","PQRS","TUV","WXYZ"};


//=============================================================================
void DisplayLineRGB(XuiWindow *pWindow,POINT *pxy,int w,int h,u32 rtg)
{
	u16 i,j,sx,sy;
	u32 *pbgra;
	sx = pxy->left;
	sy = pxy->top;
	w += sx;
	h += sy;
	if(w > pWindow->width) w=pWindow->width;
	if(h > pWindow->height) h=pWindow->height;
	for (j = sy; j < h; j++) 
	{
		pbgra=&pWindow->widget[j*pWindow->wLen + sx];
		for(i=sx; i<w; i++)
		{
			*pbgra++ = rtg;
		}
	}
}


int DisplayPaintEnd(XuiWindow *pWindow)
{
	return xui_fb_rect_push(pWindow->left,pWindow->top,pWindow->width,pWindow->height,(rgba_t*)pWindow->widget);
}



//=====================区域图片备份======malloc new space================================
/*

void* UI_ExtractMapBuff(int x,int y,int w,int h)
{
	DisplayBackup 	*pLcdBit;
	pLcdBit=(DisplayBackup *)malloc(sizeof(DisplayBackup) + w*h*4);
	if(pLcdBit==NULL) return NULL;
	pLcdBit->Rect.left		= x;
	pLcdBit->Rect.top		= y;
	pLcdBit->Rect.width		= w;
	pLcdBit->Rect.height	= h;
//	DisplayBitMapGet(&pLcdBit->Rect,(LPBYTE)pLcdBit->LcdBuff, w*h);
	return pLcdBit;
}

int UI_ReplaceMapBuff(void* pMap)
{
	//DisplayBackup* pLcdBit
	if(pMap==NULL) return -1;
//	return DisplayBitMap(&pLcdBit->Rect,(LPBYTE)pLcdBit->LcdBuff);
	return 0;
}

void UI_FreeMapBuff(void* pMap)
{
	if(pMap) free(pMap);
}
//=====================================================================
static DisplayBackup* pSaveBackup=NULL;
void UI_DisplayBitMapGet(u16 x,u16 y,u16 w,u16 h)
{
	if(pSaveBackup)
		free(pSaveBackup);
	pSaveBackup=UI_DisplayGetMap(x,y,w,h);
}

void UI_DisplayBitMapSet(void)
{
	if(pSaveBackup==NULL)
		return;
	UI_DisplaySetMap(pSaveBackup);
}

void UI_DisplayBitMapEND(void)
{
	if(pSaveBackup)
	{
		UI_DisplayBitMapFree(pSaveBackup);
		pSaveBackup=NULL;
	}
}
*/



//================================画框=======================================================
void UI_ShowColorRect(XuiWindow *pWindow,RECTL *pRect,u16 Width,u32 Color)
{
	POINT fTrg;
	fTrg.left = pRect->left; fTrg.top = pRect->top;
	DisplayLineRGB(pWindow,&fTrg,pRect->width,Width,Color);
	fTrg.top = pRect->top+pRect->height-Width;
	DisplayLineRGB(pWindow,&fTrg,pRect->width,Width,Color);
	fTrg.top = pRect->top+Width;
	DisplayLineRGB(pWindow,&fTrg,Width,pRect->height-2*Width,Color);
	fTrg.left = pRect->left+pRect->width-Width;
	DisplayLineRGB(pWindow,&fTrg,Width,pRect->height-2*Width,Color);
}






//===============清除显示内容(指定区域)=================================================
void API_GUI_ClearScreen(XuiWindow *pWindow,RECTL* pRect)
{
	POINT tFontXY;
	tFontXY.left=pRect->left;
	tFontXY.top =pRect->top;
	if(tFontXY.top < (UI_TITLE_Y+UI_TITLE_H))	//标题区域
		DisplayLineRGB(pWindow,&tFontXY,pRect->width,pRect->height,UI_TITLE_COLOUR);
	else										//内容区
		DisplayLineRGB(pWindow,&tFontXY,pRect->width,pRect->height,UI_CONT_COLOUR);
}

//================================================================
int  API_GUI_CreateWindow(XuiWindow *pWindow,const char* pTitle,const char* pOk,const char* pCancel,u32 tGuiType)
{
	POINT	tFontXY;
	int 	sWidth;
	//-------------------显示标题--------------------------------
	tFontXY.left=UI_TITLE_X;
	tFontXY.top =UI_TITLE_Y;
	DisplayLineRGB(pWindow,&tFontXY,UI_TITLE_W,UI_TITLE_H,UI_TITLE_COLOUR);//0x03ff
	if(pTitle!=NULL)
	{
		sWidth=API_strlen(pTitle)*FONT_SIZE/2;
		if(sWidth>UI_TITLE_W)
			tFontXY.left=UI_TITLE_X;
		else
			tFontXY.left=UI_TITLE_X+(UI_TITLE_W-sWidth)/2;
		tFontXY.top =UI_TITLE_Y+(UI_TITLE_H-FONT_SIZE)/2;
		ApiFont.SetFontColor(FONT_TITLE_COLOUR,UI_TITLE_COLOUR);
		ApiFont.DrawLineString(pWindow,&tFontXY,pTitle);
	}
	//-------------------显示菜单项--------------------------------
	if(tGuiType==0)
	{
		tFontXY.left=UI_CONT_X;
		tFontXY.top =UI_CONT_Y;
		DisplayLineRGB(pWindow,&tFontXY,UI_CONT_W,UI_CONT_H,UI_CONT_COLOUR);
	}
	return 0;
}


int  API_GUI_Info(XuiWindow *pWindow,const IMAGE* pImg,u32 tTextType,const char* pTextBuf)
{
	RECTL tCoordinate={0};
	u16 sLen,offset,i,line,fontN;
	
	if(pImg)
	{
		tCoordinate.width=pImg->w;
		tCoordinate.height=pImg->h;
	}
	
	if(pTextBuf)
	{
		if(tCoordinate.width)
			fontN=tCoordinate.width*2/FONT_SIZE;
		else
			fontN=(UI_CONT_W*2/FONT_SIZE);
		sLen=API_strlen(pTextBuf);
		for(offset=0,line=0;line<10;line++)
		{
			if(offset >= sLen) break;
			for(i=0;i<fontN;i++)
			{
				if(pTextBuf[offset+i]=='\n')
				{
					i++;
					break;
				}
				if(pTextBuf[offset+i]&0x80)
				{
					if((i+1) >= fontN) break;
					i++;
				}
			}
			offset += i;
		}
		if(sLen > fontN)
			tCoordinate.width=UI_CONT_W;
		else
			tCoordinate.width=sLen*FONT_SIZE/2;
		tCoordinate.height=line*FONT_SIZE;
	}
	
	switch(tTextType&TEXT_ALIGN_MASK)
	{
		case TEXT_ALIGN_RIGHT: 	//右对齐tIdle.rThis
			tCoordinate.left=UI_CONT_X+UI_CONT_W-tCoordinate.width;
			break;
		case TEXT_ALIGN_CENTER: 	//左右居中
			tCoordinate.left=UI_CONT_X+(UI_CONT_W-tCoordinate.width)/2;
			break;
		default ://case 0: 	//左对齐
			tCoordinate.left=UI_CONT_X;
			break;
	}
	switch(tTextType&TEXT_VALIGN_MASK)
	{
		case TEXT_VALIGN_BOTTOM: 	//下对齐
			tCoordinate.top=UI_CONT_Y+UI_CONT_H-tCoordinate.height;
			break;
		case TEXT_VALIGN_CENTER: 	//上下居中
			tCoordinate.top=UI_CONT_Y+(UI_CONT_H-tCoordinate.height)/2;
			break;
		case 0: 	//指定行号(0~7)
			tCoordinate.top=UI_CONT_Y + ((tTextType&TEXT_LINE_NUM_MASK)/0x10000)*(UI_CONT_H/8) + ((UI_CONT_H/8)-FONT_SIZE)/2;
			break;
		default: 		//上对齐
			tCoordinate.top=UI_CONT_Y;
			break;
	}
	if(tTextType&TEXT_EXSTYLE_BORDER)//加框
	{
		RECTL rect;
		if(tCoordinate.left>0&& tCoordinate.top>0)
		{
			rect.left=tCoordinate.left-1;
			rect.top=tCoordinate.top-1;
			rect.width=tCoordinate.width+2;
			rect.height=tCoordinate.height+2;
			UI_ShowColorRect(pWindow,&rect,1,RGB_CURR(30,30,30));
		}
	}
	else
	{
		POINT rclTrg;
		rclTrg.left=tCoordinate.left;
		if(tTextType&TEXT_EXSTYLE_UNDERLINE) //加下划线
		{
			rclTrg.top =tCoordinate.top+tCoordinate.height+1;
			DisplayLineRGB(pWindow,&rclTrg,tCoordinate.width,1,RGB_CURR(30,30,30));
		}
		if(tTextType&TEXT_EXSTYLE_OVERLINE)//加上划线
		{
			rclTrg.top =tCoordinate.top-1;
			DisplayLineRGB(pWindow,&rclTrg,tCoordinate.width,1,RGB_CURR(30,30,30));
		}
	}
	
	if(pImg)
	{
	//	DisplayBitMap(&tCoordinate,(LPBYTE)pImg->idata);
	}
	if(pTextBuf)
	{
		ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
		ApiFont.DrawRectString(pWindow,&tCoordinate,pTextBuf);
	}
	return 0;
}

void API_GUI_Edit_Prompt(XuiWindow *pWindow,u32 tFrontTextType,char* pFrontTextBuf,u32 tAfterTextType,char* pAfterTextBuf)
{
	if(pFrontTextBuf)
		API_GUI_Info(pWindow,NULL,tFrontTextType|TEXT_ALIGN_LEFT|TEXT_VALIGN_TOP|TEXT_EXSTYLE_UNDERLINE,pFrontTextBuf);
	if(pAfterTextBuf)
		API_GUI_Info(pWindow,NULL,tFrontTextType|TEXT_ALIGN_RIGHT|TEXT_VALIGN_BOTTOM|TEXT_EXSTYLE_OVERLINE,pAfterTextBuf);
}

/*
typedef struct {
	u32		Limit,Way;		//控制不要输入法切换,IME_TYPE(多选),输入法,IME_TYPE(其中之一)
	Fun_ShowNum ShowNum;	//显示内容 (u32 flagPs,char* pStrNum)	bit31=1,init,bit0~bit7 showCode
	u32		oldTimeMs;
	RECTL	Rect;
	u8		Min,Max;	//最小最大输入范围
	u8		KeyTimes,oldKey;
	u8		MaxS,MaskCode;
	u8		indexEdit,indexShow;	//当前输入长度与显示长度
	char	sEditBuff[64];

	char	*pShowInfo;
	char	*pAfterText;				//最后一行提示信息
	//int		InfoMax,InfoIndex,InfoOffset;
}UI_EDIT_MSG;

UI_EDIT_MSG tGuiEditMsg;


u32 API_UI_EditShow(u16 keyNum)
{
	u32		newTimeMs;
	u8		WayChange,i,KeyValABC;

	if(keyNum)
	{
		if(keyNum==K_IME)
		{//----切换输入法-----
			WayChange=0;
			for(i=1;i<8;i++)
			{
				if((tGuiEditMsg.Way<<i) & (tGuiEditMsg.Limit&0xff))
				{
					tGuiEditMsg.Way <<= i;
					WayChange=1;
					break;
				}
			}
			if(i == 8)
			{
				while(--i)
				{
					if((tGuiEditMsg.Way>>i) & (tGuiEditMsg.Limit&0xff))
					{
						tGuiEditMsg.Way >>= i;
						WayChange=1;
						break;
					}
				}
			}
			if(WayChange)
			{
				if(tGuiEditMsg.Way==IME_NUM)
				{
					API_GUI_Info(NULL,TEXT_ALIGN_RIGHT|TEXT_VALIGN_BOTTOM,STR_KEY_INPUT_NUMBER);
				}
				else if(tGuiEditMsg.Way==IME_abc)
				{
					API_GUI_Info(NULL,TEXT_ALIGN_RIGHT|TEXT_VALIGN_BOTTOM,KEY_INPUT_LOWERCASE_LETTERS);
				}
				else if(tGuiEditMsg.Way==IME_ABC)
				{
					API_GUI_Info(NULL,TEXT_ALIGN_RIGHT|TEXT_VALIGN_BOTTOM,KEY_INPUT_UPPERCASE_LETTERS);
				}
				DisplayPaintEnd();
			}
		}
		else if(keyNum==K_DEL)
		{
			if(tGuiEditMsg.Way == IME_SUM)
				tGuiEditMsg.indexEdit=0;
			else if(tGuiEditMsg.indexEdit>0)
				tGuiEditMsg.indexEdit--;
		}
		else if((keyNum>=K_0 && keyNum<=K_9)||keyNum == K_D)
		{
			if(keyNum!=K_D && (tGuiEditMsg.Way&(IME_ABC|IME_abc)))
			{//---0~9 的数字转化------
				if(tGuiEditMsg.oldKey != keyNum)
				{
					tGuiEditMsg.oldKey=keyNum;
					newTimeMs=0;
					tGuiEditMsg.KeyTimes=0;
				}
				else 
				{
					GetTickCount(&newTimeMs);
					newTimeMs -= tGuiEditMsg.oldTimeMs;
					//TRACE("IME_ABC key[%d]oldTimeMs[%d]time[%d]\r\n",Message,oldTimeMs,newTimeMs);
					if(newTimeMs>700)
					{
						newTimeMs=0;
						tGuiEditMsg.KeyTimes=0;
					}
					else	//连续快按
					{
						newTimeMs=1;
						if(++tGuiEditMsg.KeyTimes >= API_strlen(KeyMsgABC[keyNum-K_0]))
							tGuiEditMsg.KeyTimes=0;
					}
				}
				GetTickCount(&tGuiEditMsg.oldTimeMs);
				KeyValABC=KeyMsgABC[keyNum-K_0][tGuiEditMsg.KeyTimes];
				if(tGuiEditMsg.Way == IME_abc)
				{//-------大写转小写----------
					if(KeyValABC>='A' && KeyValABC<='Z')
						KeyValABC |= 0x20;
				}
				if(newTimeMs)
				{//------短按改变原来值--------
					keyNum=KeyValABC;
					tGuiEditMsg.indexEdit--;
					tGuiEditMsg.indexShow--;//驱动显示
				}
				else
				{
					keyNum=KeyValABC;
				}
			}
			//---------------------------------------
			if(tGuiEditMsg.indexEdit < tGuiEditMsg.Max)
			{
				tGuiEditMsg.sEditBuff[tGuiEditMsg.indexEdit++]=keyNum;
				if(tGuiEditMsg.Way == IME_SUM)
				{
					tGuiEditMsg.sEditBuff[tGuiEditMsg.indexEdit]='\0';
					if(Conv_MoneyToCentInt(tGuiEditMsg.sEditBuff) < 0)
					{
						tGuiEditMsg.sEditBuff[--tGuiEditMsg.indexEdit]='\0';
						return EVENT_NONE;
					}
				}
				else if(tGuiEditMsg.Way == (IME_SUM|IME_NUM))
				{
					if(tGuiEditMsg.indexEdit==1 && tGuiEditMsg.sEditBuff[0]=='0')
					{
						tGuiEditMsg.sEditBuff[--tGuiEditMsg.indexEdit]='\0';
						return EVENT_NONE;
					}
					if(tGuiEditMsg.sEditBuff[tGuiEditMsg.indexEdit-1]=='.')
					{
						tGuiEditMsg.sEditBuff[--tGuiEditMsg.indexEdit]='\0';
						return EVENT_NONE;
					}
				}
			}
			else
			{
				APP_HitMsg(STR_EDIT_FULL,1500);
			}
		}
	}
	//---------ShowSN--------------
	if(tGuiEditMsg.indexShow != tGuiEditMsg.indexEdit)
	{
		POINT	rclTrg;
		BYTE	sbuff[4]={0};
		//TRACE("API_UI_EditShow indexShow[%d],indexEdit[%d],keyNum[%d]\r\n",tGuiEditMsg.indexShow,tGuiEditMsg.indexEdit,keyNum);
		//---清除之前的内容------
		sbuff[0]=' ';//sbuff[0]='\0';
		while(tGuiEditMsg.indexShow > tGuiEditMsg.indexEdit)
		{
			tGuiEditMsg.indexShow--;
			if(tGuiEditMsg.indexShow >= tGuiEditMsg.MaxS)
			{
				rclTrg.top = tGuiEditMsg.Rect.top+FONT_SIZE;
				rclTrg.left = tGuiEditMsg.Rect.left+(tGuiEditMsg.indexShow-tGuiEditMsg.MaxS)*FONT_SIZE/2;
			}
			else
			{
				rclTrg.top = tGuiEditMsg.Rect.top;
				rclTrg.left = tGuiEditMsg.Rect.left+(tGuiEditMsg.indexShow)*FONT_SIZE/2;
			}
			ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
			//TRACE("API_UI_EditShow top[%d],left[%d],sbuff[%S]\r\n",rclTrg.top,rclTrg.left,sbuff);
			ApiFont.DisplayFont(&rclTrg,sbuff);
		}
		while(tGuiEditMsg.indexShow < tGuiEditMsg.indexEdit)
		{
			if(tGuiEditMsg.indexShow >= tGuiEditMsg.MaxS)
			{
				rclTrg.top = tGuiEditMsg.Rect.top+FONT_SIZE;
				rclTrg.left = tGuiEditMsg.Rect.left+(tGuiEditMsg.indexShow-tGuiEditMsg.MaxS)*FONT_SIZE/2;
			}
			else
			{
				rclTrg.top = tGuiEditMsg.Rect.top;
				rclTrg.left = tGuiEditMsg.Rect.left+(tGuiEditMsg.indexShow)*FONT_SIZE/2;
			}
			ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
			if(tGuiEditMsg.MaskCode)
				sbuff[0]=tGuiEditMsg.MaskCode;
			else
				sbuff[0]=tGuiEditMsg.sEditBuff[tGuiEditMsg.indexShow];
			//TRACE("API_UI_EditShow top[%d],left[%d],sbuff[%S]\r\n",rclTrg.top,rclTrg.left,sbuff);
			ApiFont.DisplayFont(&rclTrg,sbuff);
			tGuiEditMsg.indexShow++;
		}
		if(keyNum) DisplayPaintEnd();
		tGuiEditMsg.sEditBuff[tGuiEditMsg.indexShow]='\0';	//增加结束符号
	}
	return EVENT_NONE;
}



void API_GUI_Edit(u32 tTextType,char* pTextDef,int tMaxLen,u32 tImeType,u32 tImeLimit)
{
	tGuiEditMsg.Rect.width	= UI_CONT_W-FONT_SIZE*2+8;
	tGuiEditMsg.Rect.height = FONT_SIZE*2+8;
	tGuiEditMsg.Rect.left	= UI_CONT_X+FONT_SIZE-4;
	tGuiEditMsg.Rect.top	= UI_CONT_Y+(UI_CONT_H-tGuiEditMsg.Rect.height)/2;
	UI_ShowColorRect(&tGuiEditMsg.Rect,1,RGB_CURR(192,192,192));
	tGuiEditMsg.Rect.height = FONT_SIZE*2;
	tGuiEditMsg.Rect.width	= UI_CONT_W-FONT_SIZE*2;
	tGuiEditMsg.Rect.left	= UI_CONT_X+FONT_SIZE;
	tGuiEditMsg.Rect.top	= UI_CONT_Y+(UI_CONT_H-tGuiEditMsg.Rect.height)/2;;
	tGuiEditMsg.MaxS=(tGuiEditMsg.Rect.width*2/FONT_SIZE);
	if(tMaxLen > 32) tMaxLen = 32;
	tGuiEditMsg.Max=tMaxLen;
	tGuiEditMsg.Way=tImeType&IME_MASK;
	tGuiEditMsg.Limit=tImeLimit&IME_MASK;
	if(tGuiEditMsg.Way == IME_SUM)
		Conv_TmoneyToDmoney(tGuiEditMsg.sEditBuff,pTextDef);
	else 
		API_strcpy(tGuiEditMsg.sEditBuff,pTextDef);
		
	tGuiEditMsg.indexEdit= API_strlen(tGuiEditMsg.sEditBuff);//初始长度
	tGuiEditMsg.indexShow=0;
	//---------------------------------------------------
	if(tImeType&IME_PIN_MODE)
		tGuiEditMsg.MaskCode='*';
	else
		tGuiEditMsg.MaskCode=0x00;
	tGuiEditMsg.KeyTimes=0;
	//---------------------------------------------------
	tWaitEventMsg.pFunEvenKey=&API_UI_EditShow;
	API_UI_EditShow(0);
}

u32 API_UI_OprInfo(u16 keyNum)
{
	if(keyNum)
	{
		if(keyNum==K_UP || keyNum==K_8)
		{
			if(tGuiEditMsg.Way)
			{
				tGuiEditMsg.Way=0;
				API_GUI_ClearScreen(&tGuiEditMsg.Rect);
				ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
				tGuiEditMsg.oldTimeMs=ApiFont.DrawRectString(&tGuiEditMsg.Rect,tGuiEditMsg.pShowInfo + tGuiEditMsg.Way);
			}
		}
		else if(keyNum==K_DOWN || keyNum==K_0)
		{
			if((tGuiEditMsg.Way+tGuiEditMsg.oldTimeMs) < tGuiEditMsg.Limit)
			{
				tGuiEditMsg.Way += tGuiEditMsg.oldTimeMs;
				API_GUI_ClearScreen(&tGuiEditMsg.Rect);
				ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
				tGuiEditMsg.oldTimeMs=ApiFont.DrawRectString(&tGuiEditMsg.Rect,tGuiEditMsg.pShowInfo + tGuiEditMsg.Way);
			}
		}
	}
	else
	{
		tGuiEditMsg.Way = 0;
		tGuiEditMsg.Limit = API_strlen(tGuiEditMsg.pShowInfo); //初始长度
		ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
		tGuiEditMsg.oldTimeMs=ApiFont.DrawRectString(&tGuiEditMsg.Rect,tGuiEditMsg.pShowInfo);
		if((tGuiEditMsg.pAfterText!=NULL)&&(tGuiEditMsg.oldTimeMs<tGuiEditMsg.Limit))
		{
			POINT rclTrg;
			u16 width;
			width=API_strlen(tGuiEditMsg.pAfterText)*FONT_SIZE/2;
			if(width<tGuiEditMsg.Rect.width)
				rclTrg.left =tGuiEditMsg.Rect.width-width;
			else 
				rclTrg.left =tGuiEditMsg.Rect.left;
			rclTrg.top =tGuiEditMsg.Rect.top+tGuiEditMsg.Rect.height;
			DisplayLineRGB(&rclTrg,width,1,RGB_CURR(30,30,30));
			ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
			UI_DrawString(&rclTrg,tGuiEditMsg.pAfterText);
		}
	}
	DisplayPaintEnd();
	return EVENT_NONE;
}

void API_GUI_OprInfo(char* pShowInfo,char *pAfterText)
{
	tGuiEditMsg.Rect.left	= UI_CONT_X;
	tGuiEditMsg.Rect.top	= UI_CONT_Y;
	tGuiEditMsg.Rect.width	= UI_CONT_W;
	if(pAfterText)
		tGuiEditMsg.Rect.height=((UI_CONT_H-FONT_SIZE)/FONT_SIZE)*FONT_SIZE;
	else
		tGuiEditMsg.Rect.height=UI_CONT_H;
	
	tGuiEditMsg.pShowInfo	= pShowInfo;
	tGuiEditMsg.pAfterText	= pAfterText;
	//---------------------------------------------------
	tWaitEventMsg.pFunEvenKey=&API_UI_OprInfo;
	API_UI_OprInfo(0);
}

int API_GUI_Edit_GetText(char* pAscText,int tMaxLen)
{
	tMaxLen--;
	if(tMaxLen < tGuiEditMsg.indexShow)
		return -1;
	if(tGuiEditMsg.Way==IME_SUM)
	{
		Conv_DmoneyToTmoney(pAscText,tGuiEditMsg.sEditBuff);
	}
	else
	{
		API_strcpy(pAscText,tGuiEditMsg.sEditBuff);
	}
	return API_strlen(pAscText);
}
*/
//=========================================================================================
typedef struct {
	//u32 	tMenuType;
	XuiWindow *pWindow;
	void 	(*ShowItem)(void *,int,int,char*);
	void*	pMenu;	//菜单内容
	char*	pAfterText;	//最后一行显示
	u8		tCurInx,showInx;
	u8		tCurHead,showHead;
	u8		tNum;
	u8		ShowState,KeyFunEn;
}UI_MENU_MSG;

UI_MENU_MSG tGuiMenuMsg={0};

u32 APP_UI_MenuShow(u16 keyNum)
{
	u16 i,MaxLine;
	MaxLine=MENU_TIEM_MAX;
	if(tGuiMenuMsg.pAfterText) MaxLine--;
		
	if(keyNum)
	{
		if(keyNum>=K_1 && keyNum<(K_1+MaxLine))
		{
			keyNum -= K_1;
			if((tGuiMenuMsg.tCurHead+keyNum) >= tGuiMenuMsg.tNum)
				return EVENT_NONE;
			tGuiMenuMsg.tCurInx=(tGuiMenuMsg.tCurHead+keyNum);
			return EVENT_INDEX;
		}
		else if(keyNum==K_UP || keyNum==(K_1+MaxLine))
		{
			if(tGuiMenuMsg.tCurHead >= MaxLine)
				tGuiMenuMsg.tCurHead -= MaxLine;
			else if(tGuiMenuMsg.tCurHead > 0)
				tGuiMenuMsg.tCurHead=0;
			else
				return EVENT_NONE;
		}
		else if(keyNum==K_DOWN || keyNum==K_0)
		{
			if((tGuiMenuMsg.tCurHead+MaxLine) < tGuiMenuMsg.tNum)
				tGuiMenuMsg.tCurHead += MaxLine;
			else
				return EVENT_NONE;
		}
		else if(keyNum == K_FUNC)
		{
			if(tGuiMenuMsg.KeyFunEn)
				return (EVENT_KEY|K_FUNC);
		}
	}
	//----------------------------------------------------------
	//if(tGuiMenuMsg.ShowState)
	{//----------显示菜单--------
		POINT tFontXY;
		u32 Color=0;
		tFontXY.top =SCREEN_MENU_Y+MENU_TITLE_H;
		for(i=0;i<MENU_TIEM_MAX;i++)
		{
			tFontXY.left=SCREEN_MENU_X;
			if(i&0x01)
				Color=RGB_CURR(232,232,232);
			else
				Color=RGB_CURR(240,240,240);
			DisplayLineRGB(tGuiMenuMsg.pWindow,&tFontXY,SCREEN_MENU_W,MENU_TIEM_H,Color);

			tFontXY.top += (MENU_TIEM_H-FONT_SIZE)/2;
			if(i >= MaxLine) break;
			//-------------------显示菜单项--------------------------------
			if((tGuiMenuMsg.tCurHead+i) < tGuiMenuMsg.tNum)
			{
				char sBuff[(SCREEN_APP_W*2/FONT_SIZE)+8];
				tFontXY.left=SCREEN_MENU_X+4;
				tGuiMenuMsg.ShowItem(tGuiMenuMsg.pMenu,tGuiMenuMsg.tCurHead+i,i+1,sBuff);
				ApiFont.SetFontColor(RGB565_TIEM_FONT,Color);
				ApiFont.DrawLineString(tGuiMenuMsg.pWindow,&tFontXY,sBuff);
			}
			tFontXY.top += MENU_TIEM_H-(MENU_TIEM_H-FONT_SIZE)/2;
		}
		if(tGuiMenuMsg.pAfterText)
		{
			int tLenW=API_strlen(tGuiMenuMsg.pAfterText)*FONT_SIZE/2;
			if(tLenW < SCREEN_MENU_W)
				tFontXY.left=SCREEN_MENU_X+SCREEN_MENU_W-tLenW;
			else
			{
				tFontXY.left=SCREEN_MENU_X;
				tLenW = SCREEN_MENU_W;
			}
			//tFontXY.top=MENU_TIEM_ALL_Y+(MENU_TIEM_H-FONT_SIZE)/2+MENU_TIEM_H*i-1;
			DisplayLineRGB(tGuiMenuMsg.pWindow,&tFontXY,tLenW,1,RGB_CURR(30,30,30));
			tFontXY.top++;tFontXY.top++;
			ApiFont.SetFontColor(RGB565_TIEM_FONT,Color);
			ApiFont.DrawLineString(tGuiMenuMsg.pWindow,&tFontXY,tGuiMenuMsg.pAfterText);
		}
		DisplayPaintEnd(tGuiMenuMsg.pWindow);
	}
	return EVENT_NONE;
}

int API_GUI_Menu_GetInx(void)
{
	return tGuiMenuMsg.tCurInx;
}


int API_GUI_Menu_GetInxAndHear(int* tHead)
{
	if(tHead)
		*tHead=tGuiMenuMsg.tCurHead;
	return tGuiMenuMsg.tCurInx;
}

int API_GUI_Menu(XuiWindow *pWindow,void* pMenu,void (*pShowItem)(void *,int,int,char*),int tNum,int tCurInx,int tHead,char* pAfterText,void* KeyFunEn)
{
	tGuiMenuMsg.tNum=tNum;
	tGuiMenuMsg.tCurInx=tCurInx;
	tGuiMenuMsg.showInx=0xff;
	tGuiMenuMsg.tCurHead=tHead;
	tGuiMenuMsg.showHead=0xff;
	tGuiMenuMsg.pAfterText=pAfterText;
	if(KeyFunEn)
		tGuiMenuMsg.KeyFunEn=TRUE;
	else 
		tGuiMenuMsg.KeyFunEn=FALSE;
	tGuiMenuMsg.pMenu=pMenu;
	tGuiMenuMsg.ShowItem=pShowItem;
	tGuiMenuMsg.pWindow=pWindow;
	//-----------------------------------------------------
	APP_UI_MenuShow(0);
	tWaitEventMsg.pFunEvenKey=&APP_UI_MenuShow;
	return 0;
}


int APP_GUI_Menu(XuiWindow *pWindow,char* pTitle,int stratIndex,int tNum,int tCurInx,char** pMenuText)
{
	POINT tFontXY;
	u16 i,sWidth;
	char sBuff[(SCREEN_APP_W*2/FONT_SIZE)+8];
	tFontXY.left=SCREEN_MENU_X;
	//-------------------显示标题--------------------------------
	tFontXY.top =SCREEN_MENU_Y;
	DisplayLineRGB(pWindow,&tFontXY,SCREEN_MENU_W,MENU_TITLE_H,RGB565_TITLE_ICON);//0x5e1e
	if(pTitle!=NULL)
	{
		sWidth=API_strlen(pTitle)*FONT_SIZE/2;
		if(sWidth>SCREEN_MENU_W)
			tFontXY.left=SCREEN_MENU_X;
		else
			tFontXY.left=SCREEN_MENU_X+(SCREEN_MENU_W-sWidth)/2;
		tFontXY.top =SCREEN_MENU_Y+(MENU_TITLE_H-FONT_SIZE)/2;
		ApiFont.SetFontColor(RGB565_WITHE,RGB565_PARENT);
		ApiFont.DrawLineString(pWindow,&tFontXY,pTitle);
	}
	//-------------------显示菜单项--------------------------------
	tFontXY.left=SCREEN_MENU_X;
	tFontXY.top =SCREEN_MENU_Y+MENU_TITLE_H;
	DisplayLineRGB(pWindow,&tFontXY,SCREEN_MENU_W,SCREEN_MENU_H-MENU_TITLE_H,RGB565_TIEM_ICON);
	for(i=0; i<MENU_TIEM_MAX; i++)
	{
		if((stratIndex+i) >= tNum)
			break;
		API_sprintf(sBuff,"%d.%s",1+stratIndex+i,pMenuText[stratIndex+i]);
		tFontXY.top=SCREEN_MENU_Y+MENU_TITLE_H+MENU_TIEM_H*i;
		if((stratIndex+i)==tCurInx)
		{
			tFontXY.left=SCREEN_MENU_X;
			DisplayLineRGB(pWindow,&tFontXY,SCREEN_MENU_W,MENU_TIEM_H,RGB565_SELE_ICON);
			ApiFont.SetFontColor(RGB565_SELE_FONT,RGB565_PARENT);
		}
		else
			ApiFont.SetFontColor(RGB565_TIEM_FONT,RGB565_PARENT);
		tFontXY.left=SCREEN_MENU_X+4;
		tFontXY.top +=(MENU_TIEM_H-FONT_SIZE)/2;
		ApiFont.DrawLineString(pWindow,&tFontXY,sBuff);
	}
	DisplayPaintEnd(pWindow);
	return 0;
}

/*
//========================输入==============================================
u32 API_UI_InputEdit(u16 keyNum)
{
	if(keyNum)
	{
		if(keyNum==K_IME)
		{
			if(tGuiEditMsg.Way < tGuiEditMsg.Limit)
			{//----切换输入法-----
				u32 i,WayChange=tGuiEditMsg.Way;
				for(i=1;i<8;i++)
				{
					if((WayChange<<i) & (tGuiEditMsg.Limit&0xff))
					{
						WayChange <<= i;
						break;
					}
				}
				if(i == 8)
				{
					while(--i)
					{
						if((tGuiEditMsg.Way>>i) & (tGuiEditMsg.Limit&0xff))
						{
							WayChange >>= i;
							break;
						}
					}
				}
				if(WayChange != tGuiEditMsg.Way)
				{
					tGuiEditMsg.Way=WayChange;
					tGuiEditMsg.ShowNum(WayChange,NULL);
					DisplayPaintEnd();
				}
			}
		}
		else if(keyNum==K_DEL)
		{
			if(tGuiEditMsg.indexEdit)
			{
				if(tGuiEditMsg.Way & IME_SUM)
					tGuiEditMsg.indexEdit=0;
				else
					tGuiEditMsg.indexEdit--;
			}
			tGuiEditMsg.sEditBuff[tGuiEditMsg.indexEdit]='\0';
			tGuiEditMsg.ShowNum(tGuiEditMsg.MaskCode,tGuiEditMsg.sEditBuff);
			DisplayPaintEnd();
			tGuiEditMsg.indexShow=tGuiEditMsg.indexEdit;
		}
		else if((keyNum>=K_0 && keyNum<=K_9) || keyNum==K_D)
		{	
			if(keyNum==K_D && tGuiEditMsg.Way == (IME_SUM|IME_NUM))	// 金客加数字 屏蔽小数点
				return EVENT_NONE;
			if(keyNum!=K_D && (tGuiEditMsg.Way&(IME_ABC|IME_abc)))
			{//---0~9 的数字转化--多输入法输入----
				u32		newTimeMs;
				u8		KeyValABC;
				if(tGuiEditMsg.oldKey != keyNum)
				{
					tGuiEditMsg.oldKey=keyNum;
					newTimeMs=0;
					tGuiEditMsg.KeyTimes=0;
				}
				else 
				{
					GetTickCount(&newTimeMs);
					newTimeMs -= tGuiEditMsg.oldTimeMs;
					if(newTimeMs>700)
					{
						newTimeMs=0;
						tGuiEditMsg.KeyTimes=0;
					}
					else	//连续快按
					{
						newTimeMs=1;
						if(++tGuiEditMsg.KeyTimes >= API_strlen(KeyMsgABC[keyNum-K_0]))
							tGuiEditMsg.KeyTimes=0;
					}
				}
				GetTickCount(&tGuiEditMsg.oldTimeMs);
				KeyValABC=KeyMsgABC[keyNum-K_0][tGuiEditMsg.KeyTimes];
				if(tGuiEditMsg.Way == IME_abc)
				{//-------大写转小写----------
					if(KeyValABC>='A' && KeyValABC<='Z')
						KeyValABC |= 0x20;
				}
				if(newTimeMs)
				{//------短按改变原来值--------
					tGuiEditMsg.indexEdit--;
					tGuiEditMsg.indexShow--;//驱动显示
				}
				keyNum=KeyValABC;
			}
			
			if(tGuiEditMsg.indexEdit < tGuiEditMsg.Max)
			{
				tGuiEditMsg.sEditBuff[tGuiEditMsg.indexEdit++]=keyNum;
				tGuiEditMsg.sEditBuff[tGuiEditMsg.indexEdit]='\0';
				if(tGuiEditMsg.Way == IME_SUM)
				{
					if(Conv_MoneyToCentInt(tGuiEditMsg.sEditBuff) < 0)
					{
						tGuiEditMsg.sEditBuff[--tGuiEditMsg.indexEdit]='\0';
						return NULL;
					}
				}
				else if(tGuiEditMsg.Way == (IME_SUM|IME_NUM))
				{
					if(tGuiEditMsg.indexEdit==1 \
						&&tGuiEditMsg.sEditBuff[0]=='0')
					{
						tGuiEditMsg.sEditBuff[--tGuiEditMsg.indexEdit]='\0';
						return NULL;
					}
				}
				tGuiEditMsg.ShowNum(tGuiEditMsg.MaskCode,tGuiEditMsg.sEditBuff);
				DisplayPaintEnd();
				tGuiEditMsg.indexShow=tGuiEditMsg.indexEdit;
			}
			else
			{
				APP_HitMsg(STR_EDIT_FULL,1500);
			}
		}
	}
	else
	{
		tGuiEditMsg.ShowNum(0x80000000|tGuiEditMsg.MaskCode,tGuiEditMsg.sEditBuff);
		DisplayPaintEnd();
	}
	return EVENT_NONE;
}

void API_GUI_InputEdit(char* pStrDef,int tMaxLen,u32 Way,Fun_ShowNum pShow)
{
	if(Way)
	{
		tGuiEditMsg.Max=tMaxLen;
		if(Way==IME_SUM)
			Conv_TmoneyToDmoney(tGuiEditMsg.sEditBuff,pStrDef);
		else
			API_strcpy(tGuiEditMsg.sEditBuff,pStrDef);

		tGuiEditMsg.indexShow=API_strlen(tGuiEditMsg.sEditBuff);
		if(Way|IME_SUM)
			tGuiEditMsg.indexEdit=0;	//需要重新输入
		else
			tGuiEditMsg.indexEdit=tGuiEditMsg.indexShow;
		//----------------兼容多种输入法-----------------------------------
		tGuiEditMsg.Limit=Way;
		{//--检查多输入法----
			u32 i,j,cWay=IME_NUM;
			for(j=0,i=0;i<4;i++)
			{
				if(cWay&Way)
					j++;
				cWay <<= 1;
			}
			if(j>1)
			{
				for(cWay=IME_NUM,i=0;i<4;i++)
				{
					if(cWay&Way)
						break;
					cWay <<= 1;
				}
				Way &= (~(IME_NUM|IME_abc|IME_ABC|IME_Abc))|cWay;
			}
		}
		//---------------------------------------------------
		tGuiEditMsg.Way=Way;
		if(Way&IME_PIN_MODE)
			tGuiEditMsg.MaskCode='*';
		else tGuiEditMsg.MaskCode=0x00;
		tGuiEditMsg.ShowNum=pShow;
		//---------------------------------------------------
		tWaitEventMsg.pFunEvenKey=&API_UI_InputEdit;
		API_UI_InputEdit(0);
	}
	else
	{
		tWaitEventMsg.pFunEvenKey=&API_UI_InputEdit;
	}
}
//=======================================================================================
*/


//----由于滚显示更新信息---pOriginal 原信息空间 Originalsize原信息空间大小--pAddMsg 新加信息------
void APP_ShowChangeInfo(XuiWindow *pWindow,char *pOriginal,int Originalsize,const char* format,...)
{
	u16 OriginalLen,AddMsgLen,i,offset=0;
	//u16 lineStart,LineMax,contMax;
	RECTL tRect;
	char pAddMsg[128];
	va_list arg;	
	va_start( arg, format);
	vsnprintf(pAddMsg,sizeof(pAddMsg),format,arg);
	va_end( arg );
	//-------------------------------------------------
	OriginalLen=API_strlen(pOriginal);
	AddMsgLen=API_strlen(pAddMsg);
	Originalsize--;
	while((OriginalLen+AddMsgLen) > Originalsize)
	{//-----信息内容已经满，需要清除前面行信息-------
		for(offset=0;offset<OriginalLen;offset++)
		{
			if(pOriginal[offset] == '\n')
			{
				offset++;//跳过'\n'
				break;
			}
		}
		if((offset == OriginalLen)&&(OriginalLen > (UI_CONT_W*2/FONT_SIZE)))
		{//---跳过(UI_CONT_W*2/FONT_SIZE)字节，避开半汉字------
			int flagHz=0;
			for(i=0;i<(UI_CONT_W*2/FONT_SIZE);i++)
			{
				if(pOriginal[i]&0x80)
				{
					if(flagHz==0)
						flagHz=1;
					else
						flagHz=0;
				}
			}
			offset=i-flagHz;
		}
		OriginalLen -= offset;
		for(i=0;i<OriginalLen;i++)
			pOriginal[i]=pOriginal[offset++];
		offset++;//作为标记屏刷新自加
	}
	for(i=0;i<=AddMsgLen;i++)
	{
		if((OriginalLen+i) > Originalsize) break;
		pOriginal[OriginalLen+i]=pAddMsg[i];
	}
	pOriginal[OriginalLen+i]='\0';	
	//----clear-----
	tRect.left=UI_CONT_X; tRect.top=UI_CONT_Y;
	tRect.width=UI_CONT_W; 
	tRect.height=(UI_CONT_H/FONT_SIZE - 1)*FONT_SIZE; //显多显示n-1行
	/*
	contMax =UI_CONT_W*2/FONT_SIZE;
	LineMax =(UI_CONT_H/FONT_SIZE - 1);
	for(lineStart=0;lineStart<LineMax;lineStart++)
	{
		
	}
	*/
	if(offset) API_GUI_ClearScreen(pWindow,&tRect);	//清空区域
	ApiFont.SetFontColor(FONT_CONT_COLOUR,UI_CONT_COLOUR);
	ApiFont.DrawRectString(pWindow,&tRect,pOriginal);
	DisplayPaintEnd(pWindow);	
}

//====================================================
/*--------------
make a QR code and display it to lcd
---------------*/
/*
void API_GUI_Draw565QRcode(RECTL* prclTrg,char *pInMsg,uint32 fgColor, uint32 bgColor)
{
	unsigned short x,y,dW,dH;
	unsigned short magnificationW,magnificationH,w,h;
	unsigned short color,rowSize;
	u32				yOffset,xOffset;
	QR_ENC_CODE_T* pQRcode;
	unsigned short *pQRcode565Map;
	u8* 			pMapBit;

	pQRcode = Cam_QR_Enc(API_strlen(pInMsg),(unsigned char*)pInMsg);
	if(pQRcode==NULL)
		return ;
	//TRACE("API GUI Draw565QRcode width[%d]height[%d]rowSize[%d]\r\n",pQRcode->width,pQRcode->height,pQRcode->rowSize);
	magnificationW = prclTrg->width / pQRcode->width;
	magnificationH = prclTrg->height / pQRcode->height;
	w=magnificationW*pQRcode->width;
	h=magnificationH*pQRcode->height;

	pQRcode565Map=(unsigned short*)malloc((w * h) * sizeof(unsigned short));
	if(pQRcode565Map==NULL)
		return ;

	if(prclTrg->width > w)
		prclTrg->left +=(prclTrg->width-w)/2;
	if(prclTrg->height > h)
		prclTrg->top +=(prclTrg->height -h)/2;
	prclTrg->width=w;
	prclTrg->height=h;

	if(bgColor & RGB565_PARENT)
		DisplayBitMapGet(prclTrg, pQRcode565Map, prclTrg->height*prclTrg->width);

	rowSize=pQRcode->rowSize;
	pMapBit=pQRcode->bits;
	for(y=0; y<pQRcode->height; y++)
	{
		for(x=0; x<pQRcode->width; x++)
		{
			//if(get(x,y,pQRcode))	//
			if(pMapBit[y*rowSize + x/8] & (0x01<<(x&0x07)))		//
				color=fgColor;
			else if(bgColor & RGB565_PARENT)
				continue;
			else 
				color=bgColor;
			yOffset=y*magnificationH*w + x*magnificationW;
			//-----------放大处理单个点-----------	
			for(dH=0;dH<magnificationH;dH++)
			{
				xOffset=yOffset+dH*w;
				for(dW=0;dW<magnificationW;dW++)
				{
					pQRcode565Map[xOffset+dW] = color;
				}
			}
		}
	}
	DisplayBitMap(prclTrg,(uint8*)pQRcode565Map);
	free(pQRcode565Map);
}
*/

//=============================================================================


int APP_WaitUiEvent(int tTimeOutMS)
{
	return API_WaitEvent(tTimeOutMS,EVENT_UI,EVENT_NONE);
}

int API_GUI_Show(XuiWindow *pWindow)
{
	return DisplayPaintEnd(pWindow);
}

void APP_ShowSta(XuiWindow *pWindow,char *pTitle,char *pMsg)
{
	API_GUI_CreateWindow(pWindow,pTitle,NULL,NULL,0);
	API_GUI_Info(pWindow,NULL,TEXT_ALIGN_CENTER|TEXT_VALIGN_CENTER,pMsg);
	API_GUI_Show(pWindow);
}

int APP_ShowMsg(XuiWindow *pWindow,char *pTitle,char *pMsg,int timeOutMs)
{
	APP_ShowSta(pWindow,pTitle,pMsg);
	return APP_WaitUiEvent(timeOutMs);
}

int APP_ShowInfo(XuiWindow *pWindow,char *pTitle,char *pInfo,int timeOutMs)
{
	API_GUI_CreateWindow(pWindow,pTitle,TOK,TCANCEL,0);
//	API_GUI_OprInfo(pInfo,NULL);
	API_GUI_Show(pWindow);
	return APP_WaitUiEvent(timeOutMs);
}

/*
void APP_HitMsg(const char* pMsg,int tTimeOutMS)
{
	POINT fTrg;
	RECTL mTrg;
	int slen;
	u16 *pLcdBuf=NULL;
	slen=API_strlen(pMsg);
	mTrg.width=slen*FONT_SIZE/2 + FONT_SIZE;
	if(mTrg.width >= UI_CONT_W)
		mTrg.width=UI_CONT_W;
	if(mTrg.width < 160)
		mTrg.width = 160;

	mTrg.left = UI_CONT_X+(UI_CONT_W-mTrg.width)/2;
	mTrg.height=80;
	mTrg.top = UI_CONT_Y+68;
	if(tTimeOutMS>0)
	{
		pLcdBuf=(u16*)malloc((mTrg.height * mTrg.width) * sizeof(unsigned short));
//		if(pLcdBuf)
//			DisplayBitMapGet(&mTrg,pLcdBuf,mTrg.height * mTrg.width);
	}
#if(0)
	UI_ShowPictureFile(&mTrg,L"E:\\bmp\\popup.bin");
#else
	//-------画矩形-------------------
	fTrg.left = mTrg.left+5; fTrg.top = mTrg.top+5;
	DisplayLineRGB(&fTrg, mTrg.width-10, mTrg.height-10,0xDF1C);
	//-------画框-------------------
	UI_ShowColorRect(&mTrg,5,RGB_CURR(0xff,0xff,0x7f));
#endif
	//-------------------------------------------
	fTrg.left = mTrg.left+(mTrg.width-(slen*FONT_SIZE/2))/2; 
	fTrg.top = mTrg.top+(mTrg.height-FONT_SIZE)/2;
	ApiFont.SetFontColor(RGB565_BLACK,0xDF1C);
	//ApiFont.SetFontColor(RGB565_RED|RGB565_GREEN,RGB565_PARENT);
	ApiFont.DrawLineString(&fTrg,pMsg);
	DisplayPaintEnd();
	if(pLcdBuf)
	{
//		Sleep(tTimeOutMS);
//		DisplayBitMap(&mTrg,(uint8*)pLcdBuf);
//		free(pLcdBuf);
		DisplayPaintEnd();
	}
}
*/
//============LineMax bit15=1,显示序号---===pAfterText:末尾(第8行)靠右显示======================
void APP_GUI_LineMenu(XuiWindow *pWindow,char* pTitle,char **pLineText,u16 LineMax,char* pAfterText)
{
	POINT tFontXY;
	u16 sWidth,i;
	char sBuff[4];
	tFontXY.left=SCREEN_MENU_X;
	//-------------------显示标题--------------------------------
	tFontXY.top =SCREEN_MENU_Y;
	DisplayLineRGB(pWindow,&tFontXY,SCREEN_MENU_W,MENU_TITLE_H,RGB_CURR(30,30,30));//0x5e1e
	if(pTitle!=NULL)
	{
		sWidth=API_strlen(pTitle)*FONT_SIZE/2;
		if(sWidth>SCREEN_MENU_W)
			tFontXY.left=SCREEN_MENU_X;
		else
			tFontXY.left=SCREEN_MENU_X+(SCREEN_MENU_W-sWidth)/2;
		tFontXY.top =SCREEN_MENU_Y+(MENU_TITLE_H-FONT_SIZE)/2;
		ApiFont.SetFontColor(RGB565_WITHE,RGB565_PARENT);
		ApiFont.DrawLineString(pWindow,&tFontXY,pTitle);
	}
	//-------------------显示内容--------------------------------
	if(LineMax&0x8000)
	{
		sBuff[1]='.'; sBuff[2]='\0';
		LineMax &= ~0x8000;
	}
	else sBuff[1]=0x00;
	
	tFontXY.top =SCREEN_MENU_Y+MENU_TITLE_H;
	for(i=0;i<MENU_TIEM_MAX;i++)
	{
		tFontXY.left=SCREEN_MENU_X;
		if(i&0x01)
			DisplayLineRGB(pWindow,&tFontXY,SCREEN_MENU_W,MENU_TIEM_H,RGB_CURR(76,76,76));
		else
			DisplayLineRGB(pWindow,&tFontXY,SCREEN_MENU_W,MENU_TIEM_H,RGB_CURR(90,90,90));

		tFontXY.top += (MENU_TIEM_H-FONT_SIZE)/2;
		if((i >= (MENU_TIEM_MAX-1)) && (pAfterText!=NULL)) break;
		if(i < LineMax)
		{
			tFontXY.left=SCREEN_MENU_X+4;
			ApiFont.SetFontColor(RGB565_WITHE,RGB565_PARENT);// 设置字体颜色
			if(sBuff[1])
			{
				sBuff[0]='0'+1+i;
				ApiFont.DrawLineString(pWindow,&tFontXY,sBuff);
				tFontXY.left=SCREEN_MENU_X+4+FONT_SIZE;
			}
			ApiFont.DrawLineString(pWindow,&tFontXY,pLineText[i]);
		}
		tFontXY.top += MENU_TIEM_H-(MENU_TIEM_H-FONT_SIZE)/2;
	}
	if(pAfterText)
	{
		int With = API_strlen(pAfterText)*FONT_SIZE/2;
		if(With>0)
		{
			tFontXY.left=(SCREEN_MENU_W - With);
			DisplayLineRGB(pWindow,&tFontXY,With,1,RGB565_WITHE);
			tFontXY.top++;tFontXY.top++;
			ApiFont.SetFontColor(RGB565_WITHE,RGB565_PARENT);// 设置字体颜色
			ApiFont.DrawLineString(pWindow,&tFontXY,pAfterText);
		}
	}
	//----------------------------------推送显示----------------------------------------------------
	DisplayPaintEnd(pWindow);
}



/*

//=============数字显示函数===========服务倒计时==================
void APP_ShowNumberL(RECTL *pRect,int num)
{
	char showbuff[12];
	int slen=sizeof(showbuff);
	int wlen=pRect->width/(FONT_SIZE/2);
	showbuff[--slen]='\0';
	while(wlen--)
	{
		showbuff[--slen]=num%10+'0';
		num /=10;
	}
	ApiFont.DrawRectString(pRect,showbuff+slen);
	DisplayPaintEnd();
}
//================倒计时显示=================
static RECTL rgTimeRect;
static u32 ShowTimeS,SeedTimeS;
//===============设置倒计时相对位置=与初值时间=======================================
void gShowTimeS_SetInit(u32 tTextType,int len,int timeS)
{
	GetCurrentTime(&SeedTimeS);
	ShowTimeS =SeedTimeS+ timeS;
	if(len > 11) len=11;
	rgTimeRect.width=len*FONT_SIZE/2;
	rgTimeRect.height=FONT_SIZE;
	switch(tTextType&TEXT_ALIGN_MASK)
	{
		case TEXT_ALIGN_RIGHT: 	//右对齐tIdle.rThis
			rgTimeRect.left=UI_CONT_X+UI_CONT_W-rgTimeRect.width;
			break;
		case TEXT_ALIGN_CENTER: //左右居中
			rgTimeRect.left=UI_CONT_X+(UI_CONT_W-rgTimeRect.width)/2;
			break;
		default ://case 0: 		//左对齐
			rgTimeRect.left=UI_CONT_X;
			break;
	}
	switch(tTextType&TEXT_VALIGN_MASK)
	{
		case TEXT_VALIGN_BOTTOM: 	//下对齐
			rgTimeRect.top=UI_CONT_Y+UI_CONT_H-rgTimeRect.height;
			break;
		case TEXT_VALIGN_CENTER: 	//上下居中
			rgTimeRect.top=UI_CONT_Y+(UI_CONT_H-rgTimeRect.height)/2;
			break;
		case 0: 	//指定行号(0~7)
			rgTimeRect.top=UI_CONT_Y + ((tTextType&TEXT_LINE_NUM_MASK)/0x10000)*(UI_CONT_H/8) + ((UI_CONT_H/8)-FONT_SIZE)/2;
			break;
		default: 		//上对齐
			rgTimeRect.top=UI_CONT_Y;
			break;
	}
	APP_ShowNumberL(&rgTimeRect,timeS);
}


int gShowTimeS_GetOut(void)
{
	u32 CurrSec;
	GetCurrentTime(&CurrSec);
	if(SeedTimeS!= CurrSec)
	{
		if(((int)CurrSec- (int)ShowTimeS) >= 0)
			return -3;//EVENT_TIMEOUT;
		SeedTimeS=CurrSec;
		APP_ShowNumberL(&rgTimeRect,ShowTimeS-SeedTimeS);
	}
	return 0;//EVENT_NONE;
}
*/



const API_GUI_Def ApiGUI={
	{'G','U','I',26},
	
};

