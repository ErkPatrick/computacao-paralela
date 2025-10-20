#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    const int n = 10;
    const int a = 2;
    const int y = 5;

    vector<int> x(n), z(n);
    for (int i = 0; i < n; i++) x[i] = i;

    #pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        // Cada thread calcula um intervalo
        int inicio = id * n / nthreads;
        int fim = (id + 1) * n / nthreads;

        for (int i = inicio; i < fim; i++) {
            z[i] = a * x[i] + y;
            cout << "Thread " << id << " calculou z[" << i << "] = " << z[i] << endl;
        }
    }
}
