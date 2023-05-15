// Compile: $ mpicc questao_2.c -o questao_2
// Run: $ mpiexec -np 4 ./questao_2
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int world_size , world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int i=0, j=0;
    int matrix[N][N];
    if (world_rank == 0) {
        srand(4444);
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                matrix[i][j] = rand() % 10;
            }
        }
    }
    
    int row[N];

    MPI_Scatter(&matrix, N, MPI_INT, &row, N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Node = %d / Array = %d %d %d %d\n", world_rank,row[0],row[1],row[2],row[3]);
    
    MPI_Finalize();
    return 0;
}
