#pragma once

#include "Typedef.hpp"

namespace gbl {

	// In case of window resize, the window size MUST be a multiple of 11
	inline const uint16_t WINDOW_WIDTH{ 550 };
	inline const uint16_t WINDOW_HEIGHT{ 550 };
	inline const uint16_t TARGET_FPS{ 24 };
	inline const float TIME_BEFORE_NEXT_GENERATION{ 1.0f }; // Time between generations in seconds
	inline const uint16_t CHANCE_TO_GENERATE_LIVING_CELL{ 10 }; // in %

	inline bool is_game_paused{ true };
	inline long long int nbr_generation{ 0 };

	constexpr uint16_t TOTAL_CELLS_X_AXIS{ WINDOW_WIDTH / 11 };
	constexpr uint16_t TOTAL_CELLS_Y_AXIS{ WINDOW_HEIGHT / 11 };
	constexpr uint16_t TOTAL_CELLS{ TOTAL_CELLS_X_AXIS * TOTAL_CELLS_Y_AXIS };

} // namespace gbl

// BUG:
// - 

// TODO:
// - Skip useless cells check
// - Modification of the Timer in game
// - Random cells alive generation in game
