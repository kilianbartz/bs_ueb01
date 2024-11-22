#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main()
{
    struct timeval start, end;
    long long int num_syscalls = 100000000;

    struct timespec start_ts, end_ts;
    long long int total_elapsed_time = 0;

    for (int j = 0; j < 10; j++)
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start_ts);

        for (long long int i = 0; i < num_syscalls; i++)
        {
            getpid();
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &end_ts);

        long long int elapsed_time = (end_ts.tv_sec - start_ts.tv_sec) * 1000000000LL + (end_ts.tv_nsec - start_ts.tv_nsec);
        total_elapsed_time += elapsed_time;
    }

    long long int average_elapsed_time = total_elapsed_time / 10;
    printf("Average time taken for %lld getpid() syscalls over 10 runs: %lld nanoseconds\n", num_syscalls, average_elapsed_time);
    printf("Average time per getpid() syscall: %f nanoseconds\n", (double)average_elapsed_time / num_syscalls);

    return 0;
}
