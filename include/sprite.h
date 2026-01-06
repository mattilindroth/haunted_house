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
    Vector2 offset = {0, 0};  // Position offset for this animation
};

class Sprite {
    
public:
    Sprite() : texture{}, position{0, 0}, scale{1.0f, 1.0f}, flipX(false) {}  // Default constructor
    Sprite(Texture2D tex, Vector2 pos) : texture(tex), position(pos), scale{1.0f, 1.0f}, flipX(false) {}

    void addAnimation(const std::string& name, const std::vector<Rectangle>& frames, float speed, Vector2 offset = {0, 0});

    void setActive(const std::string& name);
    void setPosition(Vector2 pos) { position = pos; }
    void setScale(Vector2 s) { scale = s; }
    void setScale(float s) { scale = {s, s}; }
    Vector2 getScale() const { return scale; }
    void setFlipX(bool flip) { flipX = flip; }
    bool getFlipX() const { return flipX; }
    void update(float delta);
    void draw();
    std::string getActiveAnimation() const { return activeAnimation; }
private:
    Texture2D texture;
    Vector2 position;
    Vector2 scale;
    bool flipX;
    std::map<std::string, Animation> animations;
    std::string activeAnimation;
};
