#include "teller.h"

void *teller_thread(void *arg)
{
	teller *t = (teller *)arg;
	printf("%08d: [TELLER] %X has opened his/her window.\n",global_time,(s64)t);
	customer *c = NULL;

	u32 exit = 0;

	while (!exit)
	{
		usleep(TIME_DELAY);
		// Is teller taking a break?
		if (t->break_time > 0)
		{
			// Continue taking a break, do nothing else.
			t->break_time--;
			continue;
		}
		else
		{
			// Prepare to take a break...
			if (t->break_delay > 0)
			{	
				t->break_delay--;
			}
			// Time to take a break.
			else
			{
				// Set random timeout between 30m - 60m for the next break
				t->break_delay = rand_range(30 * TIME_MINUTE, 60 * TIME_MINUTE);

				// Duration of break is between 1 minute and 4 minutes
				t->break_time = rand_range(1 * TIME_MINUTE, 4 * TIME_MINUTE);
				printf("%08d: [TELLER] %X is taking a break for %d seconds.\n",global_time,(s64)t,t->break_time);
			}
		}
		// Teller is not busy, or on a break.
		if (t->busy_time == 0)
		{
			// Do we have a customer at the teller window?
			if (c)
			{
				// Record the customer's finishing time.
				c->finish_time = global_time;
				printf("%08d: [TELLER] %X is finished with customer %d\n",global_time,(s64)t,c->id);

				// Put customer onto finished queue
				pthread_mutex_lock(&out_lock);
				if (!out_queue)
				{
					out_queue = c;
				}
				else
				{
					customer_push(out_queue,c);
				}
				pthread_mutex_unlock(&out_lock);

				// We no longer have a customer at the teller
				c = NULL;
			}

			pthread_mutex_lock(&in_lock);
			// Potentially take a new customer
			if (in_queue != NULL)
			{

				// Only length of one. Workaround for queue mistake...
				c = customer_dequeue(in_queue);

				// Queue had just one element. This is a hotfix for an error.
				if (in_queue->next == NULL)
				{
					in_queue = NULL;
				}

				c->teller_time = global_time;

				// Be busy between 30 seconds and 6 minutes
				t->busy_time = rand_range(30, 60 * 6);
				t->wait_time = 0;
				printf("%08d: [TELLER] %X took in customer %d for %d seconds\n",global_time, (s64)t,c->id,t->busy_time);
			}
			else
			{
				t->wait_time++;
				if (t->wait_time > t->longest_wait)
				{
					t->longest_wait = t->wait_time;
				}
			}
			pthread_mutex_unlock(&in_lock);
		}
		else
		{
			t->busy_time--;
		}

		// Bank doors closed, teller isn't busy, no customers in line still
		if (global_time >= BANK_CLOSING && t->busy_time == 0 && !in_queue)
		{
			exit = 1;
			continue;
		}
	}
	pthread_exit(NULL);
}

void teller_init(teller *t)
{
	t->longest_wait = 0;
	t->wait_time = 0;
	t->busy_time = 0;
	t->break_time = 0;
	t->break_delay = 0; 
}