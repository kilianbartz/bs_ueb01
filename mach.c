#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <mach/mach_time.h>

#define NUM_ITERATIONS 10000000

int main() {
    uint64_t start_time, end_time, total_time = 0;
    pid_t pid;
    
    // Get the timebase information for conversion to nanoseconds
    mach_timebase_info_data_t timebase_info;
    mach_timebase_info(&timebase_info);
    
    // Warm-up loop to allow processor to stabilize
    for (int i = 0; i < 1000; i++) {
        pid = getpid();
    }

    // Measure latency over NUM_ITERATIONS times
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Record the time before the getpid() call
        start_time = mach_absolute_time();
        
        // Call getpid() to measure the latency
        pid = getpid();
        
        // Record the time after the getpid() call
        end_time = mach_absolute_time();
        
        // Convert the time difference to nanoseconds
        uint64_t elapsed_time = end_time - start_time;
        elapsed_time = elapsed_time * timebase_info.numer / timebase_info.denom;
        
        // Add the time in nanoseconds for this iteration
        total_time += elapsed_time;
    }

    // Calculate the average time in nanoseconds per getpid() call
    double avg_latency_ns = (double)total_time / NUM_ITERATIONS;
    
    // Output the result
    printf("Average latency of getpid() over %d iterations: %.2f nanoseconds\n", NUM_ITERATIONS, avg_latency_ns);

    return 0;
}
