#include "game.h"
#include "audio.h"
#include "debug.h"
#include <algorithm>

// Global instance
Game game;

void Game::Init() {
    InitWindow(screenWidth, screenHeight, "Haunted House - Dodge the Books!");
    SetTargetFPS(60);
    
    // Initialize audio
    audio.Init();
    
    // Initialize scoreboard
    scoreboard.Init();

    // Load ghost sprite
    ghostTexture = LoadTexture("../resources/Ghost2.png");
    ghostSprite = Sprite(ghostTexture, { 100, 30 });

    // Define ghost idle animation frames
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

    // Define ghost angry animation frames
    const float angryFrameWidth = 50.0f;
    const float angryFrameHeight = 90.0f;
    const float angryStartX = 23.0f;
    const float angryStartY = 105.0f;
    const float angryFrameSpacing = 50.0f;

    std::vector<Rectangle> angryFrames = {
        { angryStartX + 0 * angryFrameSpacing, angryStartY, angryFrameWidth, angryFrameHeight },
        { angryStartX + 1 * angryFrameSpacing, angryStartY, angryFrameWidth + 10, angryFrameHeight },
        { angryStartX + 2 * angryFrameSpacing + 30, angryStartY, angryFrameWidth, angryFrameHeight },
        { angryStartX + 3 * angryFrameSpacing + 45, angryStartY, angryFrameWidth + 10, angryFrameHeight }
    };
    
    // Store for debug mode
    debugAngryFrames = angryFrames;
    debugIdleFrames = idleFrames;
    
    // Setup debug mode references
    debug.angryFrames = &debugAngryFrames;
    debug.idleFrames = &debugIdleFrames;
    debug.texture = &ghostTexture;
    debug.screenWidth = screenWidth;
    debug.screenHeight = screenHeight;

    ghostSprite.addAnimation("angry", angryFrames, 12.0f);
    ghostSprite.setActive("idle");

    // Create ghost enemy
    ghost = Ghost(100, 30, 50, 87, screenWidth, screenHeight, 6.0f, ghostSprite);

    // Create player
    Sprite emptySprite;
    player = Player((float)screenWidth / 2 - 25, (float)screenHeight - 60, 40, 50, screenWidth, 5.0f, emptySprite);

    // Create the splash screen
    splashScreen = new SplashScreen("../resources/splash.png", 3.0f, 1.0f, 1.0f);
}

void Game::UpdateBooks() {
    for (int i = 0; i < (int)fallingObjects.size(); i++) {
        fallingObjects[i].Update();

        // Collision detection with player
        if (CheckCollisionRecs(*player.GetRectangle(), fallingObjects[i].rect)) {
            audio.PlayHitSound();
            audio.PlaySplashMusic();
            
            // Check if player made the scoreboard
            scoreboard.StartNameEntry(survivalTime);
            
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

void Game::Update(float deltaTime) {
    // Update audio
    audio.Update();
    
    // Toggle sound with M key (but not while entering name)
    if (IsKeyPressed(KEY_M) && !scoreboard.IsEnteringName()) {
        audio.ToggleSound();
    }
    
    // Debug frame mode toggle (F1)
    if (IsKeyPressed(KEY_F1)) {
        debug.Toggle();
    }
    
    // Handle debug mode
    if (debug.IsActive()) {
        debug.Update();
        return;
    }
    
    switch(currentState) {
        case SPLASH:
            splashScreen->Update();
            if (splashScreen->IsFinished()) {
                currentState = PLAYING;
                survivalTime = 0.0f;
                audio.PlayGameMusic();
            }
            break;
            
        case MENU:
            // Not implemented yet
            break;
            
        case PLAYING: {
            survivalTime += deltaTime;
            
            // Increase difficulty over time
            float difficultyProgress = std::min(survivalTime / difficultyRampTime, 1.0f);
            ghost.throwCooldown = baseThrowCooldown - (baseThrowCooldown - minThrowCooldown) * difficultyProgress;
            
            // Update ghost enemy
            ghost.Update(deltaTime);
            
            // Ghost throws books at player
            if (ghost.CanThrow()) {
                Vector2 playerPos = { 
                    player.GetRectangle()->x + player.GetRectangle()->width / 2,
                    player.GetRectangle()->y 
                };
                FallingObject book = ghost.ThrowBook(playerPos);
                fallingObjects.push_back(book);
                ghost.ResetThrowTimer();
                
                audio.PlayWhooshSound();
                audio.PlayThrowSound();
            }
            
            // Update player
            player.Update();
            
            // Update falling books
            UpdateBooks();
            break;
        }
        
        case GAME_OVER:
            // Handle name entry for scoreboard
            if (scoreboard.IsEnteringName()) {
                scoreboard.UpdateNameEntry();
            }
            
            // Update high score from scoreboard
            if (!scoreboard.GetScores().empty()) {
                highScore = scoreboard.GetScores()[0].score;
            }
            break;
    }
}

void Game::DrawPlaying() {
    // Draw bookshelf background at top
    DrawRectangle(0, 0, screenWidth, 120, DARKBROWN);
    DrawRectangle(0, 115, screenWidth, 10, BROWN);
    
    // Draw the ghost
    ghost.Draw();
    
    // Draw the player
    DrawRectangleRec(*player.GetRectangle(), BLUE);
    DrawRectangle((int)player.GetRectangle()->x + 10, (int)player.GetRectangle()->y + 5, 20, 15, BEIGE);
    
    // Draw falling books
    for (auto& book : fallingObjects) {
        book.Draw();
    }
    
    // Draw survival timer
    DrawText(TextFormat("Time: %.1f s", survivalTime), screenWidth - 150, 130, 24, WHITE);
    
    // Draw high score
    if (highScore > 0) {
        DrawText(TextFormat("Best: %.1f s", highScore), screenWidth - 150, 160, 18, YELLOW);
    }
    
    // Draw difficulty indicator
    float difficultyPercent = std::min(survivalTime / difficultyRampTime, 1.0f) * 100.0f;
    DrawText(TextFormat("Difficulty: %.0f%%", difficultyPercent), 10, 130, 18, 
             difficultyPercent > 75 ? RED : (difficultyPercent > 50 ? ORANGE : WHITE));
    
    // Draw instructions
    DrawText("Use LEFT/RIGHT arrows to dodge the books!", 10, screenHeight - 25, 16, WHITE);
}

void Game::DrawGameOver() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.9f));
    
    // Draw header
    DrawText("GAME OVER!", screenWidth / 2 - 120, 30, 40, RED);
    DrawText(TextFormat("You survived: %.1f seconds", survivalTime), screenWidth / 2 - 130, 75, 20, YELLOW);
    
    // Draw the scoreboard
    scoreboard.Draw(screenWidth, screenHeight, survivalTime);
    
    // Only allow restart if not entering name
    if (!scoreboard.IsEnteringName() && IsKeyPressed(KEY_SPACE)) {
        RestartGame();
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    
    // Handle debug mode drawing
    if (debug.IsActive()) {
        debug.Draw();
        EndDrawing();
        return;
    }
    
    switch(currentState) {
        case SPLASH:
            splashScreen->Draw();
            break;
            
        case MENU:
            DrawText("Menu (not implemented)", screenWidth / 2 - 100, screenHeight / 2 - 20, 20, WHITE);
            break;
            
        case PLAYING:
            DrawPlaying();
            break;
            
        case GAME_OVER:
            DrawGameOver();
            break;
    }
    
    EndDrawing();
}

void Game::RestartGame() {
    fallingObjects.clear();
    survivalTime = 0.0f;
    player = Player((float)screenWidth / 2 - 25, (float)screenHeight - 60, 40, 50, screenWidth, 5.0f, Sprite());
    ghost = Ghost(100, 30, 50, 87, screenWidth, screenHeight, 6.0f, ghostSprite);
    ghost.throwCooldown = baseThrowCooldown;
    audio.PlayGameMusic();
    currentState = PLAYING;
}

void Game::Cleanup() {
    delete splashScreen;
    audio.Cleanup();
}
