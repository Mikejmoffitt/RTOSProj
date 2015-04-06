#include "globals.h"
#include "includes.h"

teller tellers[NUM_TELLERS];

int line_max_len;

void *timer_thread(void *arg)
{
	
	txtcolor(COL_BRIGHT,COL_GREEN,COL_BLACK);
	printf(">> Starting timer thread.\n");
	line_max_len = 0;
	for(;;)
	{
		if (global_time == BANK_OPENING)
		{
			txtcolor(COL_BLINK,COL_WHITE,COL_BLUE);
			printf("The bank has opened.\n");
		}
		else if (global_time == BANK_CLOSING)
		{
			txtcolor(COL_BLINK,COL_WHITE,COL_RED);
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
			customer_add_countdown = rand_range(CUSTOMER_SPAWN_LOW, CUSTOMER_SPAWN_HIGH);

			// Customer add function also marks arrival timestamp
			pthread_mutex_lock(&in_lock);
			txtcolor(COL_BLINK,COL_MAGENTA,COL_BLACK);
			printf("%08d: [TIMER] Customer %d has entered the bank.\n",global_time, total_customers);
			in_queue = customer_add(in_queue);
			txtcolor(COL_RESET,COL_WHITE,COL_BLACK);
			customer_print(in_queue);
			if (customer_queue_len(in_queue) > line_max_len)
			{
				line_max_len = customer_queue_len(in_queue);
			}
			pthread_mutex_unlock(&in_lock);
		}	
		else
		{
			customer_add_countdown--;
		}
	}
}

void print_customer_stats(void)
{
	u32 queue_time_total = 0;
	u32 queue_time_worst = 0;
	u32 teller_time_total = 0;
	u32 transaction_time_worst = 0;
	customer *c = out_queue;
	while (c != NULL)
	{
		queue_time_total += (c->teller_time - c->arrival_time);
		teller_time_total += (c->finish_time - c->teller_time);
		// Longest time spent in line
		if (queue_time_worst < (c->teller_time - c->arrival_time))
		{
			queue_time_worst = (c->teller_time - c->arrival_time);
		}
		if (transaction_time_worst < (c->finish_time - c->teller_time))
		{
			transaction_time_worst = (c->finish_time - c->teller_time);
		}
		c = c->next;
	}
	printf("Average time customer spends in queue: %d seconds\n",(queue_time_total / total_customers));
	printf("Average time customer spends with teller: %d seconds\n",(teller_time_total / total_customers));
	printf("Longest time spent waiting in line: %d seconds\n",queue_time_worst);
	printf("Longest transaction: %d seconds\n",transaction_time_worst);
}

void print_teller_stats(void)
{
	u32 wait_time_total = 0;
	u32 longest_wait = 0;
	for (int i = 0; i < NUM_TELLERS; i++)
	{
		wait_time_total += tellers[i].total_wait;
		if (longest_wait < tellers[i].total_wait)
		{
			longest_wait = tellers[i].total_wait;
		}
	}
	printf("Average time a teller waited for a customer: %d seconds\n",(wait_time_total / total_customers));
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	pthread_mutex_init(&in_lock,NULL);
	pthread_mutex_init(&out_lock,NULL);

	// Create tellers
	for (int i = 0; i < NUM_TELLERS; i++)
	{
		teller_init(&tellers[i]);
	}

	pthread_t threads[NUM_TELLERS + 1];
	int thread_ids[NUM_TELLERS + 1];

	global_time = BANK_OPENING;

	txtcolor(COL_RESET,COL_CYAN,COL_BLACK);
	printf("The bank will open at ");
	print_time(BANK_OPENING);
	printf(" and close at ");
	print_time(BANK_CLOSING);
	printf("\n");
	txtcolor(COL_RESET,COL_WHITE,COL_BLACK);

	for (int i = 0; i < NUM_TELLERS; i++)
	{
		thread_ids[i] = pthread_create(&threads[i],NULL,teller_thread,&tellers[i]);
	}
	thread_ids[NUM_TELLERS] = pthread_create(&threads[NUM_TELLERS],NULL,timer_thread,NULL);
	// Block until bank activity is over
	for (int i = 0; i < NUM_TELLERS; i++)
	{
		pthread_join(threads[i],NULL);	
		txtcolor(COL_RESET,COL_RED,COL_BLACK);
		printf("Teller %d has closed his/her window.\n",i);
	}
	// Kill timer thread
	pthread_cancel(thread_ids[NUM_TELLERS]);
	txtcolor(COL_RESET,COL_WHITE,COL_BLACK);
	printf("-----------------------------------------------\n");
	printf("Total customers processed: %d\n",total_customers);
	printf("Max queue length: %d\n",line_max_len);
	print_customer_stats();
	print_teller_stats();
	//customer_print(out_queue);

	// Clean-up
	pthread_mutex_destroy(&in_lock);
	pthread_mutex_destroy(&out_lock);

	return 0;
}
