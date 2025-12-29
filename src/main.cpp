#include "raylib.h"
#include "falling_object.h"
#include "player.h"
#include "ghost.h"
#include "splash.h"

#include <vector>
#include <cstdlib>  // For rand()
#include <ctime>    // For time()
#include <algorithm> // For std::max

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    SPLASH
};

// Screen dimensions
const int screenWidth = 800;
const int screenHeight = 600;

// Ghost enemy (at top of screen, throws books)
Texture2D ghostTexture;
Sprite ghostSprite;
Ghost ghost;

// Player (human at bottom, dodging books)
Player player;

SplashScreen *splashScreen = nullptr;
enum GameState currentState = SPLASH;

std::vector<FallingObject> fallingObjects;  // Books thrown by ghost

// Scoring and difficulty
float survivalTime = 0.0f;          // How long player has survived
float highScore = 0.0f;             // Best survival time
float baseThrowCooldown = 2.0f;     // Starting throw cooldown
float minThrowCooldown = 0.3f;      // Minimum cooldown (max difficulty)
float difficultyRampTime = 60.0f;   // Time to reach max difficulty (seconds)

// Function to initialize the game
void InitGame() {
    InitWindow(screenWidth, screenHeight, "Haunted House - Dodge the Books!");
    SetTargetFPS(60);

    // Load ghost sprite (enemy at top of screen)
    ghostTexture = LoadTexture("../resources/Ghost2.png");
    ghostSprite = Sprite(ghostTexture, { 100, 30 });

    // Define ghost idle animation frames from sprite sheet
    // Each frame is 50x86 pixels, starting at (25, 5)
    // 4 frames laid out horizontally
    const float idleFrameWidth = 50.0f;
    const float idleFrameHeight = 86.0f;
    const float idleStartX = 25.0f;
    const float idleStartY = 5.0f;
    const float idleFrameSpacing = 50.0f;

    std::vector<Rectangle> idleFrames = {
        { idleStartX + 0 * idleFrameSpacing, idleStartY, idleFrameWidth, idleFrameHeight },
        { idleStartX + 1 * idleFrameSpacing, idleStartY, idleFrameWidth, idleFrameHeight },
        { idleStartX + 2 * idleFrameSpacing, idleStartY, idleFrameWidth, idleFrameHeight },
        { idleStartX + 3 * idleFrameSpacing, idleStartY, idleFrameWidth, idleFrameHeight }
    };

    ghostSprite.addAnimation("idle", idleFrames, 8.0f);

    // Define ghost angry animation frames from sprite sheet
    // Frame starts at (23, 105) and ends at (70, 192), so frame is 47x87 pixels
    // 4 frames laid out horizontally
    const float angryFrameWidth = 47.0f;
    const float angryFrameHeight = 87.0f;
    const float angryStartX = 23.0f;
    const float angryStartY = 105.0f;
    const float angryFrameSpacing = 50.0f;  // Adjust based on your sprite sheet

    std::vector<Rectangle> angryFrames = {
        { angryStartX + 0 * angryFrameSpacing, angryStartY, angryFrameWidth, angryFrameHeight },
        { angryStartX + 1 * angryFrameSpacing, angryStartY, angryFrameWidth, angryFrameHeight },
        { angryStartX + 2 * angryFrameSpacing, angryStartY, angryFrameWidth, angryFrameHeight },
        { angryStartX + 3 * angryFrameSpacing, angryStartY, angryFrameWidth, angryFrameHeight }
    };

    ghostSprite.addAnimation("angry", angryFrames, 12.0f);  // Faster animation when angry!
    ghostSprite.setActive("idle");

    // Create ghost enemy at top of screen - fast supernatural speed!
    ghost = Ghost(100, 30, 50, 87, screenWidth, screenHeight, 6.0f, ghostSprite);

    // Create player (human) at bottom of screen - simple rectangle for now
    // You can add a player sprite later
    Sprite emptySprite;
    player = Player((float)screenWidth / 2 - 25, (float)screenHeight - 60, 40, 50, screenWidth, 5.0f, emptySprite);

    // Create the splash screen
    splashScreen = new SplashScreen("../resources/splash.png", 3.0f, 1.0f, 1.0f);
}



// Function to handle player input
void UpdatePlayer() {
    if (IsKeyDown(KEY_LEFT)) player.GetRectangle()->x -= player.speed;
    if (IsKeyDown(KEY_RIGHT)) player.GetRectangle()->x += player.speed;

    // Boundary check for player movement
    if (player.GetRectangle()->x < 0) player.GetRectangle()->x = 0;
    if (player.GetRectangle()->x + player.GetRectangle()->width > screenWidth) player.GetRectangle()->x = screenWidth - player.GetRectangle()->width;
}

// Function to update falling books and check collisions
void UpdateBooks() {
    for (int i = 0; i < (int)fallingObjects.size(); i++) {
        fallingObjects[i].Update();

        // Collision detection with player
        if (CheckCollisionRecs(*player.GetRectangle(), fallingObjects[i].rect)) {
            currentState = GAME_OVER;
            return;
        }

        // Remove books that go off the screen
        if (fallingObjects[i].rect.y > screenHeight || 
            fallingObjects[i].rect.x < -50 || 
            fallingObjects[i].rect.x > screenWidth + 50) {
            fallingObjects.erase(fallingObjects.begin() + i);
            i--;
        }
    }
}

void Update() {
    float deltaTime = GetFrameTime();
    
    switch(currentState) {
        case SPLASH:
            splashScreen->Update();
            if (splashScreen->IsFinished()) {
                currentState = PLAYING;
                survivalTime = 0.0f;  // Reset survival time when starting
            }
            break;
        case MENU:
            // Handle menu input (not implemented yet)
            break;
        case PLAYING: {
            // Update survival time
            survivalTime += deltaTime;
            
            // Increase difficulty over time - ghost throws faster!
            // Linear interpolation from baseThrowCooldown to minThrowCooldown over difficultyRampTime
            float difficultyProgress = std::min(survivalTime / difficultyRampTime, 1.0f);
            ghost.throwCooldown = baseThrowCooldown - (baseThrowCooldown - minThrowCooldown) * difficultyProgress;
            
            // Update ghost enemy
            ghost.Update(deltaTime);
            
            // Ghost throws books at player when ready
            if (ghost.CanThrow()) {
                Vector2 playerPos = { 
                    player.GetRectangle()->x + player.GetRectangle()->width / 2,
                    player.GetRectangle()->y 
                };
                FallingObject book = ghost.ThrowBook(playerPos);
                fallingObjects.push_back(book);
                ghost.ResetThrowTimer();
            }
            
            // Update player
            player.Update();
            
            // Update falling books
            UpdateBooks();
            break;
        }
        case GAME_OVER:
            // Update high score
            if (survivalTime > highScore) {
                highScore = survivalTime;
            }
            break;
    }
}





// Function to draw the game
void DrawGame() {
    BeginDrawing();
    ClearBackground(DARKGRAY);  // Dark spooky background
    
    switch(currentState) {
        case SPLASH:
            // Draw the splash screen
            splashScreen->Draw();
            break;
        case MENU:
            // Draw the menu (not implemented yet)
            DrawText("Menu (not implemented)", screenWidth / 2 - 100, screenHeight / 2 - 20, 20, WHITE);
            break;
        case PLAYING: {
            // Draw bookshelf background at top
            DrawRectangle(0, 0, screenWidth, 120, DARKBROWN);
            DrawRectangle(0, 115, screenWidth, 10, BROWN);
            
            // Draw the angry ghost (enemy)
            ghost.Draw();
            
            // Draw the player (human) - simple rectangle for now
            DrawRectangleRec(*player.GetRectangle(), BLUE);
            DrawRectangle((int)player.GetRectangle()->x + 10, (int)player.GetRectangle()->y + 5, 20, 15, BEIGE);  // Face
            
            // Draw the falling books
            for (auto& book : fallingObjects) {
                book.Draw();
            }
            
            // Draw survival timer (top right)
            DrawText(TextFormat("Time: %.1f s", survivalTime), screenWidth - 150, 130, 24, WHITE);
            
            // Draw high score if we have one
            if (highScore > 0) {
                DrawText(TextFormat("Best: %.1f s", highScore), screenWidth - 150, 160, 18, YELLOW);
            }
            
            // Draw difficulty indicator
            float difficultyPercent = std::min(survivalTime / difficultyRampTime, 1.0f) * 100.0f;
            DrawText(TextFormat("Difficulty: %.0f%%", difficultyPercent), 10, 130, 18, 
                     difficultyPercent > 75 ? RED : (difficultyPercent > 50 ? ORANGE : WHITE));
            
            // Draw instructions
            DrawText("Use LEFT/RIGHT arrows to dodge the books!", 10, screenHeight - 25, 16, WHITE);
            break;
        }
        case GAME_OVER:
            // Draw game over screen
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
            DrawText("GAME OVER!", screenWidth / 2 - 120, screenHeight / 2 - 80, 40, RED);
            DrawText("The ghost got you!", screenWidth / 2 - 100, screenHeight / 2 - 30, 20, WHITE);
            
            // Draw survival time
            DrawText(TextFormat("You survived: %.1f seconds", survivalTime), screenWidth / 2 - 130, screenHeight / 2 + 10, 24, YELLOW);
            
            // Draw high score
            if (survivalTime >= highScore) {
                DrawText("NEW HIGH SCORE!", screenWidth / 2 - 100, screenHeight / 2 + 50, 24, GREEN);
            } else {
                DrawText(TextFormat("Best: %.1f seconds", highScore), screenWidth / 2 - 90, screenHeight / 2 + 50, 20, LIGHTGRAY);
            }
            
            DrawText("Press SPACE to restart", screenWidth / 2 - 110, screenHeight / 2 + 100, 20, WHITE);
            
            // Allow restart
            if (IsKeyPressed(KEY_SPACE)) {
                fallingObjects.clear();
                survivalTime = 0.0f;
                player = Player((float)screenWidth / 2 - 25, (float)screenHeight - 60, 40, 50, screenWidth, 5.0f, Sprite());
                ghost = Ghost(100, 30, 50, 87, screenWidth, screenHeight, 6.0f, ghostSprite);
                ghost.throwCooldown = baseThrowCooldown;  // Reset difficulty
                currentState = PLAYING;
            }
            break;
    }
    
    EndDrawing();
}

// Main game loop
int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    InitGame();

    while (!WindowShouldClose()) {
        
        //UpdatePlayer();
        Update();
        //UpdateFallingObjects();
        DrawGame();
    }

    delete splashScreen;
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
