#include <iostream>
#include <omp.h>
#include <queue>
#include <unistd.h>

using namespace std;

int main() {
    queue<int> buffer;
    const int capacidade = 5;
    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel sections num_threads(5)
    {
        // Produtor 1
        #pragma omp section
        {
            for (int i = 1; i <= 5; i++) {
                omp_set_lock(&lock);
                if (buffer.size() < capacidade) {
                    buffer.push(i);
                    cout << "Produtor 1 produziu: " << i << endl;
                }
                omp_unset_lock(&lock);
                sleep(1);
            }
        }

        // Produtor 2
        #pragma omp section
        {
            for (int i = 100; i < 105; i++) {
                omp_set_lock(&lock);
                if (buffer.size() < capacidade) {
                    buffer.push(i);
                    cout << "Produtor 2 produziu: " << i << endl;
                }
                omp_unset_lock(&lock);
                sleep(1);
            }
        }

        // Consumidor 1
        #pragma omp section
        {
            for (int i = 0; i < 4; i++) {
                omp_set_lock(&lock);
                if (!buffer.empty()) {
                    int val = buffer.front();
                    buffer.pop();
                    cout << "Consumidor 1 consumiu: " << val << endl;
                }
                omp_unset_lock(&lock);
                sleep(2);
            }
        }

        // Consumidor 2
        #pragma omp section
        {
            for (int i = 0; i < 4; i++) {
                omp_set_lock(&lock);
                if (!buffer.empty()) {
                    int val = buffer.front();
                    buffer.pop();
                    cout << "Consumidor 2 consumiu: " << val << endl;
                }
                omp_unset_lock(&lock);
                sleep(2);
            }
        }

        // Consumidor 3
        #pragma omp section
        {
            for (int i = 0; i < 4; i++) {
                omp_set_lock(&lock);
                if (!buffer.empty()) {
                    int val = buffer.front();
                    buffer.pop();
                    cout << "Consumidor 3 consumiu: " << val << endl;
                }
                omp_unset_lock(&lock);
                sleep(2);
            }
        }
    }

    omp_destroy_lock(&lock);
    return 0;
}
