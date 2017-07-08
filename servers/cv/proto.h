#ifndef _MUTEX_PROTO_H_
#define _MUTEX_PROTO_H_

#include "cv.h"
#include <limits.h>


#define NR_MUTEXES 1024


struct condition_awaiter {
    endpoint_t who;  // waiting process pid
    int awaited_condition;
    int lock; // lock that it gave up
    short free; // whether or not struct contains info about process or not
};

struct FIFO_node
{
    int lock; // mutex number
    endpoint_t who; // process 
    int next, prev; // indexes of next and previous nodes in FIFO of this lock
    short free; // whether or not slot has valid info
};


EXTERN struct condition_awaiter condition_awaiters[NR_PROCS+1];

// first one in FIFO is holding the lock
EXTERN struct FIFO_node lock_FIFOS[NR_MUTEXES + NR_PROCS + 1];
EXTERN int table_index_to_mutex[NR_MUTEXES+1];
EXTERN short reply;
EXTERN endpoint_t who_e;

// table.c
EXTERN int (*call_vec[])(message*);

// lock.c
int do_lock(message* mptr);        /*  get mutex */
int do_unlock(message* mptr);      /*  release mutex */
int do_remove(message* mptr);       /*  remove process from structures */

// condition_variables.c
int do_broadcast(message* mptr);   /*  broadcast event */
int do_await(message* mptr);       /*  await condition */

// signals.c
int do_unhang(message* mptr);





#endif