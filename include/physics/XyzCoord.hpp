/**
 * @file      XyzCoord.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     Coordenada 3D + gerador aleatório global
 * @version   0.2
 */

#pragma once

#include <random>
#include <cmath>

inline constexpr double kPi = 3.14159265358979323846;

inline std::random_device rd;
inline std::mt19937 gen(rd());

struct XyzCoord
{
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;

    XyzCoord() = default;
    XyzCoord(double x, double y, double z) : X(x), Y(y), Z(z) {}

    void operator+=(const XyzCoord& o) {
        X += o.X;
        Y += o.Y;
        Z += o.Z;
    }

    void init() { X = Y = Z = 0.0; }
};