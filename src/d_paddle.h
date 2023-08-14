#ifndef __D_PADDLE__
#define __D_PADDLE__

//
// Defining Paddle for rendering and
// collision
//


typedef struct
{
	float r;
	float g;
	float b;
} color_t;

typedef struct
{
	float x;
	float y;

	float width;
	float height;

	color_t color;
} paddle_t;


extern paddle_t player;

extern paddle_t ball;
extern float ballvelocity[2];

extern int paddledimensions[2];
extern paddle_t paddles[15];
extern int paddleactive[15];

paddle_t D_CreatePaddle(float x, float y, float w, float h, color_t color);

#endif
