#include "render.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>


SDL_Window* render_initialize(const char title[], int x, int y, int width, int height, int flags){
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf(SDL_GetError());
		exit(1);
	}

	SDL_Window* window = SDL_CreateWindow(title,x,y,width,height,flags);

	if (!window) {
		printf(SDL_GetError());
		exit(1);
	}

	return window;
}

SDL_Surface* get_surface(SDL_Window* window) {
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	if (!surface) {
		printf(SDL_GetError());
		exit(1);
	}
	return surface;
}