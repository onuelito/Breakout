//
// Timer to delay update
//

#ifndef __I_TIMER__
#define __I_TIMER__

void I_InitTimer(void);

void I_UpdateTicks(void);

double I_GetTimeMS(void);

extern double deltaTime;

#endif
