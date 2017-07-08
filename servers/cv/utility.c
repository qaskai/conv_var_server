#ifndef _CV_UTILITY_C_
#define _CV_UTILITY_C_

#include "cv.h"
#include "proto.h"


// remove process from lock_FIFOS
void remove_from_FIFOS(endpoint_t proc) 
{
    int i;
    for (i=0; i<NR_MUTEXES + NR_PROCS; ++i) {
        struct FIFO_node* cur = &lock_FIFOS[i];
        if (!cur->free && cur->who == proc) {
            if (cur->prev != -1)
                lock_FIFOS[cur->prev].next = cur->next;
            if(cur->next != -1)
                lock_FIFOS[cur->next].prev = cur->prev;
            cur->free = 1;

            if (cur->prev == -1 && cur->next!=-1) {
                wake_process(lock_FIFOS[cur->next].who, 0);
            }
        }
    }
}

// remove process from FIFO it is waiting in, return 0 if removed sth, -1 otherwise
int remove_waiting_from_FIFOS(endpoint_t proc) 
{
    int i;
    for (i=0; i<NR_MUTEXES + NR_PROCS; ++i) {
        struct FIFO_node* cur = &lock_FIFOS[i];
        if (!cur->free && cur->prev != -1 && cur->who == proc) {
            if (cur->prev!=-1)
                lock_FIFOS[cur->prev].next = cur->next;
            if(cur->next!=-1)
                lock_FIFOS[cur->next].prev = cur->prev;
            cur->free = 1;
            return 0;
        }
    }   
    return -1;
}

// returns index of first process in FIFO for given lock_nr, or -1 if there is none
int get_first_from_FIFO(int lock_nr) {
    int i;
    for (i=0; i<NR_PROCS + NR_MUTEXES; ++i) {
        struct FIFO_node* cur = &lock_FIFOS[i];
        if (!cur->free && cur->prev==-1 && cur->lock == lock_nr)
            return i;
    }
    return -1;
}

// return index of first free slot in lock_FIFOS 
int get_free_slot_FIFOS() {
    int i;
    for (i=0; i< NR_MUTEXES + NR_PROCS; ++i)
        if (lock_FIFOS[i].free)
            return i;

    return -1;
}


// get index of process in awaiting struct, it can't wait for more than one mutex 
// returns -1 if process was not waiting on condition
int get_from_condition_awaiting(endpoint_t who)
{
    int i;
    for (i=0; i<NR_PROCS; ++i)
        if (!condition_awaiters[i].free && condition_awaiters[i].who == who)
            return i;
    return -1;
}




void wake_process(endpoint_t who, int ret) 
{
    message m;
    m.m_type = ret;
    int r;
    if ( (r = sendnb(who, &m)) != OK)
        fprintf(stderr, "CV send error %d.\n", r);
}

#endif