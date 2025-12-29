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
    GAME_OVER,
    SPLASH
};

class Game {
public:
    // Screen dimensions
    static const int screenWidth = 800;
    static const int screenHeight = 600;

    // Game state
    GameState currentState = SPLASH;
    
    // Ghost enemy
    Texture2D ghostTexture;
    Sprite ghostSprite;
    Ghost ghost;
    
    // Player
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
    void DrawGameOver();
};

// Global game instance
extern Game game;
