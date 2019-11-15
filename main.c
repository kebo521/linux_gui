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
//#include <fcntl.h>
#include <sys/wait.h>
#include<sys/stat.h>

#include "types_def.h"
#include "xui_fb.h"
#include "xui_show.h"
#include "xui_font.h"
#include "xui_ui.h"
#include "xui_gui.h"
#include "ui_menu.h"
#include "EvenMsg.h"
#include "key_hard.h"
#include "main.h"

#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>


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


void OsLogSetTag(const char *Tag)
{

}

int OsLog(LOG_T Prio,const char *fmt,...)
{
	int		ret;
	char	sTraceBuff[4096];
	//------------------------------------------
	if(Prio <  CURR_LOG_T) return 0;
	va_list arg;
	va_start(arg, fmt);
	ret=vsnprintf(sTraceBuff,sizeof(sTraceBuff),fmt,arg);	//ret=
	va_end(arg);
	printf("Trace[%d]->%s",ret,sTraceBuff);//stdout
	return ret;
}



int test_Printf(char* title)
{
	printf("test_Printf[%s]->\r\n",title);//stdout
	return 0;
}
//#include <unistd.h>
 
/* 
void *PrintHello(void *args)
{
    return NULL;
}
 */
int InitProcessPool(int noclose)
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return -1;
	}
	TRACE("1->InitProcessPool getpid[%d]fork[%d]\r\n",getpid(),pid);
	if(pid != 0)
		return 1;
	pid=setsid();
	TRACE("2->InitProcessPool getpid[%d]setsid[%d]\r\n",getpid(),pid);
	if(pid < 0)
	{
		printf("setsid failed\n");
		return 2;
	}
	//chdir("/");
	if (!noclose)
	{
		int fd;
		fd = open ("/dev/null", O_RDWR, 0);
		if (fd != -1)
		{
			dup2 (fd, STDIN_FILENO);
			dup2 (fd, STDOUT_FILENO);
			dup2 (fd, STDERR_FILENO);
			if (fd > 2)
				close(fd);
		}
	}
	umask(0);	//子进程权限
	while(1) 
	{//---守护进程-------
		pid=fork();
		if (pid < 0)
		{
			perror("fork");
			return -2;
		}
		if (pid != 0)
		{ 
			pid_t _pid_t = wait(&pid); //---等待孙进程结束------
			printf("------reRunAPP-[%d,%d]---------\n",_pid_t,pid);
		} 
		else break; //孙进程，跳出外面执行
	} 
	umask(0); //孙进程权限
	return 0;
}




int main(int argc, char* argv[]) {
	int ret;
	char *pHardMsg[]={
	"FB=/dev/fb0",
	"INPUT=/dev/input/event2",
	"ROTATE=0",
//	"TSDEV=",
	"STATUSBAR=24",
	};

	ret=InitProcessPool(0);
	if(ret)
	{
		TRACE("->main InitProcessPool ret[%d]\r\n",ret);
		return 1;
	}
	ApiUI.open(sizeof(pHardMsg)/sizeof(pHardMsg[0]) ,pHardMsg);
	if(ApiUI.RootCanvas() == 0)
	{
		ApiFont.DisplaySysString(0,"0 abcdefghirstuvwxyz",3*1000);
		
		ApiEven.Init(0,0);
		
		ApiEven.LoadThread(NULL);
		ApiFont.InitFontLib("./bin/ks.res");
	
		APP_FactoryMeun("硬件测试");
		APP_ShowProsseMenu();

		ApiFont.DeInitFontLib();
		ApiEven.KillThread(NULL);

		

		ApiEven.DeInit();
	}
	ApiUI.close();
	
	//------------------------------------------------------------
	return 0;
}








