#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;


void gerar_imagem(const string &arquivo, int N)
{
    ofstream fout(arquivo);
    if (!fout)
    {
        cerr << "Erro ao criar o arquivo " << arquivo << endl;
        exit(1);
    }

    srand(time(0));

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            fout << rand() % 256;
            if (j < N - 1)
                fout << " ";
        }
        fout << "\n";
    }

    fout.close();
}

vector<vector<int>> ler_imagem(const string &arquivo, int N)
{
    ifstream fin(arquivo);
    if (!fin)
    {
        cerr << "Erro ao abrir o arquivo " << arquivo << endl;
        exit(1);
    }

    vector<vector<int>> imagem(N, vector<int>(N));

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            fin >> imagem[i][j];

    fin.close();
    return imagem;
}

void salvar_imagem(const vector<vector<int>> &imagem, const string &arquivo)
{
    ofstream fout(arquivo);
    if (!fout)
    {
        cerr << "Erro ao criar o arquivo " << arquivo << endl;
        exit(1);
    }

    int N = imagem.size();
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            fout << imagem[i][j];
            if (j < N - 1)
                fout << " ";
        }
        fout << "\n";
    }

    fout.close();
}

vector<vector<int>> blur_sequencial(const vector<vector<int>> &imagem)
{
    int N = imagem.size();
    vector<vector<int>> saida = imagem; // mantém bordas originais

    for (int i = 1; i < N - 1; ++i)
    {
        for (int j = 1; j < N - 1; ++j)
        {
            int soma = 0;
            for (int di = -1; di <= 1; ++di)
                for (int dj = -1; dj <= 1; ++dj)
                    soma += imagem[i + di][j + dj];
            saida[i][j] = soma / 9;
        }
    }

    return saida;
}

vector<vector<int>> blur_paralelo(const vector<vector<int>> &imagem)
{
    int N = imagem.size();
    vector<vector<int>> saida = imagem;

    #pragma omp parallel for schedule(static)
    for (int i = 1; i < N - 1; ++i)
    {
        for (int j = 1; j < N - 1; ++j)
        {
            int soma = 0;
            for (int di = -1; di <= 1; ++di)
                for (int dj = -1; dj <= 1; ++dj)
                    soma += imagem[i + di][j + dj];
            saida[i][j] = soma / 9;
        }
    }

    return saida;
}

int main()
{
    int N = 2000;
    string arquivo_entrada = "imagem.txt";
    string arquivo_saida_seq = "saida_seq.txt";
    string arquivo_saida_par = "saida_par.txt";

    int num_threads = 4;
    omp_set_num_threads(num_threads); 

    gerar_imagem(arquivo_entrada, N);

    vector<vector<int>> imagem = ler_imagem(arquivo_entrada, N);

    // Sequencial
    double t0 = omp_get_wtime();
    vector<vector<int>> seq = blur_sequencial(imagem);
    double t_seq = omp_get_wtime() - t0;
    salvar_imagem(seq, arquivo_saida_seq);

    // Paralelo
    double t1 = omp_get_wtime();
    vector<vector<int>> par = blur_paralelo(imagem);
    double t_par = omp_get_wtime() - t1;
    salvar_imagem(par, arquivo_saida_par);

    // Resultados
    double speedup = t_seq / t_par;
    double eficiencia = speedup / num_threads;

    cout << fixed << setprecision(6);
    cout << "\n--- RESULTADOS ---\n";
    cout << "Threads utilizadas: " << num_threads << endl;
    cout << "Tempo sequencial:   " << t_seq << " s\n";
    cout << "Tempo paralelo:     " << t_par << " s\n";
    cout << "Speedup:            " << speedup << endl;
    cout << "Eficiência:         " << eficiencia << endl;

    return 0;
}
