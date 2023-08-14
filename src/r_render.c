#include <GL/gl.h>

#include "d_paddle.h"

#include "g_game.h"

#include "r_render.h"


void ClearScene(void);
void R_RenderPaddle(paddle_t pad);

void R_RenderScene(void)
{
	ClearScene();

	for (int i=0; i < paddledimensions[0] * paddledimensions[1]; i++)
		if (paddleactive[i])
			R_RenderPaddle(paddles[i]);

	R_RenderPaddle(player);
	R_RenderPaddle(ball);
}

void R_RenderPaddle(paddle_t pad)
{
	glBegin(GL_QUADS);
		glVertex3f(pad.x, pad.y, 0.0);		
		glColor3f(pad.color.r, pad.color.g, pad.color.b);

		glVertex3f(pad.x + pad.width, pad.y, 0.0);
		glColor3f(pad.color.r, pad.color.g, pad.color.b);
		
		glVertex3f(pad.x + pad.width, pad.y + pad.height, 0.0);
		glColor3f(pad.color.r, pad.color.g, pad.color.b);
		
		glVertex3f(pad.x, pad.y + pad.height, 0.0);
		glColor3f(pad.color.r, pad.color.g, pad.color.b);
	glEnd();
}

void ClearScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
