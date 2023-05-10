// Compile: $ mpicc questao_1.c -o questao_1
// Run: $ mpiexec -n 4 ./questao_1
#include <stdio.h>
#include <mpi.h>

#define NUM_COLORS 3
#define MASTER_RANK 0

int main(int argc, char** argv) {
    int rank, num_procs;
    // Cores: 0 -> branca | 1 -> vermelha | 2 -> verde
    int colors[NUM_COLORS] = {0, 1, 2};
    int my_color = 2; // cor inicial verde

    // Inicialização
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == MASTER_RANK) {
        // Nó mestre envia o vetor de cores em broadcast
        MPI_Bcast(colors, NUM_COLORS, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    }

    // Cada nó envia sua cor inicial para o nó mestre
    MPI_Send(&my_color, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);

    if (rank == MASTER_RANK) {
        int i, color;
        printf("Cores iniciais dos nós:\n");
        printf("Nó mestre (verde)\n");
        for (i = 1; i < num_procs; i++) {
            MPI_Recv(&color, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Nó %d (%s)\n", i, color == 0 ? "branco" : color == 1 ? "vermelho" : "verde");
        }
    }

    // Cada nó redefini sua cor de acordo com o rank
    if (rank % 2 == 0) {
        my_color = 0; // par é branco
    } else {
        my_color = 1; // ímpar é vermelho
    }

    // Cada nó envia sua nova cor para o nó mestre
    MPI_Send(&my_color, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);

    if (rank == MASTER_RANK) {
        int i, color;
        printf("\nCores dos nós após redefinição:\n");
        printf("Nó mestre (verde)\n");
        for (i = 1; i < num_procs; i++) {
            MPI_Recv(&color, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Nó %d (%s)\n", i, color == 0 ? "branco" : color == 1 ? "vermelho" : "verde");
        }
    }

    MPI_Finalize();
    return 0;
}
