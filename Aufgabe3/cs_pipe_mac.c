#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <mach/mach_time.h> // For mach_absolute_time()

uint32_t COUNTER;
pthread_mutex_t LOCK;
pthread_mutex_t START;
pthread_cond_t CONDITION;

void *threads(void *unused)
{
    pthread_mutex_lock(&START);
    pthread_mutex_unlock(&START);

    pthread_mutex_lock(&LOCK);
    if (COUNTER > 0)
    {
        pthread_cond_signal(&CONDITION);
    }

    for (;;)
    {
        COUNTER++;
        pthread_cond_wait(&CONDITION, &LOCK);
        pthread_cond_signal(&CONDITION);
    }
    pthread_mutex_unlock(&LOCK);
}

int64_t timeInMS(void)
{
    static mach_timebase_info_data_t timebase = {0};
    if (timebase.denom == 0)
    {
        mach_timebase_info(&timebase);
    }

    uint64_t absolute = mach_absolute_time();
    uint64_t nanoseconds = absolute * timebase.numer / timebase.denom;
    return nanoseconds / NSEC_PER_MSEC;
}

int main(int argc, char **argv)
{
    pthread_t t1;
    pthread_t t2;
    int64_t myTime;

    pthread_mutex_init(&LOCK, NULL);
    pthread_mutex_init(&START, NULL);
    pthread_cond_init(&CONDITION, NULL);

    pthread_mutex_lock(&START);
    COUNTER = 0;

    pthread_create(&t1, NULL, threads, NULL);
    pthread_create(&t2, NULL, threads, NULL);
    pthread_detach(t1);
    pthread_detach(t2);

    myTime = timeInMS();
    pthread_mutex_unlock(&START);

    sleep(10);

    pthread_mutex_lock(&LOCK);
    myTime = timeInMS() - myTime;

    COUNTER = (uint32_t)(((uint64_t)COUNTER * 1000) / myTime);
    printf("Number of thread switches in about one second was %u\n", COUNTER);

    return 0;
}
