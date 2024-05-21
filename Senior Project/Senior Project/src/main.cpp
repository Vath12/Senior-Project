#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <SDL.h>
#include "render.h"
#include "image_renderer.h"
#include "globals.h"

int window_width = 1920;
int window_height = 1080;
double camera_x = 0;
double camera_y = 0;
double camera_viewportWidth = 50;
//spritesheets have 128x128 cells and anims play at 10fps
int main(int argc, char* args[]) {
	bool run = true;

	SDL_Window* window = render_initialize("SeniorProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1920,1080,SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);

	SDL_Rect rect = { 150, 150, 100, 100 };

	SDL_Texture* runAnim = loadTexture(renderer,"resources/images/run_alice_sheet.bmp");

	double lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	double deltaTime = 0;
	
	while (run) {

		deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - lastTime) / 1000;
		lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
		}


		//Clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		rect = { 0,0,2,2 };
		rect = worldToCamera(&rect);

		SDL_Rect src = { 0, 0, 64, 64 };

		renderTexturePortion(renderer,runAnim, &rect, &src);

		SDL_RenderPresent(renderer);

	}

	unloadTextures();
	SDL_DestroyWindowSurface(window);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
