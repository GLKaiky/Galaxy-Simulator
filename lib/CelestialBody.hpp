/**
 * @file      CelestialBody.cpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-07-15
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */


#pragma once
#include <vector>
#include <random>
#include <cmath>

#include "XYZ_coord.hpp"
#include "Force.hpp"
#include "Velocity.hpp"

#define MIN_MASS 1.0
#define MAX_MASS 10.0
#define GALAXY_GRAVITATIONAL_FORCE 5.0 

class CelestialBody {
    private:
        double mass;
        XYZ_coord body_coordinates;
        Force appliedForce;
        Velocity velocity;

        /* Para pequenas exponenciações "std::pow()" pode ser um pouco de mais...
           então apenas uma função que multiplica um valor por ele mesmo está ótimo*/
        double exponentiationByTwo(double value) {
            return value * value;
        }
    public:

        double getMass() {
            return this->mass;
        }

        XYZ_coord getBody_coordinates() {
            return this->body_coordinates;
        }

        Force getAppliedForce() {
            return this->appliedForce;
        }

        Velocity getVelocity () {
            return velocity;
        }

        void setAppliedForcetoCenter() {
            this->appliedForce.F_x = -this->body_coordinates.X;
            this->appliedForce.F_y = -this->body_coordinates.Y;
            
        }


        CelestialBody(double maxThickness, double maxRadius) : body_coordinates(maxThickness, maxRadius){ //momento do big bang, nasce um universo
            std::uniform_real_distribution mass(MIN_MASS, MAX_MASS);

            //Definição aleatória de uma massa sorteada entre 1 e 10 para a estrela (Quanto maior a massa, maior a força de atração)
            this->mass = mass(gen);
            
            //Fazendo a arco-tangente de X e Y, se descobre o ângulo posicionado da estrela no plano cartesiano
            double alpha = std::atan2(this->body_coordinates.Y,this->body_coordinates.X);

            //Somando o Ângulo encontrando, pelo ângulo de 90 graus (pi/2), para ter o movimento circular de uma órbita
            double velAngle = alpha + (M_PI/2);

            //Pelo teorema de pitágoras, vai descobrir a distância que aquela estrela está do centro da galáxia 
            //h² = c1² + c2² (D = raiz de c1 + c2)
            double distance = std::sqrt(exponentiationByTwo(this->body_coordinates.X) 
                                        + exponentiationByTwo(this->body_coordinates.Y));


            /* A linha imaginária que liga o planeta ao Sol "varre" áreas iguais em intervalos de tempo iguais. 
               Isso significa que a velocidade do planeta varia ao longo da órbita: ele se move mais rápido quando 
               está mais próximo do Sol (periélio) e mais devagar quando está mais distante (afélio) (Segunda Lei de Kepler). 
               Então simplificando a fórmula do newton, para ter uma velocidade maior quando
               próximo ao centro e uma menor quando longe do centro */
            double velBase = GALAXY_GRAVITATIONAL_FORCE * (1.0/std::sqrt(distance));

            this->velocity.V_x = velBase * cos(velAngle);
            this->velocity.V_y = velBase * sin(velAngle);
            this->velocity.V_z = 0.0;
        }

        ~CelestialBody(){ }

        void updateMovement(double dt) {
            Velocity dV = this->appliedForce.calculateDeltaV(this->mass, dt); //Variação de velocidade calculada
            this->velocity+=dV; //acrescenta o desvio de velocidade 

            XYZ_coord newPosition = this->velocity * dt; //calcula a nova posição
            this->body_coordinates+= newPosition; //acrescenta a nova posição aos eixos X, Y e Z

            this->appliedForce.reset(); //reseta a força (são muitos corpos fazendo força sob muitos corpos ao mesmo tempo)
        }
};