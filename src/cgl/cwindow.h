#ifndef _CWINDOW_H_
#define _CWINDOW_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef void (*InitCb)(void);
typedef void (*RenderCb)(void);
typedef void (*UpdateCb)(double);
typedef void (*ShutdownCb)(void);
typedef void (*KeyboardCb)(XEvent ev);

typedef struct
{
	Window win;

	int width;
	int height;

	//Core Functions//
	InitCb init_cb;
	RenderCb render_cb;
	UpdateCb update_cb;
	KeyboardCb keyboard_cb;
	ShutdownCb shutdown_cb;
} CGLWindow;

void cgl_run(CGLWindow*, int width, int height);

#endif //_CWINDOW_H_
