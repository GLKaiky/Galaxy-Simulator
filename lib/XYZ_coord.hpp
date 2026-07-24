/**
 * @file      XYZ_coord.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-07-15
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */

#pragma once

#include <random>
#include <cmath>

inline std::random_device rd;
inline std::mt19937 gen(rd());

struct XYZ_coord
{
    double X;
    double Y;
    double Z;

    XYZ_coord() { }
    
    XYZ_coord(double maxThickness, double maxRadius) {


        /*Define o eixo Z onde, sorteado um numero aleatório, vai definir em que altura estará a estrela*/
        std::uniform_real_distribution<double> distThickness(-maxThickness, maxThickness);

        /* Utilizando da curva de gauss para fazer uma distribuição mais homogênea 
        das estrelas pela galáxia garantindo mais naturalidade, com um desvio padrão 
        calculado do raio máximo do centro da galáxia até sua borda e divide por 3
        */
        std::normal_distribution<double> distRadius(0.0, maxRadius / 3.0);

        /*Define um ângulo entre 0 e 2 multiplicando pelo valor de PI (3,14...) 
        dando então aleatóriamente as coordenadas polares da estrela */
        std::uniform_real_distribution<double> angle(0.0, 2.0 * M_PI);
        
        double alpha = angle(gen);
        double r = distRadius(gen);

        this->Z = distThickness(gen);
        this->X = r * cos(alpha);
        this->Y = r * sin(alpha);

    }

    void operator+=(const XYZ_coord& newPosition) {
        this->X += newPosition.X;
        this->Y += newPosition.Y;
        this->Z += newPosition.Z;
    }
};
