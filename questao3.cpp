#include <mpi.h>
#include <iostream>
#include <climits>
#include <algorithm>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 8;
    int vet_local[N];
    int sum_vet[N];
    int max_vet[N];

    // inicializa o vetor de cada processo
    for (int i = 0; i < N; ++i)
        vet_local[i] = rank + i;

    // prepara vetores auxiliares
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) {         // par = soma
            sum_vet[i] = vet_local[i];
            max_vet[i] = INT_MIN;  //elemento neutro do máximo
        } else {                  // ímpar = máximo
            sum_vet[i] = 0;  // elemento neutro da soma
            max_vet[i] = vet_local[i];
        }
    }

    int result_sum[N];
    int result_max[N];

    MPI_Reduce(sum_vet, result_sum, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(max_vet, result_max, N, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int final_vet[N];

        for (int i = 0; i < N; i++) {
            if (i % 2 == 0)
                final_vet[i] = result_sum[i];
            else
                final_vet[i] = result_max[i];
        }

        std::cout << "Resultado final:\n";
        for (int i = 0; i < N; i++)
            std::cout << final_vet[i] << " ";
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
