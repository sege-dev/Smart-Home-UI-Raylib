#include "raylib.h"

Color ColorLerpManual(Color c1, Color c2, float amount) {
    if (amount < 0.0f) amount = 0.0f;
    if (amount > 1.0f) amount = 1.0f;
    return (Color){
        (unsigned char)(c1.r + (c2.r - c1.r) * amount),
        (unsigned char)(c1.g + (c2.g - c1.g) * amount),
        (unsigned char)(c1.b + (c2.b - c1.b) * amount),
        (unsigned char)(c1.a + (c2.a - c1.a) * amount)
    };
}

void DrawModernPanel(Rectangle rect, Color color, float borderSize, Color borderColor) {
    DrawRectangleRounded(rect, 0.15f, 10, color);
    DrawRectangleRoundedLines(rect, 0.15f, 10, borderSize, borderColor);
}

int main() {
    
    const int screenWidth = 1280;
    const int screenHeight = 850;
    const int dashboardWidth = 400;

    InitWindow(screenWidth, screenHeight, "OmniGuard Premium - sege-dev - Fullscreen Room");

    
    Texture2D roomTex = LoadTexture("home.png");
    SetTextureFilter(roomTex, TEXTURE_FILTER_BILINEAR);

    
    float scaleX = (float)screenWidth / (float)roomTex.width;
    float scaleY = (float)screenHeight / (float)roomTex.height;
    
    float finalScale = (scaleX > scaleY) ? scaleX : scaleY; 
    
    
    Vector2 finalPos = { (screenWidth - (roomTex.width * finalScale)) / 2.0f, (screenHeight - (roomTex.height * finalScale)) / 2.0f };

    
    float temp = 22.0f;
    bool lightState = true;
    bool draggingSlider = false;
    Color bgNavy = (Color){ 14, 28, 44, 255 };

    
    Color coldColor = (Color){ 180, 215, 255, 255 };
    Color hotColor = (Color){ 255, 195, 150, 255 };

    
    Rectangle sliderBar = { (float)screenWidth - 350, 450, 300, 10 };
    float sliderKnobX = sliderBar.x + (temp - 10.0f) * (sliderBar.width / 30.0f);

    SetTargetFPS(60);

    
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        
        
        Rectangle lightBtnRect = { (float)screenWidth - 350, 320, 300, 80 };
        if (CheckCollisionPointRec(mousePos, lightBtnRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            lightState = !lightState;
        }

    
        Rectangle knobClickArea = { sliderKnobX - 15, sliderBar.y - 15, 30, 40 };
        
        if (CheckCollisionPointRec(mousePos, knobClickArea) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) draggingSlider = true;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) draggingSlider = false;

        if (draggingSlider) {
            sliderKnobX = mousePos.x;
            if (sliderKnobX < sliderBar.x) sliderKnobX = sliderBar.x;
            if (sliderKnobX > sliderBar.x + sliderBar.width) sliderKnobX = sliderBar.x + sliderBar.width;
            
            float percent = (sliderKnobX - sliderBar.x) / sliderBar.width;
            temp = 10.0f + (percent * 30.0f);
        }

        
        Color atmosColor = WHITE;
        if (temp > 25.0f) atmosColor = ColorLerpManual(WHITE, hotColor, (temp - 25.0f) / 5.0f);
        else if (temp < 19.0f) atmosColor = ColorLerpManual(WHITE, coldColor, (19.0f - temp) / 5.0f);

        
        BeginDrawing();
            
            ClearBackground(bgNavy);
            
            
            DrawCircleGradient(200, 200, 800, (Color){ 30, 55, 90, 150 }, (Color){ 0, 0, 0, 0 });

            
            if (IsTextureReady(roomTex)) {
                
                DrawTextureEx(roomTex, finalPos, 0.0f, finalScale, atmosColor);
            }

            
            int dashX = screenWidth - dashboardWidth;
            
            
            DrawModernPanel((Rectangle){ (float)dashX + 20, 20, 360, 810 }, (Color){ 18, 32, 50, 200 }, 1.5f, (Color){ 255, 255, 255, 20 });
            DrawText("System Controls", dashX + 55, 60, 24, WHITE);

            
            DrawRectangleRounded((Rectangle){ (float)dashX + 50, 120, 300, 170 }, 0.1f, 10, (Color){ 10, 20, 35, 255 });
            DrawText("LIVE FEED: MASTER ROOM", dashX + 70, 140, 11, SKYBLUE);

            
            Color btnColor = CheckCollisionPointRec(mousePos, lightBtnRect) ? (Color){ 45, 65, 105, 180 } : (Color){ 35, 50, 85, 150 };
            DrawModernPanel(lightBtnRect, btnColor, 1.2f, (Color){ 255, 255, 255, 25 });
            DrawText("Smart Lighting", dashX + 75, 345, 18, WHITE);
            DrawCircle(dashX + 315, 360, 12, lightState ? YELLOW : GRAY);
            DrawText(lightState ? "ON" : "OFF", dashX + 300, 385, 12, lightState ? YELLOW : GRAY);

            
            DrawText("Room Temperature", dashX + 55, 420, 16, LIGHTGRAY);
            DrawRectangleRec(sliderBar, DARKGRAY);
            DrawRectangle(sliderBar.x, sliderBar.y, sliderKnobX - sliderBar.x, sliderBar.height, SKYBLUE);
            DrawCircle(sliderKnobX, sliderBar.y + 5, 12, WHITE);
            DrawText(TextFormat("%.1f C", temp), dashX + 280, 420, 16, WHITE);

        
            DrawModernPanel((Rectangle){ 40, 40, 240, 130 }, (Color){ 25, 45, 75, 180 }, 1.2f, (Color){ 255, 255, 255, 35 });
            DrawText("CLIMATE", 60, 55, 11, SKYBLUE);
            DrawText(TextFormat("%.1f C", temp), 60, 75, 36, WHITE);
            DrawCircle(240, 95, 8, (temp > 25.0f ? RED : GREEN));

            
            DrawModernPanel((Rectangle){ 40, 755, 300, 65 }, (Color){ 20, 40, 70, 180 }, 1.0f, (Color){ 255, 255, 255, 25 });
            DrawCircle(65, 785, 18, DARKGRAY);
            DrawText("sege-dev", 100, 770, 18, WHITE);
            DrawText("Status: Online", 100, 790, 12, GREEN);

        EndDrawing();
    }

    
    UnloadTexture(roomTex);
    CloseWindow();

    return 0;
}
