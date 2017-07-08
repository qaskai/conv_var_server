#ifndef _CONDITION_VARIABLES_C_
#define _CONDITION_VARIABLES_C_

#include "proto.h"


/*  broadcast event */
int do_broadcast(message* mptr) 
{
    int broadcasted_condition = mptr->m1_i1;
    endpoint_t who;
    int i;
    for (i=0; i<NR_PROCS; ++i) {
        struct condition_awaiter* cur = &condition_awaiters[i];
        if ( !cur->free &&  cur->awaited_condition == broadcasted_condition) {
            who = cur->who;
            cur->free = 1;
            wake_process(who, 0);
        }
    }
    reply = 1;
    return 0;
}


/*  await condition */
int do_await(message* mptr) 
{
    int lock = mptr->m1_i1;
    int cond = mptr->m1_i2;
    int proc = who_e;

    // check if process holds specified lock
    int i = get_first_from_FIFO(lock);
    if (i==-1 || lock_FIFOS[i].who != proc) {
        reply =1;
        return EINVAL;
    }
    int k=0;
    while (!condition_awaiters[k].free)
        ++k;
    condition_awaiters[k].who = proc;
    condition_awaiters[k].lock = lock;
    condition_awaiters[k].awaited_condition = cond;
    condition_awaiters[k].free = 0;

    lock_FIFOS[i].free = 1;
    if (lock_FIFOS[i].next != -1) {
        int tmp = lock_FIFOS[i].next;
        lock_FIFOS[tmp].prev = -1;
        wake_process(lock_FIFOS[tmp].who, 0);
    }

    reply = 0;
    return 0;
}

#endif