#ifndef __D_EVENT_H__
#define __D_EVENT_H__

//
//
//

typedef enum
{
	ev_keydown,
	ev_keyup,
} ev_type;

typedef struct
{
	ev_type type;
	int data;
} event_t;

#define MAXEVENTS 64

extern event_t events[MAXEVENTS];
extern int eventhead;
extern int eventtail;

#endif
