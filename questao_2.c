// Compile: $ mpicc -o questao_2 questao_2.c 
// Run: $ mpirun -np 4 questao2
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4

int main(int argc, char **argv) {
    
    // Inicialização
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int matrix[N][N];
    // criando uma matrix de forma aleatoria 
    if (world_rank == 0) {
        srand(4444);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = rand() % 10;
            }
        }
    }
    
    int row[N];
    MPI_Scatter(matrix, N, MPI_INT, row, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    printf("Processo %d: ", world_rank);
    for (int i = 0; i < N; i++) {
        printf("%d ", row[i]);
    }
    printf("\n");
    
    MPI_Finalize();
    return 0;
}
