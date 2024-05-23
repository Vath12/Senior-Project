#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <SDL.h>
#include "render.h"
#include "image_renderer.h"
#include "eventHandler.h"
#include "globals.h"
#include "sprite.h"
#include "entity.h"
#include "vector2.h"

int window_width = 1920;
int window_height = 1080;
double camera_x = 0;
double camera_y = 0;
double camera_viewportWidth = 50;
double deltaTime = 0;
input keys = input();
//spritesheets have 128x128 cells and anims play at 10fps

double getTimeMillis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
}

int main(int argc, char* args[]) {
	bool run = true;

	sprite::init();
	SDL_Window* window = render_initialize("SeniorProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1920,1080,SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);

	SDL_Rect rect = { 150, 150, 100, 100 };

	SDL_Texture* runAnim = loadTexture(renderer,"resources/images/run_alice_sheet.bmp");

	double lastTime = getTimeMillis();

	
	sprite testMan = sprite(runAnim,10,10,2,2,64);

	for (int i = 0; i < 100; i++) {
		for (int k = 0; k < 100; k++) {
			entity* soldier = entity::create(vector2(i * 2, k*2), 0, &testMan);
			soldier->playAnimation(k % 8, 6, 0, 8);
		}
	}

	while (run) {

		deltaTime = (getTimeMillis() - lastTime);
		lastTime = getTimeMillis();
		
		std::string newTitle = std::format("{} fps", std::round(std::min(1/deltaTime,1024.0)));

		SDL_SetWindowTitle(window,newTitle.c_str());

		SDL_Event event;
		resetSingleFrameEvents();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
			else {
				handleEvent(&event);
			}
		}
		if (keys.release[SDL_SCANCODE_ESCAPE]) {
			SDL_ShowCursor(false);
		}
		
		if (keys.key[SDL_SCANCODE_W]) { camera_y -= 10 * deltaTime; }
		if (keys.key[SDL_SCANCODE_S]) { camera_y += 10 * deltaTime; }
		if (keys.key[SDL_SCANCODE_A]) { camera_x -= 10 * deltaTime; }
		if (keys.key[SDL_SCANCODE_D]) { camera_x += 10 * deltaTime; }
		if (keys.key[SDL_SCANCODE_Z]) { 
			camera_x -= 16/2.0 * deltaTime;
			camera_y -= 16/2.0 * deltaTime;
			camera_viewportWidth += 8 * deltaTime;
		}
		if (keys.key[SDL_SCANCODE_C]) { 
			camera_x += 16/2.0 * deltaTime;
			camera_y += 16/2.0 * deltaTime;
			camera_viewportWidth -= 8 * deltaTime;
		}

		//Clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < entity::numEntities(); i++) {
			entity::getEntity(i)->update(deltaTime);
			entity::getEntity(i)->draw(renderer);
		}

		SDL_RenderPresent(renderer);
	}

	entity::freeAll();
	unloadTextures();
	SDL_DestroyWindowSurface(window);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

