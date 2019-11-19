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
#include "xui_ui.h"
#include "xui_fb.h"
#include "xui_font.h"
#include "xui_gui.h"
#include "ui_menu.h"
#include "EvenMsg.h"
#include "key_hard.h"
#include "xui_show.h"

#include "main.h"

#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>
#include <sys/shm.h>  
//#include <sys/ipc.h>


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

	/*
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
	*/
	return 0;
}


int test_Printf(char* title)
{
	printf("test_Printf[%s]->\r\n",title);//stdout
	return 0;
}

void OsSleep(unsigned int Ms)
{
	while(Ms)
	{
		if(Ms>1000)
		{
			sleep(Ms/1000);
			Ms -= 1000;
		}
		else
		{
			usleep(Ms*1000);
			Ms = 0;
		}
	}
}


typedef struct{
	char Id[64];
	char Name[64];
	char Bin[64];
	char Artwork[64];
	char Desc[64];
	char Vender[32];
	char Version[32] ;
} ST_APP_INFO;

int OsGetAppInfo(ST_APP_INFO AppInfo[],int InfoCnt)
{

	return 0;
}
/*
参数
OptInfo 【输出】
指向 ST_OPT_INFO 实体数组的可选系
统组件信息结构体指针，该指针不能为
NULL。
InfoCnt 【输入】 OptInfo 数组可以存放可选系统组件的
数目
返回
>=0 返回获取到的可选系统组件信息个数
ERR_FILE_NOT_FOUND 存放可选系统组件文件的路径不存在
ERR_INVALID_PARAM 非法参数
用法 当存在的可选系统组件个数大于 InfoCnt 时，只获取前 InfoCnt 个可
选系统组件的信息。
*/
typedef struct {
	char Name[64];
	char Version[32];
} ST_OPT_INFO;
int OsGetOptInfo(ST_OPT_INFO OptInfo[],int InfoCnt)
{

	return 0;
}

typedef void (*RUNAPP_CB)(char *appid, char *str, void *data);


int OsRunApp(char *AppId, char **Argv, void *Data, RUNAPP_CB CbOut,RUNAPP_CB CbErr)
{



	execv(AppId, Argv);
	return 0;
}

















typedef struct	
{
	char Mask[4]; 	// "API"
	int (*test)(char*);	//测试函数
	const API_UI_Def *ui;
	const API_FONT_Def *font;
	const API_Even_Def *event;
}API_SDK;


const API_SDK SdkApi={
	{'A','P','I',3},
	test_Printf,
	&ApiUI,
	&ApiFont,
	&ApiEven,
};

/*
char **environ; 
int execl (const char *path, const char *arg0, ..., (char*)0);
int execlp(const char *file, const char *arg0, ..., (char*)0);
int execle(const char *path, const char *arg0, ..., (char*)0, char *const envp[]); 
int execv (const char *path, char *const argv[]);
int execvp(cosnt char *file, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
*/

int main(int argc, char* argv[]) {
//	int ret;
	
	char *pHardMsg[]={
	"FB=/dev/fb0",
	"INPUT=/dev/input/event2",
	"ROTATE=0",
//	"TSDEV=",
	"STATUSBAR=24",
	};
	/*
	ret=InitProcessPool(0);
	if(ret)
	{
		TRACE("->main InitProcessPool ret[%d]\r\n",ret);
		return 1;
	}
	//*/
	/*
	int shmid;  
	TRACE("Create a shared memory Init\r\n");
    shmid=shmget(0xA019,sizeof(SdkApi),0666|IPC_CREAT);  
	TRACE("shared memory shmget %d\r\n",shmid);
	if(shmid == -1)
	{
		return -1;
	}
	API_SDK *psdkAPI;
	//psdkAPI = malloc(sizeof(SdkApi));
	
	psdkAPI=shmat(shmid,0,0); //SHM_RDONLY|SHM_RND
	TRACE("shared memory[%X]segment\r\n",psdkAPI);
	if(psdkAPI)
		memcpy(psdkAPI,&SdkApi,sizeof(SdkApi));
	
	TRACE(" a execl [%d]\r\n",ret);
	sleep(20);

	ret=shmdt(psdkAPI);
	TRACE(" a shared memory shmdt[%d]\r\n",ret);
	ret=shmctl(shmid,IPC_RMID,NULL);
	TRACE(" a shared memory shmctl[%d]\r\n",ret);
	*/

	
	XuiWindow* pWindow;
		
	XuiOpen(sizeof(pHardMsg)/sizeof(pHardMsg[0]) ,pHardMsg);
	if((pWindow=XuiRootCanvas()) != NULL)
	{
		UI_DisplaySysEn(pWindow,0,0,TEXT_12,"0 yz131234&&*()_+~!@#$%^&*");
		UI_DisplaySysEn(pWindow,0,TEXT_12,TEXT_16,"0 yz131234&&*()_+~!@#$%^&*");
		UI_DisplaySysEn(pWindow,0,TEXT_12+TEXT_16,TEXT_24,"0 yz131234&&*()_+~!@#$%^&*");
		UI_Push(pWindow,NULL);
		sleep(2);
		API_GUI_SetTheme(pWindow,NULL);
		
		

		API_GUI_CreateWindow(NULL,TOK,TCANCEL,GUI_MENU_LINE);
		API_GUI_Show();
		sleep(1);
		API_GUI_CreateWindow("我在哪",TOK,TCANCEL,GUI_SHOW_MSG);
		API_GUI_Show();
		sleep(1);

		
		ApiEven.Init(0,0);
		
		ApiEven.LoadThread(NULL);
		ApiFont.InitFontLib("./bin/ks.res");
	
		APP_FactoryMeun(pWindow,"硬件测试");
		APP_ShowProsseMenu(pWindow,NULL);

		ApiFont.DeInitFontLib();
		ApiEven.KillThread(NULL);
		
		ApiUI.DestroyWindow(pWindow);

		ApiEven.DeInit();
	}
	XuiClose();
	//*/
	TRACE("->Main End\r\n");
	//------------------------------------------------------------
	return 0;
}








