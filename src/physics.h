#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b

int get_win_width();
int get_win_height();

void pset_set_cglwindow(void* cglwindow);
void pset_rect_color(float color[12]);
void pset_rect_size(float vertices[12], int width, int height);
void pset_rect_position(float data[12], float rx, float ry);
float pset_player_position(float data[12], float width, float rx, float ry);

#endif //_PHYSICS_H_
