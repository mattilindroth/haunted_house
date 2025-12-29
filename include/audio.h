#pragma once
#include "raylib.h"

class AudioManager {
public:
    // Sound effects
    Sound throwSound;
    Sound hitSound;
    Sound whooshSound;
    
    // Music
    Music backgroundMusic;
    Music splashMusic;
    
    // Settings
    bool soundEnabled = true;

    // Lifecycle
    void Init();
    void Update();
    void Cleanup();
    
    // Music control
    void PlaySplashMusic();
    void PlayGameMusic();
    void ToggleSound();
    
    // Sound effects
    void PlayThrowSound();
    void PlayHitSound();
    void PlayWhooshSound();
};

// Global audio manager
extern AudioManager audio;
