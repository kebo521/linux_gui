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
	APP_ShowSta("∂˛Œ¨¬Î≤‚ ‘",NULL);
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
	APP_ShowMsg(pTitle,"≤‚ ‘œÓ1234",2000);
	return 0;
}

int APP_QianTest(char *pTitle)
{
	XuiWindow *babyWindow,*pCurrWindow;
	pCurrWindow = API_GUI_GetCurrWindow();
	babyWindow = XuiCreateCanvas(pCurrWindow,24,60,192,296-130);
	API_GUI_Init(babyWindow,NULL);
	
	APP_ShowMsg(pTitle,"–°≤‚ ‘¥∞ø⁄",2000);

	
	XuiDestroyWindow(babyWindow);
	API_GUI_Init(pCurrWindow,NULL);
	return 0;
}


int APP_HardTestMenu(char* title)
{
	CMenuItemStru MenuStruPar[]=
	{
		{"œ‘ æ∆¡≤‚ ‘",		APP_AutoTest},
		{"∞¥º¸≤‚ ‘",		APP_AutoTest},
		{"SIMø®≤‚ ‘",		APP_AutoTest},
		{"…®¬ÎÕ∑≤‚ ‘",		APP_AutoTest},
		{"≤•∑≈”Ô“Ù≤‚ ‘",	APP_AutoTest},
		{"Õ¯¬Á≤‚ ‘",		APP_AutoTest},
		{"µÁ≥ÿ≤‚ ‘",		APP_AutoTest},
		{"NFCø®≤‚ ‘",		APP_AutoTest},
	};
	APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
	//APP_AddCurrentMenuOtherFun(MENU_OUT_FUN,APP_SaveTestResult,NULL);
	return 0;
}


int APP_FactoryMeun(char* title)
{
	CMenuItemStru MenuStruPar[]=
	{
		{"µ•œÓ≤‚ ‘",			APP_HardTestMenu},
		{"◊‘∂Ø≤‚ ‘",			APP_AutoTest},
		{"≤‚ ‘Ω·π˚",			APP_AutoTest},
		{"¿œªØ≤‚ ‘",			APP_AutoTest},
		{"«∂Ã◊≤Àµ•",			APP_QianTest},
		{"∂˛Œ¨¬Î≤‚ ‘",			APP_QrCodeTest}

		
	//	"µ•œÓ≤‚ ‘",				NULL,
	//	"◊‘∂Ø≤‚ ‘",				NULL,
	//	"≤‚ ‘Ω·π˚",				NULL,
	//	"¿œªØ≤‚ ‘",				NULL,
	//	"≤¶¥ÚµÁª∞≤‚ ‘",			NULL,
	};
	return APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
}








