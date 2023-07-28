#include "cloading.h"
#include "ctimer.h"
#include "cwindow.h"

#include <assert.h>
#include <stdio.h>

//Shared elements of windows
static struct
{
	Display *dpy;
	int screenId;

	XEvent ev;
	XVisualInfo *vi;
	XSetWindowAttributes swa;
	GLXContext glc;

	CGLTimer timer;
} CGL;

int create_cgl_context(CGLWindow* cw)
{
	if( (CGL.glc = glXCreateContext(CGL.dpy, CGL.vi, NULL, GL_TRUE)) == NULL )
		return 0;

	cgl_load_opengl_funcs();
	glXMakeCurrent(CGL.dpy, cw->win, CGL.glc);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return 1;
}

int init_cgl(CGLWindow* cw, int width, int height)
{
	//Initialize by creating the window//
	
	if ( (CGL.dpy = XOpenDisplay(NULL)) == NULL ) return 0;

	CGL.screenId = DefaultScreen(CGL.dpy);
	GLint glXAttribs[] = {
			GLX_RGBA,
			GLX_DOUBLEBUFFER,
			GLX_DEPTH_SIZE,		24,
			GLX_RED_SIZE,		8,
			GLX_GREEN_SIZE,		8,
			GLX_BLUE_SIZE,		8,
			None
	};

	if ( (CGL.vi = glXChooseVisual(CGL.dpy, CGL.screenId, glXAttribs)) == NULL ) return 0;

	Colormap cmap = XCreateColormap(CGL.dpy, RootWindow(CGL.dpy, CGL.screenId), CGL.vi->visual, AllocNone);
	if (cmap == 0) return 0;

	CGL.swa.colormap = cmap;
	CGL.swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;

	cw->win = XCreateWindow(CGL.dpy, RootWindow(CGL.dpy, CGL.screenId), 0, 0, width, height, 0, CGL.vi->depth, InputOutput, 
						CGL.vi->visual, CWColormap | CWEventMask, &CGL.swa);

	//XAutoRepeatOff(CGL.dpy);
	XStoreName(CGL.dpy, cw->win, "BreakOut");
	XClearWindow(CGL.dpy, cw->win);
	XMapRaised(CGL.dpy, cw->win);

	if (!create_cgl_context(cw)) return 0;

	cw->width 	= width;
	cw->height 	= height;

	return 1;
}

void cgl_run(CGLWindow* cw, int width, int height)
{
	assert(init_cgl(cw, width, height) && "Could not initialize CGL");
	init_timer(&CGL.timer); //Getting FPS ready

	Atom atomDeleteWindow = XInternAtom(CGL.dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(CGL.dpy, cw->win, &atomDeleteWindow, True);

	int running = 1;
	cw->init_cb();

	while (running)
	{
		if(XPending(CGL.dpy))
		{
			XNextEvent(CGL.dpy, &CGL.ev);


			switch (CGL.ev.type)
			{
				case KeyPress:
					cw->keyboard_cb(CGL.ev);
					break;
				case KeyRelease:
					if (XEventsQueued(CGL.dpy, QueuedAfterReading))
					{
						XEvent nev;
						XPeekEvent(CGL.dpy, &nev);

						if(nev.type == KeyPress && nev.xkey.time ==  CGL.ev.xkey.time &&
							nev.xkey.keycode == CGL.ev.xkey.keycode)
							XNextEvent(CGL.dpy, &CGL.ev); //Flushing current press event
						else
							cw->keyboard_cb(CGL.ev);
					}
					else
						cw->keyboard_cb(CGL.ev);
					break;

				case ClientMessage:
					if(CGL.ev.xclient.data.l[0] == atomDeleteWindow)
						running = 0;
					break;
			}
		}

		cw->update_cb(CGL.timer.deltaTime);
		cw->render_cb();

		glXSwapBuffers(CGL.dpy, cw->win);
		update_timer(&CGL.timer);

	}
	
	cw->shutdown_cb();

	glXDestroyContext(CGL.dpy, CGL.glc);
	XFree(CGL.vi);
	XFreeColormap(CGL.dpy, CGL.swa.colormap);
	XDestroyWindow(CGL.dpy, cw->win);
	XCloseDisplay(CGL.dpy);
}

