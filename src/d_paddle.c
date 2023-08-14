#include "d_paddle.h"



paddle_t D_CreatePaddle(float x, float y, float w, float h, color_t color)
{
	paddle_t c_paddle;

	c_paddle.x = x;
	c_paddle.y = y;

	c_paddle.width  = w;
	c_paddle.height = h;
	c_paddle.color = color;

	return c_paddle;
}

