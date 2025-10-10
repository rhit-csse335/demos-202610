#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_threads> <count_to>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int count_to = atoi(argv[2]);
    int count = 0;

    printf("Running with %d threads, each counting to %d\n", num_threads, count_to);
    printf("Expected final count: %d\n", num_threads * count_to);

    #pragma omp parallel num_threads(num_threads)
    for (int i = 0; i < count_to; i++) {
        count++;
    }

    printf("Actual final count: %d\n", count);

    return 0;
}
