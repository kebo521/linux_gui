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
#include "xui_gui.h"
#include "ui_menu.h"



screen_buffer *screenfb;

void API_Trace(char* pMsg,...)
{
	int		ret;
	char	sTraceBuff[4096];
	//------------------------------------------
	va_list arg;
	va_start(arg, pMsg);
	ret=vsnprintf(sTraceBuff,sizeof(sTraceBuff),pMsg,arg);	//ret=
	va_end(arg);
	printf("Trace[%d]->%s",ret,sTraceBuff);//stdout
}

void APP_Trace_Hex(char* msg,void* pBuff,int Len)
{
	int i;
	printf("%s[%d]:",msg,Len);
	for(i=0;i<Len;i++)
	{
		printf("%02X ",((u8*)pBuff)[i]);
	}
	printf("\r\n");
}


void u32coy(void *u1,void *u2)
{
	*(unsigned long *)u1 = *(unsigned long *)u2;
}


int APP_AutoTest(char *pTitle)
{
	APP_ShowMsg(pTitle,"测试项1234",0);
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
		{"拨打电话测试",		APP_AutoTest}
	//	"单项测试",				NULL,
	//	"自动测试",				NULL,
	//	"测试结果",				NULL,
	//	"老化测试",				NULL,
	//	"拨打电话测试",			NULL,
	};
	return APP_CreateNewMenuByStruct(title,sizeof(MenuStruPar)/sizeof(CMenuItemStru),MenuStruPar,30*1000);
}


#define FB_WIDTH	260
#define FB_HEIGHT	340

int main(int argc, char* argv[]) {
	rgba_t rgba;
	s16 staX,staY;
	screen_buffer* fb;
	TRACE("->main in[%d][%d][%d][%d] \r\n",sizeof(char),sizeof(int),sizeof(long),sizeof(char*));
	fb=open_screen("/dev/fb0");
	if(fb == NULL) 
	{
		TRACE("->main open_screen ret NULL \r\n");
		return 0;
	}
	InitExtResLib("./bin/ks.res");
	screenfb = fb;
	memset(&rgba,0x7f,sizeof(rgba));
	staX=fb->width-FB_WIDTH;
	staY=0;
 	xui_fb_fill_rect(fb,staX,staY,FB_WIDTH,FB_HEIGHT,&rgba);
	//------------------------------------------------------------
	if(XuiRootCanvas(fb))
		return 1;
	
	XuiPush();
	
	

//	APP_FactoryMeun("硬件测试");
//	APP_ShowProsseMenu();
	rgba.r=0x00;
	rgba.g=0x00;
	rgba.b=0x00;
//	xui_fb_vline(fb,staX+SHOW_WIDTH/2,staY,SHOW_HEIGHT,&rgba);
// 	xui_fb_hline(fb,staX,staY+SHOW_HEIGHT/2,SHOW_WIDTH,&rgba);

//	xui_fb_line(fb,staX,staY,staX+SHOW_WIDTH-1,staY+SHOW_HEIGHT-1,&rgba);
//	xui_fb_line(fb,staX,staY+SHOW_HEIGHT-1,staX+SHOW_WIDTH-1,staY,&rgba);
//	xui_fb_circle(fb,staX+SHOW_WIDTH/2,staY+SHOW_HEIGHT/2,5,&rgba);
//	xui_fb_circle(fb,staX+SHOW_WIDTH/2,staY+SHOW_HEIGHT/2,10,&rgba);
	rgba.r=0xff;
	rgba.b=0xff;
//	xui_fb_fill_circle(fb,staX+FB_HEIGHT/2,staY+FB_HEIGHT/2,140,50,&rgba);
	rgba.r=0;
	rgba.b=0;

//	xui_fb_circle(fb,staX+SHOW_WIDTH/2,staY+SHOW_HEIGHT/2,20,&rgba);
	rgba.r=0xff;
//	xui_fb_circle(fb,staX+SHOW_WIDTH/2,staY+SHOW_HEIGHT/2,40,&rgba);
	rgba.r=0x00;
	rgba.g=0xff;
//	xui_fb_circle(fb,staX+SHOW_WIDTH/2,staY+SHOW_HEIGHT/2,80,&rgba);
	rgba.g=0x00;
	rgba.b=0xff;
//	xui_fb_circle(fb,staX+SHOW_WIDTH/2,staY+SHOW_HEIGHT/2,100,&rgba);

	

	POINT tpin;
	tpin.left=0;
	tpin.top=0;
	char *pshowfont="我和我的祖国123abcdefghijkmnopk";
	//TRACE_HEX("pshowfont",pshowfont,strlen(pshowfont));
	UI_DrawString(&tpin,pshowfont);
	XuiPush();

	RECTL rpin;
	rpin.top=96;
	rpin.left=0;
	rpin.width=240;
	rpin.height=48;
//	UI_DrawRectString(&rpin,pshowfont);
//	XuiPush();
	sleep(1);
	APP_ShowSta("二维码测试","内容显示区");
	sleep(1);
	rpin.top=24+40;
	rpin.left=10;
	rpin.width=220;
	rpin.height=320-24-40;

	API_UI_ShowQrCode(&rpin,pshowfont,RGB_CURR(0,0,0xff));

	


	ExtResLib_DeInit();
	XuiClose();
	//sleep(2);
	close_screen(fb);
	
	

	return 0;
}



