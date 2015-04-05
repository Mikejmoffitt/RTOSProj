#include "customer.h"
#include "types.h"

void customer_destroy(customer *c)
{
	customer *next_node = NULL;
	// If passed in queue is null, don't do anything
	while (c)
	{
		// Back up address of next node to jump to if we can
		if (c->next != NULL)
		{
			next_node = c->next;
		}
		// Otherwise, we will remain in the spot which will end the process
		else
		{
			next_node = c;
		}
		// Dealloc the current node
		free(c);
		// Jump to the next one (or to nowhere)
		c = next_node;
	}
}

customer *customer_create(void)
{
	// Create our new customer
	customer *c = (customer *)malloc(sizeof(customer));
	c->teller_time = 0;
	c->finish_time = 0;
	
	// Give unique ID and then increment global counter
	c->id = total_customers++;
	return c;
}

customer *customer_add(customer *h)
{
	// Create our new customer
	customer *c = customer_create();
	c->arrival_time = global_time;
	// Add new customer to the queue
	c->next = h;
	return c;	
}

customer *customer_peek_end(customer *h)
{
	while (h->next != NULL)
	{
		h = h->next;
	}
	return h;
}

void customer_push(customer *h, customer *q)
{
	customer *end = customer_peek_end(h);
	end->next = q;
}

customer *customer_dequeue(customer *ret)
{
	// Queue is empty.
	if (ret == NULL)
	{
		return NULL;
	}
	// Queue has one element, return it. User must manually empty queue...
	else if (ret->next == NULL)
	{
		return ret;
	}
	// Queue has more than one element. Find second to last element
	else
	{
		while (ret->next->next)
		{
			ret = ret->next;
		}
	}

	customer *tail = ret->next;

	// Remove element from LL
	ret->next = NULL;
	return tail;
}

void customer_print(customer *c)
{
	if (!c)
	{
		printf("[]\n");
	}
	while (c)
	{
		printf("[%d]",c->id);
		if (c->next)
		{
			printf(" -> ");
		}
		else
		{
			printf("\n");
		}
		c = c->next;
	}
}

u32 customer_queue_len(customer *c)
{
	u32 count = 0;
	while (c)
	{
		count++;
		c = c->next;
	}
	return count;
}
