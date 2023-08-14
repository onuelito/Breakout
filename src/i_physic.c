#include <stdlib.h>
#include <stdbool.h>

#include "d_paddle.h"

#include "i_video.h"

#include "i_timer.h"

#include "i_physic.h"

float* LinesPoint(float lineA[4], float lineB[4])
{
	float *point = NULL;
	
	float d = ((lineA[0] - lineA[2]) * (lineB[1] - lineB[3])) - ((lineA[1] - lineA[3]) * (lineB[0] - lineB[2]));
	if( d == 0 ) return point;

	float t = ((lineA[0] - lineB[0]) * (lineB[1] - lineB[3])) - ((lineA[1] - lineB[1]) * (lineB[0] - lineB[2]));
	float u = ((lineA[0] - lineB[0]) * (lineA[1] - lineA[3])) - ((lineA[1] - lineB[1]) * (lineA[0] - lineA[2]));

	t /= d;
	u /= d;

	//printf("%f %f\n", u, t);

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		point = (float*) malloc(sizeof(float)*2);
		point[0] = lineA[0] + t * (lineA[2] - lineA[0]);
		point[1] = lineA[1] + t * (lineA[3] - lineA[1]);
	}

	return point;
}


float* Raycast(paddle_t target)
{
	
	float pointT[8] = {
		target.x, target.y,
		target.x, target.y + target.height,
		target.x + target.width, target.y + target.height,
		target.x + target.width, target.y};

	float pointB[8] = {
		ball.x, ball.y,
		ball.x, ball.y + ball.height,
		ball.x + ball.width, ball.y +ball.height,
		ball.x + ball.width, ball.y};

	
	float lineT[4] = {pointT[2], pointT[3], pointT[4], pointT[5]}; 	// Target Line Raycast
	float direc[4] = {ballvelocity[0] * deltaTime, ballvelocity[1] * deltaTime};

	float rayA[4] = {pointB[0], pointB[1], pointB[0] + direc[0], pointB[1] + direc[1]};
	float rayB[4] = {pointB[6], pointB[7], pointB[6] + direc[0], pointB[7] + direc[1]};

	float offset[2] = {ball.width, 0};

	if(ballvelocity[1] > 0)
	{
		if(pointB[1] + ballvelocity[1] < pointT[1])
			return NULL;

		// Changing line check for bottom line
		lineT[0] = pointT[0];	lineT[1] = pointT[1];
		lineT[2] = pointT[6];	lineT[3] = pointT[7];


		// Changing raycast vertices for top line
		rayA[0] = pointB[2];
		rayA[1] = pointB[3];
		rayA[2] = pointB[2] + direc[0];
		rayA[3] = pointB[3] + direc[1];

		rayB[0] = pointB[4];
		rayB[1] = pointB[5];
		rayB[2] = pointB[4] + direc[0];
		rayB[3] = pointB[5] + direc[1];

		offset[1] = -ball.height;
	}

	// Localizing the ray collision points
	float *pointR1 = LinesPoint(rayA, lineT);

	if (pointR1)
	{
		pointR1[1] += offset[1];
		return pointR1;
	}

	float *pointR2 = LinesPoint(rayB, lineT);

	if (pointR2)
	{
		pointR2[0] -= offset[0];
		pointR2[1] += offset[1];
		return pointR2;
	}

	return NULL;
}

void I_CheckCollisions(void)
{
	int modifier = 0;
	float *point = NULL;

	// Screen Borders Checking
	if (ball.x < 0) 
	{
		ball.x = 0;
		ballvelocity[0] *= -1;
	} else if (ball.x + ball.width > X_width)
	{
		ball.x = X_width - ball.width;
		ballvelocity[0] *= -1;
	}
		
	if( ballvelocity[1] < 0 && ball.y > player.y + player.height)
	{
		point = Raycast(player);

		if (point)
		{
			ball.x = point[0];
			ball.y = point[1];
			ballvelocity[1] *= -1;
			free(point);
		}
	}
	else if(ballvelocity[1] > 0)
	{
		for(int i = 10; i < paddledimensions[0] * paddledimensions[1]; ++i)
		{
			modifier = 0;

			if(!paddleactive[i])
			{
				modifier = paddledimensions[1];

				if(!paddleactive[i-modifier])
					modifier = paddledimensions[1]*2;
				
				if(!paddleactive[i-modifier])
					continue;
			}

			if( (point = Raycast(paddles[i-modifier])) != NULL)
			{
				paddleactive[i-modifier] = 0;
				break;
			}
		}

		if(point)
		{
			ball.x = point[0];
			ball.y = point[1];
			ballvelocity[1] *= -1;
			
			free(point);
		}
	}


	if(ball.y <-50 || ball.y > X_height + ball.height)
	{
		if (ballvelocity[1] > 0) ballvelocity[1] *= -1;
		ball.x = X_width/2.0 - ball.width/2.0;
		ball.y = X_height/2.0 - ball.height/2.0;
	}
}
