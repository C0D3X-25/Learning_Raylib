#pragma once

#include "Typedef.hpp"
#include "Config.hpp"
#include "Globals.hpp"

struct Timer {
	float lifetime{ 0.0f };
};

// Start or restart the timer.
void startTimer(Timer* timer, float lifetime);

// Update the timer with the current frame time.
void updateTimer(Timer* timer);

// Check if the timer has reached its lifetime.
bool isTimerDone(Timer* timer);