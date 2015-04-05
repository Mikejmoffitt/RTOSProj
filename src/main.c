#include "globals.h"
#include "includes.h"

#define NUM_TELLERS 3

void *timer_thread(void *arg)
{
	printf(">> Starting timer thread.\n");
	for(;;)
	{
		if (global_time == BANK_OPENING)
		{
			printf("The bank has opened.\n");
		}
		else if (global_time == BANK_CLOSING)
		{
			printf("The bank has closed.\n");
		}
		// Roughly 600 "frames" per second 
		usleep(TIME_DELAY);
		global_time++;

		if (global_time < BANK_OPENING || global_time > BANK_CLOSING)
		{
			continue;
		}

		// Add customers if bank is open
		if (customer_add_countdown == 0)
		{
			// Set a timeout for the next time this will happen
			customer_add_countdown = rand_range(TIME_MINUTE,4 * TIME_MINUTE);

			// Customer add function also marks arrival timestamp
			pthread_mutex_lock(&in_lock);
			printf("%08d: [TIMER] Customer %d has entered the bank.\n",global_time, total_customers);
			in_queue = customer_add(in_queue);
			customer_print(in_queue);
			pthread_mutex_unlock(&in_lock);
		}	
		else
		{
			customer_add_countdown--;
		}
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	pthread_mutex_init(&in_lock,NULL);
	pthread_mutex_init(&out_lock,NULL);

	// Create tellers
	teller tellers[NUM_TELLERS];
	for (int i = 0; i < NUM_TELLERS; i++)
	{
		teller_init(&tellers[i]);
	}

	pthread_t threads[NUM_TELLERS + 1];
	int thread_ids[NUM_TELLERS + 1];

	global_time = BANK_OPENING;

	printf("The bank will open at ");
	print_time(BANK_OPENING);
	printf(" and close at ");
	print_time(BANK_CLOSING);
	printf("\n");

	for (int i = 0; i < NUM_TELLERS; i++)
	{
		thread_ids[i] = pthread_create(&threads[i],NULL,teller_thread,&tellers[i]);
	}
	thread_ids[NUM_TELLERS] = pthread_create(&threads[NUM_TELLERS],NULL,timer_thread,NULL);
	// Block until bank activity is over
	for (int i = 0; i < NUM_TELLERS; i++)
	{
		pthread_join(threads[i],NULL);	
		printf("Teller %d has closed his/her window.\n",i);
	}
	// Kill timer thread
	pthread_cancel(thread_ids[NUM_TELLERS]);
	printf("-----------------------------------------------\n");
	printf("Total customers processed: %d\n",total_customers);
	printf("Finished customers: \n");
	customer_print(out_queue);

	// Clean-up
	pthread_mutex_destroy(&in_lock);
	pthread_mutex_destroy(&out_lock);

	return 0;
}
