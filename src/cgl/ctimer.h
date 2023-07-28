#ifndef _CTIMER_H_
#define _CTIMER_H_

//Struct used to calculate fps

typedef struct
{
	double prevTime;
	double currTime;
	double deltaTime;

	long nextTick;
	long sleepTime;
} CGLTimer;

void init_timer(CGLTimer*);
void update_timer(CGLTimer*);

#endif //_CTIMER_H_
