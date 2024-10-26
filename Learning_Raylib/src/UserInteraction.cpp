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

		gbl::game_paused = !gbl::game_paused;

#if DEBUG
		std::cout << "Game is " << (gbl::game_paused ? "pause\n" : "resume\n");
#endif // DEBUG
	}


	void userResetGrid(Grid& game_of_life) {

		if (gbl::game_paused) {
			game_of_life.clearGrid();
		}
	}


	void userGenerateRandomGrid(Grid& game_of_life) {

		if (gbl::game_paused) {
			game_of_life.generateRandomNewGrid();
		}
	}

} // namespace inter