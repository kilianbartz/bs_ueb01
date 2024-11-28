#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <x86intrin.h> // For __rdtsc()
#include <sched.h>

#define NUM_ITERATIONS 10000000

int main()
{
    uint64_t start_tsc, end_tsc, total_cycles = 0;
    double result = 0;

    // Measure latency over NUM_ITERATIONS times
    for (int j = 0; j < 10; j++)
    {
        total_cycles = 0;

        for (int i = 0; i < NUM_ITERATIONS; i++)
        {
            // Record the time-stamp counter before the getpid() call
            start_tsc = __rdtsc();

            // Call getpid() to measure the latency
            sched_yield();

            // Record the time-stamp counter after the getpid() call
            end_tsc = __rdtsc();

            // Add the number of cycles it took for this iteration
            total_cycles += (end_tsc - start_tsc);
        }

        // Calculate the average number of cycles per getpid() call
        double avg_latency_cycles = (double)total_cycles / NUM_ITERATIONS;
        if (j > 0)
            result += avg_latency_cycles;

        // Output the result
        printf("Average latency of sched_yield over %d iterations: %.2f cycles\n", NUM_ITERATIONS, avg_latency_cycles);
    }
    printf("Average latency of sched_yield over 10 runs: %.2f cycles\n", result / 9);

    return 0;
}
