#pragma once
#include "raylib.h"
#include "entity.h"
#include "sprite.h"
#include "falling_object.h"
#include <vector>

enum class GhostState {
    IDLE,
    GETTING_ANGRY,
    THROWING
};

class Ghost : public Entity {
public:
    float moveSpeed = 8.0f;          // Fast supernatural movement
    float throwCooldown = 2.0f;       // Seconds between throws
    float timeSinceLastThrow = 0.0f;
    float angryAnimationTime = 0.0f;  // Time spent in angry animation
    float angryAnimationDuration = 0.5f;  // How long to play angry animation before throwing
    int screenWidth;
    int screenHeight;
    int direction = 1;                // 1 = right, -1 = left
    GhostState state = GhostState::IDLE;
    bool bookReady = false;           // Flag to signal book should be thrown

    Ghost();
    Ghost(float x, float y, float width, float height, int screenWidth, int screenHeight, float speed, Sprite sprite);

    Rectangle* GetRectangle() { return &rect; }
    
    // Get the sprite for adding animations
    Sprite& GetSprite() { return sprite; }

    // Update ghost movement and check if it should throw
    void Update(float deltaTime);

    // Throw a book - returns a FallingObject aimed at target position
    FallingObject ThrowBook(Vector2 targetPos);

    // Check if ghost is ready to throw (finished angry animation)
    bool CanThrow() const { return bookReady; }

    // Reset throw timer after throwing
    void ResetThrowTimer();

    void Draw();
};
