#ifndef __MUTEX__H__
#define __MUTEX__H__

int cs_lock(int mutex_id);
int cs_unlock(int mutex_id);
int cs_wait(int cond_var_id, int mutex_id);
int cs_broadcast(int cond_var_id);

#endif