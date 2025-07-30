#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 10000000
#define ITERATIONS 10

// Version 1: Chain of dependencies - pipeline stalls
double sum_with_dependencies(double *arr, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        // Each operation depends on the previous result
        // This creates a long dependency chain that prevents pipelining
        sum = sum + arr[i];
        sum = sum * 1.0000001;  // Tiny multiplication to create dependency
        sum = sum - 0.0000001;  // Subtraction depends on multiplication
    }
    return sum;
}

// Version 2: Multiple independent accumulators - pipeline friendly
double sum_independent_accumulators(double *arr, int size) {
    // Use 2 independent accumulators to break dependency chains
    double sum1 = 0.0, sum2 = 0.0;
    
    // Process 2 elements at a time with independent operations
    int i;
    for (i = 0; i < size - 1; i += 2) {
        // These operations can execute in parallel - no dependencies between them
        sum1 = sum1 + arr[i];
        sum2 = sum2 + arr[i+1];
        
        // Apply the same transformations independently
        sum1 = sum1 * 1.0000001;
        sum2 = sum2 * 1.0000001;
        
        sum1 = sum1 - 0.0000001;
        sum2 = sum2 - 0.0000001;
    }
    
    // Handle remaining elements
    for (; i < size; i++) {
        sum1 = sum1 + arr[i];
        sum1 = sum1 * 1.0000001;
        sum1 = sum1 - 0.0000001;
    }
    
    // Combine results at the end
    return sum1 + sum2;
}

// Version 3: Even more aggressive unrolling (for comparison)
double sum_unrolled(double *arr, int size) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    double sum5 = 0.0, sum6 = 0.0, sum7 = 0.0, sum8 = 0.0;
    
    int i;
    for (i = 0; i < size - 7; i += 8) {
        // 8-way unrolling with independent accumulators
        sum1 += arr[i] * 1.0000001 - 0.0000001;
        sum2 += arr[i+1] * 1.0000001 - 0.0000001;
        sum3 += arr[i+2] * 1.0000001 - 0.0000001;
        sum4 += arr[i+3] * 1.0000001 - 0.0000001;
        sum5 += arr[i+4] * 1.0000001 - 0.0000001;
        sum6 += arr[i+5] * 1.0000001 - 0.0000001;
        sum7 += arr[i+6] * 1.0000001 - 0.0000001;
        sum8 += arr[i+7] * 1.0000001 - 0.0000001;
    }
    
    // Handle remaining elements
    for (; i < size; i++) {
        sum1 += arr[i] * 1.0000001 - 0.0000001;
    }
    
    return sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8;
}

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    double *arr = malloc(ARRAY_SIZE * sizeof(double));
    if (!arr) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Initialize array with random values
    srand(42);  // Fixed seed for reproducibility
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = (double)rand() / RAND_MAX * 100.0;
    }
    
    printf("Pipeline Performance Demo\n");
    printf("Array size: %d elements\n", ARRAY_SIZE);
    printf("Iterations: %d\n\n", ITERATIONS);
    
    struct timespec start, end;
    double total_time;
    double result;
    
    printf("\n=== CAVEAT:\n");
    printf("It was really hard to outsmart the processor's out-of-order\n");
    printf("execution. To see the effect of pipelining vs not pipelineing,\n");
    printf("I actually had to force the functions to compute slightly \n");
    printf("different things that weren't mathematically equivalent. \n\n"); 
   
    // Test Version 1: Chain dependencies
    printf("=== Version 1: Chain Dependencies (Pipeline Stalls) ===\n");
    total_time = 0.0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = sum_with_dependencies(arr, ARRAY_SIZE);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total_time += get_time_diff(start, end);
    }
    printf("Average time: %.8f seconds\n", total_time / ITERATIONS);
    printf("result: %.1f\n", result);

    // Test Version 2: Independent accumulators
    printf("=== Version 2: Independent Accumulators (Pipeline Friendly) ===\n");
    total_time = 0.0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = sum_independent_accumulators(arr, ARRAY_SIZE);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total_time += get_time_diff(start, end);
    }
    printf("Average time: %.8f seconds\n", total_time / ITERATIONS);
    printf("result: %.1f\n", result);
    
    // Test Version 3: Aggressive unrolling
    printf("=== Version 3: 8-way Unrolling (Maximum Pipeline Utilization) ===\n");
    total_time = 0.0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = sum_unrolled(arr, ARRAY_SIZE);
        clock_gettime(CLOCK_MONOTONIC, &end);
        total_time += get_time_diff(start, end);
    }
    printf("Average time: %.8f seconds\n", total_time / ITERATIONS);
    printf("result: %.1f\n", result);
    
    printf("Key Teaching Points:\n");
    printf("1. Version 1 has data dependencies that prevent instruction-level parallelism\n");
    printf("2. Version 2 breaks dependencies with independent accumulators\n");
    printf("3. Version 3 shows maximum pipeline utilization with aggressive unrolling\n");
    printf("4. Performance difference demonstrates pipeline efficiency\n");
    printf("5. Modern processors perform out-of-order execution to find pipeline efficiencies even when they aren't present in the source code.\n");
    
    free(arr);
    return 0;
}
