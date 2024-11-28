#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *switch_affinity(void *arg)
{
    cpu_set_t cpuset;
    pid_t pid = getpid();

    for (int i = 0; i < 1000000; ++i)
    {
        // Set affinity to CPU 0
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset) != 0)
        {
            perror("sched_setaffinity");
            exit(EXIT_FAILURE);
        }

        // Set affinity to CPU 1
        CPU_ZERO(&cpuset);
        CPU_SET(1, &cpuset);
        if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset) != 0)
        {
            perror("sched_setaffinity");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

int main()
{
    pthread_t thread;

    // Create a thread to perform the context switches
    if (pthread_create(&thread, NULL, switch_affinity, NULL) != 0)
    {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    // Wait for the thread to finish
    if (pthread_join(thread, NULL) != 0)
    {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    printf("Completed 1 million context switches.\n");
    return EXIT_SUCCESS;
}
