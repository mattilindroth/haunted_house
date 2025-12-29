#include "falling_object.h"

FallingObject::FallingObject(float x, float y, float width, float height, float speedX, float speedY) {
    rect = { x, y, width, height };
    speed = { speedX, speedY };
}


void FallingObject::Update() {
    // Apply gravity to Y velocity
    speed.y += gravity;
    
    // Update position
    rect.x += speed.x;  // Horizontal movement for angled throws
    rect.y += speed.y;  // Vertical movement (accelerated by gravity)
}

void FallingObject::Draw() {
    // Draw as a book - brown rectangle with darker spine
    DrawRectangleRec(rect, BROWN);
    DrawRectangle((int)rect.x, (int)rect.y, 5, (int)rect.height, DARKBROWN);
    // Draw some page lines
    DrawLine((int)rect.x + 8, (int)rect.y + 5, (int)rect.x + (int)rect.width - 3, (int)rect.y + 5, BEIGE);
    DrawLine((int)rect.x + 8, (int)rect.y + 10, (int)rect.x + (int)rect.width - 3, (int)rect.y + 10, BEIGE);
    DrawLine((int)rect.x + 8, (int)rect.y + 15, (int)rect.x + (int)rect.width - 3, (int)rect.y + 15, BEIGE);
}
