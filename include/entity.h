#pragma once
#include "raylib.h"
#include "sprite.h"

class Entity {
    public:
        Entity() : rect{0, 0, 0, 0}, speed(0), sprite() {}  // Default constructor
        Entity(Rectangle rect, float speed, const Sprite& sprite)
            : rect(rect), speed(speed), sprite(sprite) {}
        
        
    protected:
        Rectangle rect;
        float speed ;  
        Sprite sprite;
};