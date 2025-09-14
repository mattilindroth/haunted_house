#include "raylib.h"
#include "falling_object.h"
#include "player.h"
#include "splash.h"

#include <vector>
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    SPLASH
};

// Screen dimensions
const int screenWidth = 800;
const int screenHeight = 600;

// Player properties
Texture2D playerTexture = LoadTexture("../resources/Ghost2.png");
Sprite playerSprite(playerTexture, { (float)screenWidth / 2 - 25, (float)screenHeight - 50 });
Player player((float)screenWidth / 2 - 25, (float)screenHeight - 50, 50, 50, screenWidth, 5.0f, playerSprite);
SplashScreen *splashScreen;
enum GameState currentState = SPLASH;

std::vector<FallingObject> fallingObjects;
float spawnRate = 1.0f; // In seconds
float lastSpawnTime = 0.0f;

// Function to initialize the game
void InitGame() {
    //player = { (float)screenWidth / 2 - 25.0f, (float)screenHeight - 50.0f, 50.0f, 50.0f, screenWidth }; // Start position of the ghost
    InitWindow(screenWidth, screenHeight, "Haunted House!");
    SetTargetFPS(60);

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

// Function to spawn falling objects (haunted artifacts)
void SpawnFallingObject() {
    FallingObject obj(
        (float)(rand() % (screenWidth - 50)), // Random x-position
        -50, // Start above the screen
        50, // Width
        50, // Height
        0, // Speed X (no horizontal movement)
        (float)(rand() % 3 + 2) // Random speed between 2 and 4
    );
    fallingObjects.push_back(obj);
}

// Function to update falling objects
void UpdateFallingObjects() {
    float currentTime = GetTime();
    if (currentTime - lastSpawnTime > spawnRate) {
        SpawnFallingObject();
        lastSpawnTime = currentTime;
    }

    for (int i = 0; i < fallingObjects.size(); i++) {
        fallingObjects[i].rect.y += fallingObjects[i].speed.y;

        // Collision detection with player
        if (CheckCollisionRecs(*player.GetRectangle(), fallingObjects[i].rect)) {
            // End game on collision (you can expand this later)
            DrawText("Game Over!", screenWidth / 2 - 60, screenHeight / 2 - 20, 20, RED);
            EndDrawing();
            return;
        }

        // Remove objects that go off the screen
        if (fallingObjects[i].rect.y > screenHeight) {
            fallingObjects.erase(fallingObjects.begin() + i);
            i--; // Adjust index after removal
        }
    }
}

void Update() {
    switch(currentState) {
        case SPLASH:
            splashScreen->Update();
            if (splashScreen->IsFinished()) {
                currentState = PLAYING;
            }
            break;
        case MENU:
            // Handle menu input (not implemented yet)
            break;
        case PLAYING:
            UpdatePlayer();
            UpdateFallingObjects();
            break;
        case GAME_OVER:
            // Handle game over input (not implemented yet)
            break;
    }
}





// Function to draw the game
void DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch(currentState) {
        case SPLASH:
            // Draw the splash screen
            splashScreen->Draw();
            break;
        case MENU:
            // Draw the menu (not implemented yet)
            DrawText("Menu (not implemented)", screenWidth / 2 - 100, screenHeight / 2 - 20, 20, DARKGRAY);
            break;
        case PLAYING:
            // Draw the player (ghost)
            DrawRectangleRec(*player.GetRectangle(), BLUE); // You can replace with an image later

            // Draw the falling objects (haunted artifacts)
            for (const auto& obj : fallingObjects) {
                DrawRectangleRec(obj.rect, DARKGRAY); // Replace with artifact image if you have one
            }
            break;
        case GAME_OVER:
            // Draw the game over screen (not implemented yet)
            DrawText("Game Over (not implemented)", screenWidth / 2 - 100, screenHeight / 2 - 20, 20, DARKGRAY);
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
