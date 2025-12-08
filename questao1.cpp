#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int valor = rank;

    const int tag = 100;

    if (size < 2) {
        if (rank == 0) {
            std::cout << "O pipeline precisa de pelo menos 2 processos.\n";
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        MPI_Send(&valor, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);

    } else {
        int recebido = 0;
        // sincronização é feita com o MPI_Recv que é bloqueante
        MPI_Recv(&recebido, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        valor += recebido;

        if (rank < size - 1) {
            MPI_Send(&valor, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
        } else {
            std::cout << "Rank " << rank << " recebeu o total agregado = " << valor << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
