#pragma once
#include "raylib.h"
#include "sprite.h"

class Entity {
    public:
        Entity(Rectangle rect, float speed, const Sprite& sprite)
            : rect(rect), speed(speed), sprite(sprite) {}
        
        
    protected:
        Rectangle rect;
        float speed ;  
        Sprite sprite;
};