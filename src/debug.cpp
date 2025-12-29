#include "debug.h"

// Global instance
DebugMode debug;

void DebugMode::Toggle() {
    enabled = !enabled;
    if (enabled) {
        currentFrame = 0;
    }
}

void DebugMode::Update() {
    if (!enabled || !angryFrames || !idleFrames) return;
    
    std::vector<Rectangle>& currentFrames = showAngry ? *angryFrames : *idleFrames;
    
    // TAB to switch between idle/angry
    if (IsKeyPressed(KEY_TAB)) {
        showAngry = !showAngry;
        currentFrame = 0;
    }
    
    // LEFT/RIGHT or A/D to step through frames
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        currentFrame = (currentFrame + 1) % (int)currentFrames.size();
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        currentFrame = (currentFrame - 1 + (int)currentFrames.size()) % (int)currentFrames.size();
    }
    
    // Number keys 1-4 to jump to specific frame
    if (IsKeyPressed(KEY_ONE)) currentFrame = 0;
    if (IsKeyPressed(KEY_TWO) && currentFrames.size() > 1) currentFrame = 1;
    if (IsKeyPressed(KEY_THREE) && currentFrames.size() > 2) currentFrame = 2;
    if (IsKeyPressed(KEY_FOUR) && currentFrames.size() > 3) currentFrame = 3;
}

void DebugMode::Draw() {
    if (!enabled || !angryFrames || !idleFrames || !texture) return;
    
    std::vector<Rectangle>& currentFrames = showAngry ? *angryFrames : *idleFrames;
    const char* animName = showAngry ? "ANGRY" : "IDLE";
    
    // Draw title
    DrawText("FRAME DEBUG MODE", 20, 20, 30, YELLOW);
    DrawText(TextFormat("Animation: %s", animName), 20, 60, 24, WHITE);
    DrawText(TextFormat("Frame: %d / %d", currentFrame + 1, (int)currentFrames.size()), 20, 90, 24, WHITE);
    
    // Draw frame rectangle info
    Rectangle frame = currentFrames[currentFrame];
    DrawText(TextFormat("X: %.1f  Y: %.1f", frame.x, frame.y), 20, 130, 20, LIGHTGRAY);
    DrawText(TextFormat("W: %.1f  H: %.1f", frame.width, frame.height), 20, 155, 20, LIGHTGRAY);
    
    // Draw the sprite frame at center of screen, scaled up for inspection
    float scale = 4.0f;
    float drawX = screenWidth / 2 - (frame.width * scale) / 2;
    float drawY = screenHeight / 2 - (frame.height * scale) / 2;
    
    // Draw frame scaled up
    DrawTexturePro(*texture, frame,
        { drawX, drawY, frame.width * scale, frame.height * scale },
        { 0, 0 }, 0, WHITE);
    
    // Draw border around the frame
    DrawRectangleLinesEx({ drawX - 2, drawY - 2, frame.width * scale + 4, frame.height * scale + 4 }, 2, RED);
    
    // Draw original size on the side
    DrawText("Original size:", 20, 220, 18, GRAY);
    DrawTexturePro(*texture, frame,
        { 20, 245, frame.width, frame.height },
        { 0, 0 }, 0, WHITE);
    DrawRectangleLinesEx({ 18, 243, frame.width + 4, frame.height + 4 }, 1, GRAY);
    
    // Draw controls
    DrawText("Controls:", 20, screenHeight - 140, 20, WHITE);
    DrawText("LEFT/RIGHT or A/D: Step frames", 20, screenHeight - 115, 16, LIGHTGRAY);
    DrawText("1-4: Jump to frame", 20, screenHeight - 95, 16, LIGHTGRAY);
    DrawText("TAB: Switch idle/angry", 20, screenHeight - 75, 16, LIGHTGRAY);
    DrawText("F1: Exit debug mode", 20, screenHeight - 55, 16, LIGHTGRAY);
}
