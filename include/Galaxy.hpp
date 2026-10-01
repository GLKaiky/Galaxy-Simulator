/**
 * @file      Galaxy.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-07-24
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */

#pragma once

#include "CelestialBody.hpp"
#include "./barnes_hut/OctreeNode.hpp"
#include <iostream>
#include <vector>

class Galaxy {
    private:
        std::vector<CelestialBody> clusterOfStars;
        unsigned long numberofStars;
        
        // Tamanho máximo do espaço da simulação para a raiz da Octree
        double galaxyRadius; 
        
        // Constantes da Simulação
        const double G = 1.0;       // Constante gravitacional (ajustável para a escala visual)
        const double THETA = 0.5;  // Precisão do Barnes-Hut (0.5 é um bom equilíbrio)
        const double EPSILON = 2.0; // Fator de suavização para evitar colisões catastróficas
    
    public:
        
        Galaxy(unsigned long numberofStars, double maxThickness, double maxRadius) { 
            this->numberofStars = numberofStars;
            
            // Damos uma folga (3x o raio) para que estrelas em órbitas elípticas 
            // não saiam dos limites da árvore e sejam ignoradas no cálculo.
            this->galaxyRadius = maxRadius * 3.0; 

            clusterOfStars.reserve(numberofStars + 1); 
            
            XyzCoord centerCoord; 
            centerCoord.init();

            Velocity centerVel; 
            centerVel.V_x = 0.0; centerVel.V_y = 0.0; centerVel.V_z = 0.0;

            double centerMass = 10000.0;

            clusterOfStars.emplace_back(centerCoord, centerVel, centerMass); 

            for(unsigned long i = 0; i<numberofStars; i++) {
                clusterOfStars.emplace_back(maxThickness, maxRadius);
            }
        }

        void updateGalaxy(double dt) {
            
            // 1. Inicia o centro da árvore na origem (0, 0, 0)
            XyzCoord rootCenter;
            rootCenter.init();
            
            // 2. Instancia a Octree raiz que engloba toda a galáxia
            OctreeNode* root = new OctreeNode(this->galaxyRadius, rootCenter);

            // 3. Popula a árvore fatiando o espaço recursivamente
            for(auto& star: this->clusterOfStars) {
                root->insert(&star);
            }

            // 4. Calcula a nova força gravitacional para cada estrela otimizada pela árvore
            for(auto& star: this->clusterOfStars) {
                root->calculateForce(&star, THETA, G, EPSILON);
            }
            
            // 5. Atualiza a posição e velocidade usando as forças acumuladas
            for(size_t i = 1; i < this->clusterOfStars.size(); i++) {
                this->clusterOfStars[i].updateMovement(dt);
            }

            // 6. Limpa a memória destruindo a árvore deste frame
            delete root;
        }

        void printToConsole() const {
            const int WIDTH = 379; // Largura do terminal (caracteres)
            const int HEIGHT = 108; // Altura do terminal (linhas)
            
            // Cria uma matriz vazia preenchida com espaços
            std::vector<std::string> screen(HEIGHT, std::string(WIDTH, ' '));
            
            // O raio original de visualização (lembra que multiplicamos por 3 no construtor?)
            double viewRadius = this->galaxyRadius / 3.0;

            for (size_t i = 0; i < this->clusterOfStars.size(); i++) {
                const auto& star = this->clusterOfStars[i];
                double x = star.getBody_coordinates().X;
                double y = star.getBody_coordinates().Y;

                // Mapeia de coordenadas cartesianas para índices da matriz
                // O Y do terminal cresce para baixo, então invertemos.
                // Multiplicamos o WIDTH por 2 porque a fonte do terminal é retangular (mais alta que larga)
                int col = static_cast<int>((x / viewRadius) * (WIDTH / 2.0) + (WIDTH / 2.0));
                int row = static_cast<int>((y / viewRadius) * (HEIGHT / 2.0) + (HEIGHT / 2.0));

                // Garante que a estrela não saia dos limites do terminal
                if (col >= 0 && col < WIDTH && row >= 0 && row < HEIGHT) {
                    if (i == 0) {
                        screen[row][col] = 'O'; // Buraco Negro
                    } else {
                        screen[row][col] = '*'; // Estrelas
                    }
                }
            }

            // Código ANSI para limpar a tela do terminal e mover o cursor para o topo
            std::cout << "\033[2J\033[1;1H";

            // Imprime a tela
            for (const auto& line : screen) {
                std::cout << line << "\n";
            }
            
            // Imprime informações básicas
            std::cout << "Simulador Barnes-Hut | Estrelas: " << this->numberofStars << "\n";
        }
};