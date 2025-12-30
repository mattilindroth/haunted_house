#include "ghost.h"
#include <cmath>

Ghost::Ghost() : Entity() {
    screenWidth = 800;
    screenHeight = 600;
    state = GhostState::IDLE;
    bookReady = false;
}

Ghost::Ghost(float x, float y, float width, float height, int screenWidth, int screenHeight, float speed, Sprite sprite)
    : Entity((Rectangle){x, y, width, height}, speed, sprite) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->moveSpeed = speed;
    state = GhostState::IDLE;
    bookReady = false;
}

void Ghost::Update(float deltaTime) {
    // Update throw cooldown when idle
    if (state == GhostState::IDLE) {
        timeSinceLastThrow += deltaTime;
        
        // Time to get angry and throw?
        if (timeSinceLastThrow >= throwCooldown) {
            state = GhostState::GETTING_ANGRY;
            angryAnimationTime = 0.0f;
            sprite.setActive("angry");
        }
    }
    else if (state == GhostState::GETTING_ANGRY) {
        angryAnimationTime += deltaTime;
        
        // Finished angry animation - ready to throw!
        if (angryAnimationTime >= angryAnimationDuration) {
            state = GhostState::THROWING;
            bookReady = true;
        }
    }
    // THROWING state is handled by main.cpp calling ThrowBook() and ResetThrowTimer()

    // Move ghost horizontally with supernatural speed (slower when angry)
    float currentSpeed = (state == GhostState::GETTING_ANGRY) ? moveSpeed * 0.3f : moveSpeed;
    rect.x += currentSpeed * direction;

    // Bounce off screen edges
    if (rect.x <= 0) {
        rect.x = 0;
        direction = 1;
    }
    if (rect.x + rect.width >= screenWidth) {
        rect.x = screenWidth - rect.width;
        direction = -1;
    }

    // Add some erratic movement - occasionally change direction randomly (only when idle)
    if (state == GhostState::IDLE && GetRandomValue(0, 100) < 2) {
        direction *= -1;
    }

    // Update sprite position and animation
    sprite.setPosition({rect.x, rect.y});
    sprite.update(deltaTime);
}

void Ghost::ResetThrowTimer() {
    timeSinceLastThrow = 0.0f;
    bookReady = false;
    state = GhostState::IDLE;
    sprite.setActive("idle");
}

FallingObject Ghost::ThrowBook(Vector2 targetPos) {
    // Calculate throw origin (bottom center of ghost)
    float ghostCenterX = rect.x + rect.width / 2;
    float ghostCenterY = rect.y + rect.height;

    // Calculate distance to target
    float dx = targetPos.x - ghostCenterX;
    float dy = targetPos.y - ghostCenterY;
    
    // Calculate time to reach player based on gravity
    // Using physics: dy = vy*t + 0.5*g*t^2, we estimate time
    // For simplicity, we use a base flight time and calculate required velocities
    float gravity = 0.15f;  // Must match FallingObject gravity
    float flightTime = 45.0f;  // Frames to reach player (approximately)
    
    // Calculate initial velocities to hit the target
    // vx = dx / t (constant horizontal velocity)
    // vy = (dy - 0.5*g*t^2) / t (initial vertical velocity accounting for gravity)
    float speedX = dx / flightTime;
    float speedY = (dy - 0.5f * gravity * flightTime * flightTime) / flightTime;
    
    // Add randomness for inaccurate throws (ghost is angry, not precise!)
    float randomX = (float)(GetRandomValue(-30, 30)) / 10.0f;  // -3 to +3
    float randomY = (float)(GetRandomValue(-10, 10)) / 10.0f;  // -1 to +1
    
    speedX += randomX;
    speedY += randomY;
    
    // Clamp minimum Y speed so books always fall eventually
    if (speedY < 1.0f) speedY = 1.0f;

    // Create book at ghost's position
    FallingObject book(
        ghostCenterX - 15,  // Center the book
        ghostCenterY,       // Start from bottom of ghost
        30,                 // Book width
        40,                 // Book height
        speedX,
        speedY
    );

    return book;
}

void Ghost::Draw() {
    sprite.draw();
}
