/*
 * MPI Deadlock Demo - Looks innocent but deadlocks every time!
 * 
 * This program appears to implement a simple data exchange between
 * two processes, but contains a subtle deadlock bug.
 * 
 * Compile: mpicc -o deadlock_demo deadlock_demo.c
 * Run: mpirun -np 2 ./deadlock_demo
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// KLW: I'm seeing 10000 not triggering deadlock, and 100000 does
#define ARRAY_SIZE 1000
#define TAG_DATA 100

int main(int argc, char* argv[]) {
    int rank, size;
    int *send_data, *recv_data;
    MPI_Status status;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size != 2) {
        if (rank == 0) {
            printf("Error: This program requires exactly 2 processes!\n");
        }
        MPI_Finalize();
        exit(1);
    }
    
    // Allocate arrays
    send_data = malloc(ARRAY_SIZE * sizeof(int));
    recv_data = malloc(ARRAY_SIZE * sizeof(int));
    
    // Initialize data based on rank
    for (int i = 0; i < ARRAY_SIZE; i++) {
        send_data[i] = rank * 1000 + i;
    }
    
    printf("Process %d: Starting data exchange...\n", rank);
    fflush(stdout);
    
    /*
     * THE DEADLOCK BUG:
     * Both processes call MPI_Send first, then MPI_Recv.
     * Since MPI_Send can block when the message is large,
     * both processes will wait for the other to receive
     * their message, but neither can because they're both
     * stuck in MPI_Send!
     * 
     * This looks reasonable - "send my data, then receive theirs"
     * but creates a circular wait condition.
     */
    
    if (rank == 0) {
        printf("Process 0: Sending data to process 1...\n");
        fflush(stdout);
        
        // Send to process 1 - THIS WILL BLOCK!
        MPI_Send(send_data, ARRAY_SIZE, MPI_INT, 1, TAG_DATA, MPI_COMM_WORLD);
        
        printf("Process 0: Send complete, now receiving...\n");
        fflush(stdout);
        
        // Receive from process 1 - WILL NEVER GET HERE!
        MPI_Recv(recv_data, ARRAY_SIZE, MPI_INT, 1, TAG_DATA, MPI_COMM_WORLD, &status);
        
        printf("Process 0: Data exchange complete!\n");
        
    } else if (rank == 1) {
        printf("Process 1: Sending data to process 0...\n");
        fflush(stdout);
        
        // Send to process 0 - THIS WILL ALSO BLOCK!
        MPI_Send(send_data, ARRAY_SIZE, MPI_INT, 0, TAG_DATA, MPI_COMM_WORLD);
        
        printf("Process 1: Send complete, now receiving...\n");
        fflush(stdout);
        
        // Receive from process 0 - WILL NEVER GET HERE!
        MPI_Recv(recv_data, ARRAY_SIZE, MPI_INT, 0, TAG_DATA, MPI_COMM_WORLD, &status);
        
        printf("Process 1: Data exchange complete!\n");
    }
    
    // This will never be reached due to deadlock
    printf("Process %d: Verifying received data...\n", rank);
    int other_rank = 1 - rank;
    int errors = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int expected = other_rank * 1000 + i;
        if (recv_data[i] != expected) {
            errors++;
        }
    }
    
    printf("Process %d: Verification complete. Errors: %d\n", rank, errors);
    
    // Cleanup
    free(send_data);
    free(recv_data);
    
    MPI_Finalize();
    return 0;
}
