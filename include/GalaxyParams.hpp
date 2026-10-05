/**
 * @file      GalaxyParams.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-10-03
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */



#pragma once

struct GalaxyParams {
    // Parâmetros Gerais
    unsigned long numStars = 12000;
    double maxThickness    = 25.0;
    double maxRadius       = 150.0;

    // Física e Integração
    double G           = 1.0;
    double theta       = 1.5;
    double epsilon     = 2.0;
    double centerMass  = 10000.0;
    double starMass    = 0.5;

    // Morfologia (Bojo e Disco)
    double innerRadius   = 25.0;
    double bulgeFraction = 0.25;
    double armFraction   = 0.70;
    int    nArms         = 2;
    double armTwist      = 0.7;
    double armSpread     = 0.15;

    // Cinemática (Dispersão de Velocidade)
    double sigR = 0.10;
    double sigT = 0.07;
    double sigZ = 0.05;

    double haloVelocity   = 14.0; // Velocidade orbital forçada nas bordas da galáxia
    double haloCoreRadius = 25.0; // Suavização (softening) para evitar singularidade no centro
};