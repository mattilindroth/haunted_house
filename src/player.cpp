#include "player.h"

// Player::Player(float x, float y, float width, float height, int screenWidth, float speed, Sprite sprite) {
//     this->rect.x = x;
//     this->rect.y = y;
//     this->rect.width = width;
//     this->rect.height = height;
//     this->screenWidth = screenWidth;
//     this->speed = speed;
//     this->sprite = sprite;
// }

void Player::SetState(PlayerState newState) {
    if (state != newState) {
        state = newState;
        switch (state) {
            case PlayerState::IDLE:
                sprite.setActive("idle");
                break;
            case PlayerState::MOVING:
                sprite.setActive("move");
                break;
            case PlayerState::DEAD:
                sprite.setActive("dead");
                break;
        }
    }
}

void Player::Update() {
    bool isMoving = false;
    
    if (IsKeyDown(KEY_LEFT)) {
        rect.x -= speed;
        isMoving = true;
        sprite.setFlipX(false);  // Face left (original orientation)
    }
    if (IsKeyDown(KEY_RIGHT)) {
        rect.x += speed;
        isMoving = true;
        sprite.setFlipX(true);   // Face right (flipped)
    }

    // Update animation state based on movement
    if (state != PlayerState::DEAD) {
        if (isMoving) {
            SetState(PlayerState::MOVING);
        } else {
            SetState(PlayerState::IDLE);
        }
    }

    // Boundary check for player movement
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.width > screenWidth) rect.x = screenWidth - rect.width;

    // Update sprite animation
    sprite.setPosition({rect.x, rect.y});
    sprite.update(GetFrameTime());
}

void Player::UpdateSprite(float deltaTime) {
    sprite.update(deltaTime);
}

void Player::Draw() {
    sprite.draw();
}