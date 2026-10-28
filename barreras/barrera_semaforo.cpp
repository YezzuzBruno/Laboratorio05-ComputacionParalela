#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>

int num_hilos;
int contador = 0;
sem_t count_sem;
sem_t barrier_sem;

void Barrera_Semaforo() {
    sem_wait(&count_sem); 
    if (contador == num_hilos - 1) {
        contador = 0;
        sem_post(&count_sem); 
        for (int j = 0; j < num_hilos - 1; j++) {
            sem_post(&barrier_sem);
        }
    } else {
        contador++;
        sem_post(&count_sem); 
        sem_wait(&barrier_sem);
    }
}

void* Trabajo_Hilo(void* arg) {
    intptr_t id = (intptr_t) arg;
    
    std::cout << "Hilo " << id << " llego a la Fase 1.\n";
    
    Barrera_Semaforo();
    
    std::cout << "Hilo " << id << " comenzo la Fase 2.\n";
    
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    num_hilos = std::stoi(argv[1]);

    sem_init(&count_sem, 0, 1);
    sem_init(&barrier_sem, 0, 0);

    pthread_t* hilos = new pthread_t[num_hilos];
    
    auto inicio = std::chrono::high_resolution_clock::now();

    for (long i = 0; i < num_hilos; ++i)
        pthread_create(&hilos[i], NULL, Trabajo_Hilo, (void*)(intptr_t)i);
    for (long i = 0; i < num_hilos; ++i)
        pthread_join(hilos[i], NULL);

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = fin - inicio;
    std::cout << "\nTiempo total: " << tiempo.count() << " s\n";

    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);
    delete[] hilos;
    return 0;
}