#include "audio.h"
#include <cstdlib>

// Global instance
AudioManager audio;

void AudioManager::Init() {
    InitAudioDevice();
    
    // Load sound effects
    throwSound = LoadSound("../resources/throw.mp3");
    hitSound = LoadSound("../resources/hit.mp3");
    whooshSound = LoadSound("../resources/whoosh.mp3");
    deathScream = LoadSound("../resources/death-scream.mp3");
    
    // Load music
    backgroundMusic = LoadMusicStream("../resources/spooky_music.mp3");
    SetMusicVolume(backgroundMusic, 0.3f);
    
    splashMusic = LoadMusicStream("../resources/horror-piano.mp3");
    SetMusicVolume(splashMusic, 0.4f);
    
    // Start with splash music
    PlayMusicStream(splashMusic);
}

void AudioManager::Update() {
    if (soundEnabled) {
        UpdateMusicStream(backgroundMusic);
        UpdateMusicStream(splashMusic);
    }
}

void AudioManager::Cleanup() {
    UnloadSound(throwSound);
    UnloadSound(hitSound);
    UnloadSound(whooshSound);
    UnloadSound(deathScream);
    UnloadMusicStream(backgroundMusic);
    UnloadMusicStream(splashMusic);
    CloseAudioDevice();
}

void AudioManager::PlaySplashMusic() {
    if (soundEnabled) {
        StopMusicStream(backgroundMusic);
        PlayMusicStream(splashMusic);
    }
}

void AudioManager::PlayGameMusic() {
    if (soundEnabled) {
        StopMusicStream(splashMusic);
        PlayMusicStream(backgroundMusic);
    }
}

void AudioManager::ToggleSound() {
    soundEnabled = !soundEnabled;
    if (soundEnabled) {
        ResumeMusicStream(backgroundMusic);
        ResumeMusicStream(splashMusic);
    } else {
        PauseMusicStream(backgroundMusic);
        PauseMusicStream(splashMusic);
    }
}

void AudioManager::PlayThrowSound() {
    if (soundEnabled && throwSound.frameCount > 0) {
        SetSoundPitch(throwSound, 0.9f + (float)(rand() % 20) / 100.0f);
        PlaySound(throwSound);
    }
}

void AudioManager::PlayHitSound() {
    if (soundEnabled && hitSound.frameCount > 0) {
        PlaySound(hitSound);
    }
}

void AudioManager::PlayWhooshSound() {
    if (soundEnabled && whooshSound.frameCount > 0) {
        SetSoundPitch(whooshSound, 0.85f + (float)(rand() % 30) / 100.0f);
        PlaySound(whooshSound);
    }
}

void AudioManager::PlayDeathScream() {
    if (soundEnabled && deathScream.frameCount > 0) {
        PlaySound(deathScream);
    }
}
