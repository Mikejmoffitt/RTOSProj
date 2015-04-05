#include "globals.h"

u32 total_customers = 0;
u32 global_time = 0;
u32 customer_add_countdown = 0;

customer *in_queue = NULL;
customer *out_queue = NULL;

pthread_mutex_t in_lock;
pthread_mutex_t out_lock;

