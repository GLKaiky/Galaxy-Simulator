/**
 * @file      CelestialBody.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     Corpo celeste (estrela / buraco negro)
 * @version   0.2
 */

#pragma once
#include <cmath>

#include "./physics/XyzCoord.hpp"
#include "./physics/Force.hpp"
#include "./physics/Velocity.hpp"

class CelestialBody {
    private:
        double mass = 1.0;
        XyzCoord body_coordinates;
        Force appliedForce;
        Velocity velocity;

    public:
        CelestialBody() = default;

        CelestialBody(const XyzCoord& coords, const Velocity& vel, double m)
            : mass(m), body_coordinates(coords), velocity(vel) {}

        double getMass() const { return mass; }
        const XyzCoord& getBody_coordinates() const { return body_coordinates; }
        const Force& getAppliedForce() const { return appliedForce; }
        const Velocity& getVelocity() const { return velocity; }

        void resetForce() { appliedForce.reset(); }

        // Meio "chute" de velocidade (leapfrog): v += a * h
        void kick(double h) {
            velocity += appliedForce.calculateDeltaV(mass, h);
        }

        // Deriva: x += v * dt
        void drift(double dt) {
            body_coordinates += velocity * dt;
        }

        // Força gravitacional (com softening) de uma massa pontual qualquer.
        // Serve tanto para outro corpo quanto para o centro de massa de um nó da árvore.
        void applyPointMassForce(const XyzCoord& p, double m, double G, double epsilon) {
            const double dx = p.X - body_coordinates.X;
            const double dy = p.Y - body_coordinates.Y;
            const double dz = p.Z - body_coordinates.Z;

            const double distSq = dx * dx + dy * dy + dz * dz + epsilon * epsilon;
            const double dist = std::sqrt(distSq);
            const double s = (G * mass * m) / (distSq * dist);

            Force f;
            f.F_x = s * dx;
            f.F_y = s * dy;
            f.F_z = s * dz;
            appliedForce += f;
        }

        void applyGravitationalForce(const CelestialBody& other, double G, double epsilon) {
            if (this == &other) return;
            applyPointMassForce(other.body_coordinates, other.mass, G, epsilon);
        }

        void addForce(double x, double y, double z) {
            this->appliedForce.F_x = x;
            this->appliedForce.F_y = y;
            this->appliedForce.F_z = z;
            
        }

};