#ifndef TELLER_H
#define TELLER_H

#include "includes.h"

typedef struct teller teller;
struct teller
{
	u32 longest_wait; // Record "high score" for wait time
	u32 busy_time; // If non-zero, teller is working with customer.
	u32 wait_time; // Counts up from zero when teller is free.
	u32 break_delay; // Countdown to take a break
	u32 break_time; // On a break
	u32 total_wait; // Total amount of time spent waiting
	
};

void *teller_thread(void *args);
void teller_init(teller *t);
#endif
