/**
 * File:   linux_fb.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  linux_framebuffer
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

#include "pixel.h"
#include "bitmap.h"

#ifndef LINUX_FB_H
#define LINUX_FB_H

typedef union {
	unsigned char *pixel;	//Window 关联画布指针
	pixel_rgb565_t *rgb565;
	pixel_bgra8888_t *bgra8888;
}area_buffer;



typedef struct {
	int dev_fd;
	u16 openFlagOK;
	u16 bytes_per_pixel;
	u16 width,height;	
	int line_length;
	int memlen;
	union 
	{
		unsigned char *buffer;	//Window 关联画布指针
		pixel_rgb565_t *rgb565buff;
		pixel_bgra8888_t *bgra8888buff;
		unsigned long *u32add;	//Window 关联画布指针
	};
}screen_buffer;



//===========================================================================================
typedef struct{
  u16 width;
  u16 height;
  u16 wLen;
  u16 pixel;
  unsigned char *buffer;
}SCREEN_AREA;

	

extern int open_screen(const char* filename,XuiWindow *pHardWindow);
extern void close_screen(void);

extern int xui_fb_line(int xs, int ys, int xe, int ye, const rgba_t* rgba);
extern int xui_fb_circle(signed short x, signed short y, signed short r, const rgba_t* rgba);
extern int xui_fb_fill_circle(signed short x, signed short y, signed short r,signed short ar, const rgba_t* rgba);
extern int xui_fb_vline(int x, int y, int h, const rgba_t* rgba);
extern int xui_fb_hline(int x, int y, int w, const rgba_t* rgba);
extern int xui_fb_fill_rect(int x, int y, int w, int h,const rgba_t* rgba);
extern int xui_fb_stroke_rect(int x, int y, int w, int h,const rgba_t* rgba);
//extern int xui_fb_close(screen_buffer* fb);
extern int xui_fb_show_rect(int x, int y, int w, int h,rgba_t* rgba);
extern int xui_fb_rect_push(int x, int y, int w, int h,rgba_t* pInrgb);
extern int xui_rect_push(RECTL* pRect,int mWidth,u32* pInrgb);


#endif /*LINUX_FB_H*/
