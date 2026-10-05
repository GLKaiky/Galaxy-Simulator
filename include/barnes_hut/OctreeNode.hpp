 /**
  * @file      OctreeNode.hpp
  * @author    Kaiky França dos Reis Silva
  * @brief     
  * @version   0.1
  * @date      2026-07-24
  * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
  */
 
#pragma once
#include "SimBox.hpp"
#include "../CelestialBody.hpp"
#include <vector>


class OctreeNode; 

struct NodeArena {
    std::vector<OctreeNode> pool;
    size_t count = 0;

    NodeArena(size_t capacity = 500000) {
        pool.resize(capacity);
    }

    void reset() {
        count = 0;
    }

    OctreeNode* allocate(double radius, XyzCoord center);
};

class OctreeNode {
    private:
        SimBox boundingBox;
        CelestialBody *celestialBody;
        OctreeNode* leaf[8];
        double totalMass;
        XyzCoord centerOfMass;

    public:
        ~OctreeNode() { }

        // IMPORTANTE: O std::vector exige um construtor vazio para fazer o .resize() inicial
        OctreeNode() { } 

        OctreeNode(double maxRadius, XyzCoord center) {
            init(maxRadius, center);
        }

        void init(double newRadius, XyzCoord newCenter) {
            this->celestialBody = nullptr;
            for(int i = 0; i<8; i++) {
                this->leaf[i] = nullptr;
            }
            this->boundingBox.center = newCenter;
            this->boundingBox.radius = newRadius;
            this->totalMass = 0.0;
            this->centerOfMass.init();
        }

        int getOctant(const XyzCoord& pos) const {
                int octant = 0;
                
                // Direita (soma 1) ou Esquerda (soma 0)
                if (pos.X >= this->boundingBox.center.X) octant += 1;
                
                // Cima (soma 2) ou Baixo (soma 0)
                if (pos.Y >= this->boundingBox.center.Y) octant += 2;
                
                // Frente (soma 4) ou Trás (soma 0)
                if (pos.Z >= this->boundingBox.center.Z) octant += 4;
                
                return octant;
        }

    bool insert(CelestialBody* body, NodeArena& arena) {
            if (!this->boundingBox.contains(body->getBody_coordinates())) return false;

            if (this->leaf[0] == nullptr && this->celestialBody == nullptr) {
                this->celestialBody = body;
                this->totalMass = body->getMass();
                this->centerOfMass = body->getBody_coordinates();
                return true;
            }

            if (this->leaf[0] == nullptr && this->celestialBody != nullptr) {
                // Repassa a arena para fatiar o espaço
                this->subdivide(arena);
                
                int oldOctant = this->getOctant(this->celestialBody->getBody_coordinates());
                // Repassa a arena para a recursão
                this->leaf[oldOctant]->insert(this->celestialBody, arena);
                this->celestialBody = nullptr;
            }

            double newTotalMass = this->totalMass + body->getMass();
            double x_cm = (this->centerOfMass.X * this->totalMass + body->getBody_coordinates().X * body->getMass()) / newTotalMass;
            double y_cm = (this->centerOfMass.Y * this->totalMass + body->getBody_coordinates().Y * body->getMass()) / newTotalMass;
            double z_cm = (this->centerOfMass.Z * this->totalMass + body->getBody_coordinates().Z * body->getMass()) / newTotalMass;
            
            this->centerOfMass.X = x_cm;
            this->centerOfMass.Y = y_cm;
            this->centerOfMass.Z = z_cm;
            this->totalMass = newTotalMass;

            int newOctant = this->getOctant(body->getBody_coordinates());
            // Repassa a arena para a recursão final
            return this->leaf[newOctant]->insert(body, arena);
        }
        
        void subdivide(NodeArena& arena) {
            double newRadius = this->boundingBox.radius / 2.0;
            double cx = this->boundingBox.center.X;
            double cy = this->boundingBox.center.Y;
            double cz = this->boundingBox.center.Z;

            for (int i = 0; i < 8; i++) {
                XyzCoord newCenter;
                newCenter.X = cx + ((i & 1) ? newRadius : -newRadius);
                newCenter.Y = cy + ((i & 2) ? newRadius : -newRadius);
                newCenter.Z = cz + ((i & 4) ? newRadius : -newRadius);
                
                // ADEUS NEW! Agora pedimos para a arena
                this->leaf[i] = arena.allocate(newRadius, newCenter);
            }
        }

        void calculateForce(CelestialBody* targetBody, double theta, double G, double epsilon) {
                // Se o nó está completamente vazio (sem massa), ignoramos.
                if (this->totalMass == 0.0) return;

                // ESTADO 1: O nó é uma folha (não tem filhos).
                if (this->leaf[0] == nullptr) {
                    // Se houver uma estrela aqui e não for a própria estrela que estamos calculando
                    if (this->celestialBody != nullptr && this->celestialBody != targetBody) {
                        targetBody->applyGravitationalForce(*(this->celestialBody), G, epsilon);
                    }
                    return;
                }

                // ESTADO 2: O nó é interno. Precisamos testar o critério de abertura (Theta).
                
                // 1. Calcula a distância (d) entre a estrela alvo e o centro de massa deste nó
                double dx = this->centerOfMass.X - targetBody->getBody_coordinates().X;
                double dy = this->centerOfMass.Y - targetBody->getBody_coordinates().Y;
                double dz = this->centerOfMass.Z - targetBody->getBody_coordinates().Z;
                
                // Usamos o epsilon aqui também para consistência matemática com a gravidade
                double dist = std::sqrt((dx * dx) + (dy * dy) + (dz * dz) + (epsilon * epsilon));

                // 2. Calcula a largura da região do espaço (s)
                double s = this->boundingBox.radius * 2.0;

                // 3. O Teste de Barnes-Hut
                if ((s / dist) < theta) {
                    // SUCESSO: O nó está longe o suficiente! 
                    // Criamos um "planeta fantasma" temporário com a massa total e o centro de massa do nó.
                    // Reutilizamos o seu construtor de Buraco Negro que aceita (posição, velocidade, massa).
                    Velocity zeroVel; 
                    CelestialBody phantomPlanet(this->centerOfMass, zeroVel, this->totalMass);
                    
                    // Calculamos a força da estrela contra todo esse quadrante de uma só vez.
                    targetBody->applyGravitationalForce(phantomPlanet, G, epsilon);
                } else {
                    // FALHA: A estrela está muito perto desse aglomerado.
                    // Precisamos "abrir" a caixa e calcular a força com os quadrantes menores dentro dela.
                    for (int i = 0; i < 8; i++) {
                        if (this->leaf[i] != nullptr) {
                            this->leaf[i]->calculateForce(targetBody, theta, G, epsilon);
                        }
                    }
                }
            }
};


inline OctreeNode* NodeArena::allocate(double radius, XyzCoord center) {
    if (count >= pool.size()) {
        pool.resize(pool.size() * 2);
    }
    
    OctreeNode* node = &pool[count++];
    node->init(radius, center);
    return node;
}