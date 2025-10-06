#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define N 1000

int main(int argc, char** argv) {
    int comm_sz;    // number of processors
    int my_rank;    // process rank

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    int sqrtN = (int) sqrt(N);

    printf("I'm process %d, and root(N) is %d\n", my_rank, sqrtN);

    // next: data structures to hold results
    // todo...

    // next: each rank does its work
    // todo...

    // next: communicate results
    // todo...

    // next: display final list
    // todo...

    MPI_Finalize();
    return 0;
}
