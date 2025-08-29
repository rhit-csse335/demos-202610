#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ITERATIONS 100
#define DATA_SIZE 1000

int main(int argc, char *argv[]) {
    int rank, size;
    double start_time, end_time, total_time = 0.0;
    double times[NUM_ITERATIONS];
    int *data;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size < 2) {
        if (rank == 0) {
            printf("This program requires at least 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    // Allocate data array
    data = (int*)malloc(DATA_SIZE * sizeof(int));
    
    if (rank == 0) {
        printf("MPI Send/Recv Timing Demo\n");
        printf("Data size: %d integers\n", DATA_SIZE);
        printf("Iterations: %d\n\n", NUM_ITERATIONS);
        
        // Initialize data
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = i;
        }
        
        // Timing loop
        for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
            start_time = MPI_Wtime();
            
            MPI_Send(data, DATA_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(data, DATA_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            end_time = MPI_Wtime();
            times[iter] = end_time - start_time;
            total_time += times[iter];
        }
        
        // Calculate statistics
        double avg_time = total_time / NUM_ITERATIONS;
        double min_time = times[0], max_time = times[0];
        
        for (int i = 1; i < NUM_ITERATIONS; i++) {
            if (times[i] < min_time) min_time = times[i];
            if (times[i] > max_time) max_time = times[i];
        }
        
        printf("Results:\n");
        printf("Average time: %.6f seconds\n", avg_time);
        printf("Minimum time: %.6f seconds\n", min_time);
        printf("Maximum time: %.6f seconds\n", max_time);
        printf("Total time:   %.6f seconds\n", total_time);
        
    } else if (rank == 1) {
        // Process 1: receive and send back
        for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
            MPI_Recv(data, DATA_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(data, DATA_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    free(data);
    MPI_Finalize();
    return 0;
}
