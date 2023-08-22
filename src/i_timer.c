#include <sys/time.h>
#include <sys/select.h>

#include "i_timer.h"

#define FPS 60
#define SKIP_TICKS (1000 / FPS)

static struct timeval sclock;
static struct timeval clock;

//Time values
double nextTick;
double currentTime;
double previousTime;

double deltaTime;
double sleepTime;

double I_GetTimeMS(void)
{
	struct timeval c_time; //Current Time
	gettimeofday(&c_time, 0);

	double m_time = c_time.tv_sec * 1000; //Time with milliseconds
	m_time += c_time.tv_usec / 1000.0; //Adding milliseconds
	
	return m_time;
}

void I_InitTimer(void)
{
	sclock.tv_sec 	= 0;
	sclock.tv_usec 	= 0;

	clock.tv_sec	= 0;
	clock.tv_usec	= 0;

	previousTime 	= I_GetTimeMS();
	currentTime 	= I_GetTimeMS();
	nextTick 		= I_GetTimeMS();
}

void I_UpdateTicks(void)
{
	//Calculate sleep time based on FPS
	currentTime = I_GetTimeMS();
	deltaTime 	= (currentTime - previousTime) * 0.001;
	previousTime= currentTime;

	gettimeofday(&clock, 0);

	nextTick 	= currentTime + SKIP_TICKS;
	sleepTime 	= nextTick - ((clock.tv_sec * 1000) + (clock.tv_usec / 1000));

	sclock.tv_usec = sleepTime * 1000;
	//select(0, 0, 0, 0, &sclock);
}
