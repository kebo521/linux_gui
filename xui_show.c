/**
 * File:   fbshow.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  snap framebuffer to png file
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <unistd.h>
//#include <stdarg.h>

#include "types_def.h"
#include "xui_ui.h"

//#include "xui_fb.h"
//#include "xui_show.h"
//#include "xui_font.h"
#include "xui_gui.h"
#include "ui_menu.h"

#include <fcntl.h>
#include <linux/input.h>




int APP_QrCodeTest(char *pTitle)
{
	RECTL rpin;
	char *pshowfont="12345678abcd";
	APP_ShowSta("��ά�����",NULL);
	rpin.top=24+40;
	rpin.left=10;
	rpin.width=SCREEN_WIDTH-rpin.left;
	rpin.height=SCREEN_HEIGT-rpin.top;
	ApiUI.ShowQrCode(API_GUI_GetCurrWindow(),&rpin,pshowfont,RGB_CURR(0,0,0xff));
	ApiUI.Push(API_GUI_GetCurrWindow(),NULL);
	return APP_WaitUiEvent(5000);
}



int APP_AutoTest(char *pTitle)
{
	APP_ShowMsg(pTitle,"������1234",2000);
	return 0;
}


int APP_NineDirecTest(char *pTitle)
{
	API_GUI_CreateWindow(pTitle,NULL,NULL,API_FillShowBack);
	API_GUI_Info(NULL,TEXT_ALIGN_LEFT|TEXT_VALIGN_TOP|TEXT_EXSTYLE_UNDERLINE,"����");
	API_GUI_Info(NULL,TEXT_ALIGN_CENTER|TEXT_VALIGN_TOP,"����");
	API_GUI_Info(NULL,TEXT_ALIGN_RIGHT|TEXT_VALIGN_TOP,"����");

	API_GUI_Info(NULL,TEXT_ALIGN_LEFT|TEXT_VALIGN_CENTER,"����");
	API_GUI_Info(NULL,TEXT_ALIGN_CENTER|TEXT_VALIGN_CENTER|TEXT_EXSTYLE_BORDER,"����");	//
	API_GUI_Info(NULL,TEXT_ALIGN_RIGHT|TEXT_VALIGN_CENTER,"����");

	
	API_GUI_Info(NULL,TEXT_ALIGN_LEFT|TEXT_VALIGN_BOTTOM,"����");
	API_GUI_Info(NULL,TEXT_ALIGN_CENTER|TEXT_VALIGN_BOTTOM,"����");
	API_GUI_Info(NULL,TEXT_ALIGN_RIGHT|TEXT_VALIGN_BOTTOM|TEXT_EXSTYLE_OVERLINE,"����");
	
	API_GUI_Show();
	APP_WaitUiEvent(5000);
	return 0;
}


int APP_QianMeun(char* title)
{
	CMenuItemStru MenuStruPar[]=
	{
		{"�ŷ�λ��ʾ ",			APP_NineDirecTest},
		{"��ά�����",			APP_QrCodeTest}
	};
	return APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
}



int APP_QianTest(char *pTitle)
{
	XuiWindow *babyWindow,*pCurrWindow;
	pCurrWindow = API_GUI_GetCurrWindow();
	babyWindow = XuiCreateCanvas(pCurrWindow,24,60,192,296-130);
	API_GUI_LoadWindow(babyWindow);

	APP_QianMeun(pTitle);
	APP_ShowProsseMenu();
	
	XuiDestroyWindow(babyWindow);
	sleep(2);
	API_GUI_LoadWindow(pCurrWindow);
	return 0;
}



int APP_HardTestMenu(char* title)
{
	CMenuItemStru MenuStruPar[]=
	{
		{"��ʾ������",		APP_AutoTest},
		{"��������",		APP_AutoTest},
		{"SIM������",		APP_AutoTest},
		{"ɨ��ͷ����",		APP_AutoTest},
		{"������������",	APP_AutoTest},
		{"�������",		APP_AutoTest},
		{"��ز���",		APP_AutoTest},
		{"NFC������",		APP_AutoTest},
	};
	APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
	//APP_AddCurrentMenuOtherFun(MENU_OUT_FUN,APP_SaveTestResult,NULL);
	return 0;
}


int APP_FactoryMeun(char* title)
{
	CMenuItemStru MenuStruPar[]=
	{
		{"�������",			APP_HardTestMenu},
		//{"�Զ�����",			APP_AutoTest},
		//{"���Խ��",			APP_AutoTest},
		//{"�ϻ�����",			APP_AutoTest},
		{"�ŷ�λ��ʾ ",			APP_NineDirecTest},
		{"Ƕ�ײ˵�",			APP_QianTest},
		{"��ά�����",			APP_QrCodeTest}

		
	//	"�������",				NULL,
	//	"�Զ�����",				NULL,
	//	"���Խ��",				NULL,
	//	"�ϻ�����",				NULL,
	//	"����绰����",			NULL,
	};
	return APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
}








