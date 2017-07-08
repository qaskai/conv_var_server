#include "proto.h"

struct condition_awaiter condition_awaiters[NR_PROCS+1];
struct FIFO_node lock_FIFOS[NR_MUTEXES + NR_PROCS + 1];
int table_index_to_mutex[NR_MUTEXES+1];

endpoint_t who_e;
int call_type;
short reply;

static void sef_local_startup(void);
static void init_structs(void);


int main(int argc, char* argv[])
{
    message m;

    env_setargs(argc, argv);
    sef_local_startup();
    init_structs();

    while(TRUE) {
        int r;
        
        if ((r = sef_receive(ANY, &m)) != OK)
            printf("sef_receive failed %d.\n", r);
        who_e = m.m_source;
        call_type = m.m_type;


        int ret;
        if (call_type <= 0 || call_type>6) // not supported calls
            ret = -1;
        else
            ret = (*call_vec[call_type])(&m);

        m.m_type = ret;
        
        if ( reply && (r = sendnb(who_e, &m)) != OK)
            printf("CV send error %d.\n", r);
    }
}


static void sef_local_startup()
{
    // leave default behaviour
    sef_startup();
} 


static void init_structs(void) {
    int i=0;
    for (i=0; i<= NR_MUTEXES + NR_PROCS; ++i)
        lock_FIFOS[i].free = 1;

    for (i=0; i<=NR_PROCS; ++i)
        condition_awaiters[i].free = 1;

    /* no need to init mapping, function will check if there exists one using lock_FIFO table */
}
