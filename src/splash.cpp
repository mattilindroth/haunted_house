#include "splash.h"

SplashScreen::SplashScreen(std::string texturePath, float displayTime, float fadeInTime, float fadeOutTime) {
    // Load splash screen assets here
    Image image = LoadImage(texturePath.c_str());
    splashTexture = LoadTextureFromImage(image);
    UnloadImage(image);  // Unload the image after creating the texture

    this->displayTime = displayTime;
    this->fadeInTime = fadeInTime;
    this->fadeOutTime = fadeOutTime;
}

void SplashScreen::Update() {
    float currentTime = GetTime();
    if (currentTime < fadeInTime) {
        // Fade in
        currentAlpha = currentTime / fadeInTime;
    } else if (currentTime < fadeInTime + displayTime) {
        // Display
        currentAlpha = 1.0f;
    } else if (currentTime < fadeInTime + displayTime + fadeOutTime) {
        // Fade out
        currentAlpha = 1.0f - (currentTime - fadeInTime - displayTime) / fadeOutTime;
    } else {
        // Splash screen is done
        currentAlpha = 0.0f;
        isFinished = true;
    }
}

void SplashScreen::Draw() {
    // Draw splash screen with alpha blending
    Rectangle src = { 0, 0, (float)splashTexture.width, (float)splashTexture.height };
    Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0, 0 };
    Color tint = {255, 255, 255, (unsigned char)(currentAlpha * 255)};
    DrawTexturePro(splashTexture, src, dest, origin, 0.0f, tint);
}

bool SplashScreen::IsFinished() {
    return isFinished;
}