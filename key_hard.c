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

#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>
#include "key_hard.h"
#include "EvenMsg.h"
#include "xui_ui.h"




void *get_keyMsg(void *args)  
{  
//	int keys_fd;  
	struct input_event t;  

//	keys_fd = open("/dev/input/event2",O_RDWR);  // O_RDONLY
	if (gUiDataAll.keys_fd <= 0)  
	{  
		printf("open /dev/input/event2 device error!\n");  
		return 0;  
	}  
	while(1)  
	{  
		if (read(gUiDataAll.keys_fd, &t, sizeof(t)) == sizeof(t))  
		{  
			if (t.type == EV_KEY) 
			{
				if(t.value == 1)
				{
					int keyVar=0;
					printf (" -key[%d][%d]\n", t.code,t.value);
					if(t.code == KEY_END) break;
					switch(t.code)
					{
						case KEY_0: keyVar=K_0; break;
						case KEY_1: keyVar=K_1; break;
						case KEY_2: keyVar=K_2; break;
						case KEY_3: keyVar=K_3; break;
						case KEY_4: keyVar=K_4; break;
						case KEY_5: keyVar=K_5; break;
						case KEY_6: keyVar=K_6; break;
						case KEY_7: keyVar=K_7; break;
						case KEY_8: keyVar=K_8; break;
						case KEY_9: keyVar=K_9; break;
						case KEY_UP: keyVar=K_UP; break;
						case KEY_DOWN: keyVar=K_DOWN; break;
						case KEY_LEFT: keyVar=K_LEFT; break;
						case KEY_RIGHT: keyVar=K_RIGHT; break;
						case KEY_ESC: keyVar=K_CANCEL; break;
						case KEY_ENTER: keyVar=K_OK; break;
						case KEY_DELETE: keyVar=K_DEL; break;
						case KEY_TAB: keyVar=K_IME; break;
					}
					FIFO_OperatSetMsg(EVEN_ID_KEY_DOWN,keyVar);
				}
			}
			else if(t.type == EV_REL)
			{
				printf (" -REL[%d][%d]\n", t.code,t.value);	
				if(t.code == KEY_ESC)  
					break;	
			}
			else if(t.type == EV_ABS)
			{
				printf (" -ABS[%d][%d]\n", t.code,t.value);	
			}
		//	printf (" [%d,%d,%d][%d]\n",EV_KEY,EV_REL,EV_ABS,t.type);	
		}  
	}  
//	close (keys_fd); 
	printf (" -key_end-pthread_exit\n");	
	pthread_exit(NULL);
	return 0;  
}  


static pthread_t key_thread=NULL;


void Start_Key_thread(void)
{
	int rc;
	rc = pthread_create(&key_thread, NULL, get_keyMsg,NULL);
	if (rc)
	{
	   TRACE("ERROR; return code is %d\n", rc);
	}
}

void Stop_Key_thread(void)
{
	//pthread_join(key_thread, NULL);
	int ret;
	ret=pthread_cancel(key_thread);
	TRACE("Stop Key thread %d\n", ret);
}




