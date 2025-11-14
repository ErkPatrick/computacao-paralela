#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    const int n = 12;
    const int escalar = 3;
    vector<int> vetor(n);
    for (int i = 0; i < n; i++) vetor[i] = i + 1;

    int maior = -1;

    #pragma omp parallel num_threads(4)
    {
        // Multiplicação e busca paralela
        #pragma omp for reduction(max:maior)
        for (int i = 0; i < n; i++) {
            vetor[i] *= escalar;
            if (vetor[i] > maior)
                maior = vetor[i];
        }
    }

    cout << "Maior elemento após multiplicação: " << maior << endl;

    return 0;
}
