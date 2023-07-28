#include "cgl/cloading.h"
#include "cgl/cshader.h"

#include "graphics.h"
#include "physics.h"

#include <stdio.h>

int pcount = 15;
int prows = 3;
int pcols = 5;
Rectangle Paddles[15];

void InitRectBuffer(Rectangle* rect, GLuint DBO[], float ddata[], size_t dsize)
{
	glBindBuffer(GL_ARRAY_BUFFER, DBO[1]);
	glBufferData(GL_ARRAY_BUFFER, dsize, ddata, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect->_links), rect->_links, GL_STATIC_DRAW);

	glVertexAttribPointer(DBO[0], 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(DBO[0]);
}

Rectangle create_rect(float x, float y, int width, int height)
{
	Rectangle rect;

	rect.x = x;
	rect.y = y;
	rect.width 	= width;
	rect.height = height;

	rect._links[0] = 0; rect._links[1] = 1; rect._links[2] = 2;
	rect._links[3] = 0; rect._links[4] = 2; rect._links[5] = 3;

	pset_rect_size(rect.vdat, width, height);	
	pset_rect_position(rect.tdat, x, y);
	pset_rect_color(rect.cdat);

	//Creating the shader program and assigning locations
	rect.program = create_program("./shdr/rect.vert", "./shdr/rect.frag");
	rect.VBO[0] = glGetAttribLocation(rect.program, "position");
	rect.CBO[0] = glGetAttribLocation(rect.program, "colorIn");
	rect.TBO[0] = glGetAttribLocation(rect.program, "translate");

	//Creating Vertex and Element Buffers//
	glGenVertexArrays(1, &rect.VAO);
	glGenBuffers(1, &rect.VBO[1]); glGenBuffers(1, &rect.VBO[2]);
	glGenBuffers(1, &rect.CBO[1]); glGenBuffers(1, &rect.CBO[2]);
	glGenBuffers(1, &rect.TBO[1]); glGenBuffers(1, &rect.TBO[2]);

	glBindVertexArray(rect.VAO);
	InitRectBuffer(&rect, rect.VBO, rect.vdat, sizeof(rect.vdat));
	InitRectBuffer(&rect, rect.CBO, rect.cdat, sizeof(rect.cdat));
	InitRectBuffer(&rect, rect.TBO, rect.tdat, sizeof(rect.tdat));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return rect;
}


void create_paddles()
{
	int width 	= 110;
	int height 	= 15;

	int wWidth	= get_win_width();
	int wHeight	= get_win_height();

	for (int i=0; i < prows; i++)
	{
		for (int j=0; j < pcols; j++)
		{
			float x = (wWidth/pcols * j) + 5 + width/2.0;
			float y = (float)wHeight - 40 * (i+1);
			Paddles[j+i*pcols] = create_rect(x, y, width, height);
		}
	}

}

void move_rect(Rectangle *rect, float dx, float dy)
{
	rect->x += dx;
	rect->y += dy;
	pset_rect_position(rect->tdat, rect->x, rect->y);
	glBindVertexArray(rect->VAO);
	InitRectBuffer(rect, rect->TBO, rect->tdat, sizeof(rect->tdat));

}

void move_player(Rectangle *rect, float dx, float dy)
{
	rect->x += dx;
	rect->y += dy;
	rect->x = pset_player_position(rect->tdat, rect->width, rect->x, rect->y);
	glBindVertexArray(rect->VAO);
	InitRectBuffer(rect, rect->TBO, rect->tdat, sizeof(rect->tdat));
}

int collide_rect(Rectangle *bodyA, Rectangle *bodyB)
{
	if (bodyA->vdat[0] + bodyA->tdat[0] > bodyB->vdat[3] + bodyB->tdat[0] ||
		bodyA->vdat[3] + bodyA->tdat[0] < bodyB->vdat[0] + bodyB->tdat[0] ||
		
		bodyA->vdat[1] + bodyA->tdat[1] > bodyB->vdat[7] + bodyB->tdat[1] ||
		bodyA->vdat[7] + bodyA->tdat[1] < bodyB->vdat[1] + bodyB->tdat[1])
		return 0;


	return 1;
}

void move_ball(Rectangle *ball, Rectangle *player)
{
	ball->x += ball->velocity[0];
	ball->y += ball->velocity[1];
	pset_rect_position(ball->tdat, ball->x, ball->y);
	glBindVertexArray(ball->VAO);
	InitRectBuffer(ball, ball->TBO, ball->tdat, sizeof(ball->tdat));

	//printf("Collision: %d\n", collide_rect(ball, player));
	
	if( collide_rect(ball, player))
		ball->velocity[1] *= -1;
}

void draw_rect(Rectangle rect)
{
	glUseProgram(rect.program);
	glBindVertexArray(rect.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void draw_paddles()
{
	for (int i=0; i < pcount; i++)
	{

		draw_rect(Paddles[i]);
	}
}


void destroy_rect(Rectangle rect)
{
	glDeleteBuffers(1, &rect.VBO[1]);
	glDeleteBuffers(1, &rect.CBO[1]);
	glDeleteBuffers(1, &rect.TBO[1]);
	glDeleteProgram(rect.program);
}
