#include "raylib.h"

class FallingObject {
public:
    Rectangle rect;
    Vector2 speed;

    FallingObject(float x, float y, float width, float height, float speedX, float speedY);

    void Update();

    void Draw();
};