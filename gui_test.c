#include "raylib.h"

// Glassmorphism Panelleri
void DrawModernPanel(Rectangle rect, Color color, float borderSize, Color borderColor) {
    DrawRectangleRounded(rect, 0.15f, 10, color);
    DrawRectangleRoundedLines(rect, 0.15f, 10, borderSize, borderColor);
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 850;
    const int dashboardWidth = 400;

    // Resmindeki lacivert ton
    Color bgNavy = (Color){ 14, 28, 44, 255 };

    InitWindow(screenWidth, screenHeight, "OmniGuard Premium - Final Layout");

    Texture2D roomTex = LoadTexture("home.png");
    SetTextureFilter(roomTex, TEXTURE_FILTER_BILINEAR);

    // Resmi orantılı ölçekle ve hafif sola yasla
    float scale = (float)(screenHeight - 60) / (float)roomTex.height;
    Vector2 roomPos = { 30, (screenHeight - (roomTex.height * scale)) / 2 };

    float temp = 22.0f;
    bool lightState = true;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Kontroller
        if (IsKeyDown(KEY_RIGHT)) temp += 0.05f;
        if (IsKeyDown(KEY_LEFT)) temp -= 0.05f;
        if (IsKeyPressed(KEY_L)) lightState = !lightState;

        BeginDrawing();
            ClearBackground(bgNavy);
            
            // Işık Efekti
            DrawCircleGradient(200, 200, 800, (Color){ 30, 55, 90, 150 }, (Color){ 0, 0, 0, 0 });

            // 1. Odayı Çiz (Atmosfer Filtreli)
            Color atmosColor = WHITE;
            if (temp > 25.0f) atmosColor = (Color){ 255, 200, 160, 255 };
            else if (temp < 19.0f) atmosColor = (Color){ 180, 210, 255, 255 };

            if (IsTextureReady(roomTex)) {
                DrawTextureEx(roomTex, roomPos, 0.0f, scale, atmosColor);
            }

            // 2. DASHBOARD: Tam Sağda ve Üstte
            Rectangle dashRect = { (float)screenWidth - dashboardWidth, 0, (float)dashboardWidth, (float)screenHeight };
            // Panel hafif şeffaf, arkadaki evren hissediliyor
            DrawModernPanel((Rectangle){ dashRect.x + 20, 20, dashRect.width - 40, dashRect.height - 40 }, 
                           (Color){ 18, 32, 50, 200 }, 1.5f, (Color){ 255, 255, 255, 25 });

            // Dashboard İçeriği
            DrawText("System Controls", dashRect.x + 50, 60, 24, WHITE);
            
            // CCTV Ekranı
            DrawRectangleRounded((Rectangle){ dashRect.x + 50, 120, 300, 180 }, 0.1f, 10, (Color){ 10, 20, 30, 255 });
            DrawText("LIVE FEED: GARDEN", dashRect.x + 70, 140, 12, SKYBLUE);

            // Cihaz Kartları
            DrawModernPanel((Rectangle){ dashRect.x + 50, 320, 300, 85 }, (Color){ 35, 50, 85, 150 }, 1.0f, (Color){ 255, 255, 255, 20 });
            DrawText("Smart Lighting", dashRect.x + 70, 350, 18, WHITE);
            DrawCircle(dashRect.x + 310, 362, 12, lightState ? YELLOW : GRAY);
            DrawText(lightState ? "ON" : "OFF", dashRect.x + 300, 385, 12, lightState ? YELLOW : GRAY);

            // Klima Kartı (Sol Üst Yüzen Panel)
            DrawModernPanel((Rectangle){ 40, 40, 240, 130 }, (Color){ 25, 45, 75, 180 }, 1.2f, (Color){ 255, 255, 255, 35 });
            DrawText("CLIMATE", 60, 55, 12, SKYBLUE);
            DrawText(TextFormat("%.1f C", temp), 60, 75, 34, WHITE);
            DrawCircle(240, 95, 8, (temp > 25 ? RED : GREEN));

            // Alt Profil Barı
            DrawModernPanel((Rectangle){ 40, (float)screenHeight - 100, 300, 70 }, (Color){ 20, 40, 70, 150 }, 1.0f, (Color){ 255, 255, 255, 25 });
            DrawCircle(75, screenHeight - 65, 20, DARKGRAY);
            DrawText("Robert T.", 110, screenHeight - 75, 18, WHITE);
            DrawText("At Home", 110, screenHeight - 55, 13, GREEN);

        EndDrawing();
    }

    UnloadTexture(roomTex);
    CloseWindow();
    return 0;
}