#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 20000;
    vector<int> vetor;

    int particao = N / size;  
    vector<int> parte(particao);

    if (rank == 0) {
        vetor.resize(N);
        srand(time(NULL));
        for (int i = 0; i < N; i++)
            vetor[i] = rand() % 100000;

        for (int p = 1; p < size; p++) {
            MPI_Send(&vetor[p * particao], particao, MPI_INT, p, 0, MPI_COMM_WORLD);
        }

        // master copia sua parte
        for (int i = 0; i < particao; i++)
            parte[i] = vetor[i];
    }
    else {
        MPI_Recv(parte.data(), particao, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int menorLocal = parte[0];
    for (int x : parte)
        if (x < menorLocal)
            menorLocal = x;

    if (rank == 0) {
        int menorGeral = menorLocal;
        int menorRecebido;

        for (int p = 1; p < size; p++) {
            MPI_Recv(&menorRecebido, 1, MPI_INT, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (menorRecebido < menorGeral)
                menorGeral = menorRecebido;
        }

        cout << "Menor número encontrado: " << menorGeral << endl;
    }
    else {
        MPI_Send(&menorLocal, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
