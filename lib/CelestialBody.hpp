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

inline std::random_device rd;
inline std::mt19937 gen(rd());

#define MIN_MASS 1.0
#define MAX_MASS 10.0
#define GALAXY_GRAVITATIONAL_FORCE 5.0 

class CelestialBody {
    private:
        double mass;
        XYZ_coord body_coordinates;
        Force forceOfBody;
        Velocity velocity;

        double exponentiationByTwo(double value) {
            return value * value;
        }
    public:


        CelestialBody(double maxThickness, double maxRadius) : body_coordinates(maxThickness, maxRadius){
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
};