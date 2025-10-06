#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int n = 10;
    int vetor[n] = {1,2,3,4,5,6,7,8,9,10};
    int soma = 0;

    #pragma omp parallel for reduction(+:soma)  //cria a variável local soma para cada thread. Por fim é feita uma redução com base no operador, nesse caso o + (soma), em todas as variáveis que contem o valor parcial calculado por cada thread
    for (int i = 0; i < n; i++) {
        soma += vetor[i];
    }

    cout << "Soma total dos elementos: " << soma << endl;
    return 0;
}
