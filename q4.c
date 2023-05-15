// Compile: $ mpicc -o q4 q4.c -lm
// Run: $ mpirun -np <num_procs> q4


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

#define N 1000000

int main(int argc, char** argv) {
    int i, rank, size;
    int myN = N;
    int mysum = 0;
    int sum = 0;
    int* vec = NULL;
    struct timeval start_time, end_time;
    double seq_time, par_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // dividir o tamanho do vetor entre os processos
    myN = N / size;

    // alocar espaço para o vetor em cada processo
    vec = (int*)malloc(sizeof(int) * myN);

    // inicializar o vetor
    for (i = 0; i < myN; i++) {
        vec[i] = i + myN * rank;
    }

    // medir o tempo sequencial
    if (rank == 0) {
        gettimeofday(&start_time, NULL);
        for (i = 0; i < N; i++) {
            sum += i;
        }
        gettimeofday(&end_time, NULL);
        seq_time = ((end_time.tv_sec - start_time.tv_sec) * 1000000u +
                    end_time.tv_usec - start_time.tv_usec) / 1.e6;
    }

    // medir o tempo paralelo
    gettimeofday(&start_time, NULL);

    // distribuir o vetor entre os processos
    MPI_Scatter(vec, myN, MPI_INT, vec, myN, MPI_INT, 0, MPI_COMM_WORLD);

    // somar os elementos do vetor local
    for (i = 0; i < myN; i++) {
        mysum += vec[i];
    }

    // somar as somas locais
    MPI_Reduce(&mysum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // imprimir o tempo paralelo somente no processo raiz
    if (rank == 0) {
        gettimeofday(&end_time, NULL);
        par_time = ((end_time.tv_sec - start_time.tv_sec) * 1000000u +
                    end_time.tv_usec - start_time.tv_usec) / 1.e6;
        printf("Tempo sequencial: %f\n", seq_time);
        printf("Tempo paralelo: %f\n", par_time);
        printf("Speedup: %f\n", seq_time / par_time);
    }

    // liberar a memória alocada
    free(vec);

    MPI_Finalize();

    return 0;
}
