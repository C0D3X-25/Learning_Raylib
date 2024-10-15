#pragma once

#include "Typedef.hpp"

// In case of window resize, the window size MUST be a multiple of 11
inline const uint16_t WINDOW_WIDTH{ 1100 };
inline const uint16_t WINDOW_HEIGHT{ 1100 };
inline const uint16_t TARGET_FPS{ 24 };
inline const float TIME_GENERATION{ 1.0f }; // Time between generations in seconds

inline bool is_game_paused{ true };


// TODO
// - Timer for the generation of new cells
// - Skip useless cells check