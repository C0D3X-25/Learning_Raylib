#pragma once

#include "raylib.h" 
#include "Typedef.hpp"
#include "Grid.hpp"
#include <iostream>


int main(void) {

	Grid gameOfLife;

#if DEBUG
    gameOfLife.printArrayGrid();
#endif // DEBUG

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of Life");
    SetTargetFPS(TARGET_FPS);


    // ===== Main game loop =====
    while (!WindowShouldClose()) {

		// ===== Event handling =====
 
        // Pause mode
        if (IsKeyPressed(KEY_SPACE)) {
            if (is_game_paused) {
				is_game_paused = false;
				std::cout << "Game paused\n";
            }
			else {
			    is_game_paused = true;
				std::cout << "Game resumed\n";
            }
        }


		// ===== Update =====
        if (!is_game_paused) {
            gameOfLife.nextGenerationGrid();
        }
        else {
            gameOfLife.userChangeCellState();
        }

		// ===== Drawing =====
        BeginDrawing();
        ClearBackground(BLACK);

		gameOfLife.drawGrid();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}