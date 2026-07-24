/**
 * @file      Galaxy.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     
 * @version   0.1
 * @date      2026-07-24
 * @copyright Copyright (c) 2026 Kaiky França dos Reis Silva
 */

#pragma once

#include "CelestialBody.hpp"
#include <vector>
class Galaxy {
    private:
        std::vector<CelestialBody> clusterOfStars;
        unsigned long numberofStars;
    
    public:
        
        Galaxy(unsigned long numberofStars, double maxThickness, double maxRadius) { //Big Bang de estrelas, uma galaxia explode e nasce
            this->numberofStars = numberofStars;

            clusterOfStars.reserve(numberofStars +1); //reserva extra para o buraco negro
            
            /*Definição do Buraco Negro, uma estrela central, 
            ultra massiva (maior do que qualquer coisa que 
            a galáxia consiga criar)*/
            XYZ_coord centerCoord; 
            centerCoord.X = 0.0; centerCoord.Y = 0.0; centerCoord.Z = 0.0;

            Velocity centerVel; 
            centerVel.V_x = 0.0; centerVel.V_y = 0.0; centerVel.V_z = 0.0;

            double centerMass = 100000.0;

            clusterOfStars.emplace_back(centerCoord, centerVel, centerMass); 

            for(unsigned long i = 0; i<numberofStars; i++) {
                clusterOfStars.emplace_back(maxThickness, maxRadius);
            }
        }

        void updateGalaxy(double dt) {
            
            for(auto& star: this->clusterOfStars) {
                star.updateMovement(dt);
            }
        }


};