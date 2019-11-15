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
#include "xui_fb.h"
#include "xui_show.h"
#include "xui_font.h"
#include "xui_ui.h"
#include "xui_gui.h"
#include "ui_menu.h"

#include <fcntl.h>
#include <linux/input.h>




int APP_QrCodeTest(char *pTitle)
{
	RECTL rpin;
	char *pshowfont="12345678abcd";
	APP_ShowSta("二维码测试",NULL);
	rpin.top=24+40;
	rpin.left=10;
	rpin.width=SCREEN_WIDTH-rpin.left;
	rpin.height=SCREEN_HEIGT-rpin.top;
	ApiUI.ShowQrCode(&rpin,pshowfont,RGB_CURR(0,0,0xff));
	ApiUI.Push(NULL);
	return APP_WaitUiEvent(5000);
}



int APP_AutoTest(char *pTitle)
{
	APP_ShowMsg(pTitle,"测试项1234",2000);
	return 0;
}

int APP_HardTestMenu(char* title)
{
	CMenuItemStru MenuStruPar[]=
	{
		{"显示屏测试",		APP_AutoTest},
		{"按键测试",		APP_AutoTest},
		{"SIM卡测试",		APP_AutoTest},
		{"扫码头测试",		APP_AutoTest},
		{"播放语音测试",	APP_AutoTest},
		{"网络测试",		APP_AutoTest},
		{"电池测试",		APP_AutoTest},
		{"NFC卡测试",		APP_AutoTest},
	};
	APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
	//APP_AddCurrentMenuOtherFun(MENU_OUT_FUN,APP_SaveTestResult,NULL);
	return 0;
}


int APP_FactoryMeun(char* title)
{
	CMenuItemStru MenuStruPar[]=
	{
		{"单项测试",			APP_HardTestMenu},
		{"自动测试",			APP_AutoTest},
		{"测试结果",			APP_AutoTest},
		{"老化测试",			APP_AutoTest},
	//	{"拨打电话测试",		APP_AutoTest}
		{"二维码测试",			APP_QrCodeTest}

		
	//	"单项测试",				NULL,
	//	"自动测试",				NULL,
	//	"测试结果",				NULL,
	//	"老化测试",				NULL,
	//	"拨打电话测试",			NULL,
	};
	return APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
}








