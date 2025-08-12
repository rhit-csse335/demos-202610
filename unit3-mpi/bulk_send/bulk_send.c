#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <mpi.h>

/******
 * Program to illustrate that sending one value, 1000 times
 * is much slower than sending 1000 values, 1 time.
 ******/

const int PROBLEM_SIZE = 10000;

void randomize_array(double* data_array) {
    for (int i = 0; i < PROBLEM_SIZE; ++i) 
        data_array[i] = ((double) rand()) / INT_MAX;
}

void transfer_array_loop(double* data_array, int my_rank) {
    // main process: send data
    if (my_rank == 0) {
        for (int i = 0; i < PROBLEM_SIZE; ++i)
            MPI_Send(&data_array[i], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    // other processes: receive data
    } else {
        for (int i = 0; i < PROBLEM_SIZE; ++i)
            MPI_Recv(&data_array[i], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void transfer_array_batch(double* data_array, int my_rank) {
    if (my_rank == 0)
        MPI_Send(data_array, PROBLEM_SIZE, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    else
        MPI_Recv(data_array, PROBLEM_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}


int main(int argc, char** argv) {

    double data_array[PROBLEM_SIZE];
    int comm_sz;    // number of processors
    int my_rank;    // process rank

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
        randomize_array(data_array);

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    if (strcmp("loop", argv[1]) == 0) {
        transfer_array_loop(data_array, my_rank);
    } else {
        transfer_array_batch(data_array, my_rank);
    }

    struct timeval stop_time;
    gettimeofday(&stop_time, NULL);
    
    // do something useless with the array, so the compiler can't
    // play reduction games
    double total = 0;
    for (int i = 0; i < PROBLEM_SIZE; ++i)
        total += data_array[i];
    printf("Process %d: total=%f\n", my_rank, total);

    if (my_rank == 0) {
        printf("Time elapsed: %ld microseconds\n", 
            (stop_time.tv_sec*1000000 + stop_time.tv_usec) -
            (start_time.tv_sec*1000000 + start_time.tv_usec)
        );
    }

    MPI_Finalize();
    return 0;
}
