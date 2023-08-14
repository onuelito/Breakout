//
// Graphics management script using
// X11

#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/keysym.h> //Deprecated
#include <X11/XKBlib.h>

#include "d_main.h"
#include "d_event.h"
#include "breakdef.h"

#include "i_video.h"
#include "i_system.h"



//Window Variables
Display*		X_display;
Window			X_mainWindow;
XEvent			X_event;
Colormap 		X_cmap;
XVisualInfo*	X_visualinfo;
int				X_screen;
GLXContext		X_context;
int				X_width=600;
int				X_height=600;

void UpdateViewport(void);

int xlatekey(void)
{
	int rc;

	switch(rc = XkbKeycodeToKeysym(X_display, X_event.xkey.keycode, 0, 0))
	{
		case XK_Right:
			rc = KEY_RIGHTARROW;
			break;
		case XK_Left:
			rc = KEY_LEFTARROW;
			break;
		default:
			break;
	}

	return rc;
}

void I_GetEvent(void)
{
	event_t event;

	XNextEvent(X_display, &X_event);
	switch(X_event.type)
	{
		case KeyPress:
			event.type = ev_keydown;
			event.data = xlatekey();
			D_PostEvent(&event);
			break;
		case KeyRelease:
			event.type = ev_keyup;
			event.data = xlatekey();
			D_PostEvent(&event);
			break;
		case Expose:
			UpdateViewport();
			break;
	}
}

void I_UpdateEvent(void)
{
	while(XPending(X_display))
		I_GetEvent();
	glXSwapBuffers(X_display, X_mainWindow);
}

void I_ShutdownGraphics(void)
{

}


void I_InitGraphics(void)
{
	//Create window and graphic it's context

	int						x=0;
	int						y=0;
	char*					name="Breakout";

	XSetWindowAttributes 	attribs;
	unsigned long 			attrib_mask;

	X_display = XOpenDisplay(NULL);
	if(!X_display)
		I_Error("Could not open display");

	X_screen = DefaultScreen(X_display);

	GLint glxattr[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE,		24,
		GLX_RED_SIZE,		8,
		GLX_GREEN_SIZE,		8,
		GLX_BLUE_SIZE,		8,
		None};

	X_visualinfo = glXChooseVisual(X_display, X_screen, glxattr);
	if (!X_visualinfo)
		I_Error("Could not create visual info");

	X_cmap = XCreateColormap(X_display, RootWindow(X_display, X_screen), X_visualinfo->visual, AllocNone);
	if(!X_cmap)
		I_Error("Could not create colormap");

	//Setting Window attributes
	attrib_mask = CWEventMask | CWColormap | CWBorderPixel;
	attribs.colormap = X_cmap;
	attribs.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask;

	//Creating window
	X_mainWindow = XCreateWindow(	X_display, 
									RootWindow(X_display, X_screen), 
									x, 
									y, 
									X_width, 
									X_height,
									0,	//Border width
									X_visualinfo->depth,
									InputOutput,
									X_visualinfo->visual, 
									attrib_mask, 
									&attribs );



	XStoreName(X_display, X_mainWindow, name);
	XClearWindow(X_display, X_mainWindow);
	XMapRaised(X_display, X_mainWindow);

	//Creating graphic context
	X_context  = glXCreateContext(X_display, X_visualinfo, NULL, GL_TRUE);
	if(!X_context)
		I_Error("Could not create graphic context");
	glXMakeCurrent(X_display, X_mainWindow, X_context);
}

void UpdateViewport(void)
{
	glViewport(0, 0, X_width, X_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, X_width, 0, X_height, -1, 1);

}
