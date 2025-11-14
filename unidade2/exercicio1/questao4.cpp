#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int n = 6;
    int A[n] = {10, 20, 30, 40, 50, 60};
    int B[n] = {1, 2, 3, 4, 5, 6};
    int C[n], D[n]; // C = soma, D = subtração

    #pragma omp parallel num_threads(2)
    {
        int id = omp_get_thread_num();

        if (id == 0) { // Thread 0 faz soma
            for (int i = 0; i < n; i++)
                C[i] = A[i] + B[i];
            cout << "Thread " << id << " realizou a soma.\n";
        } 
        else { // Thread 1 faz subtração
            for (int i = 0; i < n; i++)
                D[i] = A[i] - B[i];
            cout << "Thread " << id << " realizou a subtração.\n";
        }
    }

    cout << "\nResultado da soma (C): ";
    for (int i = 0; i < n; i++) cout << C[i] << " ";

    cout << "\nResultado da subtração (D): ";
    for (int i = 0; i < n; i++) cout << D[i] << " ";
    cout << endl;

    return 0;
}
