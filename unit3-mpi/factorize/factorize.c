#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define N 87093284

int main(int argc, char** argv) {
    int comm_sz;    // number of processors
    int my_rank;    // process rank

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    int sqrtN = (int) sqrt(N);

    printf("I'm process %d, and root(N) is %d\n", my_rank, sqrtN);

    // next: data structures to hold results
    int* isFactor = (int*) malloc( (sqrtN + 1) * sizeof(int));
    for (int i = 0; i <= sqrtN; ++i)
        isFactor[i] = 0;

    // next: each rank does its work
    for (int i = 1 + my_rank; i <= sqrtN; i += comm_sz) {
        if (N % i == 0)
            isFactor[i] = 1;
    }

    // next: communicate results
    if (my_rank == 0) {
        int* buffer = (int*) malloc( (sqrtN + 1) * sizeof(int));
        for (int other_rank = 1; other_rank < comm_sz; ++other_rank) {
            MPI_Recv(buffer, sqrtN + 1, MPI_INT, other_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 1; i < sqrtN + 1; ++i) 
                isFactor[i] |= buffer[i];
        }
        free(buffer);
    } else {
        MPI_Send(isFactor, sqrtN + 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // next: display final list
    if (my_rank == 0) {
        for (int i = 1; i <= sqrtN; ++i)
            if (isFactor[i])
                printf("%d\n", i);
        for (int i = sqrtN; i > 0; --i)
            if (isFactor[i])
                printf("%d\n", N / i);
    }

    free(isFactor);
    MPI_Finalize();
    return 0;
}
