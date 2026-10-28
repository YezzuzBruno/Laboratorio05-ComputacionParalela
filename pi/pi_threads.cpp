#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <pthread.h>

long long num_terminos;
int num_hilos;
double suma_global = 0.0;
pthread_mutex_t candado_suma;

void* Calcular_pi(void* argumento) {
    intptr_t id_hilo = (intptr_t) argumento;

    long long primer_termino = (long long) id_hilo * num_terminos / num_hilos;
    long long ultimo_termino = (long long)(id_hilo + 1) * num_terminos / num_hilos;

    double factor;
    if (primer_termino % 2 == 0) factor = 1.0;
    else factor = -1.0;

    double suma_parcial = 0.0;

    for (long long i = primer_termino; i < ultimo_termino; ++i) {
        suma_parcial += factor / (2.0 * i + 1.0);
        factor = -factor;
    }

    pthread_mutex_lock(&candado_suma);
    suma_global += suma_parcial;
    pthread_mutex_unlock(&candado_suma);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <num_terminos> <num_hilos>\n";
        return 1;
    }
    num_terminos = std::stoll(argv[1]);
    num_hilos = std::stoi(argv[2]);

    pthread_mutex_init(&candado_suma, NULL);
    pthread_t* hilos = new pthread_t[num_hilos];

    auto inicio = std::chrono::high_resolution_clock::now();

    for (long i = 0; i < num_hilos; ++i) {
        pthread_create(&hilos[i], NULL, Calcular_pi, (void*)(intptr_t) i);
    }
    for (long i = 0; i < num_hilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = fin - inicio;

    double pi = 4.0 * suma_global;
    double pi_real = 3.14159265358979323846;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tiempo: " << tiempo.count() << " s | ";
    std::cout << "Pi: " << std::setprecision(9) << pi << " | ";
    std::cout << "Error: " << std::scientific << std::abs(pi - pi_real) << "\n";

    delete[] hilos;
    pthread_mutex_destroy(&candado_suma);
    return 0;
}