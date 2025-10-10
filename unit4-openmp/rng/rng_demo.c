#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_threads> <samples_per_thread>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    long samples_per_thread = atol(argv[2]);
    long total_samples = num_threads * samples_per_thread;

    printf("=== Thread-Unsafe rand() Demo ===\n");
    printf("Threads: %d, Samples per thread: %ld, Total samples: %ld\n\n",
           num_threads, samples_per_thread, total_samples);

    // Seed the RNG
    srand(42);

    // Test 1: Compute average of random numbers in [-1, 1]
    printf("--- Test 1: Computing Average ---\n");
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(num_threads)
    {
        for (long i = 0; i < samples_per_thread; i++) {
            double r = 2.0 * rand() / RAND_MAX - 1.0;  // Map to [-1, 1]
            #pragma omp atomic
            sum += r;
        }
    }

    double end_time = omp_get_wtime();
    double parallel_time = end_time - start_time;

    double average = sum / total_samples;
    double expected_average = 0.0;

    // For uniform distribution on [-1, 1]: variance = (b-a)^2/12 = 4/12 = 1/3
    double population_variance = 1.0 / 3.0;
    double std_error = sqrt(population_variance / total_samples);
    double z_score = (average - expected_average) / std_error;

    printf("Expected average: %.6f (uniform distribution on [-1, 1])\n", expected_average);
    printf("Actual average:   %.6f\n", average);
    printf("Difference:       %.6f\n", average - expected_average);
    printf("Standard error:   %.6f\n", std_error);
    printf("Z-score:          %.2f (|z| > 3 suggests non-random behavior)\n", z_score);
    printf("Parallel execution time: %.6f seconds\n\n", parallel_time);

    // Test 2: Check for serialization (mutex contention)
    printf("--- Test 2: Serialization Check ---\n");
    printf("Comparing parallel vs serial performance...\n");
    printf("Parallel time (%d threads): %.6f seconds\n", num_threads, parallel_time);

    // Serial version
    srand(42);
    sum = 0.0;
    start_time = omp_get_wtime();

    for (long i = 0; i < total_samples; i++) {
        sum += 2.0 * rand() / RAND_MAX - 1.0;
    }

    end_time = omp_get_wtime();
    double serial_time = end_time - start_time;

    printf("Serial time (1 thread):   %.6f seconds\n", serial_time);
    printf("Speedup: %.2fx\n", serial_time / parallel_time);
    printf("Efficiency: %.1f%% (100%% = perfect scaling)\n",
           100.0 * serial_time / (parallel_time * num_threads));

    printf("\n=== Interpretation ===\n");
    printf("Skewed average => race conditions corrupting internal RNG state\n");
    printf("Low speedup/efficiency => stdlib using internal mutex for thread safety\n");

    return 0;
}
