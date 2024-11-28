#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/types.h>
#include <x86intrin.h>

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;
    char byte = 'A';
    struct sched_param param;
    cpu_set_t set;

    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Set CPU affinity to CPU 0
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(0, sizeof(set), &set) == -1)
    {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

    // Set real-time priority
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1)
    {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { // Child process
        unsigned long long start, end;
        char recv_byte;

        // Set same CPU affinity and priority for child
        if (sched_setaffinity(0, sizeof(set), &set) == -1)
        {
            perror("child sched_setaffinity");
            exit(EXIT_FAILURE);
        }
        if (sched_setscheduler(0, SCHED_FIFO, &param) == -1)
        {
            perror("child sched_setscheduler");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < 1000; i++)
        {
            // Wait for parent's signal
            read(pipe1[0], &recv_byte, 1);

            // Send back to parent
            write(pipe2[1], &byte, 1);
        }
        exit(EXIT_SUCCESS);
    }
    else
    { // Parent process
        unsigned long long start, end;
        char recv_byte;
        double total_cycles = 0;
        const int iterations = 10000;

        for (int i = 0; i < iterations; i++)
        {
            start = __rdtsc();

            // Signal child
            write(pipe1[1], &byte, 1);

            // Wait for child's response
            read(pipe2[0], &recv_byte, 1);

            end = __rdtsc();
            total_cycles += (end - start) / 2.0; // Divide by 2 for one-way latency
        }

        // Wait for child to finish
        wait(NULL);

        printf("Average context switch latency: %.2f CPU cycles\n",
               total_cycles / iterations);
    }

    return 0;
}
