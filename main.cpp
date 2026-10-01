#include <iostream>
#include "include/CelestialBody.hpp"
#include "./include/Galaxy.hpp"
#include <chrono>
#include <thread>

int main() {
    // Parâmetros: Quantidade de estrelas, Espessura Z, Raio Máximo
    // Comece com 500 estrelas para não poluir demais o console
    unsigned long numStars = 500;
    double maxThickness = 10.0;
    double maxRadius = 100.0;

    Galaxy myGalaxy(numStars, maxThickness, maxRadius);

    // Delta time (passo da simulação)
    double dt = 0.1;

    while (true) {
        // Atualiza a física (Construção da árvore, forças e movimento)
        myGalaxy.updateGalaxy(dt);

        // Renderiza no terminal
        myGalaxy.printToConsole();

        // Pausa de 50 milissegundos para dar o efeito de vídeo (~20 frames por segundo)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}