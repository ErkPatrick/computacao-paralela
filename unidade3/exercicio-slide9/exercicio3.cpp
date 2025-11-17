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

    if (rank == 0) {
        vetor.resize(N);
        srand(time(NULL));
        for (int i = 0; i < N; i++)
            vetor[i] = rand() % 100000;

        int particao = N / (size - 1);

        for (int p = 1; p < size; p++)
            MPI_Send(&vetor[(p - 1) * particao], particao, MPI_INT, p, 0, MPI_COMM_WORLD);
    }

    else {
        int particao = N / (size - 1);
        vector<int> parte(particao);

        MPI_Recv(parte.data(), particao, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // cada slave encontra seu menor local
        int menorLocal = parte[0];
        for (int x : parte)
            if (x < menorLocal)
                menorLocal = x;

        // envia o menor local ao master
        MPI_Send(&menorLocal, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        // master recebe os menores locais e busca o menor geral
        int menorGeral, menorRecebido;

        MPI_Recv(&menorGeral, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int p = 2; p < size; p++) {
            MPI_Recv(&menorRecebido, 1, MPI_INT, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (menorRecebido < menorGeral)
                menorGeral = menorRecebido;
        }

        cout << "Menor número encontrado: " << menorGeral << endl;
    }

    MPI_Finalize();
    return 0;
}
