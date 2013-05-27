#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>


void* read_and_print( void* arg)
{
    FILE* trace_fd = 0;
    long ts, offset, blknum, other;
    char op;
    int retval = 0;
    char *tracefile = (char *)arg;

    trace_fd = fopen(tracefile, "r");
    assert(trace_fd > 0);

READIN_REQ:
    retval = fscanf(trace_fd,"%ld,%ld,%ld,%c,%ld",&ts, &offset, &blknum, &op,  &other);
    while( 5 == retval) {
        fprintf(stderr, "%ld,%ld,%ld,%c,%ld\n",ts, offset, blknum, op,  other);

        goto READIN_REQ;
    }

    return;
}

int main(void)
{
    int thrdnum = 2;
    pthread_t *thrdpool = (pthread_t *) calloc(thrdnum, sizeof(pthread_t));
    int iter = 0;
    int retval = 0;
    void *res;

    char *trace[] = {
        "./w1.csv.final",
        "./w2.csv.final"
    };

    printf("%s\n%s", trace[0], trace[1]);

    for (iter=0; iter<thrdnum; iter++) {
        retval = pthread_create(&(thrdpool[iter]), NULL, read_and_print, trace[iter]);
        assert(0 == retval);
    }

    for (iter=0; iter<thrdnum; iter++) {
        retval = pthread_join((thrdpool[iter]), &res);
        assert(0 == retval);
    }

    return 0;

}
