#include "Timer.hpp"
#include "raylib.h"

void startTimer(Timer* timer, float lifetime) {
	timer->lifetime = lifetime;
}


void updateTimer(Timer* timer) {
	// Substract frame time from timer lifetime
	if (timer->lifetime > 0) {
		timer->lifetime -= GetFrameTime();
	}
}


bool isTimerDone(Timer* timer) {

	if (timer->lifetime <= 0) {
		return true;
	}

	return false;
}