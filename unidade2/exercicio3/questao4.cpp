#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    const int n = 8;
    vector<int> a(n), b(n), c(n), d(n), e(n);
    int f = 2;
    for (int i = 0; i < n; i++) {
        b[i] = i;
        c[i] = i + 1;
        e[i] = i + 2;
    }

    #pragma omp parallel
    {
        //utilizo nowait para remover as barreiras implícitas que tem no omp for
        #pragma omp for nowait
        for (int j = 0; j < n; j++) {
            a[j] = b[j] + c[j];
        }

        #pragma omp barrier // só aqui é obrigatória: z depende de a

        #pragma omp for nowait
        for (int j = 0; j < n; j++) {
            d[j] = e[j] * f;
        }

        #pragma omp barrier  //para garantir sincronismo geral

        #pragma omp for
        for (int j = 0; j < n - 1; j++) {
            double z = (a[j] + a[j + 1]) * 0.5;
            cout << "z[" << j << "] = " << z << endl;
        }
    }
}
