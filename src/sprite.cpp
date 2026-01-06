#include "raylib.h"
#include "sprite.h"
#include <string>
#include <map>
#include <vector>
#include <cmath>

void Sprite::addAnimation(const std::string& name, const std::vector<Rectangle>& frames, float speed, Vector2 offset) {
    Animation anim;
    anim.frames = frames;
    anim.frameSpeed = speed;
    anim.offset = offset;
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
    Rectangle source = anim.frames[anim.currentFrame];
    
    // Flip horizontally by using negative width in source rectangle
    if (flipX) {
        source.width = -source.width;
    }
    
    Rectangle dest = {
        position.x + anim.offset.x * scale.x,
        position.y + anim.offset.y * scale.y,
        std::abs(source.width) * scale.x,
        source.height * scale.y
    };
    DrawTexturePro(texture, source, dest, {0, 0}, 0.0f, WHITE);
}

