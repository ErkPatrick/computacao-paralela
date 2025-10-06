#include <iostream>
#include <omp.h>
#include <ctime>
using namespace std;

int main() {
    #pragma omp parallel
    {
        // Espera todas as threads chegarem aqui antes de continuar de forma que o time seja executado prasticamente no mesmo momento para todas as threads
        #pragma omp barrier  

        time_t agora = time(nullptr);
        cout << "Thread " << omp_get_thread_num()
             << " iniciou às " << ctime(&agora);
    }
}
