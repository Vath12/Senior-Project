#include "render.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "globals.h"


double worldToCameraScale(double x) {
	return x / camera_viewportWidth * window_width;
}
double cameraToWorldScale(double x) {
	return x / window_width * camera_viewportWidth;
}

vector2 worldToCameraScale(vector2 x) {
	return x / camera_viewportWidth * window_width;
}
vector2 cameraToWorldScale(vector2 x) {
	return x / window_width * camera_viewportWidth;
}

vector2 worldToCamera(vector2 position) {
	double scale = window_width / camera_viewportWidth;
	return vector2(
		(position.x - camera_x) * scale,
		(position.y - camera_y) * scale);
}
vector2 cameraToWorld(vector2 position) {
	double scale = window_width / camera_viewportWidth;
	return vector2(
		(position.x/scale)+camera_x,
		(position.y/scale)+camera_y);
}

SDL_Rect worldToCamera(SDL_Rect* input) {
	SDL_Rect output = { 
		(input->x - camera_x) /camera_viewportWidth * window_width,
		(input->y - camera_y) /camera_viewportWidth * window_width,
		input->w/camera_viewportWidth * window_width,
		input->h/camera_viewportWidth * window_width
	};
	return output;
}

SDL_Rect worldToCamera(double x, double y, double w, double h) {
	vector2 p = worldToCameraIso(vector2(x,y));
	SDL_Rect output = {
		(int)p.x,
		(int)p.y,
		w/ camera_viewportWidth * window_width,
		h / camera_viewportWidth * window_width
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


vector2 cameraToWorldIso(vector2 a) {
	a = cameraToWorld(a);
	a = vector2(a.x / tile_halfwidth, a.y / tile_halfheight);
	return vector2( (a.x+a.y)/2.0,(a.y-a.x)/2.0);
}

vector2 worldToCameraIso(vector2 a) {
	return worldToCamera(vector2((a.x - a.y)*tile_halfwidth, (a.y + a.x) * tile_halfheight));
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