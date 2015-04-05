#include "util.h"

u32 rand_range(u32 low, u32 high)
{
	return low + ((rand()% (high - low)));
}

void print_time(u32 t)
{
	u32 seconds = t % 60;
	u32 minutes = (t / TIME_MINUTE) % 60;
	u32 hours = (t / TIME_HOUR);
	u32 is_pm = 0;
	if (hours > 12)
	{
		hours -= 12;
		is_pm = 1;
	}

	printf("%02d:%02d:%02d ",hours,minutes,seconds);
	if (is_pm)
	{
		printf("P.M.");
	}
	else
	{
		printf("A.M.");
	}
}
