//#include <stdio.h>
#include <stdbool.h>

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

#include "breakdef.h"

#include "d_paddle.h"

#include "g_game.h"

#include "i_video.h"

#include "i_timer.h"

#include "i_physic.h"

#define NUMKEYS 256

bool gamekeydown[NUMKEYS];

bool paused;

int playerspeed = 500;
paddle_t player;

paddle_t ball;
float ballvelocity[2];

int paddledimensions[2] = {3, 5};
int paddleactive[15];
paddle_t paddles[15];


void G_Init(void)
{
	float width	 = 110;
	float height = 15;

	color_t color = {.r=1, .g=1, .b=1};
	player = D_CreatePaddle( (float)X_width/2 - width/2, 20, width, height, color);


	for (int i=0; i < paddledimensions[0]; i++)
	{
		for (int j=0; j < paddledimensions[1]; j++)
		{
			float x = (X_width/paddledimensions[1] * j) + 5;
			float y = (float)X_height - 40 * (i+1);

			int index = j+i*paddledimensions[1];
			paddles[index] = D_CreatePaddle(x, y, width, height, color);

			paddleactive[index] = 1;
		}
	}

	ball = D_CreatePaddle( (float)X_width/2 - height/2, (float)X_height/2, height, height, color);

	ballvelocity[0] = -30;
	ballvelocity[1] = -250;
}


//
// Responsds to Game Inputs
//

void G_Responder(event_t *ev)
{

	switch(ev->type)
	{
		case ev_keydown:
			if(ev->data == KEY_PAUSE)
			{
				paused = !paused;
                return;
			}
			if(ev->data < NUMKEYS)
				gamekeydown[ev->data] = true;
            return;
		case ev_keyup:
			if(ev->data < NUMKEYS)
				gamekeydown[ev->data] = false;
            return;

		default:
			break;
	}

	//printf("%d\n", ev->data);
    return false;
}

void G_CheckInputs(void)
{
	if(paused)
		return;

	if(gamekeydown[KEY_RIGHTARROW] || gamekeydown[KEY_D])
		player.x = MIN(player.x + playerspeed * deltaTime, X_width - player.width);

	if(gamekeydown[KEY_LEFTARROW] || gamekeydown[KEY_A])
		player.x = MAX(player.x + playerspeed * -deltaTime, 0);

	I_CheckCollisions(); // Ball Collisions

	ball.x += ballvelocity[0] * deltaTime;
	ball.y += ballvelocity[1] * deltaTime;
}
