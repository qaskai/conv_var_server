/*int cs_lock(int mutex id);
int cs_unlock(int mutex id);
int cs_wait(int cond_var_id, int mutex_id);
int cs_broadcast(int cond_var_id);*/


#include <errno.h>
#include <unistd.h>
#include <lib.h>
#include <sys/cdefs.h>

#include <mutex.h>
#include <minix/rs.h>

/* 1 - get mutex */
/* 2 - release mutex */
/* 3 - broadcast event */
/* 4 - await condition */

static int get_cv_number(endpoint_t* val) {
    return minix_rs_lookup("cv", val);
}


int cs_lock(int mutex_id) {
    message m;
    endpoint_t cv_num;
    if ( get_cv_number(&cv_num) != 0 ) {
    	errno = ENOSYS;
        return -1;
    }
     
    int stat = -1;
    // remember current ERRNO, get_lock can change it to EINTR, we want to discard that
    int errno_tmp = errno;
    while (stat==-1) {
        m.m1_i1 = mutex_id;
        stat = _syscall(cv_num, 1, &m);
    }
    // set previous errno
    errno = errno_tmp;
    return 0;
}



int cs_unlock(int mutex_id) {
    message m;
    m.m1_i1 = mutex_id;
    endpoint_t cv_num;
    if ( get_cv_number(&cv_num) != 0 ) {
    	errno = ENOSYS;
        return -1;
    }

    return _syscall(cv_num, 2, &m);
}



int cs_wait(int cond_var_id, int mutex_id) {
    message m;
    m.m1_i1 = mutex_id;
    m.m1_i2 = cond_var_id;
    endpoint_t cv_num;
    if ( get_cv_number(&cv_num) != 0 ) {
    	errno = ENOSYS;
        return -1;
    }

    int stat = _syscall(cv_num, 4, &m);
    if (stat == -1 && errno == EINVAL) { // proces didn't hold the lock
    	return stat;
    }
    return cs_lock(mutex_id);
}



int cs_broadcast(int cond_var_id) {
    message m;
    m.m1_i1 = cond_var_id;
    endpoint_t cv_num;
    if ( get_cv_number(&cv_num) != 0 ) {
    	errno = ENOSYS;
        return -1;
    }

    return _syscall(cv_num, 3, &m);
}