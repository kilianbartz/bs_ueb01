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

    // Measure latency over NUM_ITERATIONS times
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

    // Calculate the average number of cycles per getpid() call
    double avg_latency_cycles = (double)total_cycles / NUM_ITERATIONS;
    
    // Output the result
    printf("Average latency of getpid() over %d iterations: %.2f cycles\n", NUM_ITERATIONS, avg_latency_cycles);

    return 0;
}
