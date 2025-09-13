#include "falling_object.h"

FallingObject::FallingObject(float x, float y, float width, float height, float speedX, float speedY) {
    rect = { x, y, width, height };
    speed = { speedX, speedY };
}


void FallingObject::Update() {
    rect.y += speed.y;
}

void FallingObject::Draw() {
    DrawRectangleRec(rect, DARKGRAY);
}
