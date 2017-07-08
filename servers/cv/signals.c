#ifndef _MUTEX_SIGNALS_C_
#define _MUTEX_SIGNALS_C_

#include "proto.h"
#include "cv.h"

int do_unhang(message* mptr) 
{
    reply = 0;

    endpoint_t proc = mptr->m1_i1;
    int i = get_from_condition_awaiting(proc);
    if (i == -1) {  // process wasn't awaiting condition
        /* remove process from FIFO where it is waiting(isn't first one in the fifo) 
        if it was waiting, wake it to handle signals */
        if (!remove_waiting_from_FIFOS(proc))
            wake_process(proc, EINTR);
    }
    else { // process was waiting for condition, wake it, it will ask for lock (spurious wakeup)
        // remove it from condition_awaiters table
        condition_awaiters[i].free = 1;
        // wake the proceess
        wake_process(proc, EINTR);
    }


    return 0;

}

#endif