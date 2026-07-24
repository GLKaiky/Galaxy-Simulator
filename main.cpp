#include <iostream>
#include "include/CelestialBody.hpp"

int main () {
    CelestialBody star(50.0, 100.0);

    for(int i = 0; i<10; i++) {
        std::cout << "Frame " << i << " | X: " << star.getBody_coordinates().X << " | Y: " << star.getBody_coordinates().Y << "\n";
        star.setAppliedForcetoCenter();
        star.updateMovement(0.1);

    }
}