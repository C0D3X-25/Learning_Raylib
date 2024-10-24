#pragma once

#include "Typedef.hpp"
#include "Config.hpp"
#include "Globals.hpp"

#include <memory>

struct Timer {
	float lifetime{ 0.0f };
};

// Start or restart the timer.
void startTimer(std::unique_ptr<Timer>& timer, float lifetime);

// Update the timer with the current frame time.
void updateTimer(std::unique_ptr<Timer>& timer);

// Check if the timer has reached its lifetime.
bool isTimerDone(std::unique_ptr<Timer>& timer);