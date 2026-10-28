#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <pthread.h>
#include <chrono>

int num_hilos;
volatile int contador = 0; 
pthread_mutex_t candado = PTHREAD_MUTEX_INITIALIZER;

void Barrera_BusyWait() {
    pthread_mutex_lock(&candado);
    contador++;
    pthread_mutex_unlock(&candado);

    while (contador < num_hilos) {
        // Espera activa
    }
}

void* Trabajo_Hilo(void* arg) {
    intptr_t id = (intptr_t) arg;
    
    std::cout << "Hilo " << id << " llego a la Fase 1.\n";
    
    Barrera_BusyWait();
    
    std::cout << "Hilo " << id << " comenzo la Fase 2.\n";
    
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    num_hilos = std::stoi(argv[1]);
    pthread_t* hilos = new pthread_t[num_hilos];

    auto inicio = std::chrono::high_resolution_clock::now();

    for (long i = 0; i < num_hilos; ++i)
        pthread_create(&hilos[i], NULL, Trabajo_Hilo, (void*)(intptr_t)i);
    for (long i = 0; i < num_hilos; ++i)
        pthread_join(hilos[i], NULL);

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = fin - inicio;
    std::cout << "\nTiempo total: " << tiempo.count() << " s\n";

    delete[] hilos;
    return 0;
}