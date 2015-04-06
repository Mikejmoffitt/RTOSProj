#include "txtcolor.h"
#include "stdio.h"
// Thanks to http://linuxgazette.net/issue65/padala.html for this 

void txtcolor(int attr, int fg, int bg)
{
	if (attr != COL_RESET)
	{
		txtcolor(COL_RESET,fg,bg);
	}
	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg+30, bg+40);
	printf("%s",command);
}
