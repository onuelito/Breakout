#include "ctimer.h"
#include <sys/time.h>
#include <sys/select.h>

#define FPS 60
#define SKIP_TICKS (1000 / FPS)

static struct timeval sclock; //Sleep Clock
static struct timeval clock;

double GetMilliseconds()
{
	//Getting the current time
	struct timeval c_time;
	gettimeofday(&c_time, 0);

	double m_time = c_time.tv_sec * 1000; //Milliseconds time
	m_time += c_time.tv_usec / 1000.0;	//adding milliseconds
	

	return m_time;
}

void init_timer(CGLTimer* timer)
{
	sclock.tv_sec 	= 0; 
	sclock.tv_usec 	= 0;

	clock.tv_sec	= 0;
	clock.tv_usec	= 0;

	timer->prevTime = GetMilliseconds();
	timer->currTime = GetMilliseconds();
	timer->nextTick = GetMilliseconds();
}


void update_timer(CGLTimer* timer)
{
	//Calculating the elapsed time
	timer->currTime = GetMilliseconds();
	timer->deltaTime = (double)(timer->currTime - timer->prevTime) * 0.001;
	timer->prevTime = timer->currTime;

	gettimeofday(&clock, 0);

	timer->nextTick = timer->currTime + SKIP_TICKS;
	timer->sleepTime = timer->nextTick - ((clock.tv_sec * 1000) + (clock.tv_usec / 1000));

	sclock.tv_usec = timer->sleepTime * 1000;
	select(0, 0, 0, 0, &sclock);
}
