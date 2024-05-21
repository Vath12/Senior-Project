#include "render.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "globals.h"

SDL_Rect worldToCamera(SDL_Rect* input) {
	SDL_Rect output = { 
		(input->x - camera_x) /camera_viewportWidth * window_width,
		(input->y - camera_y) /camera_viewportWidth * window_width,
		input->w/camera_viewportWidth * window_width,
		input->h/camera_viewportWidth * window_width
	};
	return output;
}
SDL_Rect cameraToWorld(SDL_Rect* input) {
	SDL_Rect output = {
	(input->x+camera_x) / window_width * camera_viewportWidth,
	(input->y+camera_y) / window_width * camera_viewportWidth,
	input->w / window_width * camera_viewportWidth,
	input->h / window_width * camera_viewportWidth
	};
	return output;
}

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