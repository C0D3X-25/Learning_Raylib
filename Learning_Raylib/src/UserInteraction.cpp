#include "UserInteraction.hpp"
#include "raylib.h"
#include "Grid.hpp"

#include <iostream>

namespace inter {

	void getUserKeyPressed(Grid& game_of_life) {

		uint16_t last_key = GetKeyPressed();

		if (last_key == KEY_SPACE) {
			userPauseGame(game_of_life);
		}
		if (last_key == KEY_R) {
			userResetGrid(game_of_life);
		}
		if (last_key == KEY_G) {
			userGenerateRandomGrid(game_of_life);
		}
	}


	void userPauseGame(Grid& game_of_life) {

		if (gbl::is_game_paused) {
			gbl::is_game_paused = false;
#if DEBUG
			std::cout << "Game paused\n";
#endif // DEBUG
		}
		else {
			gbl::is_game_paused = true;
#if DEBUG
			std::cout << "Game resumed\n";
#endif // DEBUG
		}
	}


	void userResetGrid(Grid& game_of_life) {

		if (gbl::is_game_paused) {
			game_of_life.clearGrid();
		}
	}


	void userGenerateRandomGrid(Grid& game_of_life) {

		if (gbl::is_game_paused) {
			game_of_life.generateRandomNewGrid();
		}
	}

} // namespace inter