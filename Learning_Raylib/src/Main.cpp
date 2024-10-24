#pragma once
#include "Typedef.hpp"
#include "Config.hpp"
#include "raylib.h" 
#include "Grid.hpp"
#include "Timer.hpp"
#include "UserInteraction.hpp"

#include <iostream>


int main(void) {

	Grid game_of_life;
    game_of_life.generateRandomNewGrid();

#if DEBUG
    game_of_life.printArrayGrid();
#endif // DEBUG

    InitWindow(gbl::WINDOW_WIDTH, gbl::WINDOW_HEIGHT, "Game of Life");
    SetTargetFPS(gbl::TARGET_FPS);


    // ===== Main game loop =====
    while (!WindowShouldClose()) {

		// ===== Event handling =====
        inter::getUserKeyPressed(game_of_life);

		// ===== Update =====
        if (!gbl::is_game_paused) {
            if (isTimerDone(game_of_life.m_p_timer)) {
                game_of_life.nextGeneration();
                game_of_life.updateGrid();
                startTimer(game_of_life.m_p_timer, gbl::TIME_BEFORE_NEXT_GENERATION);
            }
        }
        else {
            game_of_life.userChangeCellState();
        }

		// ===== Drawing =====
        BeginDrawing();

        ClearBackground(BLACK);
		game_of_life.drawGrid();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}