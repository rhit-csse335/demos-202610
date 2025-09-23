#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Serial element-wise LU factorization without pivoting (element-wise)
// This is textbook Gaussian Elimination via row reductions.
//
// Operates in-place on matrix A (stored in column-major order)
// After completion, A contains L (below diagonal) and U (on and above diagonal)
// It is understood that L's diagonal is all ones, which are not stored in A.
void lu_factorize_serial(double *A, int n) {

    // Remember, column-major: A[r,c] -> A[r + c*n]
    // ie, moving down a column by one (r++) is unit stride in memory

    // Vanilla Gaussian elimination: outer loop over diagonal elements of A
    // For each diagonal element A[d,d], eliminate all elements below it in column d
    for (int d = 0; d < n - 1; ++d) {

        // For each row below the diagonal
        for (int r = d + 1; r < n; ++r) {
            // Calculate multiplier and store in L part
            A[r + d * n] = A[r + d * n] / A[d + d * n];

            // Update remaining submatrix: subtract multiplier times pivot row
            // (Notice: stride-n memory access. Yuck.)
            for (int c = d + 1; c < n; ++c) {
                A[r + c * n] -= A[r + d * n] * A[d + c * n];
            }
        }
    }
}

// Solve linear system Ax = b using LU factorization (serial baseline)
// This is the textbook two-phase triangular solve after LU decomposition.
//
// LU contains the factorized matrix (stored in column-major order):
//   - L below diagonal (with implicit diagonal of ones)
//   - U on and above diagonal
// b is the right-hand side vector
// x is the solution vector (output)
void solve_lu_system(const double *LU, const double *b, double *x, int n) {

    // Remember, column-major: LU[r,c] -> LU[r + c*n]
    // ie, moving down a column by one (r++) is unit stride in memory

    // Phase 1: Forward substitution to solve Ly = b
    // Since L has ones on the diagonal, we don't need to store or divide by them
    double *y = malloc(n * sizeof(double));
    for (int i = 0; i < n; ++i) {
        y[i] = b[i];

        // Subtract contributions from previous elements in this row
        for (int j = 0; j < i; ++j) {
            y[i] -= LU[i + j * n] * y[j];
        }
    }

    // Phase 2: Backward substitution to solve Ux = y
    // U has the actual values on the diagonal, so we need to divide
    for (int i = n - 1; i >= 0; --i) {
        x[i] = y[i];

        // Subtract contributions from elements to the right in this row
        for (int j = i + 1; j < n; ++j) {
            x[i] -= LU[i + j * n] * x[j];
        }

        // Divide by diagonal element of U
        x[i] /= LU[i + i * n];
    }

    free(y);
}

// Initialize matrix with some values to make it diagonally dominant (ensures solvability)
void init_matrix(double *A, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                A[i + j * n] = n + 1.0;  // Make diagonal dominant
            } else {
                A[i + j * n] = 1.0 / (1.0 + abs(i - j));
            }
        }
    }
}

// Initialize right-hand side vector
void init_vector(double *b, int n) {
    for (int i = 0; i < n; ++i) {
        b[i] = 1.0 + i * 0.1;
    }
}

// Print matrix (for small matrices only)
void print_matrix(const double *A, int n) {
    if (n > 8) {
        printf("Matrix too large to print (n=%d)\n", n);
        return;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%8.3f ", A[i + j * n]);
        }
        printf("\n");
    }
}

// Print vector (for small vectors only)
void print_vector(const double *v, int n) {
    if (n > 8) {
        printf("Vector too large to print (n=%d)\n", n);
        return;
    }
    for (int i = 0; i < n; ++i) {
        printf("%8.3f\n", v[i]);
    }
}

int main() {
    const int n = 128;

    printf("LU Factorization and Solve Demo (n=%d)\n", n);
    printf("=====================================\n\n");

    // Allocate memory
    double *A = malloc(n * n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    double *x = malloc(n * sizeof(double));

    if (!A || !b || !x) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Initialize matrix and vector
    init_matrix(A, n);
    init_vector(b, n);

    printf("Original matrix A:\n");
    print_matrix(A, n);
    printf("\nRight-hand side vector b:\n");
    print_vector(b, n);

    // Time the LU factorization
    clock_t start = clock();

    // Perform LU factorization
    lu_factorize_serial(A, n);

    clock_t mid = clock();

    // Solve the system
    solve_lu_system(A, b, x, n);

    clock_t end = clock();

    printf("\nSolution vector x:\n");
    print_vector(x, n);

    // Performance metrics
    double factorize_time = ((double)(mid - start)) / CLOCKS_PER_SEC;
    double solve_time = ((double)(end - mid)) / CLOCKS_PER_SEC;
    double total_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nPerformance Results:\n");
    printf("LU Factorization time: %.6f seconds\n", factorize_time);
    printf("System solve time:     %.6f seconds\n", solve_time);
    printf("Total time:           %.6f seconds\n", total_time);

    // Run multiple iterations for profiling
    printf("\nRunning 100 iterations for profiling...\n");
    start = clock();

    for (int iter = 0; iter < 100; ++iter) {
        // Reinitialize matrix for each iteration
        init_matrix(A, n);

        // Factorize and solve
        lu_factorize_serial(A, n);
        solve_lu_system(A, b, x, n);
    }

    end = clock();
    double avg_time = ((double)(end - start)) / CLOCKS_PER_SEC / 100.0;
    printf("Average time per iteration: %.6f seconds\n", avg_time);

    // Cleanup
    free(A);
    free(b);
    free(x);

    return 0;
}