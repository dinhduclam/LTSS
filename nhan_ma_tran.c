#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MATRIX_SIZE 4

void multiply_matrices(int A[][MATRIX_SIZE], int B[][MATRIX_SIZE], int C[][MATRIX_SIZE], int A_rows, int A_cols, int B_rows, int B_cols) {
    if (A_cols != B_rows) {
        return;
    }
    
    for (int i = 0; i < A_rows; ++i) {
        for (int j = 0; j < B_cols; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < A_cols; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]){
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    const int num_elements = MATRIX_SIZE;
    int rank, num_processes;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    int A[MATRIX_SIZE][MATRIX_SIZE];
    int B[MATRIX_SIZE][MATRIX_SIZE];

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            A[i][j] = rand() % 100 + 1;
            B[i][j] = rand() % 100 + 1;
        }
    }

    if (rank == 0){
		printf("Ma tran A:\n");
		print_matrix(A);

		printf("Ma tran B:\n");
		print_matrix(B);
    }

    int C[MATRIX_SIZE][MATRIX_SIZE];

    const int rows_per_process = num_elements / num_processes;

    int local_A[rows_per_process][MATRIX_SIZE];
    int local_C[rows_per_process][MATRIX_SIZE];

    MPI_Scatter(A, rows_per_process * MATRIX_SIZE, MPI_INT, local_A, rows_per_process * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(B, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    multiply_matrices(local_A, B, local_C, rows_per_process, MATRIX_SIZE, MATRIX_SIZE, MATRIX_SIZE);

    MPI_Gather(local_C, rows_per_process * MATRIX_SIZE, MPI_INT, C, rows_per_process * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Ma tran ket qua C:\n");
        print_matrix(C);
    }

    MPI_Finalize();
    return 0;
}