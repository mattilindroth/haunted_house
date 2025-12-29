#pragma once
#include "raylib.h"
#include <string>
#include <map>
#include <vector>

struct Animation {
    std::vector<Rectangle> frames;
    float frameSpeed;   // frames per second
    int currentFrame = 0;
    float timer = 0.0f;
};

class Sprite {
    
public:
    Sprite() : texture{}, position{0, 0} {}  // Default constructor
    Sprite(Texture2D tex, Vector2 pos) : texture(tex), position(pos) {}

    void addAnimation(const std::string& name, const std::vector<Rectangle>& frames, float speed);

    void setActive(const std::string& name);
    void setPosition(Vector2 pos) { position = pos; }
    void update(float delta);
    void draw();
private:
    Texture2D texture;
    Vector2 position;
    std::map<std::string, Animation> animations;
    std::string activeAnimation;
};
