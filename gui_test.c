#include "raylib.h"

Color LerpColorManual(Color c1, Color c2, float amount) {
    if (amount < 0.0f) amount = 0.0f;
    if (amount > 1.0f) amount = 1.0f;
    return (Color){
        (unsigned char)(c1.r + (c2.r - c1.r) * amount),
        (unsigned char)(c1.g + (c2.g - c1.g) * amount),
        (unsigned char)(c1.b + (c2.b - c1.b) * amount),
        (unsigned char)(c1.a + (c2.a - c1.a) * amount)
    };
}

Color TintColorManual(Color base, Color tint) {
    return (Color){
        (unsigned char)((float)base.r * (float)tint.r / 255.0f),
        (unsigned char)((float)base.g * (float)tint.g / 255.0f),
        (unsigned char)((float)base.b * (float)tint.b / 255.0f),
        (unsigned char)((float)base.a * (float)tint.a / 255.0f)
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
    const int availableWidth = screenWidth - dashboardWidth;

    InitWindow(screenWidth, screenHeight, "OmniGuard Premium - Seamless Fix");

    Texture2D roomTex = LoadTexture("home.png");
    SetTextureFilter(roomTex, TEXTURE_FILTER_BILINEAR);

    float scaleX = (float)availableWidth / (float)roomTex.width;
    float scaleY = (float)screenHeight / (float)roomTex.height;
    float finalScale = ((scaleX < scaleY) ? scaleX : scaleY) * 1.15f; 
    
    Vector2 finalPos = { 
        (availableWidth - (roomTex.width * finalScale)) / 2.0f, 
        (screenHeight - (roomTex.height * finalScale)) / 2.0f 
    };

    float temp = 22.0f;
    bool draggingSlider = false;
    Color bgNavyBase = (Color){ 14, 28, 44, 255 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        
        Rectangle sliderBar = { (float)screenWidth - 350, 450, 300, 10 };
        static float sliderKnobX = 0; 
        if (sliderKnobX == 0) sliderKnobX = sliderBar.x + (temp - 10.0f) * (sliderBar.width / 30.0f);

        Rectangle knobClickArea = { sliderKnobX - 15, sliderBar.y - 15, 30, 40 };
        if (CheckCollisionPointRec(mousePos, knobClickArea) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) draggingSlider = true;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) draggingSlider = false;

        if (draggingSlider) {
            sliderKnobX = mousePos.x;
            if (sliderKnobX < sliderBar.x) sliderKnobX = sliderBar.x;
            if (sliderKnobX > sliderBar.x + sliderBar.width) sliderKnobX = sliderBar.x + sliderBar.width;
            temp = 10.0f + ((sliderKnobX - sliderBar.x) / sliderBar.width) * 30.0f;
        }

        Color hotColor = (Color){ 255, 195, 150, 255 };
        Color coldColor = (Color){ 180, 215, 255, 255 };
        Color atmosColor = WHITE;
        if (temp > 25.0f) atmosColor = LerpColorManual(WHITE, hotColor, (temp - 25.0f) / 5.0f);
        else if (temp < 19.0f) atmosColor = LerpColorManual(WHITE, coldColor, (19.0f - temp) / 5.0f);

        Color dynamicBG = TintColorManual(bgNavyBase, atmosColor);

        BeginDrawing();
            ClearBackground(dynamicBG);
            
            if (IsTextureReady(roomTex)) {
                DrawTextureEx(roomTex, finalPos, 0.0f, finalScale, atmosColor);
                
                float rWidth = roomTex.width * finalScale;
                float rHeight = roomTex.height * finalScale;
                int fadeSize = 40;

                DrawRectangleGradientH(finalPos.x, finalPos.y, fadeSize, rHeight, dynamicBG, (Color){0,0,0,0});
                DrawRectangleGradientH(finalPos.x + rWidth - fadeSize, finalPos.y, fadeSize, rHeight, (Color){0,0,0,0}, dynamicBG);
                DrawRectangleGradientV(finalPos.x, finalPos.y, rWidth, fadeSize, dynamicBG, (Color){0,0,0,0});
                DrawRectangleGradientV(finalPos.x, finalPos.y + rHeight - fadeSize, rWidth, fadeSize, (Color){0,0,0,0}, dynamicBG);
            }

            int dashX = screenWidth - dashboardWidth;
            DrawModernPanel((Rectangle){ (float)dashX + 20, 20, 360, 810 }, (Color){ 10, 25, 45, 200 }, 1.5f, (Color){ 255, 255, 255, 20 });
            DrawText("System Controls", dashX + 55, 60, 24, WHITE);
            
            DrawRectangleRec(sliderBar, DARKGRAY);
            DrawRectangle(sliderBar.x, sliderBar.y, sliderKnobX - sliderBar.x, sliderBar.height, SKYBLUE);
            DrawCircle(sliderKnobX, sliderBar.y + 5, 12, WHITE);
            DrawText(TextFormat("%.1f C", temp), dashX + 280, 420, 16, WHITE);

            DrawModernPanel((Rectangle){ 40, 40, 240, 100 }, (Color){ 20, 40, 60, 220 }, 1.2f, (Color){ 255, 2
