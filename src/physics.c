#include "cgl/cwindow.h"
#include "physics.h"

#include <stdio.h>

#define swap_array(arrayA, arrayB, size) for (int i = 0; i < size; i++) arrayA[i] = arrayB[i];

static CGLWindow *currWin;

int get_win_width()
{
	return currWin->width;
}

int get_win_height()
{
	return currWin->height;
}

void pset_set_cglwindow(void* cglwindow)
{
	currWin = (CGLWindow*)cglwindow;
}

void pset_rect_color(float data[12])
{
	float color[12] = {1.0, 1.0, 1.0,
					   1.0, 1.0, 1.0,
					   1.0, 1.0, 1.0,
					   1.0, 1.0, 1.0};
	swap_array(data, color, 12);
}

void pset_rect_size(float data[12], int dwidth, int dheight)
{
	float width 	= dwidth/2.0/(float)currWin->width;
	float height 	= dheight/2.0/(float)currWin->height;

	float vertices[12] = {
			-width, -height, 1.0,
			 width, -height, 1.0,
			 width,  height, 1.0,
			-width,  height, 1.0};

	swap_array(data, vertices, 12);
}

void pset_rect_position(float data[12], float rx, float ry)
{
	float pos[3]	= {rx/(float)currWin->width, ry/(float)currWin->height, 0.0};
	for (int i=0; i <  12;  i++) data[i] = pos[i%3];
}

float pset_player_position(float data[12], float width, float rx, float ry)
{
	float minX = width/2.0;
	float maxX = currWin->width - minX;
	float RX = MIN(MAX(minX, rx), maxX);

	float x = RX/(float)currWin->width;
	float y = ry/(float)currWin->height;

	float pos[3] = {x, y, 0.0};
	for (int i=0; i <  12;  i++) data[i] = pos[i%3];

	return RX;
}
