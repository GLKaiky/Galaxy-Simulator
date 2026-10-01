 /**
  * @file      OctreeNode.hpp
  * @author    Kaiky França dos Reis Silva
  * @brief     
  * @version   0.1
  * @date      2026-07-24
  * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
  */
 
#pragma once
#include "BoundingBox.hpp"
#include "../CelestialBody.hpp"
#include <vector>

class OctreeNode {
    private:
        BoundingBox boundingBox;
        CelestialBody *celestialBody;
        OctreeNode* leaf[8];
        double totalMass;
        XyzCoord centerOfMass;

    public:

        ~OctreeNode() {
                for(int i = 0; i < 8; i++) {
                    if (this->leaf[i] != nullptr) {
                        delete this->leaf[i];
                        this->leaf[i] = nullptr;
                    }
                }
        }

        OctreeNode(double maxRadius, XyzCoord center) {
            this->celestialBody = nullptr;

            for(int i = 0; i<8; i++) {
                this->leaf[i] = nullptr;
            }

            this->boundingBox.center = center;
            this->boundingBox.radius = maxRadius;
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

        bool insert(CelestialBody* body) {
                // Proteção: Se a estrela não está dentro do limite deste nó, ela não entra.
                if (!this->boundingBox.contains(body->getBody_coordinates())) {
                    return false;
                }

                // ESTADO 1: O nó está completamente vazio.
                if (this->leaf[0] == nullptr && this->celestialBody == nullptr) {
                    this->celestialBody = body;
                    this->totalMass = body->getMass();
                    this->centerOfMass = body->getBody_coordinates();
                    return true;
                }

                // ESTADO 2: O nó já tem uma estrela (Colisão). Precisa fatiar o espaço.
                if (this->leaf[0] == nullptr && this->celestialBody != nullptr) {
                    this->subdivide();
                    
                    // Pega a estrela antiga que estava aqui e empurra para a gaveta certa abaixo
                    int oldOctant = this->getOctant(this->celestialBody->getBody_coordinates());
                    this->leaf[oldOctant]->insert(this->celestialBody);
                    
                    // Este nó agora é um nó interno (representante). Limpamos a referência física.
                    this->celestialBody = nullptr;
                }

                // ESTADO 3: O nó é interno (já tinha filhos ou acabou de ser subdividido).
                // Atualiza a massa total e recalcula o centro de massa (média ponderada).
                double newTotalMass = this->totalMass + body->getMass();
                
                double x_cm = (this->centerOfMass.X * this->totalMass + body->getBody_coordinates().X * body->getMass()) / newTotalMass;
                double y_cm = (this->centerOfMass.Y * this->totalMass + body->getBody_coordinates().Y * body->getMass()) / newTotalMass;
                double z_cm = (this->centerOfMass.Z * this->totalMass + body->getBody_coordinates().Z * body->getMass()) / newTotalMass;
                
                this->centerOfMass.X = x_cm;
                this->centerOfMass.Y = y_cm;
                this->centerOfMass.Z = z_cm;
                
                this->totalMass = newTotalMass;

                // Por fim, empurra a nova estrela para o octante correto (recursão).
                int newOctant = this->getOctant(body->getBody_coordinates());
                return this->leaf[newOctant]->insert(body);
            }
        
        void subdivide() {
            double newRadius = this->boundingBox.radius / 2.0;
            
            double cx = this->boundingBox.center.X;
            double cy = this->boundingBox.center.Y;
            double cz = this->boundingBox.center.Z;

            // O laço vai de 0 a 7 (nossos 8 octantes)
            for (int i = 0; i < 8; i++) {
                XyzCoord newCenter;
                
                // Usamos os bits de 'i' para decidir se somamos ou subtraímos o raio.
                // (i & 1) verifica se o 1º bit é 1 (Direita/Esquerda)
                // (i & 2) verifica se o 2º bit é 1 (Cima/Baixo)
                // (i & 4) verifica se o 3º bit é 1 (Frente/Trás)
                
                newCenter.X = cx + ((i & 1) ? newRadius : -newRadius);
                newCenter.Y = cy + ((i & 2) ? newRadius : -newRadius);
                newCenter.Z = cz + ((i & 4) ? newRadius : -newRadius);
                
                this->leaf[i] = new OctreeNode(newRadius, newCenter);
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