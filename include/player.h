#pragma once
#include "raylib.h"
#include "entity.h"
#include "sprite.h"

class Player : public Entity {
public:
    float speed = 5.0f;
    int screenWidth;

    Player(float x, float y, float width, float height, int screenWidth, float speed, Sprite sprite) : 
        Entity((Rectangle){x, y, width, height}, speed, sprite) 
    {
        this->screenWidth = screenWidth;
    }

    Rectangle* GetRectangle() {
        return &rect;
    }

    void Update();

    void Draw();
};