#include <mpi.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Garantindo que existam pelo menos 2 processos
    if (size < 2) {
        if (rank == 0)
            cout << "Execute com pelo menos 2 processos!" << endl;
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        string parte1 = "World";
        string parte2 = "Hello";

        MPI_Send(parte1.c_str(), parte1.size() + 1, MPI_CHAR, 1, 10, MPI_COMM_WORLD);

        MPI_Send(parte2.c_str(), parte2.size() + 1, MPI_CHAR, 1, 20, MPI_COMM_WORLD);

        cout << "Master enviou: 'World' e depois 'Hello'." << endl;

    } else if (rank == 1) {

        char buffer1[50];
        char buffer2[50];

        // Diferenciando a partir da tag 
        // Recebe "World"
        MPI_Recv(buffer1, 50, MPI_CHAR, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Recebe "Hello"
        MPI_Recv(buffer2, 50, MPI_CHAR, 0, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        string world = buffer1;
        string hello = buffer2;

        cout << "Processo " << rank << " imprimiu: "
             << hello << " " << world << endl;
    }

    MPI_Finalize();
    return 0;
}
