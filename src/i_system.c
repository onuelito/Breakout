#include <stdio.h>
#include <stdlib.h>

#include "d_main.h"

#include "i_video.h"
#include "i_system.h"

void I_Quit(void)
{
	D_ExitLoop();
}

void I_Error(char *error)
{
	printf("Error: %s\n", error);

	I_ShutdownGraphics();

	exit(0);
}
