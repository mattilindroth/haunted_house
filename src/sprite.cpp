#include "raylib.h"
#include "sprite.h"
#include <string>
#include <map>
#include <vector>

void Sprite::addAnimation(const std::string& name, const std::vector<Rectangle>& frames, float speed) {
    Animation anim;
    anim.frames = frames;
    anim.frameSpeed = speed;
    animations[name] = anim;
}

void Sprite::setActive(const std::string& name) {
    if (animations.find(name) != animations.end()) {
        activeAnimation = name;
        animations[name].currentFrame = 0;
        animations[name].timer = 0.0f;
    }
}

void Sprite::update(float delta) {
    if (activeAnimation.empty()) return;

    Animation &anim = animations[activeAnimation];
    anim.timer += delta;

    if (anim.timer >= (1.0f / anim.frameSpeed)) {
        anim.timer = 0.0f;
        anim.currentFrame++;
        if (anim.currentFrame >= anim.frames.size()) {
            anim.currentFrame = 0; // loop
        }
    }
}

void Sprite::draw() {
    if (activeAnimation.empty()) return;
    Animation &anim = animations[activeAnimation];
    DrawTextureRec(texture, anim.frames[anim.currentFrame], position, WHITE);
}

