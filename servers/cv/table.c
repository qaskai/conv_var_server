// NR_PROCS - max # of processes
#ifndef _MUTEX_TABLE_C_
#define _MUTEX_TABLE_C_

#include "proto.h"


int (*call_vec[])(message*) = {
    NULL,           /* 0 - nothing */
    do_lock,        /* 1 - get mutex */
    do_unlock,      /* 2 - release mutex */
    do_broadcast,   /* 3 - broadcast event */
    do_await,       /* 4 - await condition */
    do_remove,      /* 5 - remove process from structures */
    do_unhang       /* 6 - signal came to a process */
};


#endif