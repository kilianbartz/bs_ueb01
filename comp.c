#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <x86intrin.h> // For __rdtsc()

#define NUM_ITERATIONS 10000000

int main() {
    uint64_t start_tsc, end_tsc, total_cycles = 0;
    pid_t pid;

    // Warm-up loop to allow processor to stabilize
    for (int i = 0; i < 1000; i++) {
        pid = getpid();
    }

    // Method 1: Per-call measurement
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Record the time-stamp counter before the getpid() call
        start_tsc = __rdtsc();
        
        // Call getpid() to measure the latency
        pid = getpid();
        
        // Record the time-stamp counter after the getpid() call
        end_tsc = __rdtsc();
        
        // Add the number of cycles it took for this iteration
        total_cycles += (end_tsc - start_tsc);
    }

    // Calculate the average number of cycles per getpid() call (Method 1)
    double avg_latency_per_call = (double)total_cycles / NUM_ITERATIONS;

    // Reset the total cycles for the bulk measurement
    total_cycles = 0;

    // Method 2: Bulk measurement
    // Record the time-stamp counter before the start of the loop
    start_tsc = __rdtsc();

    // Perform all 10 million getpid() calls in bulk
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pid = getpid();
    }

    // Record the time-stamp counter after the loop ends
    end_tsc = __rdtsc();

    // Calculate the total number of cycles for the bulk measurement
    total_cycles = end_tsc - start_tsc;

    // Calculate the average number of cycles per getpid() call (Method 2)
    double avg_latency_bulk = (double)total_cycles / NUM_ITERATIONS;

    // Output the results
    printf("Average latency of getpid() per call (Method 1): %.2f cycles\n", avg_latency_per_call);
    printf("Average latency of getpid() per call (Method 2): %.2f cycles\n", avg_latency_bulk);

    return 0;
}
