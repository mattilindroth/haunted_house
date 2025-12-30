#include "raylib.h"
#include "game.h"

#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(0)));

    game.Init();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        game.Update(deltaTime);
        game.Draw();
    }

    game.Cleanup();
    CloseWindow();

    return 0;
}
