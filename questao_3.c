// Compile: $ mpicc -o questao3 questao3.c 
// Run: $ mpirun -np 4 questao3

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
    int i=0,j=0;
  
    int vector[N] ={world_rank,world_rank,world_rank,world_rank}, mat[4][4];
    //imicializando uma matrix com todos os valores 0
    int matrix[N][N];
    if (world_rank == 0) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                matrix[i][j] = 0;
            }
        }
    }
    
    // cada vetor sendo enviado ao nó pai para gerar a matrix
    MPI_Gather(vector, N, MPI_INT, matrix, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (world_rank == 0) {
        printf("Matriz recebida:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }
    
    MPI_Finalize();
    
    return 0;
}
