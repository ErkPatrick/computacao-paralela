#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    const int tamanho = 15;
    vector<int> vetor(tamanho);

    for (int i = 0; i < tamanho; i++) vetor[i] = i + 1;

    #pragma omp parallel
    {
        #pragma omp for ordered
        for (int i = 0; i < tamanho; i++) {
            #pragma omp ordered
            {
                cout << "Thread " << omp_get_thread_num()
                     << " imprimiu vetor[" << i << "] = " << vetor[i] << "\n";
            }
        }
    }
}
