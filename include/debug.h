#pragma once
#include "raylib.h"
#include <vector>

class DebugMode {
public:
    bool enabled = false;
    int currentFrame = 0;
    bool showAngry = true;
    
    // References to frame data (set from Game)
    std::vector<Rectangle>* angryFrames = nullptr;
    std::vector<Rectangle>* idleFrames = nullptr;
    Texture2D* texture = nullptr;
    int screenWidth = 800;
    int screenHeight = 600;
    
    void Toggle();
    void Update();
    void Draw();
    
    bool IsActive() const { return enabled; }
};

// Global debug mode instance
extern DebugMode debug;
