#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int vector[N];
    srand(world_rank + 1);
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 10;
    }
    
    int matrix[N][N];
    if (world_rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = 0;
            }
        }
    }
    
    MPI_Gather(vector, N, MPI_INT, matrix, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (world_rank == 0) {
        printf("Matriz recebida:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }
    
    MPI_Finalize();
    
    return 0;
}