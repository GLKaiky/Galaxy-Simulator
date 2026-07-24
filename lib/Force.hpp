/**
 * @file      Force.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-07-15
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */

#pragma once    
#include "Velocity.hpp"

struct Force
{
    double F_x;
    double F_y;
    double F_z;

    Force() {
        this->F_x = 0.0;
        this->F_y = 0.0;
        this->F_z = 0.0;
    }

    void operator+=(const Force& force) {
        this->F_x += force.F_x;
        this->F_y += force.F_y;
        this->F_z += force.F_z;
    }

    Velocity calculateDeltaV(double mass, double dt) const {
        Velocity deltaV;
        deltaV.V_x = (this->F_x / mass) * dt;
        deltaV.V_y = (this->F_y / mass) * dt;
        deltaV.V_z = (this->F_z / mass) * dt;
        return deltaV;
    }

    void reset() {
        this->F_x = 0.0;
        this->F_y = 0.0;
        this->F_z = 0.0;
    }

};

