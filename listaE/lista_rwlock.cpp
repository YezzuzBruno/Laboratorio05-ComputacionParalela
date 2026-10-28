#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <pthread.h>
#include <cstdio>
#include <chrono>
struct Nodo {
    int dato;
    Nodo* siguiente;
};

Nodo* head = NULL;
int ops_por_hilo = 10000;
int num_hilos = 4;

// EL READ-WRITE LOCK
pthread_rwlock_t rwlock;

void Insertar(int valor) {
    pthread_rwlock_wrlock(&rwlock); 
    Nodo* actual = head;
    Nodo* anterior = NULL;
    while (actual != NULL && actual->dato < valor) {
        anterior = actual;
        actual = actual->siguiente;
    }
    if (actual == NULL || actual->dato > valor) {
        Nodo* nuevo = new Nodo{valor, actual};
        if (anterior == NULL) head = nuevo;
        else anterior->siguiente = nuevo;
    }
    pthread_rwlock_unlock(&rwlock);
}

void Eliminar(int valor) {
    pthread_rwlock_wrlock(&rwlock);
    Nodo* actual = head;
    Nodo* anterior = NULL;
    while (actual != NULL && actual->dato < valor) {
        anterior = actual;
        actual = actual->siguiente;
    }
    if (actual != NULL && actual->dato == valor) {
        if (anterior == NULL) head = actual->siguiente;
        else anterior->siguiente = actual->siguiente;
        free(actual);
    }
    pthread_rwlock_unlock(&rwlock);
}

int Miembro(int valor) {
    pthread_rwlock_rdlock(&rwlock);
    Nodo* actual = head;
    while (actual != NULL && actual->dato < valor) {
        actual = actual->siguiente;
    }
    int resultado = (actual != NULL && actual->dato == valor) ? 1 : 0;
    pthread_rwlock_unlock(&rwlock);
    return resultado;
}

void* Trabajo_Hilo(void* arg) {
    intptr_t id = (intptr_t) arg;
    srand(id + 42);
    
    for (int i = 0; i < ops_por_hilo; ++i) {
        int op = rand() % 100;
        int valor = rand() % 200;
        
        if (op < 80) Miembro(valor);
        else if (op < 90) Insertar(valor);
        else Eliminar(valor);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <num_hilos> <ops_por_hilo>\n";
        return 1;
    }
    num_hilos = std::stoi(argv[1]);
    ops_por_hilo = std::stoi(argv[2]);

    pthread_rwlock_init(&rwlock, NULL);

    for (int i = 0; i < 100; i += 2) Insertar(i);

    auto inicio = std::chrono::high_resolution_clock::now();

    pthread_t* hilos = new pthread_t[num_hilos];
    for (long i = 0; i < num_hilos; ++i) {
        pthread_create(&hilos[i], NULL, Trabajo_Hilo, (void*)(intptr_t) i);
    }
    for (long i = 0; i < num_hilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = fin - inicio;

    std::cout << "Tiempo: " << tiempo.count() << " s | Terminado sin problemas.\n";

    delete[] hilos;
    pthread_rwlock_destroy(&rwlock);
    return 0;
}