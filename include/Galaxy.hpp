/**
 * @file      Galaxy.hpp
 * @author    Kaiky França dos Reis Silva
 * @brief     Galáxia: bojo + disco exponencial com braços espirais
 * @version   0.2
 */

#pragma once

#include "CelestialBody.hpp"
#include "GalaxyParams.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "./barnes_hut/OctreeNode.hpp"
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

class Galaxy {
    private:

        std::vector<Color> starColors;
        std::vector<CelestialBody> clusterOfStars; // índice 0 = buraco negro (fixo na origem)

        GalaxyParams p;
        NodeArena arena;

        void computeForces() {
            double maxAbs = 1.0;
            for (const auto& s : clusterOfStars) {
                const XyzCoord& pos = s.getBody_coordinates();
                maxAbs = std::max({maxAbs, std::fabs(pos.X), std::fabs(pos.Y), std::fabs(pos.Z)});
            }

            XyzCoord origin;
            
            // MAGIA ACONTECENDO AQUI: Zera toda a árvore do frame anterior em O(1)!
            // O C++ não precisa desalocar nada, ele apenas volta o contador para zero.
            arena.reset();

            // A raiz nasce localmente. A partir dela, todos os filhos virão da arena.
            OctreeNode root(maxAbs * 1.01 + 1.0, origin);

            for (auto& s : clusterOfStars) {
                // Passamos o Banco Central para a árvore se virar
                root.insert(&s, arena); 
            }

            const long n = static_cast<long>(clusterOfStars.size());

            #pragma omp parallel for schedule(dynamic, 256)
            for (long i = 1; i < n; i++) {
                clusterOfStars[i].resetForce();
                
                // O calculateForce não muda! O OpenMP vai apenas ler a árvore que a arena construiu.
                root.calculateForce(&clusterOfStars[i], p.theta, p.G, p.epsilon); 
                
                // 2. Gravidade da Matéria Escura (Continua exatamente igual)
                const XyzCoord& coord = clusterOfStars[i].getBody_coordinates();
                
                double r2 = (coord.X * coord.X) + (coord.Y * coord.Y) + (coord.Z * coord.Z);
                double r2_soft = r2 + (p.haloCoreRadius * p.haloCoreRadius);
                
                double f_mag = p.starMass * (p.haloVelocity * p.haloVelocity) / r2_soft;
                
                clusterOfStars[i].addForce(-f_mag * coord.X, 
                                            -f_mag * coord.Y, 
                                            -f_mag * coord.Z);
            }
        }

        Color generateStarColor(float r, size_t i) const {
            float t = std::min(r / (float)p.maxRadius, 1.0f); 

            float Rc = 255 - 90 * t;
            float Gc = 210 - 15 * t;
            float Bc = 160 + 95 * t;

            float rnd = ((i * 2654435761u) % 1000) / 1000.0f; 

            if (t > 0.20f && rnd > 0.95f) { Rc = 255; Gc = 80; Bc = 200; }

            float bright = 0.5f + 0.5f * rnd * rnd;

            return Color{ (unsigned char)(Rc * bright),
                            (unsigned char)(Gc * bright),
                            (unsigned char)(Bc * bright), 255 };
        }

    public:

        explicit Galaxy(const GalaxyParams& params) : p(params) {
            clusterOfStars.reserve(p.numStars + 1);
            starColors.reserve(p.numStars + 1);

            // ---------- Buraco negro central ----------
            clusterOfStars.emplace_back(XyzCoord(), Velocity(), p.centerMass);
            starColors.push_back(BLANK); // O buraco negro não usa esta cor

            // Variáveis matemáticas da morfologia usando a struct 'p'
            const double kTwoPi        = 2.0 * M_PI;
            const double scaleLength   = p.maxRadius * 0.25;  
            const double bulgeScale    = p.maxRadius * 0.08;  

            std::uniform_real_distribution<double> u01(0.0, 1.0);
            std::uniform_real_distribution<double> uAngle(0.0, kTwoPi);
            std::uniform_real_distribution<double> uCos(-1.0, 1.0);
            std::uniform_int_distribution<int>     uArm(0, p.nArms - 1);
            std::gamma_distribution<double>        diskR(2.0, scaleLength);  
            std::gamma_distribution<double>        bulgeR(2.0, bulgeScale);
            std::normal_distribution<double>       n01(0.0, 1.0);

            std::vector<XyzCoord> pos(p.numStars);

            // ---------- 1) Posições e Cores ----------
            for (unsigned long i = 0; i < p.numStars; i++) {
                XyzCoord coord;

                if (u01(gen) < p.bulgeFraction) {
                    double r;
                    do { r = bulgeR(gen); } while (r < p.innerRadius || r > p.maxRadius);

                    const double cz  = uCos(gen) * 0.6;
                    const double s   = std::sqrt(1.0 - cz * cz);
                    const double phi = uAngle(gen);

                    coord.X = r * s * std::cos(phi);
                    coord.Y = r * s * std::sin(phi);
                    coord.Z = r * cz;
                } else {
                    double R;
                    do { R = diskR(gen); } while (R < p.innerRadius || R > p.maxRadius);

                    double thetaCoord;
                    if (u01(gen) < p.armFraction) {
                        const int arm = uArm(gen);
                        thetaCoord = arm * kTwoPi / p.nArms
                                    - p.armTwist * std::log(R / p.innerRadius)
                                    + p.armSpread * n01(gen);
                    } else {
                        thetaCoord = uAngle(gen); 
                    }

                    const double sigmaZ = p.maxThickness * (0.06 + 0.14 * R / p.maxRadius);
                    double z = n01(gen) * sigmaZ;
                    z = std::clamp(z, -p.maxThickness, p.maxThickness);

                    coord.X = R * std::cos(thetaCoord);
                    coord.Y = R * std::sin(thetaCoord);
                    coord.Z = z;
                }
                pos[i] = coord;
                
                // CACHE: Calcula a cor imediatamente com base na posição e arquiva na RAM
                double radius2D = std::sqrt(coord.X * coord.X + coord.Y * coord.Y);
                starColors.push_back(generateStarColor(radius2D, i + 1));
            }

            // ---------- 2) Massa interna real ----------
            std::vector<double> r3(p.numStars);
            for (unsigned long i = 0; i < p.numStars; i++)
                r3[i] = std::sqrt(pos[i].X * pos[i].X + pos[i].Y * pos[i].Y + pos[i].Z * pos[i].Z);

            std::vector<unsigned long> order(p.numStars);
            std::iota(order.begin(), order.end(), 0UL);
            std::sort(order.begin(), order.end(),
                        [&](unsigned long a, unsigned long b) { return r3[a] < r3[b]; });

            std::vector<double> enclosed(p.numStars);
            for (unsigned long rank = 0; rank < p.numStars; rank++)
                enclosed[order[rank]] = p.centerMass + (rank + 0.5) * p.starMass;

            // ---------- 3) Velocidades ----------
            for (unsigned long i = 0; i < p.numStars; i++) {
                const XyzCoord& coord = pos[i];
                const double R = std::max(std::sqrt(coord.X * coord.X + coord.Y * coord.Y), 1e-3);

                // Cálculo da velocidade de atração do Disco Visível + Buraco Negro
                const double d2 = r3[i] * r3[i] + p.epsilon * p.epsilon;
                const double diskV2 = (p.G * enclosed[i] * R * R) / (d2 * std::sqrt(d2));

                // Cálculo da velocidade de atração da Matéria Escura
                const double haloV2 = p.haloVelocity * p.haloVelocity * R * R / (r3[i] * r3[i] + p.haloCoreRadius * p.haloCoreRadius);

                // A velocidade orbital real é a soma vetorial quadrática das duas forças
                const double vc = std::sqrt(diskV2 + haloV2);

                const double tx = -coord.Y / R, ty = coord.X / R; 
                const double ex =  coord.X / R, ey = coord.Y / R; 

                const double vt = vc * (1.0 + p.sigT * n01(gen));
                const double vr = vc * p.sigR * n01(gen);

                Velocity v;
                v.V_x = tx * vt + ex * vr;
                v.V_y = ty * vt + ey * vr;
                v.V_z = vc * p.sigZ * n01(gen);

                clusterOfStars.emplace_back(coord, v, p.starMass);
            }

            computeForces();
        }

        void updateGalaxy(double dt) {
            const long n = static_cast<long>(clusterOfStars.size());

            #pragma omp parallel for
            for (long i = 1; i < n; i++) {
                clusterOfStars[i].kick(dt * 0.5);
                clusterOfStars[i].drift(dt);
            }

            computeForces();

            #pragma omp parallel for
            for (long i = 1; i < n; i++) {
                clusterOfStars[i].kick(dt * 0.5);
            }
        }


    void drawRaylib(const Camera3D& cam, Texture2D glow) const {
            Vector3 f = { cam.target.x - cam.position.x,
                          cam.target.y - cam.position.y,
                          cam.target.z - cam.position.z };
            float fl = std::sqrt(f.x*f.x + f.y*f.y + f.z*f.z);
            f = Vector3{ f.x/fl, f.y/fl, f.z/fl };

            const float tanHalf = std::tan(cam.fovy * DEG2RAD * 0.5f);
            const float screenH = (float)GetScreenHeight();

            auto project = [&](Vector3 pos3D, Vector2& out, float& pxPerUnit) {
                float depth = (pos3D.x - cam.position.x)*f.x
                            + (pos3D.y - cam.position.y)*f.y
                            + (pos3D.z - cam.position.z)*f.z;
                if (depth < 0.1f) return false;
                out = GetWorldToScreen(pos3D, cam);
                pxPerUnit = screenH / (2.0f * depth * tanHalf);
                return true;
            };

            auto sprite = [&](Vector2 c, float sizePx, Color col) {
                Rectangle src = { 0, 0, (float)glow.width, (float)glow.height };
                Rectangle dst = { c.x, c.y, sizePx, sizePx };
                DrawTexturePro(glow, src, dst, Vector2{ sizePx/2, sizePx/2 }, 0.0f, col);
            };

            BeginBlendMode(BLEND_ADDITIVE);

            for (size_t i = 1; i < clusterOfStars.size(); i++) {
                const auto& s = clusterOfStars[i];
                const XyzCoord& sc = s.getBody_coordinates();
                Vector3 pos = { (float)sc.X, (float)sc.Z, (float)sc.Y };

                Vector2 sp; float ppu;
                if (!project(pos, sp, ppu)) continue;
                if (sp.x < -50 || sp.x > GetScreenWidth()  + 50 ||
                    sp.y < -50 || sp.y > GetScreenHeight() + 50) continue;

                Color c = starColors[i];

                float r = std::sqrt(pos.x*pos.x + pos.z*pos.z);
                float t = std::min(r / (float)p.maxRadius, 1.0f);   

                c.a = (unsigned char)(255 * (0.3f + 0.7f * t));     

                sprite(sp, std::max(1.5f * ppu, 2.5f), c);         
                if (i % 3 == 0) {
                    Color gas = c; gas.a = 6;
                    sprite(sp, 15.0f * ppu, gas);
                }
            }

            Vector2 c0; float ppu0;
            bool coreVisible = project(Vector3{ 0, 0, 0 }, c0, ppu0);
            if (coreVisible) {
                sprite(c0, 250.0f * ppu0, Color{ 255, 80, 20, 20 });
                sprite(c0, 120.0f * ppu0, Color{ 255, 200, 80, 90 });
            }
            EndBlendMode();

            if (coreVisible) {
                BeginMode3D(cam);
                    float time = (float)GetTime();
                    rlPushMatrix();
                        rlRotatef(15.0f, 1.0f, 0.0f, 0.3f);
                        rlRotatef(time * 70.0f, 0.0f, 1.0f, 0.0f);

                        DrawSphereEx(Vector3{0, 0, 0}, 14.0f, 32, 32, BLACK);

                        BeginBlendMode(BLEND_ADDITIVE);
                        DrawCylinder(Vector3{0,0,0}, 18.0f, 18.0f, 1.5f, 32, Color{ 255, 255, 255, 180 });
                        EndBlendMode();
                    rlPopMatrix();
                EndMode3D();
            }
        }
};