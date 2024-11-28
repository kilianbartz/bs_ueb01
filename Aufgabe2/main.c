#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

long long int num_syscalls = 10000000;

void measure_getpid(void)
{
    getpid();
}

void measure_getuid(void)
{
    getuid();
}

void measure_gettimeofday(void)
{
    gettimeofday(NULL, NULL);
}

void measure_open(void)
{
    open("/dev/null", O_RDONLY);
}

long long int measureSyscallTime(void (*syscall)())
{
    struct timespec start_ts, end_ts;
    long long int total_elapsed_time = 0;

    for (int j = 0; j < 10; j++)
    {
        printf("Run %d\n", j + 1);
        // Warm-up loop to allow processor to stabilize
        for (long long int i = 0; i < 1000; i++)
        {
            syscall();
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &start_ts);

        for (long long int i = 0; i < num_syscalls; i++)
        {
            syscall();
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &end_ts);

        long long int elapsed_time = (end_ts.tv_sec - start_ts.tv_sec) * 1000000000LL + (end_ts.tv_nsec - start_ts.tv_nsec);
        if (j > 0)
            total_elapsed_time += elapsed_time;
    }

    return total_elapsed_time;
}

int main()
{
    struct timeval start, end;

    printf("Measuring time taken for getpid() syscall\n");
    long long int getpid_time = measureSyscallTime(measure_getpid);
    printf("Measuring time taken for getuid() syscall\n");
    long long int getuid_time = measureSyscallTime(measure_getuid);
    printf("Measuring time taken for gettimeofday() syscall\n");
    long long int gettimeofday_time = measureSyscallTime(measure_gettimeofday);
    printf("Measuring time taken for open() syscall\n");
    long long int open_time = measureSyscallTime(measure_open);

    long long int average_elapsed_time = getpid_time / 9;
    printf("Average time taken for %lld getpid() syscalls over 10 runs: %lld nanoseconds\n", num_syscalls, average_elapsed_time);
    printf("Average time per getpid() syscall: %f nanoseconds\n", (double)average_elapsed_time / num_syscalls);

    average_elapsed_time = getuid_time / 9;
    printf("Average time taken for %lld getuid() syscalls over 10 runs: %lld nanoseconds\n", num_syscalls, average_elapsed_time);
    printf("Average time per getuid() syscall: %f nanoseconds\n", (double)average_elapsed_time / num_syscalls);

    average_elapsed_time = gettimeofday_time / 9;
    printf("Average time taken for %lld gettimeofday() syscalls over 10 runs: %lld nanoseconds\n", num_syscalls, average_elapsed_time);
    printf("Average time per gettimeofday() syscall: %f nanoseconds\n", (double)average_elapsed_time / num_syscalls);

    average_elapsed_time = open_time / 9;
    printf("Average time taken for %lld open() syscalls over 10 runs: %lld nanoseconds\n", num_syscalls, average_elapsed_time);
    printf("Average time per open() syscall: %f nanoseconds\n", (double)average_elapsed_time / num_syscalls);

    return 0;
}
