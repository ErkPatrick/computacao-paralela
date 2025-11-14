#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int n = 8;
    int A[n] = {1,2,3,4,5,6,7,8};
    int B[n] = {8,7,6,5,4,3,2,1};
    int C[n];

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int id = omp_get_thread_num();
        C[i] = A[i] + B[i];
        cout << "Thread " << id << " somou A[" << i << "] + B[" << i << "] = " << C[i] << endl;
    }

    cout << "\nResultado final: ";
    for (int i = 0; i < n; i++)
        cout << C[i] << " ";
    cout << endl;

    return 0;
}
