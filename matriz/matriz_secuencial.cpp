#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <N>\n";
        return 1;
    }
    int N = std::stoi(argv[1]);

    std::vector<double> A(N * N, 1.0); 
    std::vector<double> x(N, 1.0);
    std::vector<double> y(N, 0.0);

    auto inicio = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        y[i] = 0.0;
        for (int j = 0; j < N; ++j) {
            y[i] += A[i * N + j] * x[j];
        }
    }

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = fin - inicio;

    double verificacion = 0.0;
    for (int i = 0; i < N; ++i) verificacion += y[i];

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tiempo: " << tiempo.count() << " s | ";
    std::cout << "Verificacion: " << verificacion << "\n";

    return 0;
}