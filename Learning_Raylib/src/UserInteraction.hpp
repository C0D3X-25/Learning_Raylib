#pragma once

#include "Typedef.hpp"

class Grid;

namespace inter {

	// Get the last key pressed by the user (only one key)
	void getUserKeyPressed(Grid& game_of_life);

	// Pause mode with KEY_SPACE (only after a full generation)
	void userPauseGame(Grid& game_of_life);

	// Reset the grid with KEY_R
	void userResetGrid(Grid& game_of_life);

	// Generate a new random grid with KEY_G
	void userGenerateRandomGrid(Grid& game_of_life);

} // namespace inter

