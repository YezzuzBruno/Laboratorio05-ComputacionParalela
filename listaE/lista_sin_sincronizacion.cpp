#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <pthread.h>
#include <cstdio>

struct Nodo {
    int dato;
    Nodo* siguiente;
};

Nodo* head = NULL; 
int ops_por_hilo = 10000;
int num_hilos = 4;

void Insertar(int valor) {
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
}

void Eliminar(int valor) {
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
}

int Miembro(int valor) {
    Nodo* actual = head;
    while (actual != NULL && actual->dato < valor) {
        actual = actual->siguiente;
    }
    if (actual == NULL || actual->dato > valor) return 0;
    return 1;
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

    for (int i = 0; i < 100; i += 2) Insertar(i);

    pthread_t* hilos = new pthread_t[num_hilos];
    for (long i = 0; i < num_hilos; ++i) {
        pthread_create(&hilos[i], NULL, Trabajo_Hilo, (void*)(intptr_t) i);
    }
    for (long i = 0; i < num_hilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    std::cout << "Programa terminado.\n";
    delete[] hilos;
    return 0;
}