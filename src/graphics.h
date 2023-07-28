#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

typedef struct
{
	int width;
	int height;

	GLuint program;
	GLuint VAO;
	
	GLuint VBO[3];
	GLuint CBO[3];
	GLuint TBO[3];

	int _links[6]; //Indicated how the EBO behaves

	float vdat[12]; //Vertex data
	float cdat[12]; //Color data
	float tdat[12]; //Translation data
	
	float velocity[2];

	float x;
	float y;

} Rectangle;

Rectangle create_rect(float x, float y, int width, int height);
void create_paddles();
void destroy_rect(Rectangle);

void move_rect(Rectangle *rect, float dx, float dy);
void move_player(Rectangle *rect, float dx, float dy);
void move_ball(Rectangle *ball, Rectangle *player);

void draw_rect(Rectangle rect);
void draw_paddles();

#endif //_GRAPHICS_H_
