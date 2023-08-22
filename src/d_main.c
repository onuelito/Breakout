#include <stdio.h>
#include <stdbool.h>
#include <GL/gl.h>

#include "d_main.h"

#include "g_game.h"

#include "i_timer.h"
#include "i_sound.h"
#include "i_video.h"

#include "r_render.h"

event_t events[MAXEVENTS];
int		eventhead;
int		eventtail;

bool	running=true;

void D_BreakoutLoop(void);

//
// Add event to the events list
//

void D_PostEvent(event_t* ev)
{
	events[eventhead] = *ev;
	eventhead = (++eventhead)&(MAXEVENTS-1);
}

void D_ProcessEvents(void)
{
	event_t* ev;

	for( ; eventtail != eventhead; eventtail = (++eventtail)&(MAXEVENTS-1) )
	{
		ev = &events[eventtail];

		G_Responder(ev);
	}
}

void D_ExitLoop(void)
{
	running = false;
}

void D_Display(void)
{
	R_RenderScene();
}


void D_BreakoutMain(void)
{
	I_InitTimer();
	I_InitGraphics(); // Initialize X11 Window

	G_Init(); // Initialize game


    // Testing audio
    audio_t bitstones = I_CreateAudio("./res/sound/bitstones.ogg");
    I_PostAudio(&bitstones);


	D_BreakoutLoop();
}


void D_BreakoutLoop(void)
{
	while(1)
	{
		I_UpdateEvent();
		I_UpdateTicks();

		D_ProcessEvents();
        I_ProcessAudios();
		G_CheckInputs();
		D_Display();

		if(!running) break;
	}

	I_ShutdownGraphics();
}
