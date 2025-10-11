#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ITERATIONS 100000000

int main(int argc, char *argv[]) {
    int num_threads = 4;
    long iterations = ITERATIONS;

    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }
    if (argc > 2) {
        iterations = atol(argv[2]);
    }

    // Allocate array
    long *counters = calloc(num_threads, sizeof(long));

    printf("Running with %d threads, %ld iterations per thread\n\n", num_threads, iterations);

    // Parallel version with false sharing
    double start = omp_get_wtime();
    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        for (long i = 0; i < iterations; i++) {
            counters[tid]++;
        }
    }
    double time_parallel = omp_get_wtime() - start;

    // Serial baseline
    start = omp_get_wtime();
    long serial_counter = 0;
    for (long i = 0; i < iterations * num_threads; i++) {
        serial_counter++;
    }
    double time_serial = omp_get_wtime() - start;

    // Verify correctness
    long total = 0;
    for (int i = 0; i < num_threads; i++) {
        total += counters[i];
    }

    printf("Results:\n");
    printf("  Parallel (with false sharing): %.2e seconds\n", time_parallel);
    printf("  Serial baseline:               %.2e seconds\n\n", time_serial);

    free(counters);

    return 0;
}
