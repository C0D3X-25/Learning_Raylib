#pragma once

#include "Typedef.hpp"

// In case of window resize, the window size MUST be a multiple of 11
inline const uint16_t WINDOW_WIDTH{ 550 };
inline const uint16_t WINDOW_HEIGHT{ 550 };
inline const uint16_t TARGET_FPS{ 24 };
inline const float TIME_GENERATION{ 1.0f }; // Time between generations in seconds

inline bool is_game_paused{ true };

constexpr uint16_t TOTAL_CELLS_X_AXIS{ WINDOW_WIDTH / 11 };
constexpr uint16_t TOTAL_CELLS_Y_AXIS{ WINDOW_HEIGHT / 11 };

constexpr uint16_t TOTAL_CELLS{ TOTAL_CELLS_X_AXIS * TOTAL_CELLS_Y_AXIS };

// BUG:
// - Error in the cell check
//		=> Error is the cells check are done when the grid is generated
//		=> FIX: use shared ptr
// - Pause not working properly

// TODO:
// - Shared pointer to a 2D array who switch with the timer
// - Timer for the generation of new cells
// - Skip useless cells check
// - Random cells alive generation