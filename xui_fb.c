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
#include "types_def.h"
#include <unistd.h>
#include <math.h>

#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "xui_ui.h"
#include "xui_fb.h"



/*
XuiWindow* xui_fb_open(const char* filename) {
  uint32_t size = 0;
  XuiWindow* fb = NULL;
  struct fb_fix_screeninfo fix;
  struct fb_var_screeninfo var;
  return_value_if_fail(filename != NULL, NULL);

  fb = (XuiWindow*)calloc(1, sizeof(XuiWindow));
  return_value_if_fail(fb != NULL, NULL);

  fb->fd = open(filename, O_RDWR);
  if (fb->fd < 0) {
    log_debug("open %s failed(%d)\n", filename, errno);
    free(fb);
    return NULL;
  }

  if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fix) < 0) goto fail;
  if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &var) < 0) goto fail;

  var.xoffset = 0;
  var.yoffset = 0;
  ioctl(fb->fd, FBIOPAN_DISPLAY, &(var));

  log_debug("fb_info_t: %s\n", filename);
  log_debug("fb_info_t: xres=%d yres=%d bits_per_pixel=%d mem_size=%d\n", var.xres, var.yres,
            var.bits_per_pixel, fb_size(fb));
  log_debug("fb_info_t: red(%d %d) green(%d %d) blue(%d %d)\n", var.red.offset, var.red.length,
            var.green.offset, var.green.length, var.blue.offset, var.blue.length);

  fb->w = var.xres;
  fb->h = var.yres;
  fb->bpp = var.bits_per_pixel / 8;
  fb->line_length = fix.line_length;

  size = fb->height * fb->line_length;
  fb->data = (uint8_t*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);

  if (fb->data == MAP_FAILED) {
    log_debug("map framebuffer failed.\n");
    goto fail;
  }

  log_debug("line_length=%d mem_size=%d\n", fix.line_length,fb->height * fb->line_length);
  log_debug("xres_virtual =%d yres_virtual=%d xpanstep=%d ywrapstep=%d\n", var.xres_virtual,
            var.yres_virtual, fix.xpanstep, fix.ywrapstep);

  return fb;
fail:
  log_debug("%s is not a framebuffer.\n", filename);
  close(fb->fd);
  free(fb);

  return NULL;
}


ret_t xui_fb_close(XuiWindow* fb) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  munmap(fb->data, fb_size(fb));
  close(fb->fd);
  memset(fb, 0x00, sizeof(XuiWindow));
  free(fb);

  return RET_OK;
}

*/

static screen_buffer fb_screen={0};

void close_screen(void)
{
	if(fb_screen.openFlagOK)
	{
		/* Unmap the framebuffer*/
		munmap(fb_screen.buffer, fb_screen.memlen);
		/* Close framebuffer device */
		close(fb_screen.dev_fd);
		memset(&fb_screen,0x00,sizeof(fb_screen));
	}
	fb_screen.openFlagOK = 0;
}

int open_screen(const char* filename,XuiWindow *pHardWindow) //XuiWindow *pHardWindow
{
	int ret;
	struct fb_var_screeninfo var;
	struct fb_fix_screeninfo fix;
	printf("open screen open[%s]\r\n",filename);
	if(fb_screen.openFlagOK)
	{
		printf("open screen open[%s]\r\n",filename);
		close_screen();
	}
	if ((fb_screen.dev_fd = open(filename, O_RDWR)) == -1) {
		printf("open screen open[%s] Err\r\n",filename);
		return -1;
	}
	ret = ioctl(fb_screen.dev_fd, FBIOGET_VSCREENINFO, &var);
	if (ret) {
		printf("open screen in %s line %d\r\n",__FUNCTION__,__LINE__);
		return -2;
	} else {
		fb_screen.width = var.xres;
		fb_screen.height = var.yres;
		fb_screen.bytes_per_pixel = var.bits_per_pixel / 8;
		if(fb_screen.bytes_per_pixel == 3)
			fb_screen.bytes_per_pixel = 4;
	}
	if (ioctl(fb_screen.dev_fd, FBIOGET_FSCREENINFO, &fix) < 0) {
		close(fb_screen.dev_fd);
		printf("open screen ioctl FBIOGET_FSCREENINFO Err\r\n");
		return -3;
	}
	fb_screen.memlen = fb_screen.height * fix.line_length;
	fb_screen.buffer = (unsigned char *) mmap(NULL, fb_screen.memlen, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fb_screen.dev_fd, 0);
	if (fb_screen.buffer == MAP_FAILED)
	{
		printf("rw_sd_inand.c: Can't mmap frame buffer ++\r\n");
		close(fb_screen.dev_fd);
		return -4;
	}
	fb_screen.line_length=fix.line_length;
	printf("open screen [%d,%d]]memlen[%d]bytes_per_pixel[%d]line_length[%d]\r\n",fb_screen.width,fb_screen.height,fb_screen.memlen,fb_screen.bytes_per_pixel,fb_screen.line_length);
	//memset(pFb->buffer, 0, pFb->memlen);
	fb_screen.openFlagOK=1;
	if(pHardWindow)
	{
		memset(pHardWindow,0x00,sizeof(XuiWindow));
		pHardWindow->width= fb_screen.width;
		pHardWindow->height= fb_screen.height;
		pHardWindow->widget = (A_RGB*)fb_screen.buffer;
	}
	return 1;
}



#define xui_fb_get_addr(x,y) 	(fb_screen.buffer + y * fb_screen.line_length + x * fb_screen.bytes_per_pixel)

/*
static int xui_fb_get(screen_buffer* fb, int x, int y, rgba_t* rgba) {
  char* addr;
  addr.pixel = xui_fb_get_addr(fb, x, y);
  if (fb->bpp == 2) {
    pixel_rgb565_t* p = (pixel_rgb565_t*)addr;
    rgba_t pixel = pixel_rgb565_to_rgba((*p));
    *rgba = pixel;
  } else if (fb->bpp == 4) {
    pixel_bgra8888_t* p = (pixel_bgra8888_t*)addr;
    rgba_t pixel = pixel_bgra8888_to_rgba((*p));
    *rgba = pixel;
  }
  return RET_OK;
}
*/
static int xui_fb_set(int x, int y, const rgba_t* rgba) 
{
	if (fb_screen.bytes_per_pixel == 2) {
		pixel_rgb565_t* p = (pixel_rgb565_t*)xui_fb_get_addr(x, y);
		pixel_rgb565_t pixel = pixel_rgb565_from_rgb(rgba->r, rgba->g, rgba->b);
		*p = pixel;
	} 
	else if (fb_screen.bytes_per_pixel == 4) {
		pixel_bgra8888_t* p = (pixel_bgra8888_t*)xui_fb_get_addr(x, y);
		if(rgba->a == 0xff)
		{
			pixel_bgra8888_t pixel = pixel_bgra8888_from_rgb(rgba->r, rgba->g, rgba->b);
			 *p = pixel;
		}
		else if(rgba->a == 0)
		{
			
		}
		else
		{
			p->r=(rgba->r*rgba->a + p->r*(256-rgba->a))/256;
			p->g=(rgba->g*rgba->a + p->g*(256-rgba->a))/256;
			p->b=(rgba->b*rgba->a + p->b*(256-rgba->a))/256;
		}
	}
	return RET_OK;
}

//=============================画线，起到到终点，支持斜线====================================
int xui_fb_line(int xs, int ys, int xe, int ye, const rgba_t* rgba) 
{
	int w,h,max;
	float xm,ym,wm,hm;
	w = xe-xs;
	h = ye-ys;
	if(w >= 0)
	{
		if(xs >= fb_screen.width) return RET_BAD_PARAMS;
		wm = w;
	}
	else
	{
		if(xe >= fb_screen.width) return RET_BAD_PARAMS;
		wm = -w;
	}
		
	if(h >= 0)
	{
		if(ys >= fb_screen.height) return RET_BAD_PARAMS;
		hm = h;
	}
	else
	{
		if(ye >= fb_screen.height) return RET_BAD_PARAMS;
		hm = -h;
	}
	if(wm<hm) max=wm;
	else max=hm;
	wm = (float)w/max;
	hm = (float)h/max;
	xm	=	xs;
	ym	=	ys;
	//TRACE("xui fb line [%f,%f]-[%d,%d],%d,[%f,%f]\r\n",xm,ym,xe,ye,max,wm,hm);
	while(max--) 
	{
		if(ys<fb_screen.height && xs<fb_screen.width)
			xui_fb_set(xs,ys,rgba);
		xm += wm;
		ym += hm;
		xs = xm;
		ys = ym;
	}
	return RET_OK;
}



typedef struct{
	signed short x,y;
}area_pixel;


typedef union {
	unsigned long uXY;
	area_pixel pix;
}pixel_Data;



int xui_fb_circle(signed short x, signed short y, signed short r, const rgba_t* rgba) 
{
	double sci,scs;
	int jdm;
	pixel_Data curr,old;
	signed short xr,yr,xMax,yMax;

	xMax=fb_screen.width;
	yMax=fb_screen.height;
	
	jdm = r*M_PI;
	scs = M_PI/jdm;
	sci =0;
	//----4段90度弧线组成一个画圆，减少三角函数使用量，提升效率--------------
	jdm += 3;
	jdm >>= 2;
	
	old.uXY=0;	
	while(jdm--)
	{
		curr.pix.x=sin(sci)*r;
		curr.pix.y=cos(sci)*r;
		sci += scs;
		if(old.uXY == curr.uXY) continue; //避免重复写点
		
	#if(0)
		xr=x - curr.pix.x;
		yr=y - curr.pix.y;
		if(xr < xMax && yr < yMax)
			xui_fb_set(fb,xr,yr,rgba);

		xr= x + curr.pix.y;
		yr= y - curr.pix.x;
		if(xr < xMax && yr < yMax)
			xui_fb_set(fb,xr,yr,rgba);

		xr= x + curr.pix.x;
		yr= y + curr.pix.y;
		if(xr < xMax && yr < yMax)
			xui_fb_set(fb,xr,yr,rgba);
		
		xr=x - curr.pix.y;
		yr=y + curr.pix.x;
		if(xr < xMax && yr < yMax)
			xui_fb_set(fb,xr,yr,rgba);
	#else
		yr=y - curr.pix.y;
		if(yr < yMax)
		{
			xr=x - curr.pix.x;
			if( x < xMax)
				xui_fb_set(xr,yr,rgba);
			if(curr.pix.x)
			{
				xr= x + curr.pix.x;
				if( x < xMax)
					xui_fb_set(xr,yr,rgba);
			}
		}
		yr= y + curr.pix.y;
		if(yr < yMax)
		{
		//	xr= x + curr.pix.x;
			if( x < xMax)
				xui_fb_set(xr,yr,rgba);
			if(curr.pix.x)
			{
				xr=x - curr.pix.x;
				if( x < xMax)
					xui_fb_set(xr,yr,rgba);
			}
		}
		xr= x + curr.pix.y;
		if(xr < xMax)
		{
			yr= y - curr.pix.x;
			if( yr < yMax)
				xui_fb_set(xr,yr,rgba);
			if(curr.pix.x)
			{
				yr= y + curr.pix.x;
				if( yr < yMax)
					xui_fb_set(xr,yr,rgba);
			}
		}
		xr=x - curr.pix.y;
		if(xr < xMax)
		{
		//	yr=y + curr.pix.x;
			if( yr < yMax)
				xui_fb_set(xr,yr,rgba);
			if(curr.pix.x)
			{
				yr=y - curr.pix.x;
				if( yr < yMax)
					xui_fb_set(xr,yr,rgba);
			}
		}	
	#endif
		old.uXY = curr.uXY;
//	sleep(1);
	}		
	return RET_OK;
}


int xui_fb_fill_circle(signed short x, signed short y, signed short r,signed short ar, const rgba_t* rgba) 
{
	double sci,scs;
	int jdm;
	pixel_Data curr,old;
	signed short xr,yr,xtwo,yMax;

	yMax=fb_screen.height;
	
	jdm = r*M_PI;
	scs = M_PI/jdm;
	sci =M_PI/2;
	//----4段90度弧线组成一个画圆，减少三角函数使用量，提升效率--------------
	jdm ++;
	jdm >>= 1;
	
	old.pix.y=0xffff;	
	while(jdm--)
	{
		curr.pix.x=sin(sci)*r;
		curr.pix.y=cos(sci)*r;
		sci -= scs;
		if(old.pix.y == curr.pix.y) continue; //避免重复写点

		xr=x - curr.pix.x;
		if(ar <= curr.pix.y)
		{
			yr=y - curr.pix.y;
			if(yr>=0 && yr < yMax)
			{
				if(curr.pix.x)
				{
					xui_fb_hline(xr,yr,curr.pix.x*2,rgba);
				}
				else
				{
					xui_fb_set(xr,yr,rgba);
				}
			}
			yr= y + curr.pix.y;
			if(yr < yMax)
			{
				if(curr.pix.x)
				{
					xui_fb_hline(xr,yr,curr.pix.x*2,rgba);
				}
				else
				{
					xui_fb_set(xr,yr,rgba);
				}
			}
		}
		else
		{
			xtwo=sin( acos(((double)curr.pix.y)/ar) )*ar;
			yr=y - curr.pix.y;
			if(yr>=0 && yr < yMax)
			{
				xui_fb_hline(xr,yr,curr.pix.x-xtwo,rgba);
				xui_fb_hline(x+xtwo+1,yr,curr.pix.x-xtwo,rgba);
			}
			yr= y + curr.pix.y;
			if(yr < yMax)
			{
				xui_fb_hline(xr,yr,curr.pix.x-xtwo,rgba);
				xui_fb_hline(x+xtwo+1,yr,curr.pix.x-xtwo,rgba);
			}
		}
		old.uXY = curr.uXY;
	}		
	return RET_OK;
}



int xui_fb_vline(int x, int y, int h, const rgba_t* rgba) 
{
	int i;
	if(x >= fb_screen.width) 
		return RET_BAD_PARAMS;
	for (i = 0; i < h; i++) 
	{
		if((y+i) >= fb_screen.height) break;
		xui_fb_set(x, y + i, rgba);
	}
	return RET_OK;
}

int xui_fb_hline(int x, int y, int w, const rgba_t* rgba) 
{
	int i;
	pixel_bgra8888_t *pbgra;
	if(y >= fb_screen.height) 
		return RET_BAD_PARAMS;
	pbgra=&fb_screen.bgra8888buff[y*fb_screen.width + x];
	for (i = 0; i < w; i++) 
	{
		if((x+i) >= fb_screen.width) break;
		*(u32*)pbgra = *(u32*)rgba;
		pbgra++;
	//	xui_fb_set(fb, x + i, y, rgba);
	}
	return RET_OK;
}

int xui_fb_fill_rect(int x, int y, int w, int h,const rgba_t* rgba) 
{
	int i,j;
	pixel_bgra8888_t *pbgra;
	w += x;
	h += y;

	if(w > fb_screen.width) w=fb_screen.width;
	if(h > fb_screen.height) h=fb_screen.height;
	for (j = y; j < h; j++) 
	{
		pbgra=&fb_screen.bgra8888buff[j*fb_screen.width + x];
		for(i=x; i<w; i++)
		{
			*(u32*)pbgra = *(u32*)rgba;
			pbgra++;
		}
		//xui_fb_hline(fb, x, y + i, w, rgba);
	}
	return RET_OK;
}

int xui_fb_stroke_rect(int x, int y, int w, int h,const rgba_t* rgba) 
{
	if(x >= fb_screen.width || y >= fb_screen.height) 
		return RET_BAD_PARAMS;
	xui_fb_hline(x, y, w, rgba);
	xui_fb_hline(x, y + h - 1, w, rgba);
	xui_fb_vline(x, y, h, rgba);
	xui_fb_vline(x + w - 1, y, h, rgba);
	return RET_OK;
}


int xui_fb_show_rect(int x, int y, int w, int h,rgba_t* rgba) 
{
	int i,j,imax,jmax;
	pixel_bgra8888_t *pbgra;
	rgba_t	*prgb;
	imax = x+w;
	jmax = y+h;

	if(imax > fb_screen.width) imax=fb_screen.width;
	if(jmax > fb_screen.height) jmax=fb_screen.height;
	for (j = y; j < jmax; j++) 
	{
		pbgra=&fb_screen.bgra8888buff[j*fb_screen.width + x];
		prgb=rgba+(j-y)*w;
		for(i=x; i<imax; i++)
		{
			if(prgb->a == 0xff)
			{
				*(u32*)pbgra = *(u32*)prgb;
			}
			else if(prgb->a)
			{
				uint8_t a,ca;
				a=prgb->a; ca=256-prgb->a;
				pbgra->r=(prgb->r * a + pbgra->r * ca)/256;
				pbgra->g=(prgb->g * a + pbgra->g * ca)/256;
				pbgra->b=(prgb->b * a + pbgra->b * ca)/256;
			}
			pbgra++; prgb++;
		}
	}
	return RET_OK;
}

int xui_fb_rect_push(int x, int y, int w, int h,rgba_t* pInrgb) 
{
	int i,j;
	u32 *destin,*source;
	source=(u32*)pInrgb;
	#ifdef DISPLAY_HORIZONTAL_SCREEN
	destin=(u32*)fb_screen.bgra8888buff;
	w += y;
	h += x;
	for(i=x; i<h; i++)
	{
		j = w;
		while(j-- > y) 
		{
			destin[j*fb_screen.width + i] = *source++;
		}
	}
	#else
	w += x;
	h += y;
	for (j = y; j < h; j++) 
	{
		destin=(u32*)&fb_screen.bgra8888buff[j*fb_screen.width + x];
		for(i=x; i<w; i++)
		{
			*destin++ = *source++;
		}
	}
	#endif
	return RET_OK;
}


int xui_fb_push(XuiWindow *window,RECTL* pRect,A_RGB* pInrgb) 
{
	u16 i,j,x,y,w,h,mWidth;
	u16 fbX,fbY;
	A_RGB *destin,*source;
	fbX = window->left;
	fbY = window->top;
	mWidth = window->width;

	if(pRect)
	{
		x = pRect->left;
		y = pRect->top;
		w =x+ pRect->width;
		h =y+ pRect->height;
	}
	else
	{
		x = 0;
		y = 0;
		w =x+ window->width;
		h =y+ window->height;
	}
	
	#ifdef DISPLAY_HORIZONTAL_SCREEN
	destin=fb_screen.pARGB;
	j = h;
	while((j--) > y) 
	{
		source=&pInrgb[(h-j)*mWidth + x];
		for(i=x; i<w; i++)
		{
			destin[(fbY+i)*fb_screen.width + (fbX+j)]  = *source++;
		}
	}
	#else	//-----------------------------------------------------------
	for (j = y; j < h; j++) 
	{
		destin=&fb_screen.pARGB[(fbY+j)*fb_screen.width + (fbX+x)];
		source=&pInrgb[j*mWidth + x];
		for(i=x; i<w; i++)
		{
			*destin++ = *source++;
		}
	}
	#endif
	return RET_OK;
}



