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

void Player::Update() {
    if (IsKeyDown(KEY_LEFT)) rect.x -= speed;
    if (IsKeyDown(KEY_RIGHT)) rect.x += speed;

    // Boundary check for player movement
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.width > screenWidth) rect.x = screenWidth - rect.width;

    // Update sprite animation
    sprite.setPosition({rect.x, rect.y});
    sprite.update(GetFrameTime());
}

void Player::Draw() {
    sprite.draw();
}