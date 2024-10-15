#include "raylib.h" 
#include "Typedef.hpp"
#include "Grid.hpp"


int main(void) {
	Grid gameOfLife;
    gameOfLife.printArrayGrid();
    gameOfLife.nextStageGrid();
    
    // In case of window resize, the window size MUST be a multiple of 11
    const uint16_t WINDOW_WIDTH = 770;
    const uint16_t WINDOW_HEIGHT = 770;
	const uint16_t TARGET_FPS = 60;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of Life");
    SetTargetFPS(TARGET_FPS);

    

    // Main game loop
    while (!WindowShouldClose()) {

		// Event handling


		// Update


		// Drawing
        BeginDrawing();
        ClearBackground(BLACK);

		gameOfLife.drawGrid();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}