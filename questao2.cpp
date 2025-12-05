#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 5) {
        if (rank == 0) {
            std::cout << "Este programa requer exatamente 5 processos.\n";
        }
        MPI_Finalize();
        return 0;
    }

    const int tag_p0_p1 = 10; 
    const int tag_p1_p3 = 11;   
    const int tag_p2_p4 = 12;

    int valor = rank; 

    if (rank == 0) {
        // P0 envia permissão para P1
        int permisao = 1;
        MPI_Send(&permisao, 1, MPI_INT, 1, tag_p0_p1, MPI_COMM_WORLD);
    } else if (rank == 1) {
        // P1 espera permissão de P0 antes de enviar para P3
        int perm;
        MPI_Recv(&perm, 1, MPI_INT, 0, tag_p0_p1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&valor, 1, MPI_INT, 3, tag_p1_p3, MPI_COMM_WORLD);
    } else if (rank == 2) {
        // P2 manda mensagem para P4 dizendo que a operação pode ser concluída
        int concluida = 1;
        MPI_Send(&concluida, 1, MPI_INT, 4, tag_p2_p4, MPI_COMM_WORLD);
    } else if (rank == 3) {
        // P3 recebe valor de P1, soma ao seu próprio e envia para P4
        int recebido = 0;
        MPI_Recv(&recebido, 1, MPI_INT, 1, tag_p1_p3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int soma = recebido + valor;
        MPI_Send(&soma, 1, MPI_INT, 4, tag_p1_p3, MPI_COMM_WORLD);
    } else if (rank == 4) {
        // P4 recebe o valor de P3
        int valor_final = 0;
        MPI_Recv(&valor_final, 1, MPI_INT, 3, tag_p1_p3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // só imprime depois que receber a autorização de P2
        int autorizacao;
        MPI_Recv(&autorizacao, 1, MPI_INT, 2, tag_p2_p4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "P4 recebeu valor final = " << valor_final << " e recebeu autorização de P2.\n";
    }

    MPI_Finalize();
    return 0;
}
