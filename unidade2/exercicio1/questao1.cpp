#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    #pragma omp parallel num_threads(5)  //5 threads são criadas
    {
        int id = omp_get_thread_num();  //retorna o id da thread(0-4)
        cout << "Olá! Sou a thread " << id << endl;
    }
    return 0;
}
