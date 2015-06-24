
#include "twheel.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define TEST_THREAD_NUM 10
#define TEST_FUNC_PER_THREAD 100000

struct TestVal {
    uint64_t start_t;
    uint64_t delay_t;
};

static uint32_t total = 0;

static inline fetch_total(){
    __sync_fetch_and_add(&total, 1);
    //may notify more than 1 times, just test, no problem
    if (total == TEST_THREAD_NUM*TEST_FUNC_PER_THREAD)
        printf("all timer finish!!!!!\n");
}

//10ms
static inline uint64_t gettime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t t = (tv.tv_sec*100 + tv.tv_usec/10000);
    return t;
}

//if 10ms, then test 1~100s
static inline uint32_t randdelay() {
    //return (random()%MAX_DELAY_TIME + 1);
    return (random()%10000 + 1);
    //return 1000;
}

static void*
updatetime(void* p){
    for (;;) {
        uint64_t t = gettime();
        timewheel_update(t);
        //2ms
        usleep(2000);
    }
}

static void
testfunc(void* arg){
    struct TestVal* val = (struct TestVal*) arg;
    uint64_t start_t = val -> start_t;
    uint64_t delay_t = val -> delay_t;
    uint64_t curr_t = timewheel_gettime();

    fprintf(stdout, "start:%"PRIu64", delay:%"PRIu64", curr:%"PRIu64"\n", start_t, delay_t, curr_t);
    if ((start_t + delay_t) != curr_t)
        fprintf(stdout, "exist no equal!!!!!!!!!!!!!!!\n");

    free(arg);

    //finish judge
    fetch_total();
}

static void*
testthread(void* p){
    uint32_t i;
    for (i = 0; i < TEST_FUNC_PER_THREAD; i++){
        uint32_t delay = randdelay();
        uint64_t start = timewheel_gettime();

        struct TestVal* val = malloc(sizeof(*val));
        memset(val, 0, sizeof(*val));
        val -> start_t = start;
        val -> delay_t = delay;

        timewheel_add_time(testfunc, (void*)val, delay); 
    }
}


int
main(int argc, char** argv){

    //init timer
    uint64_t t = gettime();
    timewheel_create(t);

    //a thread to update timer
    pthread_t pid;
    if (pthread_create(&pid, NULL, updatetime, NULL)) {
        fprintf(stderr, "create updatetime thread failed!\n");
        return 1;
    }

    usleep(5 * 1000 * 1000);

    //run many threads to test
    pthread_t testpids[TEST_THREAD_NUM];
    uint32_t i;
    for (i = 0; i < TEST_THREAD_NUM; i++){
        if (pthread_create(&testpids[i], NULL, testthread, NULL)) {
            fprintf(stderr, "create testthread thread failed!\n");
            return 1;
        }
    }

    for (i = 0; i < TEST_THREAD_NUM; i++){
        pthread_join(testpids[i], NULL);
    }
    pthread_join(pid, NULL);

    return 0;
}

