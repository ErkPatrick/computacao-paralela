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
            vetor[i] = rand() % 1000;
    }

    int particao = N / size;
    vector<int> parte(particao);

    // Comunicação de grupo: distribuir partes
    MPI_Scatter(
        vetor.data(), particao, MPI_INT,   
        parte.data(), particao, MPI_INT, 
        0, MPI_COMM_WORLD         
    );

    // Cada processo calcula a média da sua parte
    double somaLocal = 0;
    for (int x : parte)
        somaLocal += x;

    double mediaLocal = somaLocal / particao;

    vector<double> medias(size);

    // Comunicação de grupo: coletar médias dos processos
    MPI_Gather(
        &mediaLocal, 1, MPI_DOUBLE,
        medias.data(), 1, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    if (rank == 0) {
        // Calcular média geral
        double somaTotal = 0;
        for (double m : medias)
            somaTotal += m;

        double mediaGeral = somaTotal / size;

        cout << "Média geral do vetor: " << mediaGeral << endl;
    }

    MPI_Finalize();
    return 0;
}
