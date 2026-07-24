/**
 * @file      Velocity.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-07-15
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */

#pragma once
#include "XYZ_coord.hpp"

struct Velocity {
    double V_x;
    double V_y;
    double V_z;

    Velocity() {
        this->V_x = 0.0;
        this->V_y = 0.0;
        this->V_z = 0.0;
    }

    XYZ_coord operator*(double dt) const{
        XYZ_coord newPosition;
        
        newPosition.X = V_x * dt;
        newPosition.Y = V_y * dt;
        newPosition.Z = V_z * dt;

        return newPosition;
    }

    void operator+=(const Velocity & vel){
        this->V_x += vel.V_x;   
        this->V_y += vel.V_y;
        this->V_z += vel.V_z;
    }
};  