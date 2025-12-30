#pragma once
#include "raylib.h"
#include <string>
#include <iostream>

class SplashScreen {
public:
    /**
     * Constructor for the SplashScreen class.
     * Initializes the splash screen with the specified texture and timing parameters.
     * @param texturePath Path to the splash screen texture.
     * @param displayTime Time to display the splash screen.
     * @param fadeInTime Time for the splash screen to fade in.
     * @param fadeOutTime Time for the splash screen to fade out.
     */
    SplashScreen(std::string texturePath, float displayTime, float fadeInTime, float fadeOutTime);

    /**
     * Updates the splash screen state.
     */
    void Update();

    /**
     * Draws the splash screen.
     */
    void Draw();

    /**
     * Checks if the splash screen has finished displaying.
     * @return true if the splash screen is finished, false otherwise.
     */
    bool IsFinished();

private:
    Texture2D splashTexture;
    float displayTime;
    float fadeInTime;
    float fadeOutTime;
    float currentAlpha = 0.0f;
    bool isFinished = false;
};
