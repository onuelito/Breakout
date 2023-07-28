#include <stdio.h>

#include "cgl/cloading.h"
#include "cgl/cwindow.h"
#include "cgl/keymap.h"

#include "graphics.h"
#include "physics.h" 

/*
	TODO : Paddle creation (DONE!)
	TODO : Ball movement and creation (DONE!)
	TODO : Colision Management
	TODO : Victory/Lose/Title images
	TODO : Add Sound (Possibly)
	TODO : Set max and min width

	TODO : Youtube Video
	DEADLINE : August the 1st 2023

	NEXT : Music Player remade for Linux (learning PulseAudio)
*/

typedef struct
{
	float velocity[2];
	float speed;
	Rectangle body;
} PhysicBody;

static PhysicBody Player;
static PhysicBody Ball;



static void init()
{
	//Creating Player//
	Player.speed = 300;
	Player.velocity[0] = 0;
	Player.velocity[1] = 0;
	Player.body = create_rect(300, 20, 120, 15);

	Ball.speed = 250.0;
	Ball.body = create_rect(300, 300, 15, 15);
	Ball.body.velocity[0] = 0;
	Ball.body.velocity[1] = -50;

	create_paddles();
}

static void render()
{
	//Drawing related function//
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_rect(Player.body);
	draw_rect(Ball.body);
	draw_paddles();
}

static void update(double dt)
{
	//Physics related function//
	


	if (Ball.body.y < -8) Ball.body.y = 300;

	move_player(&Player.body, Player.velocity[0]*dt, 0);
	//move_rect(&Ball.body, (Ball.velocity[0]*dt), (Ball.velocity[1]*dt));
	move_ball(&Ball.body, &Player.body);

}

static void keyboard(XEvent ev)
{
	if (ev.type == KeyPress)
	{
		if(ev.xkey.keycode == KEY_A)
			Player.velocity[0] = -Player.speed;
		else
			if(ev.xkey.keycode == KEY_D)
			Player.velocity[0] = Player.speed;
	}
	
	if (ev.type == KeyRelease)
	{
		if (ev.xkey.keycode == KEY_A && Player.velocity[0] < 0)
			Player.velocity[0] = 0;
		if (ev.xkey.keycode == KEY_D && Player.velocity[0] > 0)
			Player.velocity[0] = 0;
	}
}

static void shutdown()
{
	destroy_rect(Player.body);
	printf("Thank you for playing!\n");
}


int main(void)
{
	CGLWindow game;
	game.init_cb = init;
	game.render_cb = render;
	game.update_cb = update;
	game.keyboard_cb = keyboard;
	game.shutdown_cb = shutdown;

	pset_set_cglwindow(&game); //Initialize physics
	cgl_run(&game, 600, 600);

	return 0;
}
