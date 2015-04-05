#ifndef GLOBALS_H
#define GLOBALS_H

#define TIME_SECOND 1
#define TIME_MINUTE 60
#define TIME_HOUR (TIME_MINUTE * 60)

// Open at 9:00 AM, close at 4:00 PM
#define BANK_OPENING 9 * TIME_HOUR
#define BANK_CLOSING (TIME_HOUR * 16)

#define TIME_DELAY 1667
//#define TIME_DLEAY 1000000

#include "includes.h"

extern u32 total_customers;
extern u32 global_time; // ever-incrementing time of day
extern u32 customer_add_countdown;

typedef struct customer customer;
extern pthread_mutex_t in_lock;
extern pthread_mutex_t out_lock;

extern customer *in_queue;
extern customer *out_queue;


#endif
