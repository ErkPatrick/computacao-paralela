#include <iostream>
#include <cmath>
#include <omp.h>
#include <cstdint>
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;
using std::int64_t;
using std::size_t;


inline double f(double x)
{
    return 4.0 / (1.0 + x * x);
}


double trapezio_sequencial(double a, double b, int64_t N)
{
    double h = (b - a) / static_cast<double>(N);
    double soma = 0.5 * f(a) + 0.5 * f(b);
    for (int64_t i = 1; i < N; ++i)
    {
        double x = a + i * h;
        soma += f(x);
    }
    return soma * h;
}

double trapezio_reduction(double a, double b, int64_t N)
{
    double h = (b - a) / static_cast<double>(N);
    double soma = 0.5 * f(a) + 0.5 * f(b);

    #pragma omp parallel for reduction(+ : soma) schedule(static)  //cada thread recebe a mesma quantidade de iterações
        for (int64_t i = 1; i < N; ++i)
        {
            double x = a + i * h;
            soma += f(x);
        }

    return soma * h;
}


double trapezio_atomic(double a, double b, int64_t N)
{
    double h = (b - a) / static_cast<double>(N);
    double soma_global = 0.5 * f(a) + 0.5 * f(b);

    #pragma omp parallel 
    {
        double soma_local = 0.0; // variável privada por thread

        #pragma omp for schedule(static) nowait
            for (int64_t i = 1; i < N; ++i)
            {
                double x = a + i * h;
                soma_local += f(x);
            }

        // atomic para evitar condições de corrida.
        #pragma omp atomic
            soma_global += soma_local;
    }

    return soma_global * h;
}

int main()
{   
    int thread_max = omp_get_num_procs();
    omp_set_num_threads(thread_max); 
    int64_t N = 100000000;

    double a = 0.0, b = 1.0;

    std::cout << std::fixed << std::setprecision(10);

    // Sequencial
    double t0 = omp_get_wtime();
    double res_seq = trapezio_sequencial(a, b, N);
    double t_seq = omp_get_wtime() - t0;

    //reduction
    double t1 = omp_get_wtime();
    double res_A = trapezio_reduction(a, b, N);
    double t_A = omp_get_wtime() - t1;

    //atomic
    double t2 = omp_get_wtime();
    double res_B = trapezio_atomic(a, b, N);
    double t_B = omp_get_wtime() - t2;


    double pi_ref = acos(-1.0);
    // Resultados
    std::cout << "\nResultados:\n";
    std::cout << "PI referencia = " << pi_ref << "\n";
    std::cout << "Sequencial = " << res_seq << " | Tempo = " << t_seq << " s | Erro = " << fabs(res_seq - pi_ref) << "\n";
    std::cout << "Paralela A (reduction) = " << res_A << " | Tempo = " << t_A << " s | Erro = " << fabs(res_A - pi_ref) << "\n";
    std::cout << "Paralela B (atomic)    = " << res_B << " | Tempo = " << t_B << " s | Erro = " << fabs(res_B - pi_ref) << "\n";

    // Speedup e eficiência
    double speedupA = t_seq / t_A;
    double speedupB = t_seq / t_B;
    double effA = speedupA / thread_max;
    double effB = speedupB / thread_max;

    std::cout << "\nDesempenho:\n";
    std::cout << "Speedup A = " << speedupA << " | Eficiência A = " << effA << "\n";
    std::cout << "Speedup B = " << speedupB << " | Eficiência B = " << effB << "\n";

    std::cout << "Threads usadas = " << thread_max << "\n";

    return 0;
}
