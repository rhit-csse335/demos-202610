#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_threads> <samples_per_thread>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    long samples_per_thread = atol(argv[2]);
    long total_samples = num_threads * samples_per_thread;

    printf("=== Thread-Safe rand_r() Demo ===\n");
    printf("Threads: %d, Samples per thread: %ld, Total samples: %ld\n\n",
           num_threads, samples_per_thread, total_samples);

    // Generate seeds for each thread in serial using rand()
    srand(42);
    unsigned int *seeds = malloc(num_threads * sizeof(unsigned int));
    for (int i = 0; i < num_threads; i++) {
        seeds[i] = rand();
    }

    printf("--- Computing Average ---\n");
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        unsigned int rng_state = seeds[tid];
        double local_sum = 0.0;

        for (long i = 0; i < samples_per_thread; i++) {
            double r = 2.0 * rand_r(&rng_state) / RAND_MAX - 1.0;  // Map to [-1, 1]
            local_sum += r;
        }

        #pragma omp atomic
        sum += local_sum;
    }

    double end_time = omp_get_wtime();
    double parallel_time = end_time - start_time;

    double average = sum / total_samples;

    printf("Expected average: 0.0 (uniform distribution on [-1, 1])\n");
    printf("Actual average:   %.6f\n", average);
    printf("Parallel execution time: %.6f seconds\n\n", parallel_time);

    // Serial version with rand() for comparison
    printf("--- Serial Comparison (rand()) ---\n");
    srand(42);
    sum = 0.0;
    start_time = omp_get_wtime();

    for (long i = 0; i < total_samples; i++) {
        sum += 2.0 * rand() / RAND_MAX - 1.0;
    }

    end_time = omp_get_wtime();
    double serial_time_rand = end_time - start_time;

    printf("Serial time with rand():   %.6f seconds\n", serial_time_rand);
    printf("Speedup vs rand():         %.2fx\n", serial_time_rand / parallel_time);
    printf("Efficiency vs rand():      %.1f%%\n\n",
           100.0 * serial_time_rand / (parallel_time * num_threads));

    // Serial version with rand_r() for comparison
    printf("--- Serial Comparison (rand_r()) ---\n");
    unsigned int serial_seed = 1804289383;  // First value from srand(42)
    sum = 0.0;
    start_time = omp_get_wtime();

    for (long i = 0; i < total_samples; i++) {
        sum += 2.0 * rand_r(&serial_seed) / RAND_MAX - 1.0;
    }

    end_time = omp_get_wtime();
    double serial_time_rand_r = end_time - start_time;

    printf("Serial time with rand_r(): %.6f seconds\n", serial_time_rand_r);
    printf("Speedup vs rand_r():       %.2fx\n", serial_time_rand_r / parallel_time);
    printf("Efficiency vs rand_r():    %.1f%% (100%% = perfect scaling)\n",
           100.0 * serial_time_rand_r / (parallel_time * num_threads));

    free(seeds);
    return 0;
}
