#pragma once
#include "raylib.h"
#include "entity.h"
#include "sprite.h"

enum class PlayerState {
    IDLE,
    MOVING,
    DEAD
};

class Player : public Entity {
public:
    float speed = 5.0f;
    int screenWidth = 0;
    PlayerState state = PlayerState::IDLE;

    Player() : Entity() {}  // Default constructor

    Player(float x, float y, float width, float height, int screenWidth, float speed, Sprite sprite) : 
        Entity((Rectangle){x, y, width, height}, speed, sprite) 
    {
        this->screenWidth = screenWidth;
    }

    void SetState(PlayerState newState);

    Rectangle* GetRectangle() {
        return &rect;
    }

    void Update();
    void UpdateSprite(float deltaTime);
    void Draw();
};