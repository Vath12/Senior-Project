#include <SDL.h>
#include "globals.h"
#include "eventHandler.h"
#include <stdio.h>
#include <iostream>


void resetSingleFrameEvents() {
	for (int i = 0; i < 512; i++) {
		keys.press[i] = 0;
		keys.release[i] = 0;
	}
}

void handleEvent(SDL_Event* input) {

	if (input->type == SDL_KEYDOWN) {
		keys.press[input->key.keysym.scancode] = true;
		keys.key[input->key.keysym.scancode] = true;
	}
	if (input->type == SDL_KEYUP) {
		keys.release[input->key.keysym.scancode] = true;
		keys.key[input->key.keysym.scancode] = false;
	}
}
