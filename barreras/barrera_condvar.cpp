#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <pthread.h>
#include <chrono>

int num_hilos;
int contador = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

void Barrera_CondVar() {
    pthread_mutex_lock(&mutex);
    contador++;
    
    if (contador == num_hilos) {
        contador = 0;
        pthread_cond_broadcast(&cond_var);
    } else {
        while (pthread_cond_wait(&cond_var, &mutex) != 0);
    }
    pthread_mutex_unlock(&mutex);
}

void* Trabajo_Hilo(void* arg) {
    intptr_t id = (intptr_t) arg;
    
    std::cout << "Hilo " << id << " llego a la Fase 1.\n";
    
    Barrera_CondVar();
    
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
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    return 0;
}