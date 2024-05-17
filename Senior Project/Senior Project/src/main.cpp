#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "render.h"
//spritesheets are 128x128
int main(int argc, char* args[]) {
	bool run = true;

	SDL_Window* window = render_initialize("SeniorProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1920,1080,SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);

	SDL_Rect rect = { 150, 150, 100, 100 };

	double lastTime;

	while (run) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindowSurface(window);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
