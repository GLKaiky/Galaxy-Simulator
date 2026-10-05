#include "./include/Galaxy.hpp"
#include "./include/GalaxyParams.hpp"
#include "./include/raylib.h"
#include <cmath>
#include <algorithm>
#include <string>

int main() {
    const int W =  1280;
    const int H = 720;
    
    // Antialiasing e janela ajustada
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(W, H, "Galaxy Simulator 3D - Advanced Bloom (Barnes-Hut)");
    SetTargetFPS(250);

    // Parâmetros da Galáxia (Aumentamos o disco e a espessura para 3D)
    GalaxyParams spiralPreset;
    spiralPreset.numStars = 50000;

    Galaxy myGalaxy(spiralPreset);
    const int    substeps = 2;
    const double simSpeed = 6.0;

    Camera3D camera = { 0 };
    camera.position   = (Vector3){ 0.0f, 300.0f, 450.0f };
    camera.target     = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy       =   45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    // Textura de sprite Gaussiana/Exponencial para o brilho suave das estrelas
    Image gi = GenImageGradientRadial(128, 128, 0.0f, WHITE, BLACK);
    Texture2D glow = LoadTextureFromImage(gi);
    UnloadImage(gi);
    SetTextureFilter(glow, TEXTURE_FILTER_BILINEAR);

    // Múltiplos canais de Bloom (Cascata de Downsampling)
    RenderTexture2D scene  = LoadRenderTexture(W, H);
    RenderTexture2D bloomA = LoadRenderTexture(W / 2,  H / 2);
    RenderTexture2D bloomB = LoadRenderTexture(W / 8, H / 8);
    RenderTexture2D bloomC = LoadRenderTexture(W / 16, H / 16);
    
    SetTextureFilter(scene.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(bloomA.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(bloomB.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(bloomC.texture, TEXTURE_FILTER_BILINEAR);

    auto blit = [](Texture2D src, float dw, float dh, Color tint) {
        DrawTexturePro(src,
                       (Rectangle){ 0, 0, (float)src.width, -(float)src.height },
                       (Rectangle){ 0, 0, dw, dh },
                       (Vector2){ 0, 0 }, 0.0f, tint);
    };

    while (!WindowShouldClose()) {
        const double frame = std::min((double)GetFrameTime(), 1.0 / 30.0);
        const double h = simSpeed * frame / substeps;

        myGalaxy.updateGalaxy(h);
        UpdateCamera(&camera, CAMERA_ORBITAL);

        // 1. Renderização base da cena
        BeginTextureMode(scene);
            ClearBackground((Color){ 1, 1, 3, 255 }); // Espaço escuro profundo
            myGalaxy.drawRaylib(camera, glow);
        EndTextureMode();

        // 2. Extração de altas frequências (Desfoque A)
        BeginTextureMode(bloomA);
            ClearBackground(BLACK);
            blit(scene.texture, W /  2.0f, H / 2.0f, WHITE);
        EndTextureMode();

        // 3. Desfoque B (Espalhamento largo)
        BeginTextureMode(bloomB);
            ClearBackground(BLACK);
            blit(bloomA.texture, W / 8.0f, H / 8.0f, WHITE);
        EndTextureMode();

        // 4. Desfoque C (Espalhamento global)
        BeginTextureMode(bloomC);
            ClearBackground(BLACK);
            blit(bloomB.texture, W / 16.0f, H / 16.0f, WHITE);
        EndTextureMode();

        // Composição final utilizando Additive Blending Multiplicativo
        BeginDrawing();
            ClearBackground(BLACK);
            blit(scene.texture, (float)W, (float)H, WHITE);

            BeginBlendMode(BLEND_ADDITIVE);
                blit(bloomA.texture, (float)W, (float)H, (Color){ 255, 255, 255, 40 });
                blit(bloomB.texture, (float)W, (float)H, (Color){ 255, 255, 255, 70 });
                blit(bloomC.texture, (float)W, (float)H, (Color){ 255, 255, 255, 90 });
            EndBlendMode();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadRenderTexture(scene);
    UnloadRenderTexture(bloomA);
    UnloadRenderTexture(bloomB);
    UnloadRenderTexture(bloomC);
    UnloadTexture(glow);
    CloseWindow();
    return 0;
}