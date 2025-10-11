#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define N 32

int main(int argc, char *argv[]) {
    int fib_serial[N];
    int fib_parallel[N];
    int num_threads = 4;

    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    // Serial version (correct)
    fib_serial[0] = 0;
    fib_serial[1] = 1;
    for (int i = 2; i < N; i++) {
        fib_serial[i] = fib_serial[i-1] + fib_serial[i-2];
    }

    // Parallel version (incorrect due to loop-carried dependency)
    fib_parallel[0] = 0;
    fib_parallel[1] = 1;
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 2; i < N; i++) {
        fib_parallel[i] = fib_parallel[i-1] + fib_parallel[i-2];
    }

    // Compare results
    printf("Index\tSerial\tParallel\tMatch?\n");
    printf("-----\t------\t--------\t------\n");
    for (int i = 0; i < N; i++) {
        const char *status;
        if (i <= 1) {
            status = "INIT";
        } else {
            status = fib_serial[i] == fib_parallel[i] ? "YES" : "NO";
        }
        printf("%d\t%d\t%d\t\t%s\n",
               i, fib_serial[i], fib_parallel[i], status);
    }

    return 0;
}
