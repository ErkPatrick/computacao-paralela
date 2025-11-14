#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int n = 10;
    int vetor[n] = {1,2,3,4,5,6,7,8,9,10};
    int escalar = 3;
    printf("Número de threads: %d\n", omp_get_max_threads());  //Aqui eu verifico o máximo de threads disponíveis no meu sistema = núcleos lógicos/físicos

    #pragma omp parallel for //divide automaticamente o loop for entre várias threads
    for (int i = 0; i < n; i++) {  //cada thread vai pegar um conjunto de índices(cada thread pega uma parte do range total do for)
        vetor[i] *= escalar;
        printf("Thread %d processou o índice %d\n", omp_get_thread_num(), i);
    }

    cout << "Vetor após multiplicação: ";
    for (int i = 0; i < n; i++)
        cout << vetor[i] << " ";
    cout << endl;

    return 0;
}
