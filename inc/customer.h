#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "includes.h"

typedef struct customer customer;
struct customer 
{
	customer *next; // Pointer to next customer in line (NULL if none)

	// Timestamps -- stamp is void if NULL (0)
	u32 arrival_time; // Timestamp for entering the bank
	u32 teller_time; // Timestamp for being decustomerd and going to a teller
	u32 finish_time; // Timestamp for finishing business with teller

	// Unique identifier
	u32 id;
};

// Destroy a customer and all customers in it
void customer_destroy(customer *c);

// Create a new customer structure
customer *customer_create(void);

// Create a new customer with timestamp
customer *customer_add(customer *h);

// Push customer onto queue
void customer_push(customer *h, customer *c);

// Return lopped off tail
customer *customer_dequeue(customer *ret);

// Print a customer on the screen for debugging.
void customer_print(customer *c);

// Get length of queue
u32 customer_queue_len(customer *c);

#endif
