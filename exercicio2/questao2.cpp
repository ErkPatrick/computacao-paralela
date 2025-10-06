#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    const int tamanho = 20;
    vector<int> vetor(tamanho);
    int elemento = 42; 
    int posicao = -1;  
    int thread_encontrou = -1;

    // Inicializa vetor
    for (int i = 0; i < tamanho; i++) vetor[i] = i * 3; // 0, 3, 6, 9, ..., 42, ...

    #pragma omp parallel for shared(posicao, thread_encontrou)
    for (int i = 0; i < tamanho; i++) {
        int id = omp_get_thread_num();
        if (vetor[i] == elemento) {
            cout << "Thread " << id << " encontrou o elemento na posição " << i << "\n";
            posicao = i;
            thread_encontrou = id;
        } else {
            cout << "Thread " << id << " verificou posição " << i << " e não encontrou.\n";
        }
    }

    if (posicao != -1)
        cout << "Elemento encontrado na posição " << posicao
             << " pela thread " << thread_encontrou << ".\n";
    else
        cout << "Elemento não encontrado.\n";
}
