#ifndef _MUTEX_LOCK_C_
#define _MUTEX_LOCK_C_


#include "proto.h"
#include "cv.h"



/*  get mutex */
int do_lock(message* mptr) 
{
    endpoint_t proc = who_e;
    int lock = mptr->m1_i1;

    int i = get_first_from_FIFO(lock);
    if (i != -1) { // FIFO is not empty
        if (lock_FIFOS[i].who == proc) { 
        // process held the lock it called for, shoudn't happen, but no need for him to wait
            reply = 1;
            return 0;
        }
        while (lock_FIFOS[i].next != -1)
            i = lock_FIFOS[i].next;
        int k = get_free_slot_FIFOS();
        lock_FIFOS[i].next = k;

        lock_FIFOS[k].prev = i;
        lock_FIFOS[k].next = -1;
        lock_FIFOS[k].who = proc;
        lock_FIFOS[k].lock = lock;
        lock_FIFOS[k].free = 0;
        reply = 0;
    }
    else { // FIFO empty, can give the lock immediately
        i = get_free_slot_FIFOS();
        lock_FIFOS[i].who = proc;
        lock_FIFOS[i].lock = lock;
        lock_FIFOS[i].next = -1;
        lock_FIFOS[i].prev = -1;
        lock_FIFOS[i].free = 0;
        reply = 1;
    }
    return 0;
}


/*  release mutex */
int do_unlock(message* mptr) 
{
    reply = 1;
    int lock = mptr->m1_i1;
    endpoint_t proc = who_e;
    int i = get_first_from_FIFO(lock);

    // check if process holds the lock
    if (i==-1 || lock_FIFOS[i].who != proc ) {
        return EPERM;
    }
    // remove from FIFO and wake next if there is one
    struct FIFO_node* node = &lock_FIFOS[i];
    node->free = 1;
    if (node->next != -1) {
        lock_FIFOS[node->next].prev = -1;
        wake_process(lock_FIFOS[node->next].who, 0);
    }
    return 0;
}      


/*  remove process from structures */
int do_remove(message* mptr) 
{
    // msg came with send from pm
    reply = 0;
    endpoint_t proc = mptr->m1_i1;
    remove_from_FIFOS(proc);
    int i= get_from_condition_awaiting(proc);
    if (i != -1)
        condition_awaiters[i].free = 1;
    return 0;
}      


#endif