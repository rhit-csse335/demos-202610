#include <stdio.h>
#include <omp.h>

int main() {
    int x = 100;

    printf("Before parallel region: x = %d\n\n", x);

    #pragma omp parallel private(x) num_threads(4)
    {
        int tid = omp_get_thread_num();

        // Private x is uninitialized - show the value
        printf("Thread %d: initial private x = %d (uninitialized)\n", tid, x);

        // Each thread modifies its own private copy
        x = tid * 10;
        printf("Thread %d: set private x = %d\n", tid, x);
    }

    printf("\nAfter parallel region: x = %d (unchanged)\n", x);

    return 0;
}
