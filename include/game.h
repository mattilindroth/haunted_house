#pragma once
#include "raylib.h"
#include "falling_object.h"
#include "player.h"
#include "ghost.h"
#include "splash.h"
#include "sprite.h"
#include "scoreboard.h"

#include <vector>

enum GameState {
    MENU,
    PLAYING,
    DYING,
    GAME_OVER,
    SPLASH
};

class Game {
public:
    // Screen dimensions (internal game resolution)
    static const int screenWidth = 1280;
    static const int screenHeight = 800;
    
    // Fullscreen rendering
    RenderTexture2D targetTexture;
    float renderScale = 1.0f;
    Vector2 renderOffset = {0, 0};

    // Game state
    GameState currentState = SPLASH;
    
    // Background
    Texture2D backgroundTexture;

    // Ghost enemy
    Texture2D ghostTexture;
    Sprite ghostSprite;
    Ghost ghost;
    
    // Player
    Texture2D playerTexture;
    Sprite playerSprite;
    Player player;
    
    // Splash screen
    SplashScreen* splashScreen = nullptr;
    
    // Falling objects
    std::vector<FallingObject> fallingObjects;
    
    // Scoring and difficulty
    float survivalTime = 0.0f;
    float highScore = 0.0f;
    float baseThrowCooldown = 2.0f;
    float minThrowCooldown = 0.3f;
    float difficultyRampTime = 60.0f;
    
    // Debug mode data
    std::vector<Rectangle> debugAngryFrames;
    std::vector<Rectangle> debugIdleFrames;
    
    // Death sequence
    float deathTimer = 0.0f;
    float deathDuration = 2.0f;
    float fadeDuration = 1.0f;
    float fadeAlpha = 0.0f;

    // Lifecycle
    void Init();
    void Update(float deltaTime);
    void Draw();
    void Cleanup();
    
    // Game actions
    void RestartGame();
    void UpdateBooks();
    
private:
    void DrawPlaying();
    void DrawDying();
    void DrawGameOver();
};

// Global game instance
extern Game game;
