#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 12;      
    int vetor[N];          
    int valorBuscado = 7;

    int particao = N / size;
    vector<int> parte(particao);

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            vetor[i] = i + 1;

        // Envia partes do vetor para os outros processos
        for (int dest = 1; dest < size; dest++) {
            MPI_Send(&vetor[dest * particao], particao, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }

        // Copia a própria parte = processo 0 não recebe via MPI
        for (int i = 0; i < particao; i++)
            parte[i] = vetor[i];

    } else {
        MPI_Recv(parte.data(), particao, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int posicaoEncontrada = -1;

    for (int i = 0; i < particao; i++) {
        int posGlobal = rank * particao + i;

        if (parte[i] == valorBuscado) {
            cout << "Processo " << rank << ": ENCONTREi o valor "
                 << valorBuscado << " na posicao global " << posGlobal << endl;
            posicaoEncontrada = posGlobal;
        } else {
            cout << "Processo " << rank << ": nao encontrei na posicao global "
                 << posGlobal << endl;
        }
    }

    MPI_Send(&posicaoEncontrada, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

    if (rank == 0) {
        int resposta;
        int achou = 0;
        int processoAchou = -1;
        int posFinal = -1;

        for (int src = 0; src < size; src++) {
            MPI_Recv(&resposta, 1, MPI_INT, src, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (resposta != -1) {
                achou = 1;
                processoAchou = src;
                posFinal = resposta;
            }
        }

        if (achou)
            cout << "\nO elemento " << valorBuscado << " foi encontrado na posicao "
                 << posFinal << " pelo processo " << processoAchou << endl;
        else
            cout << "\nO elemento " << valorBuscado << " NAO esta no vetor!" << endl;
    }

    MPI_Finalize();
    return 0;
}
