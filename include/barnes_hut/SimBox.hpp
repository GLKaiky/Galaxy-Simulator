/**
 * @file      BoundingBox.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-07-24
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */

#pragma once
#include "../physics/XyzCoord.hpp"

struct SimBox 
{
    XyzCoord center;
    double radius;

    SimBox() { }
    
    SimBox(XyzCoord center, double radius) {
        this->center = center;
        this->radius = radius;
    }

    bool contains(const XyzCoord& body_coordinates) const{    
        return(body_coordinates.X >= (center.X - radius) && body_coordinates.X <= (center.X + radius) &&
            body_coordinates.Y >= (center.Y - radius) && body_coordinates.Y <= (center.Y + radius) &&
            body_coordinates.Z >= (center.Z - radius) && body_coordinates.Z <= (center.Z + radius)
            ); 
    }
};
