#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <num_terminos>\n";
        return 1;
    }
    long long num_terminos = std::stoll(argv[1]);

    auto inicio = std::chrono::high_resolution_clock::now();

    double factor = 1.0;
    double suma = 0.0;
    for (long long i = 0; i < num_terminos; ++i) {
        suma += factor / (2.0 * i + 1.0);
        factor = -factor;
    }
    double pi = 4.0 * suma;

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = fin - inicio;

    double pi_real = 3.14159265358979323846;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tiempo: " << tiempo.count() << " s | ";
    std::cout << "Pi: " << std::setprecision(9) << pi << " | ";
    std::cout << "Error: " << std::scientific << std::abs(pi - pi_real) << "\n";

    return 0;
}