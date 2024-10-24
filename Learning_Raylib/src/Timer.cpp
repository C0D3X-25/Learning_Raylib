#include "Timer.hpp"
#include "raylib.h"

void startTimer(std::unique_ptr<Timer>& timer, float lifetime) {
	timer->lifetime = lifetime;
}


void updateTimer(std::unique_ptr<Timer>& timer) {
	// Substract frame time from timer lifetime
	if (timer->lifetime > 0) {
		timer->lifetime -= GetFrameTime();
	}
}


bool isTimerDone(std::unique_ptr<Timer>& timer) {

	if (timer->lifetime <= 0) {
		return true;
	}

	return false;
}