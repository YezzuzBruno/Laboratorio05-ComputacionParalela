#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <pthread.h>

int N;
int num_hilos;
std::vector<double> A;
std::vector<double> x;
std::vector<double> y;

void* Calcular_filas(void* id) {
    intptr_t id_hilo = (intptr_t) id;

    int primer_fila = id_hilo * N / num_hilos;
    int ultima_fila = (id_hilo + 1) * N / num_hilos;

    for (int i = primer_fila; i < ultima_fila; ++i) {
        y[i] = 0.0;
        for (int j = 0; j < N; ++j) {
            y[i] += A[i * N + j] * x[j];
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <N> <num_hilos>\n";
        return 1;
    }
    N = std::stoi(argv[1]);
    num_hilos = std::stoi(argv[2]);

    A.assign(N * N, 1.0);
    x.assign(N, 1.0);
    y.assign(N, 0.0);

    pthread_t* hilos = new pthread_t[num_hilos];

    auto inicio = std::chrono::high_resolution_clock::now();

    for (long i = 0; i < num_hilos; ++i) {
        pthread_create(&hilos[i], NULL, Calcular_filas, (void*)(intptr_t) i);
    }
    
    for (long i = 0; i < num_hilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = fin - inicio;

    double verificacion = 0.0;
    for (int i = 0; i < N; ++i) verificacion += y[i];

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tiempo: " << tiempo.count() << " s | ";
    std::cout << "Verificacion: " << verificacion << "\n";

    delete[] hilos;
    return 0;
}